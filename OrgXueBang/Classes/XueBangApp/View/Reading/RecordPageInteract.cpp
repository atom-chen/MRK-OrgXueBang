//
//  RecordPageInteract.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#include "RecordPageInteract.hpp"
#include "TipManager.hpp"
#include "Recorder.h"
#include "stdafx.h"
#include "WordFind.h"
//#include "mySpine.hpp"
#include "NewHandTip.hpp"
#include "RecordBookData.hpp"
#include "BookBody.hpp"
#include "UtilsDefine.h"
#include "FileLog.h"
#include "CostTime.hpp"
#include "ResourceManager.hpp"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

RecordPageInteract::RecordPageInteract()
{
    fRecordLimit = 180.0f;
    m_interState = eInterClose;
//    _fMin = DataTableManager::getInstance()->getValue(eTable_config_setting, str_config_setting_Ps_Earth_Min, 0)->getFloat();
//    _fMax = DataTableManager::getInstance()->getValue(eTable_config_setting, str_config_setting_Ps_Earth_Max, 0)->getFloat();
//    _fTime = DataTableManager::getInstance()->getValue(eTable_config_setting, str_config_setting_Add_Nuts_Txt, 0)->getFloat();
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RecordPageInteract::setPageAutoPlay), MSG_PAGE_AUTOPLAY, nullptr);
}

RecordPageInteract::~RecordPageInteract()
{
    __NotificationCenter::getInstance()->removeObserver(this, MSG_PAGE_AUTOPLAY);
}

void RecordPageInteract::onEnter()
{
    PageBase::onEnter();
    scheduleUpdate();
}

void RecordPageInteract::onExit()
{
    unscheduleUpdate();
    PageBase::onExit();
}

