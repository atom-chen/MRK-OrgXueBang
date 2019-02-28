//
//  BookBody.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/2/27.
//
//

#include "BookBody.hpp"
#include "TipManager.hpp"
#include "Reador.hpp"
#include "Recorder.h"
//#include "BookScene.hpp"
#include "PageContent.hpp"
#include "PageQuizCover.hpp"
#include "PageQuizOral.hpp"
#include "PageQuizTemp.hpp"
#include "QuizManager.h"
//#include "BookReport.hpp"
#include "TipWaiting.hpp"
#include "WordFind.h"
#include "BoyWithMsg.hpp"
#include "NewHandTip.hpp"
//#include "BackClick.hpp"
//#include "UserVIPCharge.h"
//#include "Login.h"
//#include "PlayVod.hpp"
#include "NewUtilManager.hpp"
//#include "Game.hpp"
#include "RecordBookData.hpp"
//#include "BookRechargeTip.hpp"
#include "BookPageContentBG.hpp"
#include "MachineManager.h"
//#include "MachineStart.hpp"

//#include "RKStdafx.h"
//#include "ReadKingManager.hpp"


#include "TimeManager.hpp"
#include "UserManager.hpp"
#include "FileLog.h"
#include "CostTime.hpp"
#include "SceneManager.hpp"
#include "UserManager.hpp"
#include "NetworkService.hpp"
#include "UtilsDefine.h"
#include "BehaviorManager.hpp"
#include "StringUtil.hpp"
#include "assist.h"

#include "UIReadingFinish.hpp"
#include "HomePageManager.hpp"

#define BOOK_SIZE   750

BookBody::BookBody(int type)
:iLastPage(-1)
,iCurPage(0)
,iAllPage(0)
,iLstPage(0)
,iRecognizePageIdx(0)
,bGuideOver(false)
,bForceExit(true)
,bDoubleClicked(false)
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
,bRecordCanceled(false)
#endif
,bInterReady(false)
,bWaitRecognize(false)
,m_recordLoadingNode(nullptr)
,isDuBa(true)
{
    talkingInterface::pageBegin("原版图书页面");
    COST_TIME_START
    m_bCanTurnPage = false;
    bRecordComplete = true;
    bReplayComplete = true;
    CDataHttp::getInstance()->regeditCallBack(this);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(BookBody::autoTurnPage), MSG_TURN_PAGE, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(BookBody::reloadOnePage), MSG_BOOK_RELOAD, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(BookBody::initInterPanel), MSG_UPDATE_INTERACT, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(BookBody::setInterVisible), MSG_SET_INTERACT, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(BookBody::onRecordTimeOut), MSG_RECORD_TIMEOUT, nullptr);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(BookBody::setAutoBtnState), MSG_SET_AUTOBTN, nullptr);
    
//    MessageManager::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(BookBody::onRevMsgDoHandle), MSG_RK_DO_HANDLE_DATA);
//
//    isDuBa = Reador::getInstance()->getIsReadKing();
    
//    _minScale = DataTableManager::getInstance()->getValue(eTable_config_setting, config_setting_Page_Scale_Min, 0)->getFloat();
//    _maxScale = DataTableManager::getInstance()->getValue(eTable_config_setting, config_setting_Page_Scale_Max, 0)->getFloat();
//    _minSlide = DataTableManager::getInstance()->getValue(eTable_config_setting, config_setting_Page_Slide_Min, 0)->getFloat();
    _bSeriour = true;
    
    m_mapNative.clear();
    m_mapAcc.clear();
    m_mapFluency.clear();
    m_mapScore.clear();
    MessageManager::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(BookBody::onRevMsgDoHandle),NSEventNameDef::ReadingAgain);
}

BookBody::~BookBody()
{
    CDataHttp::getInstance()->unregeditCallBack(this);
    __NotificationCenter::getInstance()->removeObserver(this, MSG_TURN_PAGE);
    __NotificationCenter::getInstance()->removeObserver(this, MSG_BOOK_RELOAD);
    __NotificationCenter::getInstance()->removeObserver(this, MSG_UPDATE_INTERACT);
    __NotificationCenter::getInstance()->removeObserver(this, MSG_SET_INTERACT);
    __NotificationCenter::getInstance()->removeObserver(this, MSG_RECORD_TIMEOUT);
    __NotificationCenter::getInstance()->removeObserver(this, MSG_SET_AUTOBTN);
    
    MessageManager::getInstance()->removeObserver(this);
//    m_wholeBook.clearWholeBook();//清理后，报告页就不能解析页信息了，取消回收
    talkingInterface::pageEnd("原版图书页面");
    
    Device::setKeepScreenOn(false);
    
    panel_pdf->release();
    panel_quiz_oral->release();
    panel_quiz_start->release();
    panel_quiz_temp->release();
    panel_interact->release();
    panel_content_bg->release();
}
void BookBody::onEnter()
{
    BaseLayer::onEnter();
    
    COST_TIME_END("===BookBody===")
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(false);
    
    _touchListener->onTouchBegan = CC_CALLBACK_2(BookBody::onTouchBegan, this);
    _touchListener->onTouchMoved = CC_CALLBACK_2(BookBody::onTouchMoved, this);
    _touchListener->onTouchEnded = CC_CALLBACK_2(BookBody::onTouchEnded, this);
    _touchListener->onTouchCancelled = CC_CALLBACK_2(BookBody::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_touchListener, -1);
    
    _touches.clear();
    
    {
        Recorder::getInstance()->setRecordMode(RECORD_SPX0_WAV1 == 1);
        //开始录音
        Recorder::getInstance()->addBeginRecoderCall([=](){
            m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI->setRecordTime(true);
            Reador::getInstance()->setPOrRing(true);
            bWaitRecognize = false;
            Date* date = Date::date();
            SLLog("开始录音==》", date->getTime())
            if (m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageQuziOral &&
                QuizManager::getInstance()->getQuizType(m_wholeBook.m_bookPages->at(iCurPage)) == eQuizOral) {
                PageQuizOral* quiz = (PageQuizOral*)m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI;
                quiz->startWheelAnim(1);
                quiz->setBtnState(true, 0);
            } else if (m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageContent){
                pageInter->startWheelAnim(1);
                pageInter->setBtnState(true, 0);
            }
        });
        
        //录音中
        Recorder::getInstance()->addRecordingCall([](float time){
            log("recording... %f\n", time);
        });
        
        //录音完成
        Recorder::getInstance()->addEndRecordCall([=](string filePath){
            Reador::getInstance()->setPOrRing(false);
            Date* date = Date::date();
            SLLog("结束录音==》", date->getTime())
            if(!bRecordComplete || m_wholeBook.m_mapPageViews.size() == 0)
            {
                bRecordComplete = true;
                return;
            }
            
            m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI->setRecordTime(false);
            
            m_wholeBook.m_mapRecordPath[iCurPage] = filePath;
            
            if (m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageQuziOral &&
                QuizManager::getInstance()->getQuizType(m_wholeBook.m_bookPages->at(iCurPage)) == eQuizOral) {
                PageQuizOral* quiz = (PageQuizOral*)m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI;
                quiz->stopWheelAnim(1);
                quiz->setBtnState(false, 0);
            } else if (m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageContent){
                pageInter->stopWheelAnim(1);
                pageInter->setBtnState(false, 0);
            }
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            if(bRecordCanceled)
            {
                bRecordCanceled = false;
                return;
            }
#endif
            //上传到服务器
            std::map<std::string,std::string> mapHeadValue;
            mapHeadValue["Expect"] = "";
            
            iRecognizePageIdx = iCurPage;
            OnePage* onePage = m_wholeBook.m_bookPages->at(iCurPage);
            string title_id = onePage->m_paramJson.getValue("title_id");
            //title_id不能为空
            if (title_id.length() < 1) {
                return;
            }
            
            string srtDifConfig = "";
            
            if (m_wholeBook.m_bookData->level == "AA"
                || m_wholeBook.m_bookData->level == "A"
                || m_wholeBook.m_bookData->level == "B"
                || m_wholeBook.m_bookData->level == "C"
                )
            {
                srtDifConfig = "{\"file\":\"rm_ac_levels.xml\",\"level\":\"1\"}";
            }
            else
            {
                srtDifConfig = "{\"file\":\"rm_dk_levels.xml\",\"level\":\"1\"}";
            }
            
            
            //Post参数优先
            KVPairVec* vecPostValue = new KVPairVec;
            vecPostValue->push_back(new KVPair("model", "kid4to14"));//"adult"
            vecPostValue->push_back(new KVPair("content", StringUtils::format("{\"key\":\"%s\"}", title_id.c_str())));
            vecPostValue->push_back(new KVPair("difficulty", srtDifConfig));
            
            vecPostValue->push_back(new KVPair("format", RECORD_SPX0_WAV1 == 1 ? RECORD_FORMAT : "spx"));
            vecPostValue->push_back(new KVPair("encode", "byte"));
            vecPostValue->push_back(new KVPair("token", UserManager::getInstance()->getUserModel()->getUserToken()));
            
            //Get参数备用
//            KVPairVec* vecGetValue = new KVPairVec;
//            vecGetValue->push_back(new KVPair("model", "kid4to14"));//"adult"
//            vecGetValue->push_back(new KVPair("content", StringUtils::format("{\"key\":\"%s\"}", title_id.c_str())));
//            vecGetValue->push_back(new KVPair("difficulty", srtDifConfig));
//            vecGetValue->push_back(new KVPair("format", RECORD_SPX0_WAV1 == 1 ? RECORD_FORMAT : "spx"));
//            vecGetValue->push_back(new KVPair("encode", "byte"));
//            vecGetValue->push_back(new KVPair("token", UserManager::getInstance()->getUserModel()->getUserToken()));
            
            stuFileInfo fileInfo;
            fileInfo.m_strFileName = FileUtils::getInstance()->fullPathForFilename(filePath);
            fileInfo.m_strFileCopyName = "upload";
            fileInfo.m_strFileContentType = "audio/x-speex";
            
            vector<stuFileInfo> vecFileInfo;
            vecFileInfo.push_back(fileInfo);
            
//            stuUpload upload(NetworkService::getInstance()->getServiceUrl(eHttpType::recognize), vecFileInfo, mapHeadValue, vecPostValue, vecGetValue);
            
            stuUpload upload(NetworkService::getInstance()->getServiceUrl(eHttpType::recognize), vecFileInfo, mapHeadValue, vecPostValue);
            HttpManager::getInstance()->HttpUploadRequest(eHttpType::recognize, upload);
            vecFileInfo.clear();
            vector<stuFileInfo>(vecFileInfo).swap(vecFileInfo);
            bWaitRecognize = true;
            
            this->showRecordLoading(true);
        });
        
        //播放结束
        Recorder::getInstance()->addEndPlayCall([=](){
            if(!bReplayComplete || m_wholeBook.m_mapPageViews.size() == 0)
            {
                bReplayComplete = true;
                return;
            }
            if (m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageQuziOral &&
                QuizManager::getInstance()->getQuizType(m_wholeBook.m_bookPages->at(iCurPage)) == eQuizOral) {
                PageQuizOral* quiz = (PageQuizOral*)m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI;
                quiz->stopWheelAnim(2);
                quiz->setBtnState(false, 2);
            } else if (m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageContent){
                pageInter->stopWheelAnim(2);
                pageInter->setBtnState(false, 2);
            }
            
        });
        
        Recorder::getInstance()->addCancelRecordCall([=](){
            Reador::getInstance()->setPOrRing(false);
            bWaitRecognize = false;
            if(!bRecordComplete)
            {
                bRecordComplete = true;
                return;
            }
            m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI->setRecordTime(false);
            if (m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageQuziOral &&
                QuizManager::getInstance()->getQuizType(m_wholeBook.m_bookPages->at(iCurPage)) == eQuizOral) {
                PageQuizOral* quiz = (PageQuizOral*)m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI;
                quiz->stopWheelAnim(1);
                quiz->setBtnState(false, 0);
            } else if (m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageContent){
                pageInter->stopWheelAnim(1);
                pageInter->setBtnState(false, 0);
            }
        });
    }
    
    //分部加载，先loading，去解析书页信息
    {
        
        HttpDataToString dataStr;
        dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
        dataStr.addData("courseid", Reador::getInstance()->getCurrentBook());
        dataStr.addData("task_id", Reador::getInstance()->getCurrentTask());//任务ID（可选，值为空/0，或具体的任务ID）
        dataStr.addData("lession_id", Reador::getInstance()->getCurrentWordClass());//课程包的课ID（0或空：正常读书 其他值：课ID）
        dataStr.addData("quality_id", Reador::getInstance()->getCurrentQualityId());//精读课阶段ID (0或空：正常读书 其他值：阶段ID)
        CDataHttp::getInstance()->HttpGet(eHttpType::pagelist, dataStr.getString());
    }
    
    if (NewUtilManager::getInstance()->getJumpQuiz())//liuwei 切换场景get历史数据
    {
        m_wholeBook = BookDataManager::getInstance()->getWholeBook();//还是上一次的数据
        m_wholeBook.m_mapPageViews.clear();//清除不是data的数据, 耦合太高改不动
    }
    else
    {
        doActionOpen();
    }
    
    
    //在此页停留3秒以上提示，如用户
    //3秒内操作则此页不再出现新手提示
    //2018年01月18日11:27:03@lfj
    this->scheduleOnce([this](float dt){
        if(!bGuideOver && !NewHandTip::isShow("BookGuide"))
        {
            NewHandTip::setShow("BookGuide");
            auto node = NewHandTip::create2(UiBase::getInstance()->getRunningScene());
            node->addImg("newhand_introPage2.png", btn_auto, Vec2(0, -260));
//            node->addImg("newhand_introPage1.png", BookGuideLayer->getWidget("btn_oral"), Vec2(0, 280));
//            node->addImgAndClip("", BookGuideLayer->getWidget("btn_oral"), Vec2::ZERO, Vec2(90, 0), Size(750,250));
            node->addImgAndClip("", btn_auto, Vec2::ZERO, Vec2::ZERO, Size(310,90));
        }
    }, 2, "newhandtip");
    
    //开书日志提前初始化，防止空值，导读淡出后，会再次刷新，不会损失用户的时长
    BookDataManager::getInstance()->setOpenTime(TimeManager::getInstance()->getCurTime());
}

