//
//  PageQuizOral.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#include "PageQuizOral.hpp"
#include "TipManager.hpp"
#include "BookBody.hpp"
#include "Recorder.h"
#include "NewHandTip.hpp"
#include "RecordPageInteract.hpp"
#include "FileLog.h"

PageQuizOral::PageQuizOral()
{
    DownImg::getInstance()->registCallback(this);
    m_pageSize = UiBase::getInstance()->getRunningScene()->getContentSize();
//    _fMin = DataTableManager::getInstance()->getValue(eTable_config_setting, str_config_setting_Ps_Earth_Min, 0)->getFloat();
//    _fMax = DataTableManager::getInstance()->getValue(eTable_config_setting, str_config_setting_Ps_Earth_Max, 0)->getFloat();
//    _fTime = DataTableManager::getInstance()->getValue(eTable_config_setting, str_config_setting_Add_Nuts_Txt, 0)->getFloat();
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PageQuizOral::updateSoundSlider), MSG_SET_SOUND_SPEED, nullptr);
}

PageQuizOral::~PageQuizOral()
{
    __NotificationCenter::getInstance()->removeObserver(this, MSG_SET_SOUND_SPEED);
    DownImg::getInstance()->unregistCallback(this);
}

void PageQuizOral::onEnter()
{
    PageQuizBase::onEnter();
    scheduleUpdate();
}

void PageQuizOral::onExit()
{
    unscheduleUpdate();
    PageQuizBase::onExit();
}

