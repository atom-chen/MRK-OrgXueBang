//
//  PageInteract.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#include "PageInteract.hpp"
#include "TipManager.hpp"
#include "Recorder.h"
#include "stdafx.h"
#include "WordFind.h"
//#include "mySpine.hpp"
#include "NewHandTip.hpp"

#include "BookBody.hpp"
#include "RecordPageInteract.hpp"
#include "UtilsDefine.h"
#include "FileLog.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

PageInteract::PageInteract()
{
    fRecordLimit = 180.0f;
    m_interState = eInterClose;
//    _fMin = DataTableManager::getInstance()->getValue(eTable_config_setting, str_config_setting_Ps_Earth_Min, 0)->getFloat();
//    _fMax = DataTableManager::getInstance()->getValue(eTable_config_setting, str_config_setting_Ps_Earth_Max, 0)->getFloat();
//    _fTime = DataTableManager::getInstance()->getValue(eTable_config_setting, str_config_setting_Add_Nuts_Txt, 0)->getFloat();
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PageInteract::setPageAutoPlay), MSG_PAGE_AUTOPLAY, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PageInteract::updateSoundSlider), MSG_SET_SOUND_SPEED, nullptr);
}

PageInteract::~PageInteract()
{
    __NotificationCenter::getInstance()->removeObserver(this, MSG_PAGE_AUTOPLAY);
    __NotificationCenter::getInstance()->removeObserver(this, MSG_SET_SOUND_SPEED);
}

void PageInteract::onEnter()
{
    PageBase::onEnter();
    scheduleUpdate();
}

void PageInteract::onExit()
{
    unscheduleUpdate();
    PageBase::onExit();
}

