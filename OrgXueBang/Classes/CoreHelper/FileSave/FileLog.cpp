//#include "StdAfx.h"
#include "FileLog.h"
#include "UploadFile.h"
#include "UserManager.hpp"
#include "NetworkService.hpp"
#include "t_sprintf.h"

#define MYDEF_MAX_FILE  1024 * 1024 * 3 //3M
#define MYDEF_MIN_FILE  1024 * 1024 * 2 //2M

//#define MYDEF_MAX_FILE  1024 * 100 //4M
//#define MYDEF_MIN_FILE  1024 * 50 //2M

#define MYDEF_LOG_FILE  "ReadFileLog.txt"

CFileLog* g_CFileLog = nullptr;

char* g_pCFileLogBuf = nullptr;//(char*)malloc(1024*100);

CFileLog* CFileLog::Instance()
{
    if(g_CFileLog == nullptr)
    {
        g_CFileLog = new CFileLog();
    }
    return  g_CFileLog;
}

CFileLog::CFileLog()
{
    m_path = FileUtils::getInstance()->getWritablePath() + MYDEF_LOG_FILE;
    log("LogFile = %s", m_path.c_str());
    m_FileSize = 0;
    //以读写二进制方式打开文件
    auto pFile = fopen(m_path.c_str(), "rb+");
    //下载初始化，可用来检测文件是否有更新
    if (pFile)
    {
        //定位到文件未尾，先读取临时文件总长度
        fseek(pFile, 0, SEEK_END);
        m_FileSize = ftell(pFile);
        fclose(pFile);
    }
    cocos2d::log("file:%s,Size:%d",m_path.c_str(),(int)m_FileSize);
}

    /*保存*/
void CFileLog::LogFormat(const char* format, ...)
{
//    va_list arg_list;
//    va_start(arg_list, format);
//    std::string str = stringf(format, arg_list);
//    
//    
    va_list arg_list;
    
    va_start(arg_list, format);
    
    if(g_pCFileLogBuf == nullptr)
        g_pCFileLogBuf = (char*)malloc(1024*100);
    if (g_pCFileLogBuf != nullptr)
    {
        vsnprintf(g_pCFileLogBuf, 1024*100, format, arg_list);
        g_pCFileLogBuf[1024*100 - 1] = 0;
        CFileLog::Log(g_pCFileLogBuf);
        //free(pBuf);
    }
    va_end(arg_list);
}

void CFileLog::LogSaveFormat(const char* format, ...)
{
    //    va_list arg_list;
    //    va_start(arg_list, format);
    //    std::string str = stringf(format, arg_list);
    //
    //
    va_list arg_list;
    
    va_start(arg_list, format);
    
    if(g_pCFileLogBuf == nullptr)
        g_pCFileLogBuf = (char*)malloc(1024*100);
    if (g_pCFileLogBuf != nullptr)
    {
        vsnprintf(g_pCFileLogBuf, 1024*100, format, arg_list);
        CFileLog::Log(g_pCFileLogBuf,false);
        //free(pBuf);
    }
    va_end(arg_list);
}


