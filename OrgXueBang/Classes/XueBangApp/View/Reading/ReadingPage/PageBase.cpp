//
//  PageBase.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#include "PageBase.hpp"
#include "BehaviorManager.hpp"
#include "Recorder.h"
#include "BookBody.hpp"
//#include "BookScene.hpp"
#include "Reador.hpp"
#include "TimeManager.hpp"

#define UI_ACTION_INTERVAL  500 // 毫秒

PageBase::PageBase()
:m_pageIndex(-1)
,m_soundID(AudioEngine::INVALID_AUDIO_ID)
,bListening(false)
,fListenTime(0.0f)
,bRecording(false)
,fRecordTime(0.0f)
,m_pageSize(Size::ZERO)
{
    
}

PageBase::~PageBase()
{
    
}

bool PageBase::init()
{
    bool bRet = Layer::init();
    ::gettimeofday(&m_actionTime, NULL);
    return bRet;
}

void PageBase::onEnter()
{
    Layer::onEnter();
}

void PageBase::onExit()
{
    Layer::onExit();
    if(m_soundID != AudioEngine::INVALID_AUDIO_ID)
    {
        SoundManager::getInstance()->stopSound(m_soundID);
    }
}

bool PageBase::load(Widget* pageUI)
{
    Widget* gameUI = pageUI;
    pageUI->setAnchorPoint(Vec2(0.5f, 0.5f));
    pageUI->setPosition(Vec2::ZERO);
    
    m_pageWidget = gameUI;
    if(gameUI->getParent())
    {
        return true;
    }
    
    this->addChild(gameUI,1);
    
    addEvents();
    
    return gameUI != NULL;
}

bool PageBase::isForbiddenAction(Ref* sender,  cocos2d::ui::Widget::TouchEventType type)
{
    struct timeval now;
    ::gettimeofday(&now, NULL);
    
    unsigned long long used = (now.tv_sec - m_actionTime.tv_sec)*1000 + (now.tv_usec - m_actionTime.tv_usec)/1000;
    
    if(used < UI_ACTION_INTERVAL || cocos2d::ui::Widget::TouchEventType::ENDED != type){
        return true;
    }else{
        ::gettimeofday(&m_actionTime, NULL);
        return false;
    }
}

void PageBase::cleanup()
{
    Layer::cleanup();
}

void PageBase::setAutoPlay()
{
    ILog(m_pageIndex)
    if (m_wholeBook->haveSnd(m_pageIndex)) {
        Sequence* seqPlay = Sequence::create(DelayTime::create(1.5f), CallFunc::create([=](){m_soundID = playAudio([this](int id, const std::string &file){
            ILog(id)
            SLog(file.c_str())
            m_soundID = AudioEngine::INVALID_AUDIO_ID;
            //自动翻页模式通知下一页
            if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
            {
                __NotificationCenter::getInstance()->postNotification(MSG_TURN_PAGE, (Ref*)(intptr_t)(m_pageIndex + 1));
            }
            __NotificationCenter::getInstance()->postNotification(MSG_PAGE_AUTOPLAY, (Ref*)false);
        });
            if(m_soundID!= AudioEngine::INVALID_AUDIO_ID){//判断播放是否成功 add by hyp  2018/12/26
                __NotificationCenter::getInstance()->postNotification(MSG_PAGE_AUTOPLAY, (Ref*)true);
            }
        }), NULL);
        seqPlay->setTag(12345);
        this->runAction(seqPlay);
    } else {
        Sequence* seqOther = Sequence::create(DelayTime::create(2.0f), CallFunc::create([=](){
            if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
            {
                __NotificationCenter::getInstance()->postNotification(MSG_TURN_PAGE, (Ref*)(intptr_t)(m_pageIndex + 1));
            }
        }) ,NULL);
        seqOther->setTag(12345);
        this->runAction(seqOther);
    }
}

