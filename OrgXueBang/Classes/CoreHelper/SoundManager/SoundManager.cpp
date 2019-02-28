//
//  SoundManager.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/3.
//
//

#include "SoundManager.hpp"
#include "SoundInterface.h"
#include "FilePathConfig.h"
//#include "Reador.hpp"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "OggManager.h"
#endif

#define BACK_SOUND_ID  -1

SoundManager::SoundManager()
{
    init();
}

SoundManager::~SoundManager()
{
    
}

void SoundManager::release()
{
    
}

void SoundManager::init()
{
    m_nBackId = BACK_SOUND_ID;
    m_fVolume = 0.2f;//背景音乐专用
    m_soundSpeed = 1.0f;
    m_soundFile = "null";
    m_nSoundIdArray.clear();
}

SoundManager* SoundManager::getInstance()
{
    static SoundManager instance;
    return &instance;
}

void SoundManager::playBackSound(const std::string file,bool bLoop, const std::function<void (int, const std::string &)>  finishCall)
{
    //如果和上一个一样，需要不管
    if (m_soundFile == file) {
        resumeBackSound();
        return ;
    }
    m_soundFile = file;
    
    //停掉旧的
    SoundInterface::stopSound(m_nBackId);
    m_nBackId = BACK_SOUND_ID;
    //播放新的
    if(file == "null")
        return;
    std::string filePath = CFilePathConfig::getInstance()->getFullPathForFilename(file);
    if (filePath=="-1") {
        return ;
    }
    m_nBackId = SoundInterface::playSound(filePath, bLoop);
    SoundInterface::volumeSound(m_nBackId,m_fVolume);
    //播放失败退出
    if(m_nBackId == -1)
        return ;
    SoundInterface::setFinishCallBack(finishCall,m_nBackId);
    return ;
}

/*背景音乐音量*/
void SoundManager::VolumeBackSound(float volume)
{
    m_fVolume = volume;
    if (m_nBackId != BACK_SOUND_ID) {
        SoundInterface::volumeSound(m_nBackId,volume);
    }
}

/*暂停音乐*/
void SoundManager::pauseBackSound()
{
    if (m_nBackId != BACK_SOUND_ID) {
        SoundInterface::pauseSound(m_nBackId);
    }
}

/*继续*/
void SoundManager::resumeBackSound()
{
    if (m_nBackId != BACK_SOUND_ID) {
        SoundInterface::resumeSound(m_nBackId);
    }
}
/*播放音效*/ //add by hyp
void SoundManager::playSoundEffect(const std::string file)
{
    std::string filePath = CFilePathConfig::getInstance()->getFullPathForFilename(file);
    if (filePath=="-1") {
        return ;
    }
    SoundInterface::playSound(filePath);
}

void SoundManager::playSoundArray(std::vector<std::string> file,const std::function<void ()> finishCall)
{
    if (file.size() > 0)
    {
        std::string fileName = file[0];
        file.erase(file.begin());
        auto pCallNext = [=](int a, const std::string & b)
        {
            playSoundArray(file,finishCall);
        };
        playSound(fileName,false,pCallNext);
    }
    else
    {
        if (finishCall)
        {
            finishCall();
        }
    }
}


int SoundManager::playSound(const std::string& file,bool loop,const std::function<void (int, const std::string &)> finishCall, int iStartTag, int iTotalTag)
{
    return playSoundNew(file,loop,m_soundSpeed,finishCall,iStartTag,iTotalTag);
}
/*播放声音,返回声音ID*/
int SoundManager::playSoundNew(const std::string& file,bool loop ,float speed,const std::function<void (int, const std::string &)> finishCall , int iStartTag, int iTotalTag)
{
    std::string filePath = CFilePathConfig::getInstance()->getFullPathForFilename(file);
    if (filePath=="-1") {
        return -1;
    }
    string format =  file.substr(file.size()-3,3);
    SOUND_INFO info;
    if (format == "ogg"&&SoundManager::isIOS())
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        info.iId = COggManager::getInstance()->playSound(file, loop, finishCall, iStartTag, iTotalTag);
        COggManager::getInstance()->setPitch(info.iId,speed);
        info.type = SOUND_TYPE::OGG;
        info.state = SOUND_STATE::PLAY;
#endif
    }else
    {
        info.iId =  SoundInterface::playSound(filePath,loop,speed,finishCall,iStartTag,iTotalTag);
        info.type = SOUND_TYPE::NORMAL;
        info.state = SOUND_STATE::PLAY;
    }
    if (info.iId == -1) {
        return -1;
    }
    m_nSoundIdArray.push_back(info);
    return info.iId;
}