PageInteract* PageInteract::create()
{
    PageInteract *ret = new (std::nothrow) PageInteract();
    
    auto mUI = CSLoader::createNode("View/Reading/BookInteract.csb");
    Widget* pageUI = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_interact");
    pageUI->removeFromParent();
    pageUI->retain();
    
    if (ret && ret->init(pageUI))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

PageInteract* PageInteract::create(Widget* pageUI)
{
    PageInteract *ret = new (std::nothrow) PageInteract();
    if (ret && ret->init(pageUI))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool PageInteract::init(Widget* pageUI)
{
    if (!PageBase::init())
    {
        return false;
    }
    this->setTag(eLayerBookInter);
    this->load(pageUI);
    
    return true;
}

void PageInteract::addEvents()
{
    Widget* pageWidget = getPageWidget();
    txt_content = static_cast<Text*>(Helper::seekWidgetByName(pageWidget, "txt_content"));
    
    //交互模式下面的三个按钮
    panel_menu = (Widget*)pageWidget->getChildByName("panel_menu");
    
    img_gear_fg0 = static_cast<ImageView*>(Helper::seekWidgetByName(panel_menu, "img_gear_fg0"));
    img_gear_fg1 = static_cast<ImageView*>(Helper::seekWidgetByName(panel_menu, "img_gear_fg1"));
    img_gear_fg2 = static_cast<ImageView*>(Helper::seekWidgetByName(panel_menu, "img_gear_fg2"));
    
    btn_record = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_record_0"));
    btn_record->setActionTag(eBtnDummy);
    btn_record->addTouchEventListener(CC_CALLBACK_2(PageInteract::touchEvent, this));
    btn_record = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_record"));
    btn_record->setActionTag(eBtnInterRecord);
    btn_record->addTouchEventListener(CC_CALLBACK_2(PageInteract::touchEvent, this));
    
    skeleton_record = (SkeletonAnimation*)btn_record->getParent()->getChildByTag(eSklScore);
    if (!skeleton_record) {
        skeleton_record = spine::SkeletonAnimation::createWithJsonFile("spine/menu.json", "spine/menu.atlas" ,1.02f);
        skeleton_record->setTag(eSklScore);
        skeleton_record->setPosition(btn_record->getPosition());
        btn_record->getParent()->addChild(skeleton_record, 1);
    }
    skeleton_record->clearTracks();
    skeleton_record->setAnimation(0, "recording_stand", true);
    
    btn_play = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_play_0"));
    btn_play->setActionTag(eBtnDummy);
    btn_play->addTouchEventListener(CC_CALLBACK_2(PageInteract::touchEvent, this));
    btn_play = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_play"));
    btn_play->setActionTag(eBtnInterPlay);
    btn_play->addTouchEventListener(CC_CALLBACK_2(PageInteract::touchEvent, this));
    
    skeleton_play = (SkeletonAnimation*)btn_play->getParent()->getChildByTag(eSklScore);
    if (!skeleton_play) {
        skeleton_play = spine::SkeletonAnimation::createWithJsonFile("spine/menu.json", "spine/menu.atlas" ,1.02f);
        skeleton_play->setTag(eSklScore);
        skeleton_play->setPosition(btn_play->getPosition());
        btn_play->getParent()->addChild(skeleton_play, 1);
    }
    skeleton_play->clearTracks();
    skeleton_play->setAnimation(0, "play_stand", true);
    
    btn_replay = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_replay_0"));
    btn_replay->setActionTag(eBtnDummy);
    btn_replay->addTouchEventListener(CC_CALLBACK_2(PageInteract::touchEvent, this));
    btn_replay = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_replay"));
    btn_replay->setActionTag(eBtnInterListen);
    btn_replay->addTouchEventListener(CC_CALLBACK_2(PageInteract::touchEvent, this));
    
    skeleton_replay = (SkeletonAnimation*)btn_replay->getParent()->getChildByTag(eSklScore);
    if (!skeleton_replay) {
        skeleton_replay = spine::SkeletonAnimation::createWithJsonFile("spine/menu.json", "spine/menu.atlas" ,1.02f);
        skeleton_replay->setTag(eSklScore);
        skeleton_replay->setPosition(btn_replay->getPosition());
        btn_replay->getParent()->addChild(skeleton_replay, 1);
    }
    skeleton_replay->clearTracks();
    skeleton_replay->setAnimation(0, "playback_stand", true);
    
    img_inter_yun = static_cast<ImageView*>(Helper::seekWidgetByName(m_pageWidget, "img_inter_yun"));
    
    bmf_score = static_cast<TextBMFont*>(Helper::seekWidgetByName(pageWidget, "bmf_score"));
    bmf_anim = static_cast<TextBMFont*>(Helper::seekWidgetByName(pageWidget, "bmf_anim"));
    img_inter_scorebg = static_cast<ImageView*>(Helper::seekWidgetByName(pageWidget, "img_inter_scorebg"));
    
    SkeletonAnimation* skeleton_score = (SkeletonAnimation*)panel_menu->getChildByTag(eSklScore);
    if (!skeleton_score) {
        skeleton_score = spine::SkeletonAnimation::createWithJsonFile("spine/grade_action.json", "spine/grade_action.atlas" ,1.02f);
        skeleton_score->setTag(eSklScore);
        skeleton_score->setPosition(bmf_anim->getPosition() - Vec2(0, 10));
        panel_menu->addChild(skeleton_score, 1);
    }
    skeleton_score->setVisible(false);
    
    img_score_bg = static_cast<ImageView*>(Helper::seekWidgetByName(m_pageWidget, "img_score_bg"));
    
    //add by hyp   for  sound
    m_soundSlider = Slider::create();
    m_soundLabel =  Label::createWithSystemFont("1.0", MYDEF_TTF_CHINESE, 20);
    RecordPageInteract::addSoundSlider(pageWidget,m_soundLabel,m_soundSlider);
    m_soundSlider->addEventListener(CC_CALLBACK_2(PageInteract::soundSliderEvent, this));
}
void PageInteract::soundSliderEvent(Ref *sender, Slider::EventType type)
{
    RecordPageInteract::addSoundSliderEvent(sender,type,m_soundLabel,m_soundID);
}
void PageInteract::updateSoundSlider(Ref *sender)
{
    //add by hyp 刷新页面的声音速度
    RecordPageInteract::updateSoundSlider(sender,m_soundLabel,m_soundSlider);
}
void PageInteract::touchEvent(Ref *sender, Widget::TouchEventType type)
{
    CLICK_FILTER(sender, type);
    if(!BOOL_BUT_TOUCH_ENABLE())
    {
        return;// add by hyp 2018/11/20
    }
    ButtonType btnType = (ButtonType)(dynamic_cast<Widget*>(sender))->getActionTag();
    CFileLog::LogSaveFormat("===PageInteract::touchEvent==%d", (int)btnType);
    switch (btnType)
    {
        case eBtnBack:
        {
            
        }
            break;
        case eBtnHandle:
        {
            doInterAnimation();
        }
            break;
            
        case eBtnInterRecord:
        {
            Node* nodeBase = UiBase::getInstance()->getRunningScene();
            BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);
            if(bookBody && !bookBody->getCanTurn())
            {
                return;
            }
            if(bookBody && bookBody->getWaitRecognize())
            {
                return;
            }
            if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
            {
                BookDataManager::getInstance()->setReadMode(eReadNone);
                __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
                return;
            }
            if(getInterState() == eInterClose)
            {
                doInterAnimation();
                //return;
            }
            //检测正文页的播放状态
            if (m_wholeBook->m_mapPageViews.find(m_pageIndex) != m_wholeBook->m_mapPageViews.end())
            {
                PageCache* cache = m_wholeBook->m_mapPageViews[m_pageIndex];
                PageBase* curPage =  cache->m_pageUI;
                if (curPage->isAudioPlaying()) {
                    return;
                }
            }
            //屏蔽本页的播放状态
            if(this->isAudioPlaying())
            {
                return;
            }
            
            //            if (Director::getInstance()->getScheduler()->isScheduled(CC_SCHEDULE_SELECTOR(PageInteract::recordLimit), this)) {
            //                Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(PageInteract::recordLimit), this);
            //            }
            
            if (Recorder::getInstance()->isRecording())
            {
                Widget* tempBtn = dynamic_cast<Widget*>(sender);
                if (tempBtn) {
                    auto tempAction = tempBtn->getActionByTag(998);
                    if (tempAction) {
                        return;
                    } else {
                        tempAction = Sequence::create(DelayTime::create(0.5), CallFunc::create([=](){
                            Recorder::getInstance()->stopRecoder();
                        }), NULL);
                        tempAction->setTag(998);
                        tempBtn->runAction(tempAction);
                    }
                } else {
                    Recorder::getInstance()->stopRecoder();
                }
            }
            else
            {
                //清除本题打分记录
                if (m_wholeBook->m_mapRecordResult.find(m_pageIndex) != m_wholeBook->m_mapRecordResult.end())
                {
                    m_wholeBook->m_mapRecordResult.erase(m_pageIndex);
                }
                myText->resetColor();
                
                //原版书录音资源（bookid/index/record.spx）
                string recordPath = FileUtils::getInstance()->getWritablePath() + "net_book/" + m_wholeBook->m_bookData->id;
                recordPath += StringUtils::format("/%d/record.spx", m_pageIndex);
                
                Recorder::getInstance()->startRecoder(recordPath);
                
                //自动倒计时停止录音
                //                if (fRecordLimit != -1.0f) {
                //                    Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(PageInteract::recordLimit), this, fRecordLimit, 0, 0, false);
                //                }
            }
            talkingInterface::traceEvent("原版阅读交互按钮", "录音");
        }
            break;
            
        case eBtnInterPlay:
        {
            Node* nodeBase = UiBase::getInstance()->getRunningScene();
            BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);
            if(bookBody && !bookBody->getCanTurn())
            {
                return;
            }
            if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
            {
                BookDataManager::getInstance()->setReadMode(eReadNone);
                __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
                return;
            }
            WordFind *diction = (WordFind*)this->getPageWidget()->getChildByTag(eLayerBookDiction);
            if(diction)
            {
                return;
            }
            //正文播放屏蔽
            if (m_wholeBook->m_mapPageViews.find(m_pageIndex) != m_wholeBook->m_mapPageViews.end())
            {
                PageCache* cache = m_wholeBook->m_mapPageViews[m_pageIndex];
                PageBase* curPage =  cache->m_pageUI;
                if (curPage->isAudioPlaying()) {
                    curPage->stopAudio();
                    return;
                }
            }
            
            if(getInterState() == eInterClose)
            {
                doInterAnimation();
                //return;
            }
            //录音时禁止播放
            if(Recorder::getInstance()->isRecording())
            {
                return;
            }
            //停止回放
            if(Recorder::getInstance()->isPlayRecording())
            {
                Recorder::getInstance()->stopPlayRecord();
            }
            
            if(m_soundID != AudioEngine::INVALID_AUDIO_ID)
            {
                SoundManager::getInstance()->stopSound(m_soundID);
                m_soundID = AudioEngine::INVALID_AUDIO_ID;
            }
            else
            {
                myText->resetColor();
                playAudio();
            }
            talkingInterface::traceEvent("原版阅读交互按钮", "播放");
        }
            break;
            
        case eBtnInterListen:
        {
            Node* nodeBase = UiBase::getInstance()->getRunningScene();
            BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);
            if(bookBody && !bookBody->getCanTurn())
            {
                return;
            }
            if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
            {
                BookDataManager::getInstance()->setReadMode(eReadNone);
                __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
                return;
            }
            if (m_wholeBook->m_mapRecordPath.find(m_pageIndex) != m_wholeBook->m_mapRecordPath.end()
                && !Recorder::getInstance()->isRecording()//录音时屏蔽
                && m_soundID == AudioEngine::INVALID_AUDIO_ID)//播音时屏蔽
            {
                if(Recorder::getInstance()->isPlayRecording())
                {
                    Recorder::getInstance()->stopPlayRecord();
                }
                else
                {
                    Recorder::getInstance()->playRecoder(m_wholeBook->m_mapRecordPath[m_pageIndex]);
                    setBtnState(true, 2);
                    startWheelAnim(2);
                }
            }
            talkingInterface::traceEvent("原版阅读交互按钮", "回放");
        }
            break;
        case eBtnDummy:
        {
            SoundManager::getInstance()->playSoundEffect("music/snd_invalid.mp3");
        }
            break;
        default:
            break;
    }
}

