//
//  SoundInterface.h
//  ReadingMate
//
//  Created by 丁晶 on 2017/2/27.
//
//

#ifndef SoundInterface_h
#define SoundInterface_h

#include <stdio.h>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

typedef std::function<void (int, const std::string &)> SoundPlayCallBack;

class SoundInterface
{
public:
    /*播放声音,返回声音ID*/
    static int playSound(const std::string& file,bool loop = false,float rate = 1.0f,const std::function<void (int, const std::string &)> finishCall = nullptr, int iStartTag = 0, int iTotalTag = 0);
    /*设置回调*/
    static void setFinishCallBack(const std::function<void (int, const std::string &)> finishCall,int soundID);
    static void runFinishCallBack(int soundID);
    /*停止声音*/
    static void stopSound(int audioID,bool callback = false);
    static void clearSound(int audioID=-1,bool callback = false);      //清除声音缓存
    /*获取声音长度  -1 代表失败*/
    static float getSoundDuration(int audioID);
    /*获取当前时间  -1 代表失败*/
    static float getSoundCurrentTime(int audioID);
    
    static void pauseSound(int audioID);       //暂停音效
    static void resumeSound(int audioID);      //恢复音效
    static void volumeSound(int audioID,float volume);      //调整音量
    static void speedSound(int audioID,float speed);      //调整速度
    static bool supportSpeedChange();      //是否支持速度改变
};

#endif

