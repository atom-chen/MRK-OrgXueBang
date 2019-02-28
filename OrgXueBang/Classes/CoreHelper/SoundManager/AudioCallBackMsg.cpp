//
//  AudioCallBackMsg.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/7/19.
//
//  for:该类纯粹为安卓播放ogg的辅助类，只是为了教材音频播放控制中的分段停止功能

#include "AudioCallBackMsg.hpp"
#include "cocos2d.h"
#include "SoundManager.hpp"
using namespace cocos2d;
using namespace cocos2d::experimental;

static AudioCallBackMsg* m_instance = nullptr;

AudioCallBackMsg* AudioCallBackMsg::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new AudioCallBackMsg();
    }
    return m_instance;
}

AudioCallBackMsg::AudioCallBackMsg()
:m_ret(-1)
{
}
AudioCallBackMsg::~AudioCallBackMsg()
{
}

void AudioCallBackMsg::callBack(int ret, float dt)
{
    printf("===YY==目标oggID = %d ; 延时  %f", ret, dt);
    m_ret = ret;
    //cocos2d::Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(AudioCallBackMsg::stop), this, dt, false);
    Director::getInstance()->getRunningScene()->scheduleOnce([=](float t)
                                                             {
                                                                 stop(t);
                                                             }, dt, "AndroidAudioPartEndCallStop");
}

void AudioCallBackMsg::stop(float dt)
{
    printf("===YY==停止oggID = %d", m_ret);
    SoundManager::getInstance()->stopSound(m_ret);
}

void AudioCallBackMsg::reset()
{
    m_ret = -1;
    Director::getInstance()->getRunningScene()->unschedule("AndroidAudioPartEndCallStop");
}