void PageInteract::update(float dt)
{
    PageBase::update(dt);
}

void PageInteract::loadPage(WholeBook &wholeBook, int index)
{
    if(m_soundID != AudioEngine::INVALID_AUDIO_ID)
    {
        SoundManager::getInstance()->stopSound(m_soundID);
    }
    if(Recorder::getInstance()->isPlayRecording())
    {
        Recorder::getInstance()->stopPlayRecord();
    }
    
    PageBase::loadPage(wholeBook, index);
    
    //wholeBook.downAll(index);
    
    OnePage* onePage = m_wholeBook->m_bookPages->at(index);
    
    //有该页的录音记录，使能回放按钮
    if (m_wholeBook->m_mapRecordPath.find(m_pageIndex) != m_wholeBook->m_mapRecordPath.end())
    {
        setBtnState(2, true);
    }
    else
    {
        setBtnState(2, false);
    }
    if(m_wholeBook->haveSnd(m_pageIndex) && FileUtils::getInstance()->isFileExist(m_wholeBook->getSoundPath(m_pageIndex)))
    {
        setBtnState(1, true);
    }
    else
    {
        setBtnState(1, false);
    }
    if(m_wholeBook->haveDat(m_pageIndex) && FileUtils::getInstance()->isFileExist(m_wholeBook->getXmlPath(m_pageIndex)))
    {
        setBtnState(0, true);
    }
    else
    {
        setBtnState(0, false);
    }
    
    //解析录音时限，格式 "12:34"
    string recoring_time_limit = onePage->m_paramJson.getValue("recoring_time_limit");
    if (recoring_time_limit == "-1" || recoring_time_limit == "") {
        fRecordLimit = -1.0f;
    } else {
        int iMinutes = atoi(recoring_time_limit.substr(0, 2).data());
        int iSeconds = atoi(recoring_time_limit.substr(3).data());
        fRecordLimit = iMinutes * 60 + iSeconds;
    }
    
    Widget* pageWidget = getPageWidget();
    myText = (MyText*)txt_content->getChildByTag(eTxtQuiz);
    if(!myText)
    {
        myText = MyText::create("", Color3B(161,94,49), txt_content->getFontSize(), txt_content->getContentSize());
        myText->setTag(eTxtQuiz);
        myText->setName("CMyLabel");
        myText->setFontFile(txt_content->getFontName());
        myText->setPosition(Vec2::ZERO);
        txt_content->addChild(myText);
    }
    myText->setSizeDefault(txt_content->getFontSize());
    //加载本页资源
    if (onePage->m_pdfContent != "-1")
    {
        string strPath = m_wholeBook->getXmlPath(index);
        string xmlString = FileUtils::getInstance()->getStringFromFile(strPath);
        //设置内容
        myText->setXmlString(xmlString);
        //设置完string即能知道句子的起止时间，表示句子播放的上限
        PageScore* pageScore = m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore;
        pageScore->m_modelTime = myText->getTotalSecs();
    }
    
    auto pTouchCall = [this](int iWord, std::string sWord, int iBegin)
    {
        ILog(iWord)
        SLog(sWord.c_str())
        if(getPageWidget()->getChildByTag(eLayerBookDiction))
        {
            return;
        }
        if (Recorder::getInstance()->isRecording())
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
            Recorder::getInstance()->cancelRecoder();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            Recorder::getInstance()->stopRecoder();
            //Android cancel不完整录音
            Node* nodeBase = UiBase::getInstance()->getRunningScene();
            BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);
            if(bookBody)
                bookBody->setRecordCanceled(true);
#endif
            
        }
        if(Recorder::getInstance()->isPlayRecording())
        {
            Recorder::getInstance()->stopPlayRecord();
        }
        
        if(m_soundID != AudioEngine::INVALID_AUDIO_ID)
        {
            SoundManager::getInstance()->stopSound(m_soundID);
            m_soundID = AudioEngine::INVALID_AUDIO_ID;
        }
        
        if (iWord >= 0) {
            if (iWord == iBegin) {
                WordFind::create(this->getPageWidget(), 0, sWord);
                
                //                string oggFile = this->m_wholeBook->getSoundPath(this->m_pageIndex);
                //                WordAttr attr = this->myText->getWordAttr(iWord);
                //                p->setVoice(oggFile, attr.w_voiceBegin, attr.w_voiceEnd - attr.w_voiceBegin);
            } else {
                string oggFile = this->m_wholeBook->getSoundPath(this->m_pageIndex);
                WordAttr attr = this->myText->getWordAttr(iWord);
                WordAttr attr1 = this->myText->getWordAttr(iBegin);
                if (FileUtils::getInstance()->isFileExist(oggFile.c_str())) {
                    float speed = BookDataManager::getInstance()->getBookSoundSpeed();
                    m_soundID = SoundManager::getInstance()->playSoundNew(oggFile, false,speed,  [this](int, const std::string){
                        m_soundID = AudioEngine::INVALID_AUDIO_ID;
                        __NotificationCenter::getInstance()->postNotification(MSG_PAGE_AUTOPLAY, (Ref*)false);
                        m_soundSlider->setVisible(SoundManager::supportSpeedChange());
                    }, attr1.w_voiceBegin,(attr.w_voiceEnd - attr1.w_voiceBegin)/speed);
                    __NotificationCenter::getInstance()->postNotification(MSG_PAGE_AUTOPLAY, (Ref*)true);
                    m_soundSlider->setVisible(SoundManager::isIOS()&&SoundManager::supportSpeedChange());
                }
            }
        }
    };
    myText->setTouchListener(pTouchCall);
    
    float fSpace = img_inter_yun->getContentSize().height - txt_content->getPositionY();
    Size nodeSize = UiBase::getInstance()->getRunningScene()->getContentSize();
    SFLog("结点高度", nodeSize.height)
    SFLog("原本高度", myText->getContentSize().height)
    SFLog("菜单高度", panel_menu->getContentSize().height)
    int defaultSize = 43;
    while(myText->getContentSize().height
          + fSpace * 1.2f//拱形空白区域
          + panel_menu->getContentSize().height//菜单高度
          > nodeSize.height - 130)
    {
        defaultSize -= 2;
        myText->setSizeDefault(defaultSize);
        SFLog("逐渐缩小字体", myText->getContentSize().height)
    }
    
    m_viewHeight = myText->getContentSize().height + fSpace * 1.2 + panel_menu->getContentSize().height+30.0f;
    SILog("整体高度", m_viewHeight)
    img_inter_yun->setPosition(Vec2::ZERO);
    
    bmf_score->setString(StringUtils::format("%d", onePage->m_pageScore->m_dummy));
    if(bmf_score->getString() != "0")
    {
        img_score_bg->setVisible(true);
    }
}