void PageBase::onTurnOut()
{
    ePageType type = m_wholeBook->m_mapPageViews[m_pageIndex]->m_pageType;
    //停止回放
    if(Recorder::getInstance()->isPlayRecording())
    {
        Recorder::getInstance()->stopPlayRecord();
        Node* nodeBase = UiBase::getInstance()->getRunningScene();
        BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);
        if(bookBody)
            bookBody->setReplayComplete(false);
    }
    if(Recorder::getInstance()->isRecording())
    {
        m_wholeBook->m_mapPageViews[m_pageIndex]->m_pageUI->setRecordTime(false);
        Recorder::getInstance()->stopRecoder();
        //标记不完整录音
        Node* nodeBase = UiBase::getInstance()->getRunningScene();
        BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);
        if(bookBody)
            bookBody->setRecordComplete(false);
    }
    
    //翻出页，停止播放音效
    if(m_soundID != AudioEngine::INVALID_AUDIO_ID)
    {
        m_wholeBook->m_mapPageViews[m_pageIndex]->m_pageUI->setListenTime(false);
        SoundManager::getInstance()->stopSound(m_soundID);
    }
    
    //单页成绩
    PageScore* pageScore = m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore;
    float speed = BookDataManager::getInstance()->getBookSoundSpeed();
    //日志相关
    if(BehaviorManager::getInstance()->getEventID() == ""
       || BehaviorManager::getInstance()->getEventID() == "-1")//开书日志异常
    {
        BehaviorManager::getInstance()->postEvent("course",
                                                  StringUtils::format(TEMPLETE_LOG_OPEN,
                                                                      m_wholeBook->m_bookData->id.c_str(),
                                                                      BookDataManager::getInstance()->getOpenTime(),
                                                                      Reador::getInstance()->getCurrentTask().c_str()
                                                                      ,speed), eHttpType::postevent_openbook);
    }
    else
    {
        if (type == ePageContent) {
            BehaviorManager::getInstance()->postEvent("page",
                                                      StringUtils::format(TEMPLETE_LOG_PAGE,
                                                                          m_wholeBook->m_bookData->id.c_str(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageID,
                                                                          getOpenTime(),
                                                                          TimeManager::getInstance()->getCurTime(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageTitle.c_str(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageMode.c_str(),
                                                                          MAX(0, MIN(600,pageScore->m_listent)),//最长时限
                                                                          MAX(0, MIN(600,pageScore->m_reocrd)),//最长时限
                                                                          pageScore->m_score,
                                                                          pageScore->m_nativ,
                                                                          pageScore->m_acc,
                                                                          pageScore->m_fluency,
                                                                          Reador::getInstance()->getCurrentTask().c_str()
                                                                          ,speed));
        }
        else if (type == ePageQuziOral)
        {
            BehaviorManager::getInstance()->postEvent("page",
                                                      StringUtils::format(TMPPLETE_LOG_READ,
                                                                          m_wholeBook->m_bookData->id.c_str(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageID,
                                                                          getOpenTime(),
                                                                          TimeManager::getInstance()->getCurTime(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageTitle.c_str(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageMode.c_str(),
                                                                          MAX(0, MIN(600,pageScore->m_listent)),
                                                                          MAX(0, MIN(600,pageScore->m_reocrd)),
                                                                          pageScore->m_score,
                                                                          pageScore->m_nativ,
                                                                          pageScore->m_acc,
                                                                          pageScore->m_fluency,
                                                                          Reador::getInstance()->getCurrentTask().c_str()
                                                                          ,speed));
        }
        else if (type == ePageQuziOther)
        {
            BehaviorManager::getInstance()->postEvent("page",
                                                      StringUtils::format(TMPPLETE_LOG_QUIZ,
                                                                          m_wholeBook->m_bookData->id.c_str(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageID,
                                                                          getOpenTime(),
                                                                          TimeManager::getInstance()->getCurTime(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageTitle.c_str(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageMode.c_str(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_score,
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_properties.c_str(),
                                                                          Reador::getInstance()->getCurrentTask().c_str()
                                                                          ,speed));
        }
        else// if (type == ePageQStart || type == ePageQEnd || type == ePageCopy || type == ePageCover)
        {
            float speed = BookDataManager::getInstance()->getBookSoundSpeed();
            BehaviorManager::getInstance()->postEvent("page",
                                                      StringUtils::format(TEMPLETE_LOG_COVER,
                                                                          m_wholeBook->m_bookData->id.c_str(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageID,
                                                                          getOpenTime(),
                                                                          TimeManager::getInstance()->getCurTime(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageTitle.c_str(),
                                                                          m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageMode.c_str(),
                                                                          Reador::getInstance()->getCurrentTask().c_str(),
                                                                          speed
                                                                          ));
        }
    }
    
    //如果是Quiz也，复位坚果
    /*
     if (type == ePageQuziOther)
     {
     __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_NUT, (Ref*)(intptr_t)0);
     }
     */
    pageScore->clearScore();
}
