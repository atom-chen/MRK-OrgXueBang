#ifndef Recorder_h
#define Recorder_h


#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include "cocos2d.h"

#include "GameConfig.hpp"

typedef std::function <void()> startRecord;
typedef std::function <void(float)> recording;
typedef std::function <void(std::string)> endRecord;
typedef std::function <void()> playOver;
typedef std::function <void()> cancelRecord;


class Recorder : public cocos2d::Ref
{
    static Recorder * m_pRecordInstance;
public:
    Recorder();
    ~Recorder();
    static Recorder * getInstance();
public:
    
    /** 开始录音 */
    virtual bool startRecoder(std::string strFileName = "") = 0;
    /** 停止录音 */
    virtual void stopRecoder(bool bForce = false) = 0;
    /** 取消录音 */
    virtual void cancelRecoder() = 0;
    /** 设置录音模式 */
    virtual void setRecordMode(bool bWav = false) = 0;
    
    /** 是否正在录音 */
    virtual bool isRecording() = 0;
    /** 播放录音 */
    virtual void playRecoder(std::string strSound) = 0;
    /** 停止播放录音 */
    virtual void stopPlayRecord() = 0;
    /** 是否正在播音 */
    virtual bool isPlayRecording() = 0;
    
    /** 取得录音时长 */
    virtual int getRecordDuration(std::string strSound) = 0;
    
	/* 为android写的录音结束回调 */
    virtual void stopRecordCallBack(std::string strFile){};
    /* 为android写的播放结束回调 */
    virtual void stopPlayRecordCallBack(std::string strFile){};
public:
    /** 开始录音回调 */
    void addBeginRecoderCall(startRecord callBack);
    /** 正在录音中回调 */
    void addRecordingCall(recording callBack);
    /** 结束录音回调 */
    void addEndRecordCall(endRecord callBack);
    /** 结束录音回调 */
    void addEndPlayCall(playOver callBack);
    /** 取消录音回调 */
    void addCancelRecordCall(cancelRecord callBack);
    /** 清除录音数据 */
    void clearRecorder();
    
    
    
protected:
    void _startRecordUI();
    void _endRecordUI();
    void _cancelRecordUI();
    
    
    startRecord m_startRecordCallBack;
    recording m_recordingCallBack;
    endRecord m_endRecordCallBack;
    playOver m_endPlayCallBack;
    cancelRecord m_cancelRecordCallBack;
    
private:
    void _updateRecord(float dt);
    void _enableRecordTouch(bool bEnable);
    double m_dRecordStartTime;
    
};


#endif /* Recorder_hpp */