void PageInteract::freePage()
{
    PageBase::freePage();
}

void PageInteract::showOralResult()
{
    if (m_wholeBook->m_mapRecordResult.find(m_pageIndex) == m_wholeBook->m_mapRecordResult.end())
    {
        return;
    }
    
    PageScore* pageScore = m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore;
    
    if(pageScore->m_dummy > 0)
    {
        Vec2 target = Vec2(img_inter_scorebg->getPositionX() ,m_viewHeight - img_inter_scorebg->getContentSize().height * 0.7f + 10);
        
        SkeletonAnimation* skeleton_score = (SkeletonAnimation*)panel_menu->getChildByTag(eSklScore);
        
        float fTimeScale = 1.0f;
        bmf_anim->setString(StringUtils::format("%d", pageScore->m_dummy));
        bmf_anim->runAction(Sequence::create(Place::create(m_pageWidget->getContentSize()/2)
                                             ,CallFunc::create([=](){
            skeleton_score->setVisible(true);
            skeleton_score->clearTracks();
            skeleton_score->setTimeScale(fTimeScale);
            skeleton_score->setAnimation(0, "all", false);
            skeleton_score->runAction(Sequence::create(Place::create(Vec2(m_pageWidget->getContentSize()/2)), DelayTime::create(3.25f/ fTimeScale),MoveTo::create(0.3f / fTimeScale, target), NULL));
        })
                                             ,Spawn::create(FadeIn::create(0.05f/ fTimeScale), ScaleTo::create(0.05f/ fTimeScale, 0.1f), NULL)
                                             ,EaseElasticOut::create(ScaleTo::create(0.3f/ fTimeScale, 0.8f))
                                             ,DelayTime::create(1.9f/ fTimeScale)
                                             ,EaseElasticInOut::create(ScaleTo::create(0.9f/ fTimeScale, 0.4f))
                                             ,DelayTime::create(0.1f/ fTimeScale)
                                             ,MoveTo::create(0.3f / fTimeScale, target)
                                             
                                             ,Spawn::create(FadeOut::create(0.1f / fTimeScale), ScaleTo::create(0.1f / fTimeScale, 0.1f), NULL)
                                             ,CallFunc::create([=](){bmf_score->setString(bmf_anim->getString());
            if(bmf_score->getString() != "0")
            {
                img_score_bg->setVisible(true);
            }
            skeleton_score->setVisible(false);})
                                             , NULL));
    }
    
    //设置错误单词
    if (myText) {
        myText->setErrorIds(pageScore->m_errWords);
    }
}