RecordPageInteract* RecordPageInteract::create(Widget* pageUI)
{
    RecordPageInteract *ret = new (std::nothrow) RecordPageInteract();
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

bool RecordPageInteract::init(Widget* pageUI)
{
    if (!PageBase::init())
    {
        return false;
    }
    this->setTag(eLayerBookInter);
    this->load("ui/book/layer_recordPage.csb");
    updateUIByMode();
    return true;
}
bool RecordPageInteract::load(const char* gameUIFile)
{
    COST_TIME_TAG(gameUIFile, "")
    Widget* gameUI  = ResourceManager::getInstance()->getUIFromJson(gameUIFile);
    m_rootWidget = gameUI;
    m_pageWidget = (Widget*)gameUI->getChildByName("panel_interact");
    if(gameUI->getParent())
    {
        return true;
    }
    this->addChild(gameUI,1);
    addEvents();
    COST_TIME_TAG(gameUIFile, "add event over !")
    return gameUI != NULL;
}
void RecordPageInteract::addEvents()
{
    Widget* pageWidget = getPageWidget();
    txt_content = static_cast<Text*>(Helper::seekWidgetByName(pageWidget, "txt_content"));
    
    //交互模式下面的三个按钮
    panel_menu = (Widget*)pageWidget->getChildByName("panel_menu");
    
    img_gear_fg0 = static_cast<ImageView*>(Helper::seekWidgetByName(panel_menu, "img_gear_fg0"));
    img_gear_fg1 = static_cast<ImageView*>(Helper::seekWidgetByName(panel_menu, "img_gear_fg1"));
    img_gear_fg2 = static_cast<ImageView*>(Helper::seekWidgetByName(panel_menu, "img_gear_fg2"));
    img_gear_fg3 = static_cast<ImageView*>(Helper::seekWidgetByName(panel_menu, "img_gear_fg3"));
    
    btn_record = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_record_0"));
    btn_record->setActionTag(eBtnDummy);
    btn_record->addTouchEventListener(CC_CALLBACK_2(RecordPageInteract::touchEvent, this));
    btn_record = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_record"));
    btn_record->setActionTag(eBtnInterRecord);
    btn_record->addTouchEventListener(CC_CALLBACK_2(RecordPageInteract::touchEvent, this));
    
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
    btn_play->addTouchEventListener(CC_CALLBACK_2(RecordPageInteract::touchEvent, this));
    btn_play = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_play"));
    btn_play->setActionTag(eBtnInterPlay);
    btn_play->addTouchEventListener(CC_CALLBACK_2(RecordPageInteract::touchEvent, this));
    
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
    btn_replay->addTouchEventListener(CC_CALLBACK_2(RecordPageInteract::touchEvent, this));
    btn_replay = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_replay"));
    btn_replay->setActionTag(eBtnInterListen);
    btn_replay->addTouchEventListener(CC_CALLBACK_2(RecordPageInteract::touchEvent, this));
    
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
    
    //add by hyp 
    btn_playRecord = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_playrecord_0"));
    btn_playRecord->setActionTag(eBtnDummy);
    btn_playRecord->addTouchEventListener(CC_CALLBACK_2(RecordPageInteract::touchEvent, this));
    btn_playRecord = static_cast<Button*>(Helper::seekWidgetByName(panel_menu, "btn_playrecord"));
    btn_playRecord->setActionTag(eBtnInterOnlyListen);
    btn_playRecord->addTouchEventListener(CC_CALLBACK_2(RecordPageInteract::touchEvent, this));
    skeleton_record_replay = (SkeletonAnimation*)btn_playRecord->getParent()->getChildByTag(eSklScore);
    if (!skeleton_record_replay) {
        skeleton_record_replay = spine::SkeletonAnimation::createWithJsonFile("spine/menu.json", "spine/menu.atlas" ,1.02f);
        skeleton_record_replay->setTag(eSklScore);
        skeleton_record_replay->setPosition(btn_playRecord->getPosition());
        btn_playRecord->getParent()->addChild(skeleton_record_replay, 1);
    }
    skeleton_record_replay->clearTracks();
    skeleton_record_replay->setAnimation(0, "play_stand", true);
   
    //add by hyp   for  sound
    m_soundSlider = Slider::create();
    m_soundLabel =  Label::createWithSystemFont("1.0", MYDEF_TTF_CHINESE, 20);
    RecordPageInteract::addSoundSlider(m_pageWidget,m_soundLabel,m_soundSlider);
    m_soundSlider->addEventListener(CC_CALLBACK_2(RecordPageInteract::soundSliderEvent, this));
}
void RecordPageInteract::soundSliderEvent(Ref *sender, Slider::EventType type)
{
    RecordPageInteract::addSoundSliderEvent(sender,type,m_soundLabel,m_soundID);
}
void RecordPageInteract::addSoundSlider(Widget* pageWidget,Label* soundLabel,Slider* soundSlider)
{
    //add by hyp   for  sound
    Slider* slider = soundSlider;
    slider->loadBarTexture("SrcImg/bookslider_progress.png");
    string name = "SrcImg/bookslider_bar.png";
    slider->loadSlidBallTextures(name,name,name);
    slider->loadProgressBarTexture("SrcImg/bookslider_progress.png");
    slider->setScale9Enabled(true);
    slider->setCapInsets(Rect(0, 0, 0, 0));
    slider->setPosition(Vec2(pageWidget->getContentSize().width*0.5f,220.0f));
    pageWidget->getChildByName("soundSlider")->addChild(slider);
    float speed = BookDataManager::getInstance()->getBookSoundSpeed();
    slider->setPercent(RecordPageInteract::conversionSlider(speed,1));
    slider->setVisible(SoundManager::supportSpeedChange());
    
    Sprite* bike = Sprite::create("SrcImg/bookslider_bike.png");
    bike->setPosition(Vec2(-30,slider->getContentSize().height*0.5f));
    slider->addChild(bike,-1);
    Sprite* car = Sprite::create("SrcImg/bookslider_car.png");
    car->setPosition(Vec2(slider->getContentSize().width+30,slider->getContentSize().height*0.5f));
    slider->addChild(car,-1);
    
    Label* label2 = soundLabel;
    label2->setColor(Color3B(95, 59, 33));
    label2->setString("正常语速");
    label2->setPosition(Vec2(slider->getContentSize().width*0.5f,45.0f));
    slider->addChild(label2,-1);
    
    Label* label = Label::createWithSystemFont("|", MYDEF_TTF_CHINESE, 20);;
    label->setColor(Color3B(95, 59, 33));
    label->setPosition(Vec2(label2->getContentSize().width*0.5f,-label->getContentSize().height*0.25f));
    label->setAnchorPoint(Vec2(0,0.5f));
    label2->addChild(label);
    
//    Label* label2 = Label::createWithSystemFont("Speed x", MYDEF_TTF_CHINESE, 15);
//    label2->setColor(Color3B(95, 59, 33));
//    label2->setPosition(Vec2(slider->getContentSize().width*0.47f,slider->getContentSize().height*1.5f));
//    slider->addChild(label2);
//    Label* label = soundLabel;
//    label->setString(StringUtils::format("%.2f",speed));
//    label->setColor(Color3B(95, 59, 33));
//    label->setPosition(Vec2(label2->getContentSize().width,label->getContentSize().height*0.5f));
//    label->setAnchorPoint(Vec2(0,0.5f));
//    label2->addChild(label);
}
void RecordPageInteract::addSoundSliderEvent(Ref *sender, Slider::EventType type,Label* label,int soundId)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
        Slider* slider = dynamic_cast<Slider*>(sender);
        int percent = slider->getPercent();
        float speed = RecordPageInteract::conversionSlider(percent,2);
//        label->setString(StringUtils::format("%.2f",speed));
        BookDataManager::getInstance()->setBookSoundSpeed(speed);
        if(SoundManager::isIOS()&&SoundManager::supportSpeedChange()){
            SoundManager::getInstance()->speedSound(soundId,speed);
            __NotificationCenter::getInstance()->postNotification(MSG_SET_SOUND_SPEED_NOTICE, (Ref*)(intptr_t)(speed*100));
        }
    }
    //记录正在改变速度
    if (type == Slider::EventType::ON_SLIDEBALL_DOWN) {
        BookDataManager::getInstance()->setSpeedIsChange(true);
    }else if(type == Slider::EventType::ON_SLIDEBALL_UP||type == Slider::EventType::ON_SLIDEBALL_CANCEL) {
        BookDataManager::getInstance()->setSpeedIsChange(false);
        BookDataManager::getInstance()->saveBookSoundSpeed();
        talkingInterface::traceEvent("语音速度",StringUtils::format("%.2f",BookDataManager::getInstance()->getBookSoundSpeed()));
    }
}
void RecordPageInteract::updateSoundSlider(Ref *sender,Label* soundLabel,Slider* soundSlider)
{
    //add by hyp 刷新页面的声音速度
    float speed = BookDataManager::getInstance()->getBookSoundSpeed();
    soundSlider->setPercent(RecordPageInteract::conversionSlider(speed,1));
    soundLabel->setString(StringUtils::format("%.2f",speed));
}
float RecordPageInteract::conversionSlider(float value,int type)
{
    if (type == 1) {//声速转显示进度
        float percent = (value>1.0f?((value-1.0f)/0.5f*50.0f+50.0f):((value-1.0f)/0.4f*50.0f+50.0f));
        return percent;
    }
    if (type == 2) {//显示进度转声速
        float middel = 50.0f;
        float speed = (value>middel? (value-middel)/50.0f*0.5f+1.0f:(value-middel)/50.f*0.4f+1.0f);
        return speed;
    }
    return -1;
}
void RecordPageInteract::updateUIByMode()
{
    eRecordBookMode mode = RecordBookData::getInstance()->getRecordBookMode();
    Helper::seekWidgetByName(panel_menu, "panel_playrecord")->setVisible(mode==eRecordBookMode::ePlay);
    Helper::seekWidgetByName(panel_menu, "panel_play")->setVisible(mode==eRecordBookMode::eRecord);
    Helper::seekWidgetByName(panel_menu, "panel_record")->setVisible(mode==eRecordBookMode::eRecord);
    Helper::seekWidgetByName(panel_menu, "panel_replay")->setVisible(mode==eRecordBookMode::eRecord);
    
    bool support = SoundManager::supportSpeedChange();
    bool show = RecordBookData::getInstance()->getRecordBookMode()!=eRecordBookMode::ePlay;
    m_soundSlider->setVisible(support&&show);
    
    
    //重置状态
    vector<SkeletonAnimation*> skeleton_list = {skeleton_play,skeleton_record,skeleton_replay,skeleton_record_replay};
    vector<string> action_list = {"play_stand","recording_stand","playback_stand","play_stand"};
    for (int i = 0; i<skeleton_list.size(); i++) {
        skeleton_list.at(i)->clearTracks();
        skeleton_list.at(i)->setAnimation(0, action_list.at(i), true);
    }
}