void BookBody::onEnterTransitionDidFinish()
{
    
}

void BookBody::onExit()
{
    Recorder::getInstance()->addBeginRecoderCall(nullptr);
    Recorder::getInstance()->addRecordingCall(nullptr);
    Recorder::getInstance()->addEndRecordCall(nullptr);
    Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
    Director::getInstance()->getRunningScene()->unschedule("jump_to_report");
    if(bGuideOver && bForceExit)
    {
        PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
        PageBase* curPage =  cache->m_pageUI;
        curPage->onTurnOut();//专门提交退出页的日志
        
        dealScoreOutLog();
    }
    BaseLayer::onExit();
}

BookBody* BookBody::create(int type)
{
    BookBody *ret = new (std::nothrow) BookBody(type);
    if (ret && ret->init())
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
bool BookBody::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    this->setTag(eLayerBookBody);
    this->load("View/Reading/LayerBody.csb");
    return true;
}

void BookBody::addEvents()
{
    //关联基本Panel
    Widget* panel_root = (Widget*)getRootWidget();
    panel_center = (Widget*)panel_root->getChildByName("panel_center");
    panel_body = (Widget*)panel_center->getChildByName("panel_body");
    
    btn_left = (Button*)panel_center->getChildByName("btn_left");
    btn_left->setActionTag(eBtnLeft);
    btn_left->addTouchEventListener(CC_CALLBACK_2(BookBody::touchEvent, this));
    
    btn_right = (Button*)panel_center->getChildByName("btn_right");
    btn_right->setActionTag(eBtnRight);
    btn_right->addTouchEventListener(CC_CALLBACK_2(BookBody::touchEvent, this));
    
    _viewSize = panel_center->getContentSize();
    _viewRect = Rect(0, 0, _viewSize.height, _viewSize.width);
    
    BookDataManager::getInstance()->setBodySize(Size::ZERO);
    
    {
        //读书页，切图BookPageContent.csb;
        //语音Quiz，切图ReadOral.csb,
        //Quiz封皮，切图BookCover.csb;
        //Quiz空白，切图BookPage.csb;
        //交互页，切图BookInteract.csb;
        
        {
            auto mUI = CSLoader::createNode("View/Reading/BookPage.csb");
            panel_quiz_temp = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_quiz_temp");
            panel_quiz_temp->removeFromParent();
            panel_quiz_temp->retain();
        }
        
        {
            auto mUI = CSLoader::createNode("View/Reading/ReadOral.csb");
            panel_quiz_oral = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_quiz_oral");
            panel_quiz_oral->removeFromParent();
            panel_quiz_oral->retain();
        }
        {
            auto mUI = CSLoader::createNode("View/Reading/BookCover.csb");
            panel_quiz_start = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_quiz_start");
            panel_quiz_start->removeFromParent();
            panel_quiz_start->retain();
        }
        {
            auto mUI = CSLoader::createNode("View/Reading/BookInteract.csb");
            panel_interact = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_interact");
            panel_interact->removeFromParent();
            panel_interact->retain();
        }
        {
            auto mUI = CSLoader::createNode("View/Reading/BookPageContent.csb");
            panel_pdf = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_pdf");
            panel_pdf->removeFromParent();
            panel_pdf->retain();
        }
        {
            auto mUI = CSLoader::createNode("View/Reading/BookPageContentBG.csb");
            panel_content_bg = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_content_bg");
            panel_content_bg->removeFromParent();
            panel_content_bg->retain();
        }
        {
            BookGuideLayer = BookGuide::create(isDuBa);
            BookGuideLayer->setBookBody(this);
            panel_center->addChild(BookGuideLayer);
        }
        {
            auto mUI = CSLoader::createNode("View/Reading/BookInteract.csb");
            Widget* pageUI = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_interact");
            pageUI->removeFromParent();
            pageUI->retain();
            
            pageInter = PageInteract::create(pageUI->clone());
            pageInter->setPosition(Vec2(0,0));
            pageInter->setInterState(eInterClose);
            panel_center->addChild(pageInter);
            setInterVisible((Ref*)false);
        }
    }
    /////////导读界面的空间关联
    {
        //播放按钮
        btn_auto = static_cast<cocos2d::ui::Button*>(Helper::seekWidgetByName(m_rootWidget, "btn_produce"));
        btn_auto->setActionTag(eBtnAutoPlay);
        btn_auto->addTouchEventListener(CC_CALLBACK_2(BookBody::touchEvent, this));
        btn_auto->setVisible(false);
        
        //TopBar
        Button* btn_back = static_cast<Button*>(Helper::seekWidgetByName(m_rootWidget, "btn_back"));
        btn_back->setActionTag(eBtnBack);
        btn_back->addTouchEventListener(CC_CALLBACK_2(BookBody::touchEvent, this));
        
        //收藏该书
        btn_collect = static_cast<Button*>(Helper::seekWidgetByName(m_rootWidget, "btn_collect"));
        btn_collect->addTouchEventListener(CC_CALLBACK_2(BookBody::touchEvent, this));
    }
    
    m_bCanTurnPage = true;
}

void BookBody::touchEvent(Ref *sender, Widget::TouchEventType type)
{
    CLICK_FILTER(sender, type);
    if(!BOOL_BUT_TOUCH_ENABLE())
    {
        return;// add by hyp 2018/11/20
    }
    ButtonType btnType = (ButtonType)(dynamic_cast<Widget*>(sender))->getActionTag();
    CFileLog::LogSaveFormat("===BookBody::touchEvent==%d", (int)btnType);
    if(btnType == eBtnAutoPlay)
    {
        SoundManager::getInstance()->playSoundEffect("music/snd_auto.mp3");
    }
    switch (btnType)
    {
        case eBtnBack:
        {
            this->doBackHandle();
        }
            break;
        case eBtnAutoPlay:
        {
            if(!checkReadTip() || !m_bCanTurnPage)
            {
                return;
            }
            Node *diction = (Node*)pageInter->getPageWidget()->getChildByTag(eLayerBookDiction);
            if(diction)
                diction->removeFromParent();
            talkingInterface::traceEvent("原版导读按钮", "自动");
            
            pageInter->setInterState(eInterClose);
            
            if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
            {
                BookDataManager::getInstance()->setReadMode(eReadNone);
                __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
            }else{
                BookDataManager::getInstance()->setReadMode(eReadAuto);
                __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)true);
                if(!bGuideOver)
                {
                    if(iLstPage > iOralPage)
                    {
                        iLstPage = 0;
                    }
                    doActionClose();
                }else{
                    if (m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end()) {
                        PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
                        PageBase* curPage =  cache->m_pageUI;
                        curPage->onTurnIn();
                    }
                }
            }
        }
            break;
        case eBtnProduce:
        {
            doSimplePlay();
        }
            break;
        case eBtnLeft:
        {
            showRecordLoading(false);//关闭录音打分动画
            if(pageInter->getInterState() != eInterClose
               || pageInter->bInterDoAnim()
               || Recorder::getInstance()->isRecording()
               || Recorder::getInstance()->isPlayRecording())
                return;
            if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
            {
                BookDataManager::getInstance()->setReadMode(eReadNone);
                __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
            }
            //刷新声音速度  add by hyp
            __NotificationCenter::getInstance()->postNotification(MSG_SET_SOUND_SPEED, (Ref*)false);
            turnPreviousPage();
        }
            break;
        case eBtnRight:
        {
            showRecordLoading(false);//关闭录音打分动画
            if(pageInter->getInterState() != eInterClose
               || pageInter->bInterDoAnim()
               || Recorder::getInstance()->isRecording()
               || Recorder::getInstance()->isPlayRecording())
                return;
            if (MachineManager::getInstance()->getMachineData()->getFromMachine()) {//从人机练习跳过来的，不要Quiz  add by hyp
                if (iCurPage+1<m_wholeBook.m_bookPages->size()&&m_wholeBook.m_bookPages->at(iCurPage+1)->m_pageTitle == "Start Quiz") {
                    MachineManager::getInstance()->getMachineData()->setFromMachine(false);
                    SceneManager::getInstance()->setGameState(eStateMain);
//                    UiBase::getRunningScene()->addChild(MachineStart::create());
                    return;
                }
            }
            
            if(isDuBa){
//                printf("iCurPageiCurPage===%d==%d=\n",iCurPage,iOralPage);
                if(iCurPage >= iOralPage){
//                    printf("iCurPageiCurPageiCurPageendendened===%d==%d=\n",iCurPage,iOralPage);
                    return;
                }
            }
            
            if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
            {
                BookDataManager::getInstance()->setReadMode(eReadNone);
                __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
            }
            //刷新声音速度 add by hyp
            __NotificationCenter::getInstance()->postNotification(MSG_SET_SOUND_SPEED, (Ref*)false);
            turnNextPage();
        }
            break;
        case eBtnCollectAdd:
        {
            HttpDataToString dataStr;
            dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
            dataStr.addData("courseid", Reador::getInstance()->getCurrentBook());
            CDataHttp::getInstance()->HttpPost(eHttpType::addfavouritecourse, dataStr.getString());
            talkingInterface::traceEvent("原版导读按钮", "收藏");
        }
            break;
        case eBtnCollectSub:
        {
            HttpDataToString dataStr;
            dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
            dataStr.addData("courseid", Reador::getInstance()->getCurrentBook());
            CDataHttp::getInstance()->HttpPost(eHttpType::removefavouritecourse, dataStr.getString());
            talkingInterface::traceEvent("原版导读按钮", "收藏");
        }
            break;
        default:
        {
            
        }
            break;
    }
}