int PageInteract::playAudio(std::function<void (int, const std::string &)> finishCall)
{
    PageBase::playAudio(finishCall);
    m_soundID = AudioEngine::INVALID_AUDIO_ID;
    //Book页的句子控件
    MyText* myText = (MyText*)txt_content->getChildByTag(eTxtQuiz);
    
    //音效路径
    string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
    
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        setListenTime(true);//开始录音
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        float speed = BookDataManager::getInstance()->getBookSoundSpeed();
        setBtnState(true, 1);
        startWheelAnim(0);
        if(!finishCall)
        {
            m_soundID = SoundManager::getInstance()->playSoundNew(strPathSnd, false,speed, [=](int, const std::string){
                setListenTime(false);
                setBtnState(false, 1);
                m_soundID = AudioEngine::INVALID_AUDIO_ID;
                stopWheelAnim(0);
                myText->resetColor();
                m_soundSlider->setVisible(SoundManager::supportSpeedChange());
            });
            m_soundSlider->setVisible(SoundManager::isIOS()&&SoundManager::supportSpeedChange());
        }
        else
        {
            m_soundID = SoundManager::getInstance()->playSoundNew(strPathSnd, false,speed, finishCall);
        }
    }
    //歌词路径
    string strPathXml = m_wholeBook->getXmlPath(m_pageIndex);
    if (FileUtils::getInstance()->isFileExist(strPathXml.c_str())) {
        myText->showXml(Color3B(65,189,140));
    }
    return m_soundID;
}