void RecordPageInteract::touchEvent(Ref *sender, Widget::TouchEventType type)
{
    CLICK_FILTER(sender, type);
    if(!BOOL_BUT_TOUCH_ENABLE())
    {
        return;// add by hyp 2018/11/20
    }
    ButtonType btnType = (ButtonType)(dynamic_cast<Widget*>(sender))->getActionTag();
    CFileLog::LogSaveFormat("===RecordPageInteract::touchEvent==%d", (int)btnType);
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
            
            if (Recorder::getInstance()->isRecording())
            {
                Recorder::getInstance()->stopRecoder();
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
                recordPath += StringUtils::format("/%d/record", m_pageIndex);
                
                Recorder::getInstance()->startRecoder(recordPath);
                
                //自动倒计时停止录音
//                if (fRecordLimit != -1.0f) {
//                    Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(RecordPageInteract::recordLimit), this, fRecordLimit, 0, 0, false);
//                }
            }
//            talkingInterface::traceEvent("原版阅读交互按钮", "录音");
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
            interListen(false);
        }
            break;
        case eBtnInterOnlyListen:
        {
            interListen(true);
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



void RecordPageInteract::interListen(bool onlyListen)
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
    bool isOver = m_wholeBook->m_mapRecordPath.find(m_pageIndex) != m_wholeBook->m_mapRecordPath.end();
    bool isRecording = Recorder::getInstance()->isRecording();//录音时屏蔽
    bool INVALID = m_soundID == AudioEngine::INVALID_AUDIO_ID;//播音时屏蔽
    if (isOver&& !isRecording&& INVALID)
    {
        if(Recorder::getInstance()->isPlayRecording())
        {
            Recorder::getInstance()->stopPlayRecord();
        }
        else
        {
            Recorder::getInstance()->playRecoder(m_wholeBook->m_mapRecordPath[m_pageIndex]);
//            setBtnState(true,onlyListen?3:2);
            startWheelAnim(onlyListen?3:2);
        }
    }
}