PageQuizOral* PageQuizOral::create()
{
    PageQuizOral *ret = new (std::nothrow) PageQuizOral();
    auto mUI = CSLoader::createNode("ui/book/BookV2/ReadOral.csb");
    Widget* pageUI = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_quiz_oral");
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

PageQuizOral* PageQuizOral::create(Widget* pageUI)
{
    PageQuizOral *ret = new (std::nothrow) PageQuizOral();
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

bool PageQuizOral::init(Widget* pageUI)
{
    if (!PageQuizBase::init(pageUI))
    {
        return false;
    }
    
    this->load(pageUI);
    
    return true;
}

void PageQuizOral::addEvents()
{
    Widget* pageWidget = getPageWidget();
    txt_content = (Text*)pageWidget->getChildByName("txt_content");
    txt_tip = static_cast<Text*>(Helper::seekWidgetByName(pageWidget, "txt_tip"));
    
    myText = (MyText*)txt_content->getChildByTag(eTxtQuiz);
    if (!myText) {
        myText = MyText::create("", Color3B(161,94,49), txt_content->getFontSize(), txt_content->getContentSize());
        myText->setTag(eTxtQuiz);
        myText->setFontFile(txt_content->getFontName());
        myText->setPosition(Vec2::ZERO);
        txt_content->addChild(myText);
    }
    
    //交互模式下面的三个按钮
    panel_menu = (Widget*)pageWidget->getChildByName("panel_menu");
    img_gear_fg0 = static_cast<ImageView*>(Helper::seekWidgetByName(panel_menu, "img_gear_fg0"));
    img_gear_fg1 = static_cast<ImageView*>(Helper::seekWidgetByName(panel_menu, "img_gear_fg1"));
    img_gear_fg2 = static_cast<ImageView*>(Helper::seekWidgetByName(panel_menu, "img_gear_fg2"));
    
    //quiz专用-底部录音
    btn_record = static_cast<Button*>(Helper::seekWidgetByName(pageWidget, "btn_record_0"));
    btn_record->setActionTag(eBtnDummy);
    btn_record->addTouchEventListener(CC_CALLBACK_2(PageQuizOral::touchEvent, this));
    btn_record = static_cast<Button*>(Helper::seekWidgetByName(pageWidget, "btn_record"));
    btn_record->setActionTag(eBtnQuizRecord);
    btn_record->addTouchEventListener(CC_CALLBACK_2(PageQuizOral::touchEvent, this));
    
    skeleton_record = (SkeletonAnimation*)btn_record->getParent()->getChildByTag(eSklScore);
    if (!skeleton_record) {
        skeleton_record = spine::SkeletonAnimation::createWithJsonFile("spine/menu.json", "spine/menu.atlas" ,1.02f);
        skeleton_record->setTag(eSklScore);
        skeleton_record->setPosition(btn_record->getPosition());
        btn_record->getParent()->addChild(skeleton_record, 1);
    }
    skeleton_record->clearTracks();
    skeleton_record->setAnimation(0, "recording_stand", true);
    
    //quiz专用-播放录音
    btn_play = static_cast<Button*>(Helper::seekWidgetByName(pageWidget, "btn_play_0"));
    btn_play->setActionTag(eBtnDummy);
    btn_play->addTouchEventListener(CC_CALLBACK_2(PageQuizOral::touchEvent, this));
    btn_play = static_cast<Button*>(Helper::seekWidgetByName(pageWidget, "btn_play"));
    btn_play->setActionTag(eBtnQuizPlay);
    btn_play->addTouchEventListener(CC_CALLBACK_2(PageQuizOral::touchEvent, this));
    btn_play->setTouchEnabled(false);
    btn_play->setColor(Color3B::GRAY);
    
    skeleton_play = (SkeletonAnimation*)btn_play->getParent()->getChildByTag(eSklScore);
    if (!skeleton_play) {
        skeleton_play = spine::SkeletonAnimation::createWithJsonFile("spine/menu.json", "spine/menu.atlas" ,1.02f);
        skeleton_play->setTag(eSklScore);
        skeleton_play->setPosition(btn_play->getPosition());
        btn_play->getParent()->addChild(skeleton_play, 1);
    }
    skeleton_play->clearTracks();
    skeleton_play->setAnimation(0, "play_stand", true);
    
    //quiz专用-回放
    btn_replay = static_cast<Button*>(Helper::seekWidgetByName(pageWidget, "btn_replay_0"));
    btn_replay->setActionTag(eBtnDummy);
    btn_replay->addTouchEventListener(CC_CALLBACK_2(PageQuizOral::touchEvent, this));
    btn_replay = static_cast<Button*>(Helper::seekWidgetByName(pageWidget, "btn_replay"));
    btn_replay->setActionTag(eBtnQuizListen);
    btn_replay->addTouchEventListener(CC_CALLBACK_2(PageQuizOral::touchEvent, this));
    
    skeleton_replay = (SkeletonAnimation*)btn_replay->getParent()->getChildByTag(eSklScore);
    if (!skeleton_replay) {
        skeleton_replay = spine::SkeletonAnimation::createWithJsonFile("spine/menu.json", "spine/menu.atlas" ,1.02f);
        skeleton_replay->setTag(eSklScore);
        skeleton_replay->setPosition(btn_replay->getPosition());
        btn_replay->getParent()->addChild(skeleton_replay, 1);
    }
    skeleton_replay->clearTracks();
    skeleton_replay->setAnimation(0, "playback_stand", true);
    
    Button* btn_left = (Button*)pageWidget->getChildByName("btn_left");
    btn_left->setActionTag(eBtnLeft);
    btn_left->addTouchEventListener(CC_CALLBACK_2(PageQuizOral::touchEvent, this));
    
    Button* btn_right = (Button*)pageWidget->getChildByName("btn_right");
    btn_right->setActionTag(eBtnRight);
    btn_right->addTouchEventListener(CC_CALLBACK_2(PageQuizOral::touchEvent, this));
    btn_right->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 2), DelayTime::create(0.5), ScaleTo::create(1, 1), NULL)));
    
    bmf_score_0 = static_cast<TextBMFont*>(Helper::seekWidgetByName(pageWidget, "bmf_score_0"));
    bmf_score_1 = static_cast<TextBMFont*>(Helper::seekWidgetByName(pageWidget, "bmf_score_1"));
    bmf_score_2 = static_cast<TextBMFont*>(Helper::seekWidgetByName(pageWidget, "bmf_score_2"));
    bmf_anim = static_cast<TextBMFont*>(Helper::seekWidgetByName(pageWidget, "bmf_anim"));
    img_quiz_score = static_cast<ImageView*>(Helper::seekWidgetByName(pageWidget, "img_quiz_score"));
    
    SkeletonAnimation* skeleton_score = (SkeletonAnimation*)panel_menu->getChildByTag(eSklScore);
    if (!skeleton_score) {
        skeleton_score = spine::SkeletonAnimation::createWithJsonFile("spine/grade_action.json", "spine/grade_action.atlas" ,1.02f);
        skeleton_score->setTag(eSklScore);
        skeleton_score->setPosition(bmf_anim->getPosition() - Vec2(0, 10));
        panel_menu->addChild(skeleton_score, 1);
    }
    skeleton_score->setVisible(false);
    
    img_score_bg_0 = static_cast<ImageView*>(Helper::seekWidgetByName(m_pageWidget, "img_score_bg_0"));
    img_score_bg_1 = static_cast<ImageView*>(Helper::seekWidgetByName(m_pageWidget, "img_score_bg_1"));
    img_score_bg_2 = static_cast<ImageView*>(Helper::seekWidgetByName(m_pageWidget, "img_score_bg_2"));
    
    
    //add by hyp   for  sound
    m_soundSlider = Slider::create();
    m_soundLabel =  Label::createWithSystemFont("1.0", MYDEF_TTF_CHINESE, 20);
    RecordPageInteract::addSoundSlider(pageWidget,m_soundLabel,m_soundSlider);
    m_soundSlider->addEventListener(CC_CALLBACK_2(PageQuizOral::soundSliderEvent, this));
}
void PageQuizOral::soundSliderEvent(Ref *sender, Slider::EventType type)
{
    RecordPageInteract::addSoundSliderEvent(sender,type,m_soundLabel,m_soundID);
}
void PageQuizOral::updateSoundSlider(Ref *sender)
{
    //add by hyp 刷新页面的声音速度
    RecordPageInteract::updateSoundSlider(sender,m_soundLabel,m_soundSlider);
}
void PageQuizOral::touchEvent(Ref *sender, Widget::TouchEventType type)
{
    CLICK_FILTER(sender, type);
    ButtonType btnType = (ButtonType)(dynamic_cast<Widget*>(sender))->getActionTag();
    CFileLog::LogSaveFormat("===PageQuizOral::touchEvent==%d", (int)btnType);
    switch (btnType)
    {
        case eBtnBack:
        {
            
        }
            break;
        case eBtnQuizPlay:
        {
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
                CCLOG("=====================button stop oral, page = %d", m_pageIndex);
                SoundManager::getInstance()->stopSound(m_soundID);
            }
            else
            {
                CCLOG("=====================button play oral, page = %d", m_pageIndex);
                myText->resetColor();
                playAudio();
            }
        }
            break;
            
        case eBtnQuizListen:
        {
            this->playRecord();
        }
            break;
        case eBtnQuizRecord:
        {
            Node* nodeBase = UiBase::getInstance()->getRunningScene();
            BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);
            if(bookBody && bookBody->getWaitRecognize())
            {
                return;
            }
            this->startRecord();
        }
            break;
        case eBtnBroadcast:
        {
            this->playAudio();
        }
            break;
        case eBtnLeft:
        {
            BookBody::turnPreviousPage();
        }
            break;
        case eBtnRight:
        {
            BookBody::turnNextPage();
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

void PageQuizOral::update(float dt)
{
    PageBase::update(dt);
}

void PageQuizOral::loadPage(WholeBook &wholeBook, int index)
{
    PageBase::loadPage(wholeBook, index);
    
    OnePage* onePage = wholeBook.m_bookPages->at(index);
    m_pageIndex = index;
    
    txt_tip->setString(onePage->m_pageTitle);
    
    wholeBook.downAll(index);
}

void PageQuizOral::freePage()
{
    string strPath = m_wholeBook->getPicPath(m_pageIndex);
    
    if (strPath.length() > 0)
    {
        //释放贴图
        Director::getInstance()->getTextureCache()->removeTextureForKey(strPath);
    }
    PageBase::freePage();
}

void PageQuizOral::endDownload(string& url, string& file, int iTag, eBookResType resType)
{
#if DOWNLOAD_LOG
    SILog("下载结束", iTag)
    SSLog("下载文件", file.c_str())
    SILog("文件类型", resType)
#endif
    if (m_pageIndex == iTag) {
        if (resType == eBookResPng) {
        }
        else if (resType == eBookResSnd) {
            btn_play->setTouchEnabled(true);
            btn_play->setColor(Color3B::WHITE);
        }
        else if (resType == eBookResOth)
        {
            string xmlString = FileUtils::getInstance()->getStringFromFile(file);
            //设置内容
            myText->setXmlString(xmlString);
            
            //设置完string即能知道句子的起止时间，表示句子播放的上限
            PageScore* pageScore = m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore;
            pageScore->m_modelTime = myText->getTotalSecs();
            
            int defaultSize = 45;
            Size nodeSize = UiBase::getInstance()->getRunningScene()->getContentSize();
            while(myText->getContentSize().height
                  + panel_menu->getContentSize().height
                  + 300
                  > nodeSize.height)
            {
                defaultSize -= 2;
                myText->setSizeDefault(defaultSize);
                SFLog("逐渐缩小字体", myText->getContentSize().height)
            }
            myText->setPosition(Vec2(0,myText->getContentSize().height/2));
            //设置单词回调
            //            auto pTouchCall = [this](int iWord, std::string sWord)
            //            {
            //                ILog(iWord)
            //                SLog(sWord.c_str())
            //                if (iWord >= 0) {
            //                    //Widget* panel_root = (Widget*)getRootWidget();
            //                    //WordFind::create(panel_root);
            //                }
            //            };
            //            myText->setTouchListener(pTouchCall);
            //            if (m_mapRecordResult.find(index) != m_mapRecordResult.end())
            //            {
            //                showOralResult(index);
            //            }
        }
    }
}

int PageQuizOral::playAudio(std::function<void (int, const std::string &)> finishCall)
{
    PageQuizBase::playAudio(finishCall);
    m_soundID = AudioEngine::INVALID_AUDIO_ID;
    if(myText)
    {
        myText->resetColor();
    }
    //音效路径
    string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
    
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        setListenTime(true);//开始录音
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        startWheelAnim(0);
        setBtnState(true, 1);
        float speed = BookDataManager::getInstance()->getBookSoundSpeed();
        if(!finishCall)
        {
            CCLOG("=====================start play oral, page = %d", m_pageIndex);
            auto book = m_wholeBook;
            auto index = m_pageIndex;
            m_soundID = SoundManager::getInstance()->playSoundNew(strPathSnd, false,speed, [=](int, const std::string){
                CCLOG("=====================callback play oral, page = %d", index);
                auto p = book->m_mapPageViews.find(index);
                if (p != book->m_mapPageViews.end()) {
                    PageQuizOral* q = (PageQuizOral*)p->second->m_pageUI;
                    q->setListenTime(false);
                    q->m_soundID = AudioEngine::INVALID_AUDIO_ID;
                    q->stopWheelAnim(0);
                    q->setBtnState(false, 1);
                    q->myText->resetColor();
                } else {
                    CCLOG("=====================callback play oral but without page, page = %d", index);
                }
//                m_soundSlider->setVisible(SoundManager::supportSpeedChange());
            });
            bool flag1 = SoundManager::isIOS();
            bool flag2 = SoundManager::supportSpeedChange();
//            m_soundSlider->setVisible(flag1 && flag2);
        }
        else
        {
            CCLOG("=====================start play oral with othercallback, page = %d", m_pageIndex);
            m_soundID = SoundManager::getInstance()->playSoundNew(strPathSnd, false,speed, finishCall);
        }
        
        //歌词路径
        string strPathXml = m_wholeBook->getXmlPath(m_pageIndex);
        if (FileUtils::getInstance()->isFileExist(strPathXml.c_str())) {
            myText->showXml(Color3B(65,189,140));
        }
    }
    
    return m_soundID;
}

