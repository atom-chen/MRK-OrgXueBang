//
//  OggManager.cpp
//  ReadingFriends
//
//  Created by liuwei on 16/9/28.
//
//

#include "OggManager.h"
//#include "../cocos2d.h"
//#include "cocos2d.h"
#import "IDZOggController.h"
#import <AVFoundation/AVFoundation.h>

COggManager::COggManager()
:m_iCurrentId(std::numeric_limits<int>::max())
,m_bLazyInitLoop(true)
{
}

COggManager::~COggManager()
{
}


COggManager* COggManager::m_sOggManager = nullptr;
COggManager* COggManager::getInstance()
{
    if (m_sOggManager == nullptr)
    {
        m_sOggManager = new COggManager;
    }
    return m_sOggManager;
}




/* 播放声音 */
int COggManager::playSound(std::string strName, bool bloop, FinishCallBack finishCall, int startTag, int totalTag)
{
    _playSound(m_iCurrentId, strName, bloop, finishCall, startTag, totalTag);
    
    return m_iCurrentId--;
}



/* 播放声音 */
void COggManager::_playSound(int iId, std::string strName, bool bloop, FinishCallBack finishCall, int startTag, int totalTag)
{
    if (m_mapOggControllers.find(iId) != m_mapOggControllers.end())
    {
        stopSound(iId);
    }
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
    //设置属性
    IDZOggController* pOggController = [[IDZOggController alloc] init];
    [pOggController setOggId:iId];
    [pOggController setOggName:[NSString stringWithUTF8String:strName.c_str()]];
    m_mapOggControllers[iId] = pOggController;
    
    if (bloop)
    {
        [pOggController setFinishCallBack:^(int iId, NSString* fileName){
            _playSound(iId, [fileName UTF8String], bloop, finishCall, startTag, totalTag);
        }];
    }
    else
    {
        [pOggController setFinishCallBack:^(int iId, NSString* fileName){
            if (finishCall)
            {
                finishCall(iId, [fileName UTF8String]);
            }
        }];
    }
    
    //开始播放
    std::string strPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(strName);
    [m_mapOggControllers[iId] play:[NSString stringWithUTF8String:strPath.c_str()] start:startTag total:totalTag];
//    cocos2d::log("<--- play %d", iId);
    
    
//    if (m_bLazyInitLoop)
//    {
//        cocos2d::Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(COggManager::_checkStopSound), this, 0, false);
//        m_bLazyInitLoop = false;
//    }
    
    if(totalTag > 0)
    {
//        totalTag = MAX(5, totalTag - 15);//宁可让他缺屁股少毛，也不能让他画蛇添足
        if (totalTag > 15) {
            totalTag -= 15;
        } else if (totalTag > 10) {
            totalTag -= 5;
        } else if (totalTag > 5) {
            totalTag -= 2;
        }
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, totalTag/100.0f * NSEC_PER_SEC), dispatch_get_main_queue(), ^(void){
            stopSound(iId);
        });
    }
}



/* 停止声音 */
void COggManager::stopSound(int iId)
{
    auto itFind = m_mapOggControllers.find(iId);
    if (itFind != m_mapOggControllers.end())
    {
        [m_mapOggControllers[iId] stop];
        [m_mapOggControllers[iId] release];
        m_mapOggControllers.erase(iId);
//        cocos2d::log("---> release %d", iId);
    }
}

/* 暂停声音 */
void COggManager::pauseSound(int iId)
{
    auto itFind = m_mapOggControllers.find(iId);
    if (itFind != m_mapOggControllers.end())
    {
        [m_mapOggControllers[iId] pause];
    }
}

/* 继续声音 */
void COggManager::resumeSound(int iId)
{
    auto itFind = m_mapOggControllers.find(iId);
    if (itFind != m_mapOggControllers.end())
    {
        [m_mapOggControllers[iId] resume];
    }
}


/* 检测停止的声音 */
void COggManager::_checkStopSound(float dt)
{
    for (auto& it : m_mapOggControllers)
    {
        
        
        if ([it.second isStop] || (it.second.bPartTime && (it.second.totalSec -= dt) <= 0.0f))
        {
            stopSound(it.first);
            break;
        }
    }
    
    if (m_mapOggControllers.empty())
    {
        cocos2d::Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(COggManager::_checkStopSound), this);
        m_bLazyInitLoop = true;
    }
}

void COggManager::setPitch(int iId, float pitch)
{
    auto itFind = m_mapOggControllers.find(iId);
    if (itFind != m_mapOggControllers.end())
    {
        [m_mapOggControllers[iId] setPitch:pitch];
    }
}