void RecordPageInteract::update(float dt)
{
    PageBase::update(dt);
}

void RecordPageInteract::loadPage(WholeBook &wholeBook, int index)
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
                    }, attr1.w_voiceBegin, attr.w_voiceEnd - attr1.w_voiceBegin);
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
    
    m_viewHeight = myText->getContentSize().height + fSpace * 1.2 + panel_menu->getContentSize().height;
    SILog("整体高度", m_viewHeight)
    img_inter_yun->setPosition(Vec2::ZERO);
    
    bmf_score->setString(StringUtils::format("%d", onePage->m_pageScore->m_dummy));
    if(bmf_score->getString() != "0")
    {
        img_score_bg->setVisible(true);
    }
}

void RecordPageInteract::freePage()
{
    PageBase::freePage();
}

void RecordPageInteract::showOralResult()
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

int RecordPageInteract::playAudio(std::function<void (int, const std::string &)> finishCall)
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
        setBtnState(true, 1);
        startWheelAnim(0);
        float speed = BookDataManager::getInstance()->getBookSoundSpeed();
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

void RecordPageInteract::stopAudio()
{
    //音效路径
    string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
    if (m_soundID != AudioEngine::INVALID_AUDIO_ID && FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        SoundManager::getInstance()->stopSound(m_soundID);
    }
    PageBase::stopAudio();
}