bool BookBody::onTouchBegan(Touch* touch, Event* event)
{
    if (!boolCanTouch()) {
        return true;
    }
    //动画过程中屏蔽触摸事件
    Node* baseNode = UiBase::getInstance()->getRunningScene();
    NewHandTip* teach = (NewHandTip*)baseNode->getChildByTag(eLayerUserTeach);
    TipLayer *tip;
    if (pageInter->getInterState() != eInterClose
        || !bGuideOver
        || !m_bCanTurnPage
        || m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageQuziOral
        || m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageQuziOther
        || m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageQStart
        || teach
        || TipManager::getInstance()->isOpenTip(eTipWaiting, tip)
        )
    {
        WordFind *diction = (WordFind*)pageInter->getPageWidget()->getChildByTag(eLayerBookDiction);
        if(pageInter->getInterState() != eInterClose && !diction && !teach)//交互面板处于开启状态，无词典、引导
        {
            ImageView* img_inter_yun = static_cast<ImageView*>(Helper::seekWidgetByName(pageInter->getPageWidget(), "img_inter_yun"));
            
            Vec2 pos_arrow = img_inter_yun->convertToWorldSpace(Vec2::ZERO);
            pos_arrow = panel_center->convertToNodeSpace(pos_arrow);
            Size size_arrow = img_inter_yun->getContentSize();
            Rect rectArrow = Rect(pos_arrow.x,pos_arrow.y,size_arrow.width,size_arrow.height);
            Vec2 location = panel_center->convertToNodeSpace(touch->getLocation());
            SFLog("触点x", location.x)
            SFLog("触点y", location.y)
            if (!rectArrow.containsPoint(location)) {//触摸交互面板以外的区域
                SLog("交互面板收起动画")
                bWaitRecognize = false;
                pageInter->doInterAnimation();
            }
        }
        return false;
    }
    
    CCLOG("11----- onTouchBegan touchPos x = %f, y = %f", touch->getLocation().x, touch->getLocation().y);
    if (std::find(_touches.begin(), _touches.end(), touch) == _touches.end())
    {
        _touches.push_back(touch);
    }
    
    if (_touches.size() == 1)
    {
        m_fMove = panel_center->convertToNodeSpace(touch->getLocation());
        _touchPoint     = m_fMove;
        _touchLength    = 0.0f;
        
    }
    else if (_touches.size() == 2)
    {
        Vec2 pos0 = _touches[0]->getLocation();
        Vec2 pos1 = _touches[1]->getLocation();
        
        _touchPoint = pos0.getMidpoint(pos1);
        _touchLength = pos0.getDistance(pos1);
        _scaleStart = panel_body->getScale();
        
        bDoubleClicked = true;
    }
    return true;
}

void BookBody::onTouchMoved(Touch* touch, Event* event)
{
    if (!boolCanTouch()) {
        return ;
    }
    CCLOG("22----- onTouchMoved touchPos x = %f, y = %f", touch->getLocation().x, touch->getLocation().y);
    
    //自动翻页模式切回普通模式
    if (std::find(_touches.begin(), _touches.end(), touch) != _touches.end())
    {
        if (_touches.size() == 1)
        {
            Vec2 moveDistance, newPoint;
            Rect  frame;
            float newX, newY;
            
            newPoint     = panel_center->convertToNodeSpace(_touches[0]->getLocation());
            moveDistance = newPoint - _touchPoint;
            
            float pos = panel_body->getPosition().y;
            
            Vec2 minPos = minContainerOffset();
            Vec2 maxPos = maxContainerOffset();
            
            if (!(minPos.y <= pos && pos <= maxPos.y)) {
                moveDistance.y = 0;
            }
            
            pos = panel_body->getPosition().x;
            if (!(minPos.x <= pos && pos <= maxPos.x)) {
                moveDistance.x = 0;
            }
            
            _touchPoint = newPoint;
            
            int tarX = panel_body->getPosition().x + moveDistance.x;
            int tarY = panel_body->getPosition().y + moveDistance.y;
            
            newX     = MAX(minPos.x, MIN(maxPos.x, tarX));
            newY     = MAX(minPos.y, MIN(maxPos.y, tarY));
            
            panel_body->setPosition(Vec2(newX, newY));
        }
        else if (_touches.size() == 2)
        {
            Vec2 pos0 = _touches[0]->getLocation();
            Vec2 pos1 = _touches[1]->getLocation();
            const float len = pos0.getDistance(pos1);
            float fScale = _scaleStart*len/_touchLength;
            setZoomScale(fScale);
        }
    }
}

void BookBody::onTouchEnded(Touch* touch, Event* event)
{
    auto touchIter = std::find(_touches.begin(), _touches.end(), touch);
    
    if (touchIter != _touches.end())
    {
        CCLOG("onTouchEnded touchPos x = %f, y = %f", touch->getLocation().x, touch->getLocation().y);
        _touches.erase(touchIter);
    }
    if (!boolCanTouch()) {
        return ;
    }
    if (_touches.size() == 0)
    {
        CCLOG("onTouchEnded touchPos _touches.size() = 0");
        if(panel_body->getScale() == 1.0f && BookDataManager::getInstance()->getReadMode() != eReadAuto)
        {
            Vec2 tmpPos = panel_center->convertToNodeSpace(touch->getLocation());
            float diff = tmpPos.x - m_fMove.x;
            if (isMoveDistanceValid(abs(diff)) &&  abs(diff) > _minSlide && !bDoubleClicked)
            {
                float diffY = tmpPos.y - m_fMove.y;
                if (abs(diff) > abs(diffY * 2)) {
                    if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
                    {
                        BookDataManager::getInstance()->setReadMode(eReadNone);
                        __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
                    }
                    if (diff > 0)
                    {
                        playPageTurnAction(iCurPage-1);
                    }
                    else
                    {
                        playPageTurnAction(iCurPage+1);
                    }
                }
            }
        }
        
        bDoubleClicked = false;
    }
    else if (_touches.size() == 1)
    {
        CCLOG("onTouchEnded touchPos _touches.size() = 1");
        _touchPoint     = panel_center->convertToNodeSpace(_touches[0]->getLocation());
        m_fMove = panel_center->convertToNodeSpace(touch->getLocation());
        _touchLength    = 0.0f;
        
        if(panel_body->getScale() < 1.1f && panel_body->getScale() > 0.9f)
        {
            setZoomScale(1.0f);
        }
    }
}

void BookBody::onTouchCancelled(Touch* touch, Event* event)
{
    auto touchIter = std::find(_touches.begin(), _touches.end(), touch);
    
    if (touchIter != _touches.end())
    {
        _touches.erase(touchIter);
    }
    
    if (_touches.size() == 0)
    {
        if(panel_body->getScale() == 1.0f)
        {
            Vec2 tmpPos = panel_center->convertToNodeSpace(touch->getLocation());
            float diff = tmpPos.x - m_fMove.x;
            if (isMoveDistanceValid(abs(diff)) &&  abs(diff) > _minSlide)
            {
                float diffY = tmpPos.y - m_fMove.y;
                if (abs(diff) > abs(diffY * 2)) {
                    
                    if (diff > 0)
                    {
                        playPageTurnAction(iCurPage-1);
                    }
                    else
                    {
                        playPageTurnAction(iCurPage+1);
                    }
                }
            }
        }
    }
    else if (_touches.size() == 1)
    {
        m_fMove = panel_center->convertToNodeSpace(touch->getLocation());
        _touchPoint     = m_fMove;
        _touchLength    = 0.0f;
        
        if(panel_body->getScale() < 1.1f)
        {
            setZoomScale(1.0f);
        }
    }
}