void CFileLog::Log(const std::string& str,bool bLog)
{
    if(bLog)
        log("CFileLog:%s",str.c_str());
    CFileLog* p =   CFileLog::Instance();
    
    //文件长度判断
    if(p->m_MutexFile.try_lock() == false)
    {
        return;
    }
    if(p->m_FileSize > MYDEF_MAX_FILE)
    {
        //删除文件
        Data dataFile = FileUtils::getInstance()->getDataFromFile(p->m_path);
        if (dataFile.getSize() > MYDEF_MIN_FILE) {
            //
            Data dataSave;
            dataSave.copy(dataFile.getBytes() + (MYDEF_MAX_FILE - MYDEF_MIN_FILE), MYDEF_MIN_FILE);
            FileUtils::getInstance()->writeDataToFile(dataSave,p->m_path);
            p->m_FileSize = MYDEF_MIN_FILE;
        }
    }
    //追加写文件
    FILE *fp = fopen(p->m_path.c_str(), "a+");
    if (fp!=nullptr)
    {
        fseek(fp, 0, SEEK_END);
        //日期
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        fwrite(asctime (timeinfo), strlen(asctime (timeinfo)), 1, fp);
        p->m_FileSize += strlen(asctime (timeinfo));
        //内容
        //当内容过长的时候，优化存储内容（其实就是删点）
        if(strlen(str.c_str()) < 5000)
        {
            fwrite(str.c_str(), strlen(str.c_str()), 1, fp);
            p->m_FileSize += strlen(str.c_str());
        }
        else
        {
//            fwrite(str.c_str(), 400, 1, fp);
//            p->m_FileSize += 400;
//
//            auto pChar = "...\n";
//            fwrite(pChar, 3, strlen(pChar), fp);
//            p->m_FileSize += strlen(pChar);
//
//            pChar = str.c_str();
//            pChar += strlen(str.c_str());
//            pChar -= 400;
//            fwrite(pChar, strlen(pChar), 1, fp);
//            p->m_FileSize += strlen(pChar);
            string newStr;
            newStr = str.substr(0, 400) + "\n...\n" + str.substr(str.length() - 400);
            fwrite(newStr.c_str(), newStr.length(), 1, fp);
            p->m_FileSize += newStr.length();
        }
        //换行
        fwrite("\n", strlen("\n"), 1, fp);
        p->m_FileSize += strlen("\n");
        //over
        fclose(fp);
    }
    p->m_MutexFile.unlock();
    
}

void CFileLog::UpLoadFile()
{
    CFileLog* p =  CFileLog::Instance();
    if(p->m_MutexFile.try_lock() == false)
    {
        return;
    }
    //上传这个文件
    //p->m_path;
    //结束后要解锁
    
    //上传到服务器
    std::map<std::string,std::string> mapHeadValue;
    mapHeadValue["Expect"] = "";
    //Post参数优先
    KVPairVec* vecPostValue = new KVPairVec;
    vecPostValue->push_back(new KVPair("token", UserManager::getInstance()->getUserModel()->getUserToken()));

    
    //Get参数备用
    KVPairVec* vecGetValue = new KVPairVec;
    vecGetValue->push_back(new KVPair("token", UserManager::getInstance()->getUserModel()->getUserToken()));

    vector<stuFileInfo> vecFileInfo;

    if(FileUtils::getInstance()->isFileExist(p->m_path))
    {
        stuFileInfo fileInfo;
        fileInfo.m_strFileName = p->m_path;
        fileInfo.m_strFileCopyName = "logfile";
        fileInfo.m_strFileContentType = "text/plain";

        vecFileInfo.push_back(fileInfo);
    }

    if (vecFileInfo.size() == 0) {//必须至少上传一个资源
        return;
    }
    string strUpUrl = "https://app.readingmate.com.cn/note/up_log";
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#else
    strUpUrl = NetworkService::getInstance()->getServiceUrl(eHttpType::uploadLog);
    if(strUpUrl.find("https://api.metricschina.com") != string::npos)//默认正式服，即便Debug，也走正事上传路径
    {
        strUpUrl = "https://app.readingmate.com.cn/note/up_log";
    }
#endif
    stuUpload upload(strUpUrl, vecFileInfo, mapHeadValue, vecPostValue, vecGetValue);
    HttpManager::getInstance()->HttpUploadRequest(eHttpType::uploadLog, upload);
}

//void CFileLog::doWithHttpData(eHttpType type, bool success, std::string& strData)
//{
//    if(type == eHttpType::uploadLog)
//        m_MutexFile.unlock();
//}

void CFileLog::UnLock()
{
    CFileLog::Instance()->m_MutexFile.unlock();
}
