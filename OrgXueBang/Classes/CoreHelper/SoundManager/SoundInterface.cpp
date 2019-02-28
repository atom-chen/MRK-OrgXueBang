//
//  SoundInterface.m
//  ReadingMate
//
//  Created by hyp on 2018/10/17.
//
//

#include "SoundInterface.h"
#include "json.h"
#include "AudioCallBackMsg.hpp"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "SoundInterfaceIOS.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "AndroidMethod.h"
#endif
static std::map<std::string,SoundPlayCallBack>  _MapCallBack;
/*播放声音,返回声音ID*/
int SoundInterface::playSound(const std::string& file,bool loop ,float rate,const std::function<void (int, const std::string &)> finishCall , int iStartTag , int iTotalTag)
{
    int soundId = -1;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    soundId = SoundInterfaceIOS::playSound(file,loop,rate,iStartTag,iTotalTag);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Json::Value root;
    root["type"] = "playSound";
    root["file"] = file;
    root["loop"] = loop;
    root["rate"] = rate;
    root["iStartTag"] = iStartTag;
    root["iTotalTag"] = iTotalTag;
    string str = root.toStyledString();
    const char* soundstr = CppCallJava(MYJNICALL_TYPE_AUDIO, str);
    soundId = atoi(soundstr);
#endif
    //设置长度
    if (iTotalTag != 0&&soundId!=-1) {
        AudioCallBackMsg::getInstance()->callBack(soundId, iTotalTag/100.0f);
    }
    if (finishCall&&soundId!=-1) {//需要回调
        SoundInterface::setFinishCallBack(finishCall,soundId);
    }
    return soundId;
}
/*设置回调*/
void SoundInterface::setFinishCallBack(const std::function<void (int, const std::string &)> finishCall,int soundID)
{
    _MapCallBack[StringUtils::format("%d",soundID)]=finishCall;
}
void SoundInterface::runFinishCallBack(int soundID)
{
    //回调
    SoundPlayCallBack callback = _MapCallBack[StringUtils::format("%d",soundID)];
    if (callback) {
        _MapCallBack.erase(StringUtils::format("%d",soundID));
        callback(soundID,"");
        callback = nullptr;
    }
}
/*停止声音*/
void SoundInterface::stopSound(int audioID,bool callback)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    SoundInterfaceIOS::stopSound(audioID,callback);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Json::Value root;
    root["type"] = "stopSound";
    root["audioID"] = audioID;
    root["callback"] = callback;
    string str = root.toStyledString();
    CppCallJava(MYJNICALL_TYPE_AUDIO, str);
#endif
    
}
//清除声音缓存
void SoundInterface::clearSound(int audioID,bool callback)
{
    if(audioID==-1){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        SoundInterfaceIOS::clearSound(audioID,callback);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        Json::Value root;
        root["type"] = "clearSound";
        root["callback"] = callback;
        string str = root.toStyledString();
        CppCallJava(MYJNICALL_TYPE_AUDIO, str);
#endif
        _MapCallBack.clear();
    }else{
        _MapCallBack.erase(StringUtils::format("%d",audioID));
    }
}
/*获取声音长度  -1 代表失败*/
float SoundInterface::getSoundDuration(int audioID)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return SoundInterfaceIOS::getSoundDuration(audioID);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Json::Value root;
    root["type"] = "getSoundDuration";
    root["audioID"] = audioID;
    string str = root.toStyledString();
    const char* duration = CppCallJava(MYJNICALL_TYPE_AUDIO, str);
    return atof(duration);
#endif
    return -1;
}
/*获取当前时间  -1 代表失败*/
float SoundInterface::getSoundCurrentTime(int audioID)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return SoundInterfaceIOS::getSoundCurrentTime(audioID);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Json::Value root;
    root["type"] = "getSoundCurrentTime";
    root["audioID"] = audioID;
    string str = root.toStyledString();
    const char* current = CppCallJava(MYJNICALL_TYPE_AUDIO, str);
    return atof(current);
#endif
    return -1;
}
//暂停音效
void SoundInterface::pauseSound(int audioID)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    SoundInterfaceIOS::pauseSound(audioID);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Json::Value root;
    root["type"] = "pauseSound";
    root["audioID"] = audioID;
    string str = root.toStyledString();
    CppCallJava(MYJNICALL_TYPE_AUDIO, str);
#endif
}
//恢复音效
void SoundInterface::resumeSound(int audioID)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    SoundInterfaceIOS::resumeSound(audioID);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Json::Value root;
    root["type"] = "resumeSound";
    root["audioID"] = audioID;
    string str = root.toStyledString();
    CppCallJava(MYJNICALL_TYPE_AUDIO, str);
#endif
    
}
//调整音量
void SoundInterface::volumeSound(int audioID,float volume)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    SoundInterfaceIOS::volumeSound(audioID,volume);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Json::Value root;
    root["type"] = "volumeSound";
    root["audioID"] = audioID;
    root["volume"] = volume;
    string str = root.toStyledString();
    CppCallJava(MYJNICALL_TYPE_AUDIO, str);
#endif
    
}
//调整速度
void SoundInterface::speedSound(int audioID,float speed)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    SoundInterfaceIOS::speedSound(audioID,speed);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Json::Value root;
    root["type"] = "speedSound";
    root["audioID"] = audioID;
    root["speed"] = speed;
    string str = root.toStyledString();
    CppCallJava(MYJNICALL_TYPE_AUDIO, str);
#endif
    
}
//是否支持速度改变
bool SoundInterface::supportSpeedChange()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return true;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Json::Value root;
    root["type"] = "supportSpeedChange";
    string str = root.toStyledString();
    const char* value = CppCallJava(MYJNICALL_TYPE_AUDIO, str);
    return (atoi(value)==1?true:false);
#endif
}

