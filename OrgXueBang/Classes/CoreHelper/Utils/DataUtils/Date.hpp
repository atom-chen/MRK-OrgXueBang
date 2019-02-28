//
//  Date.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/2/18.
//
//

#ifndef Date_hpp
#define Date_hpp

//#include "PreIncludes.hpp"

#include "cocos2d.h"

class Date : public cocos2d::Ref
{
public:
    Date();
    ~Date() {};
    // 用字符串表示的时间，格式类似为 "Sat May 20 17:36:17 2000"
    char* toString();
    time_t getTime();
    void setTime(time_t time);
    time_t timeIntervalSinceDate(Date* otherDate);
    int getSecond();/* seconds after the minute [0-60] */
    int getMin();/* minutes after the hour [0-59] */
    int getHour(); /* hours since midnight [0-23] */
    int getMday(); /* day of the month [1-31] */
    int getMonth();/* months since January [0-11] */
    int getYear();/* years since 1900 */
    int getWday();/* days since Sunday - [0,6] */
    int getYday();/* days since January 1 [0-365] */
    time_t getDayTime();//获取以日为单位的时间
    static Date* date();
    
    static time_t getTimeByInt64(long long l);
    static int getInt64Year(long long l);
    static int getInt64Month(long long l);
    static int getInt64Day(long long l);
    static int getInt64Hour(long long l);
    static int getInt64Min(long long l);
    static int getInt64Sec(long long l);
    static long long getInt64ByTime(Date d);
    //返回str对应的秒数，str的格式应为"20150918151150",运算不对格式进行检测
    static uint64_t getSecondTimeByStr(std::string dateStr);
    
    
private:
    void init();
    
    // 1970年1月1号到现在的秒数 
    time_t currentTime;
    time_t currentDayTime; //忽略小时后面的精度，只以日为单位
};

#endif /* Date_hpp */
