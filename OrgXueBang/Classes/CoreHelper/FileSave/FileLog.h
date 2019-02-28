#ifndef __FILE_LOG_H__
#define __FILE_LOG_H__

/*
 文件LOG
 by：高超
 */

#include <string>
#include <stdio.h>
#include <mutex>
using namespace std;

class CFileLog //: public HttpDataDelegate
{
public:
    static CFileLog* Instance();
    CFileLog();
//    virtual void doWithHttpData(eHttpType type, bool success, std::string& strData) ;
    /*保存文件-加密*/
    static void LogFormat(const char* format, ...);
    static void LogSaveFormat(const char* format, ...);
    static void Log(const std::string& str,bool bLog = true);
    static void UpLoadFile();
    static void UnLock();
public:
    long    m_FileSize;
    std::string m_path;
    std::mutex m_MutexFile;
};

#endif