void PageInteract::stopAudio()
{
    //音效路径
    string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
    if (m_soundID != AudioEngine::INVALID_AUDIO_ID && FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        SoundManager::getInstance()->stopSound(m_soundID);
    }
    PageBase::stopAudio();
}

//void PageInteract::recordLimit(float dt)
//{
//    Recorder::getInstance()->stopRecoder();
//}

void PageInteract::onInterAnimEnded(cocos2d::Node *sender, eInterState state)
{
    m_interState = state;
    if(state == eInterClose)
    {
        //如果是正在录音，停止录音相关
        //        if (Director::getInstance()->getScheduler()->isScheduled(CC_SCHEDULE_SELECTOR(PageInteract::recordLimit), this)) {
        //            Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(PageInteract::recordLimit), this);
        //        }
        if (Recorder::getInstance()->isRecording())
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
            Recorder::getInstance()->cancelRecoder();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            Recorder::getInstance()->stopRecoder();
            //Android cancel不完整录音
            Node* nodeBase = UiBase::getInstance()->getRunningScene();
            BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);
            if(bookBody)
                bookBody->setRecordCanceled(true);
#endif
            
        }
        if(m_soundID != AudioEngine::INVALID_AUDIO_ID)
        {
            SoundManager::getInstance()->stopSound(m_soundID);
            m_soundID = AudioEngine::INVALID_AUDIO_ID;
        }
        if(Recorder::getInstance()->isPlayRecording())
        {
            Recorder::getInstance()->stopPlayRecord();
        }
        //停止播放范读
        PageBase::playAudio();
    }
    else if (state == eInterShow)
    {
        SFLog("面板坐标2", this->getPositionY());
        //if(!NewHandTip::isShow("PageInteract1"))
        //{}else
        if(!NewHandTip::isShow("PageInteract2"))
        {
            NewHandTip::setShow("PageInteract2");
            auto node = NewHandTip::create2(UiBase::getInstance()->getRunningScene());
            node->addImg("newhand_interactPage1.png", myText, Vec2(30, -30));
        }
        else if(!NewHandTip::isShow("PageInteract3") && myText && myText->getWordCount() > 10)
        {
            NewHandTip::setShow("PageInteract3");
            auto node = NewHandTip::create2(UiBase::getInstance()->getRunningScene());
            node->addImg("newhand_interactPage2.png", myText, Vec2(0, myText->getContentSize().height / 2 - 198));
            node->addImgAndClip("", myText, Vec2::ZERO, Vec2(0, myText->getContentSize().height / 2 - 20), Size(620,70));
        }
    }
    
}