//void RecordPageInteract::recordLimit(float dt)
//{
//    Recorder::getInstance()->stopRecoder();
//}

void RecordPageInteract::onInterAnimEnded(cocos2d::Node *sender, eInterState state)
{
    m_interState = state;
    if(state == eInterClose)
    {
        //如果是正在录音，停止录音相关
//        if (Director::getInstance()->getScheduler()->isScheduled(CC_SCHEDULE_SELECTOR(RecordPageInteract::recordLimit), this)) {
//            Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(RecordPageInteract::recordLimit), this);
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
        //if(!NewHandTip::isShow("RecordPageInteract1"))
        //{}else
        if(!NewHandTip::isShow("RecordPageInteract2"))
        {
            NewHandTip::setShow("RecordPageInteract2");
            auto node = NewHandTip::create2(UiBase::getInstance()->getRunningScene());
            node->addImg("newhand_interactPage1.png", myText, Vec2(30, -30));
        }
        else if(!NewHandTip::isShow("RecordPageInteract3") && myText && myText->getWordCount() > 10)
        {
            NewHandTip::setShow("RecordPageInteract3");
            auto node = NewHandTip::create2(UiBase::getInstance()->getRunningScene());
            node->addImg("newhand_interactPage2.png", myText, Vec2(0, myText->getContentSize().height / 2 - 198));
            node->addImgAndClip("", myText, Vec2::ZERO, Vec2(0, myText->getContentSize().height / 2 - 20), Size(620,70));
        }
    }
    
}

void RecordPageInteract::doInterAnimation()
{
    //去掉面板  add by hyp
    if (true) {
        return;
    }
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
        pCallback = CallFuncN::create(CC_CALLBACK_1(RecordPageInteract::onInterAnimEnded,this,eInterClose));
        SLog("交互动画Down")
    } else {
        SLog("交互动画Up")
        //SILog("可视高度", m_viewHeight);
        //SFLog("面板坐标", img_inter_yun->getPositionY());
        pMove = MoveTo::create(0.5f, Vec2(img_inter_yun->getPositionX(), m_viewHeight));
        pCallback = CallFuncN::create(CC_CALLBACK_1(RecordPageInteract::onInterAnimEnded,this,eInterShow));
        this->runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create([this](){
            //myText->resetColor();
        }), NULL));
    }
    
    Sequence* pSequence = Sequence::create(pMove, pCallback, nullptr);
    pSequence->setTag(123);
    img_inter_yun->runAction(pSequence);
}

