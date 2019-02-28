//
//  SoundInterfaceIOS.m
//  ReadingMate
//
//  Created by hyp on 2018/10/17.
//
//

#include "SoundInterfaceIOS.h"
#include "SoundPlayIOS.h"
#include "SoundInterface.h"
/*播放声音,返回声音ID*/
int SoundInterfaceIOS::playSound(const std::string& file,bool loop ,float rate, int iStartTag , int iTotalTag)
{
    return [[SoundPlayIOS getInstance] playSound:file  loop:loop rate:rate iStartTag:iStartTag  iTotalTag:iTotalTag];
}

void SoundInterfaceIOS::runFinishCallBack(int soundID)
{
    SoundInterface::runFinishCallBack(soundID);
}
/*停止声音*/
void SoundInterfaceIOS::stopSound(int audioID,bool callback)
{
    [[SoundPlayIOS getInstance] stopSound:audioID callback:callback];
}
//清除声音缓存
void SoundInterfaceIOS::clearSound(int audioID,bool callback)
{
    [[SoundPlayIOS getInstance] clearSound:callback];
}
/*获取声音长度  -1 代表失败*/
float SoundInterfaceIOS::getSoundDuration(int audioID)
{
    return [[SoundPlayIOS getInstance] getSoundDuration:audioID];
}
/*获取当前时间  -1 代表失败*/
float SoundInterfaceIOS::getSoundCurrentTime(int audioID)
{
    return [[SoundPlayIOS getInstance] getSoundCurrentTime:audioID];
}
//暂停音效
void SoundInterfaceIOS::pauseSound(int audioID)
{
    [[SoundPlayIOS getInstance] pauseSound:audioID];
}
//恢复音效
void SoundInterfaceIOS::resumeSound(int audioID)
{
    [[SoundPlayIOS getInstance] resumeSound:audioID];
}
//调整音量
void SoundInterfaceIOS::volumeSound(int audioID,float volume)
{
    [[SoundPlayIOS getInstance] volumeSound:audioID volume:volume];
}
//调整速度
void SoundInterfaceIOS::speedSound(int audioID,float speed)
{
    [[SoundPlayIOS getInstance] speedSound:audioID speed:speed];
}