void BookBody::update(float dt)
{
}
void BookBody::refreshUIView()
{
}
void BookBody::endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json)
{
    eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
    if (nOp == eHttpType::pagelist)
    {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            std::string return_code = json.getValue("return_code");
            if (return_code != "6000") {
                std::string return_msg = json.getValue("return_msg");
                TipManager::getInstance()->showTipWithString(return_msg.c_str());
                return;
            }
            handleData(json);
            
            bEnroll = MAX(0, atoi(m_wholeBook.m_bookData->enroll.c_str())) == 1;
            iAllPage = atoi(m_wholeBook.m_bookData->page_count.c_str());
            
            BookGuideLayer->setWholeBookUpdateUI(m_wholeBook);
            switchBtnCollect(!m_wholeBook.m_favourite);
            btn_auto->setVisible(true);
            
            /////////////////
            ///解析后台记录的最后一页\口语页\答题页
            iLstPage = iCurPage;
            int iPage = 0;
            BookPagesVec::iterator iter;
            for (iter = m_wholeBook.m_bookPages->begin(); iter != m_wholeBook.m_bookPages->end(); iter++) {
                OnePage* onePage = *iter;
                if (m_wholeBook.m_bookData->last_read_page_id == onePage->m_pageID) {//上次记录
                    if(isDuBa == false){
                        iLstPage = iPage;
                    }
                }
                if(onePage->m_pageTitle == "Start Quiz")
                {
                    iOralPage = iPage;
//                    iAllPage = iPage;
                }
                if(onePage->m_pageTitle == "1/5")
                {
                    iQuizPage = iPage;
                }
                iPage++;
            }
            m_wholeBook.downAll(0);
            m_wholeBook.downAll(1);
            
            initPage(iCurPage);
            initInterPanel((Ref*)iCurPage);
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            std::string return_msg = json.getValue("return_msg");
            std::string return_code = json.getValue("return_code");
            TipManager::getInstance()->showTipWithString(return_msg.c_str());
        }
        else//"网络异常"
        {
        }
        if (NewUtilManager::getInstance()->getJumpQuiz())//liuwei, 返回做题 此处是等待数据回来, page才有数据, 回到最后一个测试题
        {
            NewUtilManager::getInstance()->setJumpQuiz(false);
            iLstPage = iAllPage-1;
            if (iLstPage < 0)
            {
                iLstPage = 0;
            }
            else
            {
                btn_auto->setEnabled(false);
                btn_auto->setColor(Color3B::GRAY);
            }
            doActionClose();
        }
    }
    else if (nOp == eHttpType::commonApplePayCheck)
    {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            bEnroll = true;
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
        }
        else
        {
        }
    }
    else if (nOp == eHttpType::userInfo)
    {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            std::string return_code = json.getValue("return_code");
            stuJson returnData = json.getJson("return_data");
            eVIPStatus m_vipStatus = (eVIPStatus)atoi(returnData.getValue("member_status").c_str());
            if(m_vipStatus == eVIPHigh)
            {
                bEnroll = true;
            }
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            CCLOG("%s", CDataHttp::getInstance()->GetHttpOpMsg(eHttpType::userInfo));
        }
        else
        {
            CCLOG("网络错误");
        }
    }
    else if (nOp == eHttpType::recognize)
    {
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            std::string return_code = json.getValue("return_code");
            if (return_code != "6000") {
                std::string return_msg = json.getValue("return_msg");
                TipManager::getInstance()->showTipWithString(return_msg.c_str());
                return;
            }
            
            handleScoreData(json,iCurPage);
            PageScore* pageScore = m_wholeBook.m_bookPages->at(iCurPage)->m_pageScore;
            
            if (m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageQuziOral &&
                QuizManager::getInstance()->getQuizType(m_wholeBook.m_bookPages->at(iCurPage)) == eQuizOral) {
                PageQuizBase* quiz = (PageQuizBase*)m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI;
                //                quiz->showOralResult();
                
                //打分后保存最高分到本地map，结算之前也许会用
                int iPage = m_wholeBook.m_bookPages->at(iCurPage)->m_pageID;
                m_mapNative[iPage] = pageScore->m_nativ;
                m_mapAcc[iPage] = pageScore->m_acc;
                m_mapFluency[iPage] = pageScore->m_fluency;
                m_mapScore[iPage] = pageScore->m_score;
            } else {
                //                pageInter->showOralResult();
            }
            
            if (m_recordLoadingNode && m_recordLoadingNode->isVisible()) {
                m_bRecordReady = true;
            } else {
                showRecordLoading(false);
                string return_msg;
                dealRecordScore(return_msg);
            }
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            std::string return_msg = json.getValue("return_msg");
            
            if (m_recordLoadingNode && m_recordLoadingNode->isVisible()) {
                m_recordLoadingNode->setName(return_msg);
                m_bRecordReady = true;
            } else {
                showRecordLoading(false);
                dealRecordScore(return_msg);
            }
        }
        else//"网络异常"
        {
            showRecordLoading(false);//关闭录音打分动画
            TipManager::getInstance()->showTipWithString("请检查网络");//add by hyp
        }
        btn_record->setEnabled(true);
        btn_record->setHighlighted(true);
        btn_record->stopAllActions();
        btn_record->setRotation(0.0f);
        btn_record->setScale(1.0f);
        bWaitRecognize = false;
    }
    else if (nOp == eHttpType::postevent_closebook)
    {
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            std::string return_code = json.getValue("return_code");
            if (return_code != "6000") {
                std::string return_msg = json.getValue("return_msg");
                TipManager::getInstance()->showTipWithString(return_msg.c_str());
                return;
            }
            
            HomePageManager::getInstance()->setBookReaded(Reador::getInstance()->getCurrentBook());
            
            stuJson returnData = json.getJson("return_data");
            if(Director::getInstance()->getRunningScene()->isScheduled("jump_to_report"))
            {
                Director::getInstance()->getRunningScene()->unschedule("jump_to_report");
                SceneManager::getInstance()->setGameState(eStateReport);
            }
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            std::string return_msg = json.getValue("return_msg");
            std::string return_code = json.getValue("return_code");
            TipLayer* boyMsg = nullptr;
            if (!TipManager::getInstance()->isOpenTip(eTipBoyWithMsg, boyMsg)) {
                boyMsg = TipManager::getInstance()->showTip(eTipBoyWithMsg, TipLayer::eActionType::eCenterScale);
            }
            BoyWithMsg* tip = (BoyWithMsg*)boyMsg;
            tip->setMsgInfo(eTipMsgRecognize, return_msg);
        }
        else//"网络异常"
        {
            
        }
    }
    else if (nOp == eHttpType::addfavouritecourse)
    {
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            std::string return_code = json.getValue("return_code");
            if (return_code != "6000") {
                std::string return_msg = json.getValue("return_msg");
                TipManager::getInstance()->showTipWithString(return_msg.c_str());
                return;
            }
            switchBtnCollect(false);
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            std::string return_msg = json.getValue("return_msg");
            std::string return_code = json.getValue("return_code");
            
            if (return_code == "6018") {
                switchBtnCollect(false);
            }else{
                TipManager::getInstance()->showTipWithString(return_msg.c_str());
            }
        }
        else//"网络异常"
        {
        }
    }
    else if (nOp == eHttpType::removefavouritecourse)
    {
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            std::string return_code = json.getValue("return_code");
            if (return_code != "6000") {
                std::string return_msg = json.getValue("return_msg");
                TipManager::getInstance()->showTipWithString(return_msg.c_str());
                return;
            }
            switchBtnCollect(true);
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            std::string return_msg = json.getValue("return_msg");
            std::string return_code = json.getValue("return_code");
            
            if (return_code == "6018") {
                switchBtnCollect(true);
            }else{
                TipManager::getInstance()->showTipWithString(return_msg.c_str());
            }
        }
        else//"网络异常"
        {
        }
    }
}

NodeGrid* BookBody::createPage(int iIndex)
{
    PageBase* page = nullptr;
    NodeGrid* grid = nullptr;
    SILog("取得页码", iIndex)
    if (m_wholeBook.m_mapPageViews.find(iIndex) != m_wholeBook.m_mapPageViews.end())
    {
        PageCache* cache = m_wholeBook.m_mapPageViews[iIndex];
        page = cache->m_pageUI;
        grid = cache->m_grid;
        
    }
    else//其他类型
    {
        OnePage* onePage = m_wholeBook.m_bookPages->at(iIndex);
        
        PageCache* pCache = new PageCache;
        pCache->m_pageIndex = iIndex;
        
        if (onePage->m_isQuiz) {//Quiz类型
            eQuizType quizType = QuizManager::getInstance()->getQuizType(onePage);
            if (quizType == eQuizOral) {
                pCache->m_pageType = ePageQuziOral;
                //封装封面页方式
                page = PageQuizOral::create(panel_quiz_oral->clone());
            } else {
                pCache->m_pageType = ePageQuziOther;
                //封装封面页方式
                 page = PageQuizTemp::create(panel_quiz_temp->clone(), quizType);
            }
        } else {
            if (onePage->m_pageTitle == "Cover Page")//封面
            {
                pCache->m_pageType = ePageCover;
                //封装封面页方式
                page = PageContent::create(panel_pdf->clone());
            }
            else if (onePage->m_pageTitle == "Copy Right")//版权
            {
                pCache->m_pageType = ePageCopy;
                //封装版权页方式
                page = PageContent::create(panel_pdf->clone());
            }
            else if (onePage->m_pageTitle == "Start Quiz")//Quiz开始
            {
                pCache->m_pageType = ePageQStart;
                if(isDuBa){
                    page = UIReadingFinish::create();
//                    this->setVisibleTopBar(false);
                }else{
                    page = PageQuizCover::create(panel_quiz_start->clone());
                }
            }
            else if (onePage->m_pageTitle == "Finish")//Quiz结束
            {
                pCache->m_pageType = ePageQEnd;
                page = PageQuizCover::create(panel_quiz_start->clone());
            }
            else//内容页
            {
                pCache->m_pageType = ePageContent;
                //封装正文页
                page = PageContent::create(panel_pdf->clone());
            }
        }
        
        if (iIndex == iOralPage - 1)
        {
            Widget* bg_widget = panel_content_bg->clone();
            bg_widget->setAnchorPoint(Vec2(0.5,0.5));
            bg_widget->setPosition(Vec2::ZERO);
            page->addChild(bg_widget, -1,2345);
        }
        
        //页面属性
        pCache->m_pageUI = page;
        page->setUserObject(__Integer::create(iIndex));
        page->loadPage(m_wholeBook, iIndex);
        page->setTag(123);
        
        //套一层NodeGrid播放翻页动画用
        grid = NodeGrid::create();
        grid->setUserObject(__Integer::create(iIndex));
        grid->setLocalZOrder(iAllPage - iIndex);
        grid->addChild(page);
        pCache->m_grid = grid;
        
        grid->setVisible(false);
        panel_body->addChild(grid);
        
        //设置grid区域
        Size size_page;
        Vec2 pos_page;
        
        if (pCache->m_pageType == ePageContent || pCache->m_pageType == ePageCopy || pCache->m_pageType == ePageCover)
        {
            size_page = Director::getInstance()->getVisibleSize();//panel_body->getContentSize();page->getPageSize()
            pos_page = Director::getInstance()->getVisibleSize() / 2;//panel_body->getPosition();
        } else {
            size_page = Director::getInstance()->getVisibleSize();
            pos_page = size_page / 2;
        }
        
        grid->setGridRect(Rect(pos_page.x - size_page.width/2, pos_page.y - size_page.height/2, size_page.width, size_page.height));
        
        m_wholeBook.m_mapPageViews[iIndex] = pCache;
    }
    return grid;
}

void BookBody::playPageTurnAction(int iTarget)
{
    if (pageInter->getInterState() != eInterClose || pageInter->bInterDoAnim()) {
        pageInter->stopInterAnim();
    }
    if (!m_bCanTurnPage)
    {
        SLog("翻页触发拦截")
        return;
    }
    
    if (iTarget<0 || iTarget>=iAllPage) {
        if (iCurPage+1 >= iAllPage) {//翻到头了，跳转报告页
            if (m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end()) {
                PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
                PageBase* curPage =  cache->m_pageUI;
                if (cache->m_pageType == ePageCopy ||
                    cache->m_pageType == ePageContent ||
                    cache->m_pageType == ePageQStart ||
                    cache->m_pageType == ePageQuziOral ||
                    cache->m_pageType == ePageQuziOther)
                {
                    curPage->stopAudio();
                }
                curPage->onTurnOut();
                iLastPage = iCurPage;
            }
            bForceExit = false;
            BookDataManager::getInstance()->setWholeBook(m_wholeBook);
            
            Director::getInstance()->getRunningScene()->scheduleOnce([this](float dt){
                this->dealScoreOutLog();
            }, 0.0f, "close_the_book");
            
            Director::getInstance()->getRunningScene()->scheduleOnce([this](float dt){
                SceneManager::getInstance()->setGameState(eStateReport);
            }, 1.0f, "jump_to_report");
        }
        return;
    }
    else
    {
        if (m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end()) {
            PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
            PageBase* curPage =  cache->m_pageUI;
            if (cache->m_pageType == ePageCopy ||
                cache->m_pageType == ePageContent ||
                cache->m_pageType == ePageQStart ||
                cache->m_pageType == ePageQuziOral ||
                cache->m_pageType == ePageQuziOther)
            {
                curPage->stopAudio();
            }
        }
    }
    
    bool bNext = true;//翻页方向默认向后翻页
    if (iTarget < iCurPage) {//如果减小，按前翻页处理
        bNext = false;
    }
    
    int iTurnPage = bNext?iCurPage:iTarget;
    //创建新的下一页
    NodeGrid* pTurnPage = createPage(iTurnPage);
    
    if (!m_wholeBook.haveSnd(iTarget)
        || !m_wholeBook.haveDat(iTarget))
    {
        string strPathSnd = m_wholeBook.getSoundPath(iTarget);
        string strPathXml = m_wholeBook.getXmlPath(iTarget);
        if(!FileUtils::getInstance()->isFileExist(strPathSnd.c_str())
           || !FileUtils::getInstance()->isFileExist(strPathXml.c_str()))
        {
            bInterReady = false;
            setInterVisible((Ref*)false);
        }
    }
    
    //翻页前如果是正文最后一页，显示背景框
    if (m_wholeBook.m_mapPageViews.find(iTurnPage) != m_wholeBook.m_mapPageViews.end()) {
        PageCache* cache = m_wholeBook.m_mapPageViews[iTurnPage];
        PageBase* curPage =  cache->m_pageUI;
//        Widget* widget_bg = (Widget*)curPage->getChildByTag(2345);
        auto widget_bg = curPage->getChildByTag(2345);
        if(widget_bg)
            widget_bg->setVisible(true);
    }
    
    bInterReady = false;
    m_bCanTurnPage = false;
    
    SoundManager::getInstance()->playSoundEffect("music/snd_turn.mp3");
    float fDuration = 0.5f;
    
    Sequence* action ;
    if (bNext) {
        //后页预备
        if(m_wholeBook.m_mapPageViews.find(iTarget) != m_wholeBook.m_mapPageViews.end())
        {
            PageCache* cache = m_wholeBook.m_mapPageViews[iTarget];
            NodeGrid* grid = cache->m_grid;
            grid->setVisible(true);
        }
        
        //        auto pageAction = EaseIn::create(PageTurn3D::create(fDuration, Size(32,24)), 2);
        auto pageAction = PageTurn3D::create(fDuration, Size(32,24));
        action = Sequence::create(
                                  CallFunc::create([=]()
                                                   {
                                                       
                                                   }),
                                  pageAction,
                                  StopGrid::create(),
                                  Hide::create(),
                                  CallFunc::create([=](){turnPageOver(iTarget);}),
                                  nullptr);
    } else {
        //        auto pageAction = EaseOut::create(ReverseTime::create(PageTurn3D::create(fDuration / 2, Size(32,24))), 3);
        auto pageAction = ReverseTime::create(PageTurn3D::create(fDuration, Size(32,24)));
        action = Sequence::create(
                                  Show::create(),
                                  pageAction,
                                  StopGrid::create(),
                                  CallFunc::create([=](){turnPageOver(iTarget);}),
                                  nullptr);
    }
    
    pTurnPage->runAction(action);
}

