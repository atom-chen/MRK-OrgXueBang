//
//  AndroidRecorder.cpp
//  ReadingFriends
//
//  Created by liuwei on 17/2/10.
//
//

#include "stdafx.h"
#include "AndroidRecorder.h"
#include "ComInterface.h"
#include "AndroidMethod.h"




Recorder * Recorder::getInstance()
{
    if (m_pRecordInstance == nullptr)
    {
        m_pRecordInstance = new AndroidRecorder();
    }
    return m_pRecordInstance;
}




AndroidRecorder::AndroidRecorder()
{
}


AndroidRecorder::~AndroidRecorder()
{
}





bool AndroidRecorder::startRecoder(std::string strFilePath)
{
    const char* info = CppCallJava(MYJNICALL_TYPE_RECORDINFO, "startRecorder"+strFilePath);
    cocos2d::log("start %s", info);
    if (strcmp(info, "error") == 0)
    {
        return false;
    }
    
    if (m_startRecordCallBack != nullptr) {
        m_startRecordCallBack();
    }
    
    _startRecordUI();
    return true;
}



void AndroidRecorder::stopRecoder(bool bForce)
{
    if (bForce)
    {
        const char* info = CppCallJava(MYJNICALL_TYPE_RECORDINFO, "stopRecorder");
    }
    else
    {
        _endRecordUI();
    }
}



void AndroidRecorder::cancelRecoder()
{
    const char* info = CppCallJava(MYJNICALL_TYPE_RECORDINFO, "cancelRecorder");
    _cancelRecordUI();
}





bool AndroidRecorder::isRecording()
{
    std::string strRes = CppCallJava(MYJNICALL_TYPE_RECORDINFO, "isRecording");
    return strRes == "1";
}




void AndroidRecorder::playRecoder(std::string strSound)
{
    std::string str = "playRecoder" + strSound;
    const char* info = CppCallJava(MYJNICALL_TYPE_RECORDINFO, str);
}




void AndroidRecorder::stopPlayRecord()
{
    const char* info = CppCallJava(MYJNICALL_TYPE_RECORDINFO, "stopPlayRecord");
}



void AndroidRecorder::stopRecordCallBack(std::string strFile)
{
    if (m_endRecordCallBack)
    {
        cocos2d::log("huidiao %s" , strFile.c_str());
        m_endRecordCallBack(strFile);
    }
}

bool AndroidRecorder::isPlayRecording()
{
    std::string strRes = CppCallJava(MYJNICALL_TYPE_RECORDINFO, "isPlayRecording");
    cocos2d::log("==YY==isPlayRecording %s", strRes.c_str());
    return strRes == "1";
}

/** 取得录音时长 by yy*/
int AndroidRecorder::getRecordDuration(std::string strSound)
{
    return 0;
}

void AndroidRecorder::stopPlayRecordCallBack(std::string strFile)
{
    if (m_endPlayCallBack)
    {
        cocos2d::log("==YY==stopPlayRecordCallBack - %s" , strFile.c_str());
        m_endPlayCallBack();
    }
}

void AndroidRecorder::setRecordMode(bool bWav)
{
    CppCallJava(MYJNICALL_TYPE_RECORD_MODE,bWav?"2":"1");
}