void RecordPageInteract::startWheelAnim(int iTag)
{
    //    ImageView* img_gear = iTag == 0 ? img_gear_fg0 : (iTag == 1 ? img_gear_fg1 : img_gear_fg2);
    vector<ImageView*> list = {img_gear_fg0,img_gear_fg1,img_gear_fg2,img_gear_fg3};
    ImageView* img_gear = list.at(iTag);
    img_gear->runAction(RepeatForever::create(RotateBy::create(3.0, 360)));
    
    vector<SkeletonAnimation*> skeleton_list = {skeleton_play,skeleton_record,skeleton_replay,skeleton_record_replay};
    vector<string> click_list = {"play_click","recording_click","playback_click","play_click",};
    vector<string> rotate_list = {"play_rotate","recording_rotate","playback_rotate","play_rotate",};
    
    SkeletonAnimation* sklAnim = skeleton_list.at(iTag);
    string strClick = click_list.at(iTag);
    string strRotate =  rotate_list.at(iTag);
    
//    SkeletonAnimation* sklAnim = iTag == 0 ? skeleton_play : (iTag == 1 ? skeleton_record : skeleton_replay);
//    string strClick = iTag == 0 ? "play_click" : (iTag == 1 ? "recording_click" : "playback_click");
//    string strRotate = iTag == 0 ? "play_rotate" : (iTag == 1 ? "recording_rotate" : "playback_rotate");
    
    sklAnim->clearTracks();
    sklAnim->setAnimation(0, strClick, false);
    sklAnim->setCompleteListener([=] (spTrackEntry* entry){
        sklAnim->setAnimation(0, strRotate, true);
    });
}
void RecordPageInteract::stopWheelAnim(int iTag, bool bManue)
{
    //    ImageView* img_gear = iTag == 0 ? img_gear_fg0 : (iTag == 1 ? img_gear_fg1 : img_gear_fg2);
    //    string strClick = iTag == 0 ? "play_click" : (iTag == 1 ? "recording_click" : "playback_click");
    
    vector<ImageView*> list = {img_gear_fg0,img_gear_fg1,img_gear_fg2,img_gear_fg3};
    ImageView* img_gear = list.at(iTag);
    img_gear->stopAllActions();
    
    vector<SkeletonAnimation*> skeleton_list = {skeleton_play,skeleton_record,skeleton_replay,skeleton_record_replay};
    vector<string> click_list = {"play_click","recording_click","playback_click","play_click",};
    vector<string> rotate_list = {"play_rotate","recording_rotate","playback_rotate","play_rotate",};
    
    SkeletonAnimation* sklAnim =skeleton_list.at(iTag);
    if(!bManue)
    {
        sklAnim->clearTracks();
    }
    else
    {
        string strClick = click_list.at(iTag);
        sklAnim->setAnimation(0, strClick, false);
    }
    
}

void RecordPageInteract::setBtnState(bool bStart, int iType)
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
    if(iType == 3)
    {
        Node* layout_parent = btn_playRecord->getParent();
        if (bStart) {
            btn_playRecord->setTouchEnabled(false);
            layout_parent->setColor(Color3B::GRAY);
            skeleton_record_replay->setColor(Color3B::GRAY);
        } else {
            btn_playRecord->setTouchEnabled(true);
            layout_parent->setColor(Color3B::WHITE);
            skeleton_record_replay->setColor(Color3B::WHITE);
            skeleton_record_replay->clearTracks();
        }
    }
}

void RecordPageInteract::setBtnState(int iType, bool bEnabled)
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
    }else if (iType == 3) {
        btnTmp = btn_playRecord;
        sklTmp = skeleton_record_replay;
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

void RecordPageInteract::onRecordTimeOut()
{
    if(Recorder::getInstance()->isRecording())
    {
        Recorder::getInstance()->stopRecoder();
    }
}

void RecordPageInteract::setPageAutoPlay(Ref* isPlaying)
{
    bool playRecord = RecordBookData::getInstance()->getRecordBookMode()==eRecordBookMode::ePlay;
    bool bEnable = ((intptr_t)isPlaying == 1);
    if(bEnable)
    {
        setBtnState(true, playRecord?3:1);
        startWheelAnim(playRecord?3:0);
    }
    else
    {
        setBtnState(false, playRecord?3:1);
        stopWheelAnim(playRecord?3:0);
    }
}

void RecordPageInteract::stopInterAnim()
{
    img_inter_yun->stopActionByTag(123);
    img_inter_yun->setPositionY(0);
    onInterAnimEnded(this, eInterClose);
}