void BookBody::turnPageOver(int iIndex)
{
    //前页暂停音效
    if (m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end()) {
        PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
        PageBase* curPage =  cache->m_pageUI;
        if (cache->m_pageType == ePageCopy ||
            cache->m_pageType == ePageContent ||
            cache->m_pageType == ePageQStart ||
            cache->m_pageType == ePageQuziOral ||
            cache->m_pageType == ePageQuziOther)
        {
            curPage->stopAudio();
        }
        curPage->onTurnOut();
        iLastPage = iCurPage;
        NodeGrid* grid = cache->m_grid;
        grid->setVisible(false);
    }
    //翻页结束如果是正文最后一页，隐藏背景框
        if (m_wholeBook.m_mapPageViews.find(iIndex) != m_wholeBook.m_mapPageViews.end()) {
            PageCache* cache = m_wholeBook.m_mapPageViews[iIndex];
            PageBase* curPage =  cache->m_pageUI;
//            Widget* widget_bg = (Widget*)curPage->getChildByTag(2345);
            auto widget_bg = curPage->getChildByTag(2345);
            if(widget_bg)
                widget_bg->setVisible(false);
        }
    
    SILog("更新索引", iIndex)
    iCurPage = iIndex;
    iLstPage = iCurPage;
    if (bGuideOver && m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end()) {
        PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
        PageBase* page = cache->m_pageUI;
        page->onTurnIn();
    }
    
    m_bCanTurnPage = true;
    
    initPage(iCurPage);
    recyclePage();
    initInterPanel((Ref*)iCurPage);
    
    if(isDuBa && iCurPage == iOralPage){
        this->setVisibleTopBar(false);
//        ReadKingManager::getInstance()->reqReadKingReadEnd();
        MessageManager::getInstance()->postNotification(NSEventNameDef::ReadingEndShowAnim);
        this->dealScoreOutLog();
    }
    
    OnePage* onePage = m_wholeBook.m_bookPages->at(iCurPage);
    if (onePage->m_pageTitle == "Start Quiz")
    {
        btn_auto->setEnabled(false);
        btn_auto->setColor(Color3B::GRAY);
        m_wholeBook.m_bookData->read_flag = true;
    }
    else if (m_wholeBook.m_bookPages->size() > iCurPage + 1 && m_wholeBook.m_bookPages->at(iCurPage+1)->m_pageTitle == "Start Quiz")
    {
        btn_auto->setEnabled(true);
        btn_auto->setColor(Color3B::WHITE);
    }
}

void BookBody::initInterPanel(Ref* pIndex)
{
    if(bInterReady)
    {
        return;
    }
    int index = (intptr_t)pIndex;
    if(index == 0)
    {
        btn_left->setVisible(false);
    }else{
        btn_left->setVisible(true);
    }
    pageInter->setInterState(eInterClose);
    Size nodeSize = UiBase::getInstance()->getRunningScene()->getContentSize();
    SFLog("pageInter posx",pageInter->getPositionX())
    SFLog("pageInter posy",pageInter->getPositionY())
    SFLog("nodeSize posx",nodeSize.width/2)
    SFLog("nodeSize posy",nodeSize.height/2)
    pageInter->setPosition(Vec2(nodeSize.width/2, nodeSize.height/2));
    if (m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end()
        && m_wholeBook.haveSnd(iCurPage)
        && m_wholeBook.haveDat(iCurPage)
        )
    {
        string strPathSnd = m_wholeBook.getSoundPath(iCurPage);
        string strPathXml = m_wholeBook.getXmlPath(iCurPage);
        ePageType type = m_wholeBook.m_mapPageViews[iCurPage]->m_pageType;
        if (type == ePageContent)
        {
            btn_record = static_cast<cocos2d::ui::Button*>(Helper::seekWidgetByName(pageInter->getPageWidget(), "btn_record"));
            if(FileUtils::getInstance()->isFileExist(strPathSnd.c_str())
               && FileUtils::getInstance()->isFileExist(strPathXml.c_str()))
            {
                bInterReady = true;
                setInterVisible((Ref*)true);
                pageInter->loadPage(m_wholeBook, index);
            }
            else{
                setInterVisible((Ref*)false);
            }
        }
        else if (type == ePageQuziOral)
        {
            PageBase* oral = m_wholeBook.m_mapPageViews[index]->m_pageUI;
            btn_record = static_cast<cocos2d::ui::Button*>(Helper::seekWidgetByName(oral->getPageWidget(), "btn_record"));
            setInterVisible((Ref*)false);
        }
        else
        {
            setInterVisible((Ref*)false);
        }
    }else{
        setInterVisible((Ref*)false);
    }
}

void BookBody::resetWholeBook()
{
    m_wholeBook.m_folderPic = "";
    m_wholeBook.m_folderSnd = "";
    m_wholeBook.m_folderCnt = "";
    
    for (auto item : *m_wholeBook.m_bookPages)
    {
        CC_SAFE_DELETE(item);
    }
    m_wholeBook.m_bookPages->clear();
    
    m_wholeBook.m_mapPageViews.clear();
}

void BookBody::autoTurnPage(Ref* iPage)
{
    if ((intptr_t)iPage > iAllPage) {
        BookDataManager::getInstance()->setReadMode(eReadNone);
        return;
    }
    setZoomScale(1.0f);
    playPageTurnAction(iCurPage+1);
}

Vec2 BookBody::maxContainerOffset()
{
    Size pageSize = m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI->getPageSize();
    
    float contW       = pageSize.width * panel_body->getScaleX();
    float contH       = pageSize.height * panel_body->getScaleY();
    
    float realX = contW > _viewSize.width ? 0.5f * contW : (_bSeriour ? _viewSize.width/2 : _viewSize.width - 0.5f * contW);
    float realY = contH > _viewSize.height ? 0.5f * contH : (_bSeriour? _viewSize.height/2 : _viewSize.height - 0.5f * contH);
    return Vec2(realX, realY);
}

Vec2 BookBody::minContainerOffset()
{
    Size pageSize = m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI->getPageSize();
    
    float contW       = pageSize.width * panel_body->getScaleX();
    float contH       = pageSize.height * panel_body->getScaleY();
    
    float realX = contW <= _viewSize.width ? (_bSeriour? _viewSize.width/2 : 0.5f * contW) : _viewSize.width - 0.5f * contW;
    float realY = contH <= _viewSize.height ? (_bSeriour? _viewSize.height/2 : 0.5f * contH) : _viewSize.height - 0.5f * contH;
    
    return Vec2(realX, realY);
}


void BookBody::setZoomScale(float s)
{
    if (panel_body->getScale() != s)
    {
        panel_body->setScale(MAX(_minScale, MIN(_maxScale, s)));
        
        Vec2 minPos = minContainerOffset();
        Vec2 maxPos = maxContainerOffset();
        
        int tarX = panel_body->getPosition().x;
        int tarY = panel_body->getPosition().y;
        
        float newX     = MAX(minPos.x, MIN(maxPos.x, tarX));
        float newY     = MAX(minPos.y, MIN(maxPos.y, tarY));
        
        panel_body->setPosition(Vec2(newX, newY));
    }
}

void BookBody::turnNextPage()
{
    Node* baseNode = UiBase::getInstance()->getRunningScene();
    BookBody* layer = (BookBody*)baseNode->getChildByTag(eLayerBookBody);
    if(layer)
    {
        layer->setZoomScale(1.0f);
        if (layer->showQuizOkEffect()) {
            layer->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=](){
                layer->playPageTurnAction(layer->getCurPage() + 1);
            }), NULL));
        } else {
            layer->playPageTurnAction(layer->getCurPage() + 1);
        }
    }
}

void BookBody::turnPreviousPage()
{
    Node* baseNode = UiBase::getInstance()->getRunningScene();
    BookBody* layer = (BookBody*)baseNode->getChildByTag(eLayerBookBody);
    if(layer)
    {
        layer->setZoomScale(1.0f);
        if (layer->showQuizOkEffect()) {
            layer->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=](){
                layer->playPageTurnAction(layer->getCurPage() - 1);
            }), NULL));
        } else {
            layer->playPageTurnAction(layer->getCurPage() - 1);
        }
    }
}

void BookBody::initPage(int iPage)
{
    CCASSERT(0<= iPage && iPage < iAllPage, "Page Index Out of Range !!!");
    
    NodeGrid* pNextPage = createPage(iPage);
    pNextPage->setVisible(true);
    if (iPage > 0) {
        createPage(iPage-1);
    }
    if (iPage < iAllPage - 1) {
        createPage(iPage+1);
    }
}

void BookBody::recyclePage()
{
    //回收边界页
    if (iLastPage > -1)//曾经在某一页停留
    {
        for(int k = iLastPage - 1; k <= iLastPage+1 ; k++)
        {
            if (k >= 0 && k < iAllPage
                && abs(iCurPage - k) > 1
                && m_wholeBook.m_mapPageViews.find(k) != m_wholeBook.m_mapPageViews.end())
            {
                SILog("回收某一页", k)
                PageCache* cache = m_wholeBook.m_mapPageViews[k];
                cache->m_pageUI->freePage();
                cache->m_grid->removeFromParent();
                //                cache->m_pageUI->removeFromParentAndCleanup(true);
                //                cache->m_pageUI=nullptr;
                m_wholeBook.m_mapPageViews.erase(k);
                delete cache;
            }
        }
    }
}

void BookBody::reloadOnePage(Ref* iPage)
{
    m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI->loadPage(m_wholeBook, iCurPage);
}

void BookBody::setInterVisible(Ref* isVisible)
{
    bool bVisible = ((intptr_t)isVisible == 1);
    SILog("pageInter==>可见性",isVisible);
    pageInter->setVisible(bVisible);
    
    if(bVisible && BookDataManager::getInstance()->getReadMode() != eReadAuto)//自动翻页模式不引导
    {
        if(!NewHandTip::isShow("PageInteract1"))
        {
            NewHandTip::setShow("PageInteract1");
            auto node = NewHandTip::create2(UiBase::getInstance()->getRunningScene());
            node->addImg("newhand_bookRecord.png", btn_record, Vec2(10, 380));
            node->addImgAndClip("", btn_record, Vec2::ZERO, Vec2::ZERO, Size(580,200));
        }
    }
}