void PageInteract::doInterAnimation()
{
    //呼出交互面板，停止自动播放
    if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
    {
        BookDataManager::getInstance()->setReadMode(eReadNone);
        __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
    }
    if(img_inter_yun->getActionByTag(123) != nullptr)//动画进行中，排除干扰
    {
        SLog("交互动画拦截")
        return;
    }
    
    Size nodeSize = UiBase::getInstance()->getRunningScene()->getContentSize();
    MoveTo* pMove = nullptr;
    CallFuncN *pCallback = nullptr;
    if (img_inter_yun->getPositionY() > 0) {
        pMove = MoveTo::create(0.5f, Vec2(img_inter_yun->getPositionX(), 0));
        pCallback = CallFuncN::create(CC_CALLBACK_1(PageInteract::onInterAnimEnded,this,eInterClose));
        SLog("交互动画Down")
    } else {
        SLog("交互动画Up")
        //SILog("可视高度", m_viewHeight);
        //SFLog("面板坐标", img_inter_yun->getPositionY());
        pMove = MoveTo::create(0.5f, Vec2(img_inter_yun->getPositionX(), m_viewHeight));
        pCallback = CallFuncN::create(CC_CALLBACK_1(PageInteract::onInterAnimEnded,this,eInterShow));
        this->runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create([this](){
            //myText->resetColor();
        }), NULL));
    }
    
    Sequence* pSequence = Sequence::create(pMove, pCallback, nullptr);
    pSequence->setTag(123);
    img_inter_yun->runAction(pSequence);
}

