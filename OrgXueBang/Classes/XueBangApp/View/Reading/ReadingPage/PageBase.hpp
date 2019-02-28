//
//  PageBase.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#ifndef PageBase_hpp
#define PageBase_hpp

#include "PreIncludes.hpp"
#include "BookStruct.hpp"
#include "TimeManager.hpp"
#include "BookDataManager.hpp"
#include "GameMacro.hpp"
#include "SoundManager.hpp"
#include "UIDef.hpp"
#include "stdafx.h"


class PageBase : public Layer
{
public:
    PageBase();
    virtual ~PageBase();
    
    CREATE_FUNC(PageBase);
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool load(Widget* pageUI);
    
    // 基类继承于此， 通过这个挂接事件
    virtual void addEvents(){};
    
    //新需求，根据范读的时长限定听和录的时间上限，很讨厌在计时器中加判断，但是为了从根源上保证数据，就牺牲cpu吧
    virtual void update(float dt)
    {
        if (bListening) {
            //既然可以播放范读，说明拿到了音频和歌词，解析到了范读时间
            if(fListenTime >= m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_modelTime)
            {
                fListenTime = m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_modelTime;
            }
            else
            {
                fListenTime += dt;
            }
        }
        else if (bRecording)//听和录不能同时发生
        {
            //录音时长控制在范读的三倍，既然开始了录音，就拿到了歌词，有了时间上限
            if(fRecordTime >= m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_modelTime * fRecordRate + fRecordDelay)
            {
                fRecordTime = m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_modelTime * fRecordRate + fRecordDelay;
                __NotificationCenter::getInstance()->postNotification(MSG_RECORD_TIMEOUT, (Ref*)m_pageIndex);
            }
            else
            {
                fRecordTime += dt;
            }
        }
    };
    
    Widget* getPageWidget(){
        return m_pageWidget;
    }
    bool isForbiddenAction(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    virtual void cleanup();
    
    virtual void loadPage(WholeBook& wholeBook, int index)
    {
        SILog("加载页", index)
        m_wholeBook = &wholeBook;
        m_pageIndex = index;
    };
    virtual void freePage()
    {
        m_pageIndex = -1;
        m_soundID = AudioEngine::INVALID_AUDIO_ID;
        m_wholeBook = nullptr;
    };
    virtual int  playAudio(std::function<void (int, const std::string &)> finishCall = nullptr)
    {
        if(m_soundID != AudioEngine::INVALID_AUDIO_ID)
        {
            SoundManager::getInstance()->stopSound(m_soundID);
            m_soundID = AudioEngine::INVALID_AUDIO_ID;
        }
        return AudioEngine::INVALID_AUDIO_ID;
    };
    virtual void setAutoPlay();
    virtual void stopAudio(){ m_soundID = AudioEngine::INVALID_AUDIO_ID; };
    virtual void stopRecordAndPlayback(){};//add by dj
    virtual void showOralResult(){};
    
    virtual void btnBack(){};
    
    void getWholeBook(WholeBook*& pWholeBook, int& index){pWholeBook = m_wholeBook; index = m_pageIndex;};
    
    virtual void onTurnIn()
    {
        m_openTime = TimeManager::getInstance()->getCurTime();
        //计时统计复位
        bListening = false;
        fListenTime = 0.0f;
        bRecording = false;
        fRecordTime = 0.0f;
        
        PageCache* cache = m_wholeBook->m_mapPageViews[m_pageIndex];
        PageBase* page = cache->m_pageUI;
        if (BookDataManager::getInstance()->getReadMode() == eReadAuto) {
            if (cache->m_pageType >= ePageQStart) {//自动播到语音quiz，停止连播
                BookDataManager::getInstance()->setReadMode(eReadNone);
                __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
            }else{
                page->setAutoPlay();
            }
        } else {
            //quiz类型自动播放语音
            if ((cache->m_pageType == ePageCover /*||cache->m_pageType == ePageContent*/
                 || cache->m_pageType == ePageQuziOral || cache->m_pageType == ePageQuziOther) && m_wholeBook->haveSnd(m_pageIndex)) {
                page->playAudio();
            }
        }
    };
    
    virtual void onTurnOut();
    
    Size getPageSize()
    {
        return m_pageSize;
    }
    void setPageSize(Size mSize)
    {
        m_pageSize = mSize;
    }
protected:
    Widget*    m_pageWidget;
    timeval m_actionTime;
    
    WholeBook* m_wholeBook;
    int     m_pageIndex;
    int     m_soundID;
    
    float fListenTime;
    float fRecordTime;
    
    bool bListening;
    bool bRecording;
    
    Size m_pageSize;
public:
    time_t m_openTime;
    time_t getOpenTime()
    {
        //        CCASSERT(m_openTime > 0, "Open Time Must Be a Real Time !!!");
        if(m_openTime < 1515568718)
            m_openTime = (TimeManager::getInstance()->getCurTime());
        return m_openTime;
    }
    void setOpenTime(time_t mTime)
    {
        m_openTime = mTime;
    }
    
    void setListenTime(bool bStart)
    {
        if(bListening && !bStart)//计时开始，统计复位
        {
            m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_listent += ceil(fListenTime);
            m_wholeBook->m_bookData->sum_listening_time += ceil(fListenTime);
            SFLog("统计Listen===统计单句时长", fListenTime)
            SFLog("统计Listen===取得单句时长", m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_modelTime)
            SILog("统计Listen===当页播放时长", m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_listent)
            SILog("统计Listen===本书播放时长", m_wholeBook->m_bookData->sum_listening_time)
            
            fListenTime = 0;
        }
        bListening = bStart;
    }
    
    void setRecordTime(bool bStart)
    {
        if(bRecording && !bStart)//计时开始，统计复位
        {
            m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_reocrd += ceil(fRecordTime);
            m_wholeBook->m_bookData->sum_recording_time += ceil(fRecordTime);
            SFLog("统计Record===统计单句时长", fRecordTime)
            SFLog("统计Record===默认单句时长", m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_modelTime)
            SILog("统计Record===该页录音时长", m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_reocrd)
            SILog("统计Listen===本书录音时长", m_wholeBook->m_bookData->sum_recording_time)
            fRecordTime = 0;
        }
        bRecording = bStart;
    }
    
    bool isAudioPlaying()
    {
        return m_soundID != AudioEngine::INVALID_AUDIO_ID;
    }
    //录音超时-强制停止录音
    virtual void onRecordTimeOut(){};
private:
    float fRecordRate = 3.0f;//录音时间倍率
    float fRecordDelay = 3.0f;//录音时间延迟
    
};

#endif /* PageBase_hpp */
