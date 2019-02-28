//
//  Date.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/2/18.
//
//

#include "Date.hpp"

#define BASEYEAR 1900

#define YEAR_INT64 10000000000
#define MONTH_INT64 100000000
#define DAY_INT64 1000000
#define HOUR_INT64 10000
#define MIN_INT64 100

Date::Date() : currentTime(0)
{
    init();
}

Date* Date::date()
{
    Date* ret = new Date();
    ret->autorelease();
    return ret;
}

void Date::init()
{
    time(&currentTime);
//    log("Date_time init %s ", toString());
}

char* Date::toString()
{
    struct tm * t = localtime(&currentTime);
    if(t){
        return asctime(t);
    }else{
        return NULL;
    }
}

time_t Date::getTime()
{
    return currentTime;
}

void Date::setTime(time_t time)
{
    currentTime = time;
//    CCLOG("Date_time settime %lld %s ",(long long)time, toString());
}

time_t Date::timeIntervalSinceDate(Date* otherDate)
{
    time_t beforTime = 0;
    if (otherDate)
        beforTime = otherDate->getTime();
    else
        beforTime = currentTime;
    
    return currentTime - beforTime;
}
int Date::getSecond()
{
    int Sec = localtime(&currentTime)->tm_sec;
    return Sec;
}
int Date::getMin()
{
    int Min = localtime(&currentTime)->tm_min;
    return Min;
}
int Date::getHour(){
    tm* tmd = localtime(&currentTime);
    int hour = tmd/*localtime(&currentTime)*/->tm_hour;
    return hour;
}

int Date::getMday(){
    int Mday = localtime(&currentTime)->tm_mday;
    return Mday;
}

int Date::getMonth(){
    int Month = localtime(&currentTime)->tm_mon;
    return Month;
}

int Date::getYear(){
    int Year = localtime(&currentTime)->tm_year;
    return Year;
}

int Date::getWday(){
    int Wday = localtime(&currentTime)->tm_wday;
    return Wday;
}

int Date::getYday(){
    int Yday = localtime(&currentTime)->tm_yday;
    return Yday;
}

time_t Date::getDayTime()
{
    time_t currentDayTime = currentTime - getHour()*60*60 - getMin()*60 - getSecond();
    return currentDayTime;
}

time_t Date::getTimeByInt64(long long l){
    struct tm t;
    time_t timeDate;
    t.tm_year = getInt64Year(l);
    t.tm_mon = getInt64Month(l);
    t.tm_mday = getInt64Day(l);
    t.tm_hour = getInt64Hour(l);
    t.tm_min = getInt64Min(l);
    t.tm_sec = 0;
    timeDate = mktime(&t);
    return timeDate;
}

int Date::getInt64Year(long long l){
    /* 年份，其值等于实际年份减去1900 */
    int year =  l / YEAR_INT64 - BASEYEAR;
    CCAssert(year > 0,"Date getYear error!");
    return year;
}

int Date::getInt64Month(long long l){
    /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
    int month = (l % YEAR_INT64) / MONTH_INT64 - 1;
    CCAssert(month >= 0 && month <= 11,"Date getMonth error!");
    return month;
}

int Date::getInt64Day(long long l){
    /* 一个月中的日期 - 取值区间为[1,31] */
    int day = (l % MONTH_INT64) / DAY_INT64;
    CCAssert(day >= 1 && day <= 31,"Date getDay error!");
    return day;
}

int Date::getInt64Hour(long long l){
    /* 时 - 取值区间为[0,23] */
    int hour = (l % DAY_INT64) / HOUR_INT64;
    CCAssert(hour >= 0 && hour <= 23,"Date getHour error!");
    return hour;
}

int Date::getInt64Min(long long l){
    /* 分 - 取值区间为[0,59] */
    int min = (l % HOUR_INT64) / MIN_INT64;
    CCAssert(min >= 0 && min <= 59,"Date getMin error!");
    return min;
}

int Date::getInt64Sec(long long l){
    /* 秒 – 取值区间为[0,59] */
    int sec = l % MIN_INT64;
    CCAssert(sec >= 0 && sec <= 59,"Date getSec error!");
    return sec;
}

long long Date::getInt64ByTime(Date d){
    int second = d.getSecond();
    int min = d.getMin();
    int hour = d.getHour();
    int day = d.getMday();
    int month = d.getMonth();
    int year = d.getYear();
    return year * YEAR_INT64 + month * MONTH_INT64 + day * DAY_INT64
    + hour * HOUR_INT64 + min * MIN_INT64 + second;
}

uint64_t Date::getSecondTimeByStr(std::string dateStr)
{
    enum
    {
        yearBit = 4,
        otherBit = 2
    };
    
    struct tm t;
    time_t timeDate;
    
    if (dateStr.size() >= yearBit)
    {
        t.tm_year = getInt64Year(atoi(dateStr.substr(0, yearBit).data()));
        dateStr = dateStr.substr(yearBit, dateStr.size());
    }
    
    t.tm_mon = 1;
    if (dateStr.size() >= otherBit)
    {
        t.tm_mon = getInt64Month(atoi(dateStr.substr(0, otherBit).data()));
        dateStr = dateStr.substr(otherBit, dateStr.size());
    }
    
    t.tm_mday = 1;
    if (dateStr.size() >= otherBit)
    {
        t.tm_mday = getInt64Day(atoi(dateStr.substr(0, otherBit).data()));
        dateStr = dateStr.substr(otherBit, dateStr.size());
    }
    
    t.tm_hour = 0;
    if (dateStr.size() >= otherBit)
    {
        t.tm_hour = getInt64Hour(atoi(dateStr.substr(0, otherBit).data()));
        dateStr = dateStr.substr(otherBit, dateStr.size());
    }
    
    t.tm_min = 0;
    if (dateStr.size() >= otherBit)
    {
        t.tm_min = getInt64Min(atoi(dateStr.substr(0, otherBit).data()));
        dateStr = dateStr.substr(otherBit, dateStr.size());
    }
    
    t.tm_sec = 0;
    if (dateStr.size() >= otherBit)
    {
        t.tm_sec = getInt64Sec(atoi(dateStr.substr(0, otherBit).data()));
        dateStr = dateStr.substr(otherBit, dateStr.size());
    }
    
    timeDate = mktime(&t);
    
    return (uint64_t)timeDate;
}