void PageQuizOral::stopAudio()
{
    //音效路径
    string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
    if (m_soundID != -1 && FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        CCLOG("=====================stop play oral, page = %d", m_pageIndex);
        SoundManager::getInstance()->stopSound(m_soundID);
    }
    PageBase::stopAudio();
}

void PageQuizOral::playRecord()
{
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
            startWheelAnim(2);
            setBtnState(true, 2);
        }
    }
}

void PageQuizOral::startRecord()
{
    if(m_soundID != AudioEngine::INVALID_AUDIO_ID)
    {
        return;
    }
    if (Recorder::getInstance()->isRecording())
    {
        Widget* tempBtn = btn_record;
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
        if(myText)
        {
            myText->resetColor();
        }
        //原版书录音资源（bookid/index/record.spx）
        string recordPath = FileUtils::getInstance()->getWritablePath() + "net_book/" + m_wholeBook->m_bookData->id;
        recordPath += StringUtils::format("/%d/record.spx", m_pageIndex);
        Recorder::getInstance()->startRecoder(recordPath);
    }
}

void PageQuizOral::showOralResult()
{
    if (m_wholeBook->m_mapRecordResult.find(m_pageIndex) == m_wholeBook->m_mapRecordResult.end())
    {
        return;
    }
    
    PageScore* pageScore = m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore;
    
    //坚果容器
    Widget* panel_nuts = (Widget*)getPageWidget()->getChildByName("panel_nuts");
    
    //5颗坚果
    
    if(pageScore->m_dummy > 0)
    {
        OnePage* onePage = m_wholeBook->m_bookPages->at(m_pageIndex);
        TextBMFont* bmf_tmp;
        ImageView* img_score_bg;
        if(onePage->m_pageTitle == "1/3"){
            bmf_tmp = bmf_score_0;
            img_score_bg = img_score_bg_0;
        } else if(onePage->m_pageTitle == "2/3"){
            bmf_tmp = bmf_score_1;
            img_score_bg = img_score_bg_1;
        } else {
            bmf_tmp = bmf_score_2;
            img_score_bg = img_score_bg_2;
        }
        
        Vec2 target = Vec2(m_pageWidget->getContentSize().width / 2 + bmf_tmp->getPositionX() - bmf_score_1->getPositionX(), img_quiz_score->getPositionY() - img_quiz_score->getContentSize().height*1.7f + 10);
        
        SkeletonAnimation* skeleton_score = (SkeletonAnimation*)panel_menu->getChildByTag(eSklScore);
        
        float fTimeScale = 1.2f;
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
                                             ,CallFunc::create([=](){
            bmf_tmp->setString(bmf_anim->getString());
            if (bmf_tmp->getString() != "0") {
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

void PageQuizOral::onTurnOut()
{
    setListenTime(false);
    PageBase::onTurnOut();
    setBtnState(false, 0);
    setBtnState(false, 2);
    setBtnState(false, 1);
    btn_play->setTouchEnabled(false);
}

void PageQuizOral::onTurnIn()
{
    PageBase::onTurnIn();
    string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str()))
    {
        btn_play->setTouchEnabled(true);
    }
    OnePage* onePage = m_wholeBook->m_bookPages->at(m_pageIndex);
    
    int iPage0;
    int iPage1;
    int iPage2;
    if(onePage->m_pageTitle == "1/3"){
        iPage0 = m_pageIndex;
        iPage1 = m_pageIndex + 1;
        iPage2 = m_pageIndex + 2;
    } else if(onePage->m_pageTitle == "2/3"){
        iPage0 = m_pageIndex - 1;
        iPage1 = m_pageIndex;
        iPage2 = m_pageIndex + 1;
    } else {
        iPage0 = m_pageIndex - 2;
        iPage1 = m_pageIndex - 1;
        iPage2 = m_pageIndex;
    }
    bmf_score_0->setString(StringUtils::format("%d", m_wholeBook->m_bookPages->at(iPage0)->m_pageScore->m_dummy));
    bmf_score_1->setString(StringUtils::format("%d", m_wholeBook->m_bookPages->at(iPage1)->m_pageScore->m_dummy));
    bmf_score_2->setString(StringUtils::format("%d", m_wholeBook->m_bookPages->at(iPage2)->m_pageScore->m_dummy));
    
    if (bmf_score_0->getString() != "0") {
        img_score_bg_0->setVisible(true);
    }
    if (bmf_score_1->getString() != "0") {
        img_score_bg_1->setVisible(true);
    }
    if (bmf_score_2->getString() != "0") {
        img_score_bg_2->setVisible(true);
    }
}


void PageQuizOral::startWheelAnim(int iTag)
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
void PageQuizOral::stopWheelAnim(int iTag)
{
    ImageView* img_gear = iTag == 0 ? img_gear_fg0 : (iTag == 1 ? img_gear_fg1 : img_gear_fg2);
    img_gear->stopAllActions();
    
    SkeletonAnimation* sklAnim = iTag == 0 ? skeleton_play : (iTag == 1 ? skeleton_record : skeleton_replay);
    sklAnim->clearTracks();
}

void PageQuizOral::setBtnState(bool bStart, int iType)
{
    bool bHasRecord = false;
    //若是有录音记录，激活回放按钮
    if(m_wholeBook->m_mapRecordPath.find(m_pageIndex) != m_wholeBook->m_mapRecordPath.end())
    {
        bHasRecord = true;
        
    }
    SILog("setBtnState", iType)
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

void PageQuizOral::setBtnState(int iType, bool bEnabled)
{
    Button* btnTmp = nullptr;
    SkeletonAnimation* skeleton_tmp;
    if (iType == 0) {
        btnTmp = btn_record;
        skeleton_tmp = skeleton_record;
    } else if (iType == 1) {
        btnTmp = btn_play;
        skeleton_tmp = skeleton_play;
    } else if (iType == 2) {
        btnTmp = btn_replay;
        skeleton_tmp = skeleton_replay;
    }
    
    if (btnTmp) {
        Node* layout_parent = btnTmp->getParent();
        if (!bEnabled) {
            btnTmp->setTouchEnabled(false);
            layout_parent->setColor(Color3B::GRAY);
            skeleton_tmp->clearTracks();
        } else {
            btnTmp->setTouchEnabled(true);
            layout_parent->setColor(Color3B::WHITE);
        }
    }
}

void PageQuizOral::stopRecordAndPlayback()//add by dj
{
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
    if (Recorder::getInstance()->isPlayRecording())
    {
        Recorder::getInstance()->stopPlayRecord();
    }
}