void BookBody::setAutoBtnState(Ref* isEnable)
{
    bool bEnable = ((intptr_t)isEnable == 1);
    btn_auto->setHighlighted(bEnable);
    
    if(!bEnable && m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end())//播放过程中，手动停止自动播放，停止播音
    {
        PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
        PageBase* curPage =  cache->m_pageUI;
        if (curPage->isAudioPlaying()) {
            curPage->stopAudio();
        }
        if(curPage->getActionByTag(12345) != nullptr)//清除该页的自动播放回调
        {
            curPage->stopActionByTag(12345);
        }
    }
}

void BookBody::onRecordTimeOut(Ref* ref)
{
    int iPage = (intptr_t)ref;
    if(pageInter && iPage == iCurPage)
    {
        pageInter->onRecordTimeOut();
    }
}

bool BookBody::OnBaseKeyBack()
{
    
    //BackClick::onBookQuit();
    Node* baseNode = UiBase::getInstance()->getRunningScene();
    NewHandTip* teach = (NewHandTip*)baseNode->getChildByTag(eLayerUserTeach);
    if(teach)
    {
        teach->removeFromParent();
        return true;
    }
    
    WordFind *diction = (WordFind*)pageInter->getPageWidget()->getChildByTag(eLayerBookDiction);
    if (diction) {
        diction->removeFromParent();
        return true;
    }
    this->onRevMsgDoHandle();
    return true;
//   add by hyp 2018/12/19
//    if(pageInter->getInterState() != eInterClose)
//    {
//        pageInter->doInterAnimation();
//        return true;
//    }
//    if(bGuideOver)
//    {
//        doActionOpen();
//    }
//    else
//    {
//        Game::sharedGame()->setGameState(eStateMain);
//        Director::getInstance()->getTextureCache()->removeUnusedTextures();
//    }
//    return true;
}

eAccessRight BookBody::canRead()
{
//    UserData& userData = Reador::getInstance()->getUserData();
//    if(m_wholeBook.m_bookData->isFree == "y" || bEnroll)
//    {
//        return eAccessFree;
//    }
//    if (Reador::getInstance()->getUserRight() == right_guest)
//    {
//        return eAccessGuest;
//    }
//    if (userData.m_validatyDate > 0)
//    {
//        return  eAccessTrial;
//    }
    return eAccessNone;
}

bool BookBody::checkReadTip()
{
    eAccessRight tmpRight = canRead();
    string contentTip = "",okTip = "",cancleTip = "";
    std::function<void()> okFun = nullptr,cancleFun = nullptr;
    
//    if (tmpRight == eAccessGuest) {
//        contentTip = "此书籍需付费后才能阅读，请登录账号或充值后再继续阅读。";
//        okTip = "登录";
//        cancleTip = "充值";
//        okFun = [](void){
//            Login::create(UiBase::getInstance()->getRunningScene());
//        };
//        cancleFun = [](void){
//            UserVIPCharge::create(UiBase::getInstance()->getRunningScene());
//        };
//    }
//    else if (tmpRight == eAccessNone) {
//        contentTip = "开通VIP会员才能阅读更多书籍哦";
//        okTip = "充值";
//        cancleTip = "取消";
//        okFun = [](void){
//            UserVIPCharge::create(UiBase::getInstance()->getRunningScene());
//        };
//    }
//    else if (tmpRight == eAccessTrial) {
//        contentTip = "会员过期不能看书了，快去充值吧";
//        okTip = "充值";
//        cancleTip = "取消";
//        okFun = [](void){
//            UserVIPCharge::create(UiBase::getInstance()->getRunningScene());
//        };
//    }
//    if(contentTip != ""){
//        BookRechargeTip* tip = BookRechargeTip::create();
//        tip->setTxtTip(contentTip,okTip,cancleTip);
//        tip->setBtnHandler(okFun,cancleFun);
//        getRootWidget()->addChild(tip,1457854);
//        return false;
//    }
    return true;
}

void BookBody::doActionOpen()
{
    talkingInterface::pageBegin("原版导读页");
    //setInterVisible((Ref*)false);
    bGuideOver = false;
    
    if (iCurPage >= 0 && m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end())
    {
        PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
        PageBase* curPage =  cache->m_pageUI;
        if (curPage->isAudioPlaying()) {
            curPage->stopAudio();
        }
        curPage->stopRecordAndPlayback();//add by dj
        if(pageInter && pageInter->isAudioPlaying()){
            pageInter->stopAudio();
        }
        if(Recorder::getInstance()->isPlayRecording())
        {
            Recorder::getInstance()->stopPlayRecord();
        }
    }
    
    BookGuideLayer->doActionOpen();
    
    if (!btn_auto->isEnabled())
    {
        btn_auto->setEnabled(true);
        btn_auto->setColor(Color3B::WHITE);
    }
}

void BookBody::doActionClose()
{
    BookGuideLayer->doActionClose();
    BookGuideLayer->getWidget("img_gde_bg")->runAction(Sequence::create(CallFunc::create([this](){
        SILog("当前页索引", iCurPage)
        SILog("上一页索引", iLstPage)
        if(iCurPage != iLstPage)
        {
            initPage(iLstPage);
            
            //后页预备
            if(m_wholeBook.m_mapPageViews.find(iLstPage) != m_wholeBook.m_mapPageViews.end())
            {
                PageCache* cache = m_wholeBook.m_mapPageViews[iLstPage];
                NodeGrid* grid = cache->m_grid;
                grid->setVisible(true);
            }
            //首页隐藏
            if(m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end())
            {
                PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
                NodeGrid* grid = cache->m_grid;
                grid->setVisible(false);
            }
        }
        //如果触发导读关闭动画，取消自动导读页的引导，防止错位
        if(this->isScheduled("newhandtip")){
            this->unschedule("newhandtip");
        }
    }),FadeOut::create(1.0f), CallFunc::create([this](){
        BookGuideLayer->setVisible(false);
        bGuideOver = true;
        talkingInterface::pageEnd("原版导读页");
        iCurPage = iLstPage;
        m_bCanTurnPage = true;
        bInterReady = false;
        initInterPanel((Ref*)iCurPage);
        
        OnePage* onePage = m_wholeBook.m_bookPages->at(iCurPage);
        if (btn_auto->isEnabled() && (onePage->m_pageTitle == "Start Quiz" || onePage->m_pageTitle == "1/5"))
        {
            btn_auto->setEnabled(false);
            btn_auto->setColor(Color3B::GRAY);
        }
        else if (!btn_auto->isEnabled() && onePage->m_pageTitle == "Cover Page")
        {
            btn_auto->setEnabled(true);
            btn_auto->setColor(Color3B::WHITE);
        }
        
        if (m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end()) {
            PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
            PageBase* curPage =  cache->m_pageUI;
            curPage->onTurnIn();
        }
        
        {
            //开书日志
            BookDataManager::getInstance()->setOpenTime(TimeManager::getInstance()->getCurTime());
            BehaviorManager::getInstance()->resetData();
            float speed = BookDataManager::getInstance()->getBookSoundSpeed();
            BehaviorManager::getInstance()->postEvent("course",
                                                      StringUtils::format(TEMPLETE_LOG_OPEN,
                                                                          m_wholeBook.m_bookData->id.c_str(),
                                                                          BookDataManager::getInstance()->getOpenTime(),
                                                                          Reador::getInstance()->getCurrentTask().c_str()
                                                                          ,speed), eHttpType::postevent_openbook);
        }
    }),NULL));
}

void BookBody::doSimplePlay()
{
    if (pageInter->getInterState() == eInterClose
        && pageInter->getActionByTag(123) == nullptr
        && m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end())
    {
        PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
        PageBase* curPage =  cache->m_pageUI;
        if (cache->m_pageType == ePageCover || cache->m_pageType == ePageContent)
        {
            curPage->playAudio();
        }
    }
}

void BookBody::dealScoreOutLog()
{
    //在书中的停留时长=关书-开书
    float fOpenTime = TimeManager::getInstance()->getCurTime() - BookDataManager::getInstance()->getOpenTime();
    if (m_wholeBook.m_bookData->sum_stay_time != fOpenTime) {
        m_wholeBook.m_bookData->sum_stay_time = fOpenTime;
    }
    SILog("停留时长", m_wholeBook.m_bookData->sum_stay_time);
    SLog("流利程度统计")
    int iTotal = 0;
    for (auto oral : m_mapFluency) {//顶多三道语音打分记录，而且只记录各自最高分
        SILog("流利程度->", oral.second)
        iTotal += oral.second;
    }
    SILog("流利程度 1= ", iTotal)
#if 0 // del by zzl for 图书报告页为本书当次成绩，如返回重新阅读或做题 会更新本次成绩，现问题为录音成绩没有重新做，应该保留上次的成绩
    m_wholeBook.m_bookData->cur_fluency = iTotal > 0?iTotal / 3 : 0;
    SILog("流利程度 2= ", m_wholeBook.m_bookData->cur_fluency)
#else
    int cur_fluency = iTotal > 0?iTotal / 3 : 0;
    SILog("流利程度 2= ", cur_fluency)
    m_wholeBook.m_bookData->cur_fluency = MAX(m_wholeBook.m_bookData->cur_fluency, cur_fluency);
    SILog("流利程度 3= ", m_wholeBook.m_bookData->cur_fluency)
#endif
    
    SLog("准确度统计")
    iTotal = 0;
    for (auto oral : m_mapAcc) {//顶多三道语音打分记录，而且只记录各自最高分
        SILog("准确度->", oral.second)
        iTotal += oral.second;
    }
    SILog("准确度 1= ", iTotal)
#if 0 // del by zzl
    m_wholeBook.m_bookData->cur_acc = iTotal > 0?iTotal / 3 : 0;
    SILog("准确度 2= ", m_wholeBook.m_bookData->cur_acc)
#else
    int cur_acc = iTotal > 0?iTotal / 3 : 0;
    SILog("准确度 2= ", cur_acc)
    m_wholeBook.m_bookData->cur_acc = MAX(m_wholeBook.m_bookData->cur_acc, cur_acc);
    SILog("准确度 3= ", m_wholeBook.m_bookData->cur_acc)
#endif
    
    SLog("本地化程度统计")
    iTotal = 0;
    for (auto oral : m_mapNative) {//顶多三道语音打分记录，而且只记录各自最高分
        SILog("本地化->", oral.second)
        iTotal += oral.second;
    }
    SILog("本地化 1= ", iTotal)
#if 0 // del by zzl
    m_wholeBook.m_bookData->cur_nativ = iTotal > 0?iTotal / 3 : 0;
    SILog("本地化 2= ", m_wholeBook.m_bookData->cur_nativ)
#else
    int cur_nativ = iTotal > 0?iTotal / 3 : 0;
    SILog("本地化 2= ", cur_nativ)
    m_wholeBook.m_bookData->cur_nativ = MAX(m_wholeBook.m_bookData->cur_nativ, cur_nativ);
    SILog("本地化 3= ", m_wholeBook.m_bookData->cur_nativ)
#endif
    //    "{\"course_id\":%s,\"sum_stay_time\":%ld,\"sum_listening_time\":%d,\"sum_recording_time\":%d,\"sum_nativ\":%d,\"sum_acc\":%d,\"sum_fluency\":%d,\"read_1\":%d,\"read_2\":%d,\"read_3\":%d,\"quiz_1\":%d,\"quiz_2\":%d,\"quiz_3\":%d,\"quiz_4\":%d,\"quiz_5\":%d,\"task_id\":\"%s\",\"correct_quiz_page\":\"%s\"}"
    
    CCASSERT(m_wholeBook.m_bookData->sum_recording_time < 172800, "Total record time in one book must short than 2days");
    CCASSERT(m_wholeBook.m_bookData->sum_listening_time < 172800, "Total listen time in one book must short than 2days");
    float speed = BookDataManager::getInstance()->getBookSoundSpeed();
    BehaviorManager::getInstance()->postEvent("course",
                                              StringUtils::format(TEMPLETE_LOG_EXIT2,
                                                                  m_wholeBook.m_bookData->id.c_str(),
                                                                  TimeManager::getInstance()->getCurTime()-BookDataManager::getInstance()->getOpenTime(),
                                                                  MAX(0, MIN(MIN(m_wholeBook.m_bookData->sum_stay_time, 6000), m_wholeBook.m_bookData->sum_listening_time)),
                                                                  MAX(0, MIN(MIN(m_wholeBook.m_bookData->sum_stay_time, 6000), m_wholeBook.m_bookData->sum_recording_time)),
                                                                  m_wholeBook.m_bookData->cur_nativ,
                                                                  m_wholeBook.m_bookData->cur_acc,
                                                                  m_wholeBook.m_bookData->cur_fluency,
                                                                  m_wholeBook.m_bookData->cur_read_1,
                                                                  m_wholeBook.m_bookData->cur_read_2,
                                                                  m_wholeBook.m_bookData->cur_read_3,
                                                                  m_wholeBook.m_bookData->cur_quiz_1,
                                                                  m_wholeBook.m_bookData->cur_quiz_2,
                                                                  m_wholeBook.m_bookData->cur_quiz_3,
                                                                  m_wholeBook.m_bookData->cur_quiz_4,
                                                                  m_wholeBook.m_bookData->cur_quiz_5,
                                                                  Reador::getInstance()->getCurrentTask().c_str(),
                                                                  m_wholeBook.m_bookData->correct_pages.c_str(),
                                                                  Reador::getInstance()->getCurrentTask().c_str()
                                                                  ,speed), eHttpType::postevent_closebook);
}

