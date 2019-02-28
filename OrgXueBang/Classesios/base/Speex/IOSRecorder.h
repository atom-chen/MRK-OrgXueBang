#ifndef IOSRecorder_h
#define IOSRecorder_h

#include "Recorder.h"


class IOSRecorder : public Recorder
{
public:
    IOSRecorder();
    virtual ~IOSRecorder();
    
    /** 开始录音 */
    virtual bool startRecoder(std::string strFileName = "");
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
    /** 是否正在播放录音 */
    virtual bool isPlayRecording();
    /** 取得录音时长 by yy */
    virtual int getRecordDuration(std::string strSound);
   
    virtual void setRecordMode(bool bWav = false);
private:
    int audioID;
    bool bRecordModeWav;
};


#endif /* Recorder_hpp */
