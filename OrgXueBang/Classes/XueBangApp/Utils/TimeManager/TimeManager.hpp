//
//  TimeManager.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/2/18.
//
//

#ifndef TimeManager_hpp
#define TimeManager_hpp

#include "PreIncludes.hpp"

class TimeManager : public Ref
{
private:
    TimeManager();
public:
    ~TimeManager();
    static TimeManager* getInstance();
    
    time_t getCurTime()
    {
        time_t t= m_currentTime+(int)m_duration;
        return t;
    }
    void setSerTime(time_t time)
    {
        m_currentTime = time;
        m_duration = 0;
        m_hasSyncTimeSuccess = true;
    }
    
    bool hasSyncTimeSuccess()
    {
        return m_hasSyncTimeSuccess;
    }
    
    void update(float dt);
    void stopUpdate();
    void startUpdate();

    void resetOnlineTime();
    bool isRestTipsEnable();
private:
    time_t m_currentTime;
    float m_duration;
    bool m_hasSyncTimeSuccess;
    
    bool m_over30Mins;
    float m_onlineTime;
};

#endif /* TimeManager_hpp */