void SoundManager::stopSound(int audioID,bool callback)
{
    auto itFind = getSoundByID(audioID);
    if (!itFind)
    {
        return;
    }
    if (itFind&&itFind->type == SOUND_TYPE::OGG&&SoundManager::isIOS())
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        COggManager::getInstance()->stopSound(itFind->iId);
#endif
    }else
    {
        SoundInterface::stopSound(itFind->iId,callback);
    }
    itFind->iId = BACK_SOUND_ID;
    itFind->state = SOUND_STATE::STOP;
}


float SoundManager::getSoundDuration(int audioID)
{
    auto itFind = getSoundByID(audioID);
    if (!itFind)
    {
        return-1.0f;
    }
    if (itFind->type == SOUND_TYPE::OGG&&SoundManager::isIOS())
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        return -1.0f;
#endif
    }else
    {
        return SoundInterface::getSoundDuration(audioID);
    }
}


float SoundManager::getSoundCurrentTime(int audioID)
{
    auto itFind = getSoundByID(audioID);
    if (!itFind)
    {
        return-1.0f;
    }
    if (itFind->type == SOUND_TYPE::OGG&&SoundManager::isIOS())
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        return -1.0f;
#endif
    }else
    {
        return SoundInterface::getSoundCurrentTime(audioID);
    }
}

void SoundManager::pauseSound(int audioID)
{
    auto itFind = getSoundByID(audioID);
    if (!itFind)
    {
        return;
    }
    itFind->state = SOUND_STATE::PAUSE;
    if (itFind->type == SOUND_TYPE::OGG&&SoundManager::isIOS())
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        COggManager::getInstance()->pauseSound(audioID);
#endif
    }else
    {
        SoundInterface::pauseSound(audioID);
    }
}

void SoundManager::resumeSound(int audioID)
{
    auto itFind = getSoundByID(audioID);
    if (!itFind)
    {
        return;
    }
    itFind->state = SOUND_STATE::PLAY;
    if (itFind->type == SOUND_TYPE::OGG&&SoundManager::isIOS())
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        COggManager::getInstance()->resumeSound(audioID);
#endif
    }else
    {
        SoundInterface::resumeSound(audioID);
    }
}
void SoundManager::speedSound(int audioID,float speed)
{
    auto itFind = getSoundByID(audioID);
    if (!itFind)
    {
        return;
    }
    if (itFind->type == SOUND_TYPE::OGG&&SoundManager::isIOS())
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        COggManager::getInstance()->setPitch(audioID,speed);
#endif
    }else
    {
        SoundInterface::speedSound(audioID,speed);
    }
}

void SoundManager::stopAllSound(bool callback)
{
    for (int i = 0 ; i< m_nSoundIdArray.size(); i++)
    {
        stopSound(m_nSoundIdArray[i].iId,callback);
    }
    m_nSoundIdArray.clear();
    //关闭背景音乐
    SoundInterface::stopSound(m_nBackId);
    m_soundFile = "null";
    m_nBackId = -1;
}
/*暂停所有声音*/
void SoundManager::pauseAllSound()
{
    for (int i = 0 ; i< m_nSoundIdArray.size(); i++)
    {
        SoundInterface::pauseSound(m_nSoundIdArray[i].iId);
    }
    SoundInterface::pauseSound(m_nBackId);
}
/*恢复所有声音*/
void SoundManager::resumeAllSound()
{
    for (int i = 0 ; i< m_nSoundIdArray.size(); i++)
    {
        //恢复所有正在播放的
        if(m_nSoundIdArray[i].state == SOUND_STATE::PLAY)
        {
            SoundInterface::resumeSound(m_nSoundIdArray[i].iId);
        }
    }
    SoundInterface::resumeSound(m_nBackId);
}

SOUND_INFO* SoundManager::getSoundByID(int audioID)
{
    for(int i = 0;i<m_nSoundIdArray.size();i++)
    {
        if(m_nSoundIdArray[i].iId == audioID)
        {
            return  &m_nSoundIdArray[i];
        }
    }
    return nullptr;
}

void SoundManager::setAllSoundSpeed(float speed)
{
    m_soundSpeed = speed>2?2:(speed<0.5?0.5:speed);
}

bool SoundManager::supportSpeedChange()
{
    //#if COCOS2D_DEBUG > 0
    //    return SoundInterface::supportSpeedChange();
    //#endif
//    UserData& userData = Reador::getInstance()->getUserData();
//    return ("1" == userData.m_isAudioSpeed)&&SoundInterface::supportSpeedChange();
    return true;
}

bool SoundManager::isIOS()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    return true;
#endif
    return false;
}
