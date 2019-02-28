//
//  AndroidRecorder.hpp
//  ReadingFriends
//
//  Created by liuwei on 17/2/10.
//
//

#ifndef AndroidRecorder_hpp
#define AndroidRecorder_hpp

#include "Recorder.h"


class AndroidRecorder : public Recorder
{
public:
    AndroidRecorder();
    virtual ~AndroidRecorder();
    
    /** 开始录音 */
    virtual bool startRecoder(std::string strFilePath = "");
    /** 停止录音 */
    virtual void stopRecoder(bool bForce = false);
    /** 取消录音 */
    virtual void cancelRecoder();
    
    
    /** 是否正在录音 */
    virtual bool isRecording();
    /** 播放录音 */
    virtual void playRecoder(std::string strSound);
    /** 停止播放录音 */
    virtual void stopPlayRecord();
    /** 停止录音回调 */
    virtual void stopRecordCallBack(std::string strFile);
    
    /** 是否正在播放录音 */
    virtual bool isPlayRecording();
    /** 取得录音时长 by yy */
    virtual int getRecordDuration(std::string strSound);
    
    /** 停止播放回调 */
    virtual void stopPlayRecordCallBack(std::string strFile);
    
    virtual void setRecordMode(bool bWav = false);
};

#endif /* AndroidRecorder_hpp */
