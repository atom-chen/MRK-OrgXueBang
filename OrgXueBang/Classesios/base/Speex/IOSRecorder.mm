//
//  Recorder.cpp
//  ReadingFriends
//
//  Created by Summer on 16/7/20.
//
//

#include "IOSRecorder.h"
#include "ComInterface.h"

#include "SPRecorder.h"

#include "CHRecorder.h"
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;



Recorder * Recorder::getInstance()
{
    if (m_pRecordInstance == nullptr)
    {
        m_pRecordInstance = new IOSRecorder();
    }
    return m_pRecordInstance;
}

const static SPRecorder* m_spxRecorder = nullptr;
const static CHRecorder* m_wavRecorder = nullptr;
static id m_sRecorder = nullptr;

IOSRecorder::IOSRecorder()
{
    m_spxRecorder = [[SPRecorder alloc] init];
    [m_spxRecorder setStartRecordCallBlcok:^{
        if (m_startRecordCallBack != nullptr) {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this](){
                m_startRecordCallBack();
            });
        }
    }];
    [m_spxRecorder setEndRecordCallBlcok:^(NSURL *fileUrl) {
        if (m_endRecordCallBack != nullptr) {
            std::string strFileURL = [fileUrl.relativePath UTF8String];
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                m_endRecordCallBack(strFileURL.c_str());
            });
        }
    }];
    [m_spxRecorder setEndPlayCallBlcok:^{
        if (m_endPlayCallBack != nullptr) {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this](){
                if (m_endPlayCallBack != nullptr) {
                    m_endPlayCallBack();
                }
            });
        }
    }];
    [m_spxRecorder setCancelRecordBlcok:^{
        if (m_cancelRecordCallBack != nullptr) {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this](){
                if (m_cancelRecordCallBack != nullptr) {
                    m_cancelRecordCallBack();
                }
            });
        }
    }];
    
    m_wavRecorder = [CHRecorder shareRecorder];
    audioID = AudioEngine::INVALID_AUDIO_ID;
    
    [m_wavRecorder setStartRecordCallBlcok:^{
        if (m_startRecordCallBack != nullptr) {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this](){
                m_startRecordCallBack();
            });
        }
    }];
    [m_wavRecorder setEndRecordCallBlcok:^(NSURL *fileUrl) {
        if (m_endRecordCallBack != nullptr) {
            std::string strFileURL = [fileUrl.relativePath UTF8String];
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                m_endRecordCallBack(strFileURL.c_str());
            });
        }
    }];
    [m_wavRecorder setEndPlayCallBlcok:^{
        if (m_endPlayCallBack != nullptr) {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this](){
                m_endPlayCallBack();
                audioID = AudioEngine::INVALID_AUDIO_ID;
            });
        }
    }];
    [m_wavRecorder setCancelRecordCallBlock:^{
        if (m_cancelRecordCallBack != nullptr) {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this](){
                m_cancelRecordCallBack();
            });
        }
    }];
    m_sRecorder = m_wavRecorder;
    bRecordModeWav = true;
}


IOSRecorder::~IOSRecorder()
{
}




bool IOSRecorder::startRecoder(std::string strFileName)
{
    if (!ComInterface::checkAuthorize(eAuthorizeType_2))
    {
        return false; //没有权限
    }
    NSString *mFileName = [NSString stringWithCString:strFileName.c_str()
                       encoding:[NSString defaultCStringEncoding]];
    if ([m_sRecorder startRecord:mFileName])
    {
        _startRecordUI();
        return true;
    }
    
    return false;
}



void IOSRecorder::stopRecoder(bool bForce)
{
    if (bForce)
    {
        [m_sRecorder endRecord];
    }
    else
    {
        _endRecordUI();
    }
}



void IOSRecorder::cancelRecoder()
{
    [m_sRecorder cancelRecord];
    _cancelRecordUI();
}

void IOSRecorder::setRecordMode(bool bWav)
{
    [m_sRecorder cancelRecord];
    if (bWav) {
        m_sRecorder = m_wavRecorder;
    } else {
        m_sRecorder = m_spxRecorder;
    }
    bRecordModeWav = bWav;
}



bool IOSRecorder::isRecording()
{
    if (bRecordModeWav) {
        return m_wavRecorder.isRecording;
    } else {
        return m_spxRecorder.isRecording;
    }
    return false;
}

bool IOSRecorder::isPlayRecording()
{
    if (!bRecordModeWav)
    {
        return m_spxRecorder.isPlaying;
    }
    else
    {
        return audioID != AudioEngine::INVALID_AUDIO_ID;
    }
}


void IOSRecorder::playRecoder(std::string strSound)
{
    std::string fullpath = cocos2d::FileUtils::getInstance()->fullPathForFilename(strSound);
    NSString *str= [NSString stringWithUTF8String:fullpath.c_str()];
    if (!bRecordModeWav)
    {
        [m_sRecorder startPlaying:str];
    }
    else
    {
        AudioEngine::uncache([str UTF8String]);
        audioID = AudioEngine::play2d([str UTF8String]);
        AudioEngine::setFinishCallback(audioID, [=](int, const std::string &){
            if (m_endPlayCallBack != nullptr) {
                m_endPlayCallBack();
            }
            audioID = AudioEngine::INVALID_AUDIO_ID;
        });
    }
}

/** 取得录音时长 by yy*/
int IOSRecorder::getRecordDuration(std::string strSound)
{
    if (!bRecordModeWav)
    {
        std::string fullpath = cocos2d::FileUtils::getInstance()->fullPathForFilename(strSound);
        NSString *audioUrl= [NSString stringWithUTF8String:fullpath.c_str()];
        return [m_sRecorder getRecordDuration:[NSURL fileURLWithPath:audioUrl]];
    }
    else
    {
        return AudioEngine::getDuration(audioID);
    }
}

void IOSRecorder::stopPlayRecord()
{
    if (!bRecordModeWav)
    {
        [m_sRecorder stopPlaying];
    }
    else
    {
        AudioEngine::stop(audioID);
        if (m_endPlayCallBack != nullptr) {
            m_endPlayCallBack();
        }
        audioID = AudioEngine::INVALID_AUDIO_ID;
    }
}