void BookBody::switchBtnCollect(bool bPositive)
{
    btn_collect->setVisible(true);
    if (bPositive) {
        string name = "Image/SubImg/Reading/btn_com_clt.png";
        btn_collect->loadTextures(name, name, name, Widget::TextureResType::PLIST);
        btn_collect->setActionTag(eBtnCollectAdd);
    } else {
        string name = "Image/SubImg/Reading/btn_com_clted.png";
        btn_collect->loadTextures(name, name, name, Widget::TextureResType::PLIST);
        btn_collect->setActionTag(eBtnCollectSub);
    }
    btn_collect->ignoreContentAdaptWithSize(true);
}

void BookBody::showRecordLoading(bool bShow)
{
    m_bRecordReady = false;
    if (!bShow) {
        if (m_recordLoadingNode) {
            m_recordLoadingNode->removeFromParent();
            m_recordLoadingNode = nullptr;
        }
        return;
    }
    if (m_recordLoadingNode == nullptr) {
        auto node = SkeletonAnimation::createWithJsonFile("spine/end_loading.json", "spine/end_loading.atlas",1.0f);
        auto root = getRootWidget();
        auto size = root->getContentSize();
        node->setPosition(Vec2(size.width / 2, size.height / 2));
        auto track = node->setAnimation(0, "stand", true);
        node->setTrackCompleteListener(track, [=](spTrackEntry* entry){
            int count = (intptr_t)m_recordLoadingNode->getUserData();
            count++;
            m_recordLoadingNode->setUserData((void *)count);
            if (count > 1) {
                m_recordLoadingNode->setVisible(true);
            }
            CCLOG("record loading end !!! count = %d", count);
            if (m_bRecordReady) {
                m_bRecordReady = false;
                
                m_recordLoadingNode->setVisible(false);
                m_recordLoadingNode->runAction(Sequence::create(DelayTime::create(0.01), CallFunc::create([=](){
                    m_recordLoadingNode->removeFromParent();
                    m_recordLoadingNode = nullptr;
                }), NULL));
                
                string return_msg = m_recordLoadingNode->getName();
                this->dealRecordScore(return_msg);
            }
        });
        root->addChild(node, 100);
        m_recordLoadingNode = node;
        m_recordLoadingNode->setUserData(nullptr);
        m_recordLoadingNode->setVisible(false);
        m_recordLoadingNode->runAction(Sequence::create(DelayTime::create(12), CallFunc::create([=](){
            this->showRecordLoading(false);
        }), NULL));
    }
}

bool BookBody::showQuizOkEffect()
{
    auto pPage = dynamic_cast<PageQuizTemp*>(m_wholeBook.m_mapPageViews[getCurPage()]->m_pageUI);
    if (pPage == nullptr) {
        return false;
    }
    return pPage->showQuizOkEffect();
}

void BookBody::dealRecordScore(string &return_msg)
{
    if (return_msg.length() < 1) {
        PageScore* pageScore = m_wholeBook.m_bookPages->at(iCurPage)->m_pageScore;
        if(pageScore->m_nuts)
        {
            SoundManager::getInstance()->playSoundEffect("music/snd_score.mp3");
        }
        if (iRecognizePageIdx != iCurPage) { //add by zzl, 如果请求打分结果的页数和当前页数对不上，直接丢弃上一次的成绩显示
            CCLOG("丢弃此次成绩显示 iCurPage=%d:%d", iCurPage, iRecognizePageIdx);
            return;
        }
        if (m_wholeBook.m_mapPageViews[iCurPage]->m_pageType == ePageQuziOral &&
            QuizManager::getInstance()->getQuizType(m_wholeBook.m_bookPages->at(iCurPage)) == eQuizOral) {
            PageQuizBase* quiz = (PageQuizBase*)m_wholeBook.m_mapPageViews[iCurPage]->m_pageUI;
            quiz->showOralResult();
        } else {
            pageInter->showOralResult();
        }
    } else {
        TipLayer* boyMsg = nullptr;
        if (!TipManager::getInstance()->isOpenTip(eTipBoyWithMsg, boyMsg)) {
            boyMsg = TipManager::getInstance()->showTip(eTipBoyWithMsg, TipLayer::eActionType::eCenterScale);
        }
        BoyWithMsg* tip = (BoyWithMsg*)boyMsg;
        tip->setMsgInfo(eTipMsgRecognize, return_msg);
        if(return_msg == "有进步哦~试试下一页吧")
        {
            SoundManager::getInstance()->playSoundEffect("music/snd_try.mp3");
        }
        else if(return_msg == "还需要再多读一些哦！")
        {
            SoundManager::getInstance()->playSoundEffect("music/snd_try.mp3");
        }
        else if(return_msg == "没关系，再试一次吧！")
        {
            SoundManager::getInstance()->playSoundEffect("music/snd_keepon.mp3");
        }
        else if(return_msg == "多练练，下次肯定会更好的！")
        {
            SoundManager::getInstance()->playSoundEffect("music/snd_keepon.mp3");
        }
        else
        {
            SoundManager::getInstance()->playSoundEffect("music/snd_easy.mp3");
        }
    }
}
//btnIndex:1阅读，2朗读，3答题，4录音秀，5next
bool BookBody::doBtnHandler(int btnIndex)
{
    if((btnIndex >= 1 && btnIndex <= 3) || btnIndex == 5){
        if(!checkReadTip())
        {
            return false;
        }
    }
    switch (btnIndex) {
        case 1:
            iLstPage = 0;
            for (auto& it : *m_wholeBook.m_bookPages)
            {
                if (atoi(it->m_pageTitle.c_str()) == 1)
                {
                    break;
                }
                else
                {
                    ++iLstPage;//liuwei 跳过版权页
                }
            }
            //iLstPage = iAllPage-1;//方便测试, 跳转到最后一页
            doActionClose();
            break;
        case 2:
            iLstPage = iOralPage;
            doActionClose();
            break;
        case 3:
            iLstPage = iQuizPage;
            doActionClose();
            break;
        case 4:
            RecordBookData::getInstance()->setEnroll(bEnroll);
            RecordBookData::getInstance()->setBookData(m_wholeBook);
            SceneManager::getInstance()->setGameState(eStateRecord);
            break;
        case 5:
            iLstPage = 0;
            doActionClose();
            break;
        default:
            break;
    }
    return true;
}
bool BookBody::boolCanTouch(){
    if(true == BookDataManager::getInstance()->getSpeedIsChange()){
        return false;//正在改变速度 模式不允许翻页 add by hyp
    }
    return true;
}

void BookBody::handleScoreData(const stuJson& jsonData,int iCurPage)
{
    stuJson returnData = jsonData.getJson("return_data");
    m_wholeBook.m_mapRecordResult[iCurPage] = returnData;
    
    //解析成绩
    OnePage* onePage = m_wholeBook.m_bookPages->at(iCurPage);
    PageScore* pageScore = m_wholeBook.m_bookPages->at(iCurPage)->m_pageScore;
    pageScore->m_dummy = MAX(atoi(returnData.getValue("dis_score").c_str()), 0);
    int iTmpScore = MAX(atoi(returnData.getValue("score").c_str()), 0);
    if(onePage->m_pageTitle == "1/3")
    {
        m_wholeBook.m_bookData->cur_read_1 = MAX(m_wholeBook.m_bookData->cur_read_1, iTmpScore);
    }
    else if(onePage->m_pageTitle == "2/3")
    {
        m_wholeBook.m_bookData->cur_read_2 = MAX(m_wholeBook.m_bookData->cur_read_2, iTmpScore);
    }
    else if(onePage->m_pageTitle == "3/3")
    {
        m_wholeBook.m_bookData->cur_read_3 = MAX(m_wholeBook.m_bookData->cur_read_3, iTmpScore);
    }
    pageScore->m_score = MAX(iTmpScore, pageScore->m_score);
    pageScore->m_nuts = MAX(atoi(returnData.getValue("nuts").c_str()), pageScore->m_nuts);
    int iNative = atoi(returnData.getValue("nativ").c_str());
    if (iNative <=0) {
        iNative = 20;
    } else if (iNative >=80){
        iNative = 100;
    }else{
        iNative += 20;
    }
    pageScore->m_nativ = MAX(iNative, pageScore->m_nativ);
    pageScore->m_acc = MAX(atoi(returnData.getValue("acc").c_str()), pageScore->m_acc);
    pageScore->m_fluency = MAX(atoi(returnData.getValue("fluency").c_str()), pageScore->m_fluency);
    
    //error node
    int word_num = returnData.GetArraySize("words");
    pageScore->m_errWords.clear();
    for (int i = 0; i < word_num; ++i)
    {
        const stuJson& word = returnData.GetArrayJson("words", i);
        
        int state = atoi(word.getValue("state").c_str());
        
        WordResult* oneWord = new WordResult;
        oneWord->w_id = atoi(word.getValue("id").c_str());
        oneWord->w_text = word.getValue("text");
        oneWord->w_state = state == -1 ? eWordDefault :((state == 0 || state == 1) ? eWordNormal : eWordWrong);
        
        /*
         phon:音素指标
         f    是否正确true/false
         gr    音素的字母符号
         ph    音素符号
         s    音素评分[-1.5 - 1.5]
         t    音素文字特征
         */
        int phon_num = word.GetArraySize("phon");
        int idx = 0;
        for (int j = 0; j < phon_num; ++j)
        {
            const stuJson& phon = word.GetArrayJson("phon", j);
            string wrong = phon.getValue("f");
            bool isWrong = (wrong == "1");
            
            string letters = phon.getValue("t");
            string::iterator itstr = letters.begin();
            for (; itstr != letters.end(); itstr++)
            {
                if (!isWrong) {
                    oneWord->w_bitvec.set(idx);
                }
                idx ++;
            }
        }
        pageScore->m_errWords.push_back(oneWord);
    }
}

