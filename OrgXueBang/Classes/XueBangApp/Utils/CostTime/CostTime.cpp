//
//  CostTime.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/10/18.
//
//

#include "CostTime.hpp"
#include "cocos2d.h"
USING_NS_CC;

static timeval t_costTimeVal;

static std::unordered_map<std::string, timeval>     m_mapTime;

void CostTime_Start()
{
    ::gettimeofday( &t_costTimeVal, NULL);
}

void CostTime_End(const char* express)
{
    struct timeval now;
    ::gettimeofday(&now, NULL);
    
    unsigned long long used = (now.tv_sec - t_costTimeVal.tv_sec)*1000 + (now.tv_usec - t_costTimeVal.tv_usec)/1000;
    
    static char costtime[256];
    snprintf(costtime,256,"CostTime: %s ; time = %lld(mics)",express,used);
    log("%s",costtime);
    
    ::gettimeofday(&t_costTimeVal, NULL);
}

void CostTime_Tag(const char* tag, const char* tip)
{
    timeval t_now;
    ::gettimeofday( &t_now, NULL);
    if(m_mapTime.find(tag) != m_mapTime.end() && strcmp(tip, "") != 0)
    {
        
        unsigned long long used = (t_now.tv_sec - m_mapTime[tag].tv_sec)*1000 + (t_now.tv_usec - m_mapTime[tag].tv_usec)/1000;
        
        static char costtime[256];
        snprintf(costtime,256,"===CostTime===【%s】 time = %lld(mics)@%s", tag, used, tip);
        log("%s",costtime);
        
    }
    m_mapTime[tag] = t_now;
}