void PageInteract::startWheelAnim(int iTag)
{
    ImageView* img_gear = iTag == 0 ? img_gear_fg0 : (iTag == 1 ? img_gear_fg1 : img_gear_fg2);
    img_gear->runAction(RepeatForever::create(RotateBy::create(3.0, 360)));
    
    SkeletonAnimation* sklAnim = iTag == 0 ? skeleton_play : (iTag == 1 ? skeleton_record : skeleton_replay);
    string strClick = iTag == 0 ? "play_click" : (iTag == 1 ? "recording_click" : "playback_click");
    string strRotate = iTag == 0 ? "play_rotate" : (iTag == 1 ? "recording_rotate" : "playback_rotate");
    
    sklAnim->clearTracks();
    sklAnim->setAnimation(0, strClick, false);
    sklAnim->setCompleteListener([=] (spTrackEntry* entry){
        sklAnim->setAnimation(0, strRotate, true);
    });
}
void PageInteract::stopWheelAnim(int iTag, bool bManue)
{
    ImageView* img_gear = iTag == 0 ? img_gear_fg0 : (iTag == 1 ? img_gear_fg1 : img_gear_fg2);
    img_gear->stopAllActions();
    SkeletonAnimation* sklAnim = iTag == 0 ? skeleton_play : (iTag == 1 ? skeleton_record : skeleton_replay);
    if(!bManue)
    {
        sklAnim->clearTracks();
    }
    else
    {
        string strClick = iTag == 0 ? "play_click" : (iTag == 1 ? "recording_click" : "playback_click");
        sklAnim->setAnimation(0, strClick, false);
    }
    
}

void PageInteract::setBtnState(bool bStart, int iType)
{
    bool bHasRecord = false;
    //若是有录音记录，激活回放按钮
    if(m_pageIndex > -1 && m_wholeBook->m_mapRecordPath.find(m_pageIndex) != m_wholeBook->m_mapRecordPath.end())
    {
        bHasRecord = true;
    }
    
    if(iType == 0 || iType == 1)
    {
        Node* layout_parent = btn_replay->getParent();
        if (bStart || !bHasRecord) {
            btn_replay->setTouchEnabled(false);
            layout_parent->setColor(Color3B::GRAY);
            skeleton_replay->setColor(Color3B::GRAY);
        } else {
            btn_replay->setTouchEnabled(true);
            layout_parent->setColor(Color3B::WHITE);
            skeleton_replay->setColor(Color3B::WHITE);
            skeleton_replay->clearTracks();
        }
    }
    if(iType == 1 || iType == 2)
    {
        Node* layout_parent = btn_record->getParent();
        if (bStart) {
            btn_record->setTouchEnabled(false);
            layout_parent->setColor(Color3B::GRAY);
            skeleton_record->setColor(Color3B::GRAY);
        } else {
            btn_record->setTouchEnabled(true);
            layout_parent->setColor(Color3B::WHITE);
            skeleton_record->setColor(Color3B::WHITE);
            skeleton_record->clearTracks();
        }
    }
    if(iType == 0 || iType == 2)
    {
        Node* layout_parent = btn_play->getParent();
        if (bStart) {
            btn_play->setTouchEnabled(false);
            layout_parent->setColor(Color3B::GRAY);
            skeleton_play->setColor(Color3B::GRAY);
        } else {
            btn_play->setTouchEnabled(true);
            layout_parent->setColor(Color3B::WHITE);
            skeleton_play->setColor(Color3B::WHITE);
            skeleton_play->clearTracks();
        }
    }
}

void PageInteract::setBtnState(int iType, bool bEnabled)
{
    Button* btnTmp = nullptr;
    SkeletonAnimation* sklTmp = nullptr;
    if (iType == 0) {
        btnTmp = btn_record;
        sklTmp = skeleton_record;
    } else if (iType == 1) {
        btnTmp = btn_play;
        sklTmp = skeleton_play;
    } else if (iType == 2) {
        btnTmp = btn_replay;
        sklTmp = skeleton_replay;
    }
    
    if (btnTmp) {
        Node* layout_parent = btnTmp->getParent();
        if (!bEnabled) {
            btnTmp->setTouchEnabled(false);
            layout_parent->setColor(Color3B::GRAY);
            sklTmp->clearTracks();
        } else {
            btnTmp->setTouchEnabled(true);
            layout_parent->setColor(Color3B::WHITE);
        }
    }
}

void PageInteract::onRecordTimeOut()
{
    if(Recorder::getInstance()->isRecording())
    {
        Recorder::getInstance()->stopRecoder();
    }
}

void PageInteract::setPageAutoPlay(Ref* isPlaying)
{
    bool bEnable = ((intptr_t)isPlaying == 1);
    if(bEnable)
    {
        setBtnState(true, 1);
        startWheelAnim(0);
    }
    else
    {
        setBtnState(false, 1);
        stopWheelAnim(0);
    }
}

void PageInteract::stopInterAnim()
{
    img_inter_yun->stopActionByTag(123);
    img_inter_yun->setPositionY(0);
    onInterAnimEnded(this, eInterClose);
}
