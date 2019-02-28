//
//  BehaviorManager.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/2/22.
//
//

#ifndef BehaviorManager_hpp
#define BehaviorManager_hpp

#include "stdafx.h"
#include "CDataHttp.hpp"

enum eBehaviorType
{
    eBehaviorOpen = 0,
    eBehaviorExit,
    eBehaviorCover,
    eBehaviorRead,
    eBehaviorQuiz,
};

//开始读书
#define TEMPLETE_LOG_OPEN   "{\"course_id\":%s,\"open_time\":%ld,\"open_position\":\"rosen\",\"read_type\":\"rosen\",\"task_id\":\"%s\",\"audio_speed\":\"%.2f\"}"

//结束读书
#define TEMPLETE_LOG_EXIT   "{\"course_id\":%s,\"open_time\":%ld,\"close_time\":%ld,\"read_type\":\"rosen\",\"task_id\":\"%s\",\"audio_speed\":\"%.2f\"}"
#define TEMPLETE_LOG_EXIT2   "{\"course_id\":%s,\"sum_stay_time\":%ld,\"sum_listening_time\":%d,\"sum_recording_time\":%d,\"nativ\":%d,\"acc\":%d,\"fluency\":%d,\"read_1\":%d,\"read_2\":%d,\"read_3\":%d,\"quiz_1\":%d,\"quiz_2\":%d,\"quiz_3\":%d,\"quiz_4\":%d,\"quiz_5\":%d,\"task_id\":\"%s\",\"correct_quiz_page\":\"%s\",\"task_id\":\"%s\",\"audio_speed\":\"%.2f\"}"

//普通页面
#define TEMPLETE_LOG_COVER  "{\"course_id\":%s,\"page_id\":%d,\"open_time\":%ld,\"close_time\":%ld,\"page_title\":\"%s\",\"page_mode\":\"%s\",\"task_id\":\"%s\",\"audio_speed\":\"%.2f\"}"

//正文读书
#define TEMPLETE_LOG_PAGE   "{\"course_id\":%s,\"page_id\":%d,\"open_time\":%ld,\"close_time\":%ld,\"page_title\":\"%s\",\"page_mode\":\"%s\",\"listening_time\":%d,\"recording_time\":%d,\"read_score\":%d,\"nativ\":%d,\"acc\":%d,\"fluency\":%d,\"task_id\":\"%s\",\"audio_speed\":\"%.2f\"}"

//语音Quiz
#define TMPPLETE_LOG_READ   "{\"course_id\":%s,\"page_id\":%d,\"open_time\":%ld,\"close_time\":%ld,\"page_title\":\"%s\",\"page_mode\":\"%s\",\"listening_time\":%d,\"recording_time\":%d,\"read_score\":%d,\"nativ\":%d,\"acc\":%d,\"fluency\":%d,\"is_quiz\":\"y\",\"page_type\":\"read\",\"task_id\":\"%s\",\"audio_speed\":\"%.2f\"}"

//测试读书
#define TMPPLETE_LOG_QUIZ   "{\"course_id\":%s,\"page_id\":%d,\"open_time\":%ld,\"close_time\":%ld,\"page_title\":\"%s\",\"page_mode\":\"%s\",\"quiz_score\":%d,\"item_skill\":\"%s\",\"is_quiz\":\"y\",\"page_type\":\"quiz\",\"task_id\":\"%s\"}"

//教材开书日志
#define TEMPLETE_TXT_OPEN   "{\"textbook_id\":%s,\"open_time\":%ld,\"open_position\":\"textbook\",\"read_type\":\"textbook\",\"task_id\":\"%s\"}"

//教材关书日志
#define TEMPLETE_TXT_EXIT   "{\"textbook_id\":%s,\"open_time\":%ld,\"close_time\":%ld,\"read_type\":\"textbook\",\"task_id\":\"%s\"}"

//教材空白页日志
#define TEMPLETE_TXT_EMPTY   "{\"textbook_id\":%s,\"page_id\":%s,\"open_time\":%ld,\"close_time\":%ld,\"page_title\":\"%s\",\"mode\":\"%s\",\"task_id\":\"%s\"}"

//教材读书页日志
#define TEMPLETE_TXT_READ   "{\"textbook_id\":%s,\"page_id\":%s,\"open_time\":%ld,\"close_time\":%ld,\"page_title\":\"%s\",\"mode\":\"%s\",\"listening_time\":%d,\"recording_time\":%d,\"read_score\":%d,\"nativ\":%d,\"acc\":%d,\"fluency\":%d,\"task_id\":\"%s\"}"



class BehaviorManager : public Ref, public CDataHttpDelegate
{
public:
    BehaviorManager();
    virtual ~BehaviorManager();
    static BehaviorManager* getInstance();
    
    virtual void endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json) override;
    
    void postEvent(const string  type, const string log, eHttpType netOp = eHttpType::postevent);
    
    void resetData()
    {
        event_id = "";
        bReady = false;
    }
    string getEventID()
    {
        return event_id;
    }
private:
    string event_id;
    bool bReady;
    int iPostCount;
};


#endif /* BehaviorManager_hpp */