void BookBody::handleData(const stuJson& jsonData)
{
    m_wholeBook.m_bookPages->clear();
    
    stuJson returnData = jsonData.getJson("return_data");
    stuJson course = returnData.getJson("course");
    
    m_wholeBook.m_bookData->page_count = course.getValue("page_count");
    m_wholeBook.m_bookData->id = course.getValue("course_id");
    m_wholeBook.m_bookData->idnumber = course.getValue("idnumber");
    m_wholeBook.m_bookData->name = course.getValue("name");
    m_wholeBook.m_bookData->intro = course.getValue("intro");
    m_wholeBook.m_bookData->cover = course.getValue("cover");
    m_wholeBook.m_bookData->keywords = course.getValue("keywords");
    m_wholeBook.m_bookData->topics = course.getValue("topics");
    m_wholeBook.m_bookData->read_flag = (course.getValue("read_page_flag") == "1");
    m_wholeBook.m_bookData->cohort = course.getValue("cohorts");
    m_wholeBook.m_bookData->word_count = course.getValue("word_count");
    m_wholeBook.m_bookData->last_read_page_id = MAX(0,atoi(course.getValue("last_read_page_id").c_str()));
    m_wholeBook.m_bookData->level = course.getValue("level");
    m_wholeBook.m_bookData->enroll = course.getValue("enroll");
    
    //    bEnroll = MAX(0, atoi(course.getValue("enroll").c_str())) == 1;
    
//    auto r = Reador::getInstance()->getUserRight();
//    if (r == userRight::right_guest && course.getValue("is_rm_guest") == "y") {
//        m_wholeBook.m_bookData->isFree = "y";
//    }
//    else if (r == userRight::right_normal && course.getValue("is_rm_free") == "y"){
//        m_wholeBook.m_bookData->isFree = "y";
//    }
//    else {
//        m_wholeBook.m_bookData->isFree = "n";
//    }
    
    if (course.getValue("is_rm_guest") == "y") {
        m_wholeBook.m_bookData->isFree = "y";
    }
    else {
        m_wholeBook.m_bookData->isFree = "n";
    }
    
    //////////////历史成绩////////////
    stuJson jsonScore = returnData.getJson("course_score");
    m_wholeBook.m_bookData->sum_stay_time = MAX(0, atoi(jsonScore.getValue("stay_time").c_str()));
    m_wholeBook.m_bookData->sum_read_nuts = MAX(0, atoi(jsonScore.getValue("read_nuts").c_str()));
    m_wholeBook.m_bookData->sum_quiz_nuts = MAX(0, atoi(jsonScore.getValue("quiz_nuts").c_str()));
    m_wholeBook.m_bookData->sum_nativ = MAX(0, atoi(jsonScore.getValue("nativ").c_str()));
    m_wholeBook.m_bookData->sum_acc = MAX(0, atoi(jsonScore.getValue("acc").c_str()));
    m_wholeBook.m_bookData->sum_fluency = MAX(0, atoi(jsonScore.getValue("fluency").c_str()));
    m_wholeBook.m_bookData->sum_status = (jsonScore.getValue("status") == "1");
    
    m_wholeBook.m_bookData->sum_read_1 = MAX(0, atoi(jsonScore.getValue("read_1").c_str()));
    m_wholeBook.m_bookData->sum_read_2 = MAX(0, atoi(jsonScore.getValue("read_2").c_str()));
    m_wholeBook.m_bookData->sum_read_3 = MAX(0, atoi(jsonScore.getValue("read_3").c_str()));
    if(m_wholeBook.m_bookData->sum_quiz_nuts >= 5)
    {
        m_wholeBook.m_bookData->sum_quiz_1 = 100;
        m_wholeBook.m_bookData->sum_quiz_2 = 100;
        m_wholeBook.m_bookData->sum_quiz_3 = 100;
        m_wholeBook.m_bookData->sum_quiz_4 = 100;
        m_wholeBook.m_bookData->sum_quiz_5 = 100;
    }else{
        m_wholeBook.m_bookData->sum_quiz_1 = MAX(0, atoi(jsonScore.getValue("quiz_1").c_str()));
        m_wholeBook.m_bookData->sum_quiz_2 = MAX(0, atoi(jsonScore.getValue("quiz_2").c_str()));
        m_wholeBook.m_bookData->sum_quiz_3 = MAX(0, atoi(jsonScore.getValue("quiz_3").c_str()));
        m_wholeBook.m_bookData->sum_quiz_4 = MAX(0, atoi(jsonScore.getValue("quiz_4").c_str()));
        m_wholeBook.m_bookData->sum_quiz_5 = MAX(0, atoi(jsonScore.getValue("quiz_5").c_str()));
    }
    
    //刷新UI
    //收藏状态
    m_wholeBook.m_favourite = (course.getValue("favourite_flag") == "y") ? true : false;
    
    stuJson url = returnData.getJson("url");
    m_wholeBook.m_folderPic = url.getValue("page_pic_folder");        //图片资源
    m_wholeBook.m_folderSnd = url.getValue("page_sound_folder");      //语音资源
    m_wholeBook.m_folderCnt = url.getValue("page_content_folder");    //数据资源
    
    //分享数据
    stuJson share = returnData.getJson("share");
    m_wholeBook.m_shareData.m_imgUrl = share.getValue("image_name");
    if (m_wholeBook.m_shareData.m_imgUrl.length() > 4) {
        m_wholeBook.m_shareData.m_imgUrl = url.getValue("share_image_folder") + m_wholeBook.m_shareData.m_imgUrl;
        m_wholeBook.m_shareData.m_subjects[0] = share.getValue("book_subject");
        string s = share.getValue("book_subjects");
        vector<string> out;
        StringUtil::splitString(s.c_str(), ',', out);
        int m = MIN(5, out.size());
        for(int i = 0; i < m; i++)
        {
            m_wholeBook.m_shareData.m_subjects[i + 1] = out[i];
        }
        DownImg::getInstance()->DownFile(m_wholeBook.m_shareData.m_imgUrl.c_str(), nullptr);
    }
    
    std::string strTmp;
    string::size_type pos;
    int iAllPage = atoi(m_wholeBook.m_bookData->page_count.c_str());
    for (int i = 0; i < iAllPage; i++) {
        stuJson page = returnData.GetArrayJson("pages", i);
        
        OnePage* onePage = new OnePage;
        onePage->m_pageID = atoi(page.getValue("page_id").c_str());
        onePage->m_pageTitle = page.getValue("page_title");
        onePage->m_pageMode = page.getValue("page_mode");
        
        onePage->m_pageName = StringUtils::format("%d", i);
        strTmp = page.getValue("pdf_pic");
        if (strTmp != "-1") {
            onePage->m_pdfPic = strTmp;
        }
        
        strTmp = page.getValue("pdf_sound");
        UiBase::StringReplace(strTmp, ".ogg", ".mp3");//使用mp3的资源 add by hyp
        if (strTmp != "-1") {
            onePage->m_pdfSound = strTmp;
        }
        
        strTmp = page.getValue("pdf_content");
        if (strTmp != "-1") {
            onePage->m_pdfContent = strTmp;
        }
        
        strTmp = page.getValue("properties");
        if (strTmp != "-1" && strTmp != "{}") {
            if (strTmp.at(1) == '\\') {
                strTmp = "\"" + strTmp + "\"";
                {
                    rapidjson::Document json_doc;
                    json_doc.Parse<0>(strTmp.c_str());
                    
                    if (json_doc.HasParseError())
                    {
                    }
                    else
                    {
                        strTmp = json_doc.GetString();
                    }
                }
            }
            stuJson tmpJson;
            CBaseLoad::LoadJsonString(tmpJson, strTmp);
            strTmp = tmpJson.getValue("p_reading_skills");
            if (strTmp != "-1") {
                onePage->m_properties = strTmp;
            } else {
                onePage->m_properties = "";
            }
        }
        
        onePage->m_sortIndex = atoi(page.getValue("sort_index").c_str());
        onePage->m_isQuiz = (page.getValue("is_quiz") == "y") ? true : false;
        onePage->m_parameters = page.getValue("parameters");
        CBaseLoad::LoadJsonString(onePage->m_paramJson, onePage->m_parameters);
        onePage->m_courseID = atoi(page.getValue("course_id").c_str());
        
        m_wholeBook.m_bookPages->push_back(onePage);
    }
    
    //////////////////////////
    string version = course.getValue("sync_time");
    if (version != "-1" && version != "0000-00-00 00:00:00") {
        DownImg::getInstance()->checkBookVersion(m_wholeBook.m_bookData->id, version);
    }
}

void BookBody::setVisibleTopBar(bool isShow)
{
    Widget* panel_root = (Widget*)getRootWidget();
    
    vector<string> vecBtnName = {"panel_head","btn_left","btn_right",};
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(panel_root, name);
        btn->setVisible(isShow);
    }
}

void BookBody::doBackHandle(bool isAllBack)
{
    //刷新声音速度  add by hyp
    __NotificationCenter::getInstance()->postNotification(MSG_SET_SOUND_SPEED, (Ref*)false);
    
    if (m_wholeBook.m_mapPageViews.find(iCurPage) != m_wholeBook.m_mapPageViews.end()) {
        PageCache* cache = m_wholeBook.m_mapPageViews[iCurPage];
        PageBase* curPage =  cache->m_pageUI;
        curPage->btnBack();
    }
    
    if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
    {
        BookDataManager::getInstance()->setReadMode(eReadNone);
        __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
    }
    Node *diction = (Node*)pageInter->getPageWidget()->getChildByTag(eLayerBookDiction);
    if(diction)
        diction->removeFromParent();
    
    pageInter->setInterState(eInterClose);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //add by zzl for stop play skeleton animation 播放按钮会一直播放，但是没有声音
    pageInter->stopWheelAnim(0);
#endif
    //如果触发导读关闭动画，取消自动导读页的引导，防止错位
    if(this->isScheduled("newhandtip")){
        this->unschedule("newhandtip");
    }
    
    showRecordLoading(false);//关闭录音打分动画
    
    if(bGuideOver && isAllBack == false)
    {
        setInterVisible((Ref*)false);
        doActionOpen();
    }
    else
    {
        if (MachineManager::getInstance()->getMachineData()->getFromMachine()) {//从人机练习跳过来的，打个标签 add by hyp
            MachineManager::getInstance()->getMachineData()->setFromMachine(false);
            MachineManager::getInstance()->runOnExitCallBack();
        }
        SceneManager::getInstance()->setGameState(eStateMain);
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
        talkingInterface::pageEnd("原版书");
    }
}

void BookBody::onRevMsgDoHandle(Ref* ref)
{
//    msg_param_readking* param = (msg_param_readking*)ref;
//    printf("onRevMsgDoHandle=====%d\n",param->_type);
//    if(param->_type == msg_param_readking::eTYPE::RK_READING_BACK){
//        doBackHandle(true);
//    }
//    else if(param->_type == msg_param_readking::eTYPE::RK_READING_AGAIN){
//        setVisibleTopBar(true);
//        doBackHandle(false);
//
//        iLstPage = 0;
//    }
    
    
    MsgParam* param = (MsgParam*)ref;
    
    setVisibleTopBar(true);
    doBackHandle(false);

    iLstPage = 0;
}
