
#include "stdafx.h"
//#include <io.h>
//#include <iostream>

#include "CurlDownload.h"

#define safeCurlSetopt(curl,op,param) if (curl_easy_setopt(curl,op,param) != CURLE_OK) {CCLOG("curl_easy_setopt error %d", op);return false;};



int g_iRangeTestSize = 0;

CURLSH* CurlDownloader::sharedns_handle = NULL;

#define MAX_PATH 1024

void fflushex(FILE* stream)
{
    int duphandle;
    fflush(stream);
    duphandle=dup(fileno(stream));
    close(duphandle);
}

void StringReplace(std::string& src, const std::string& target, const std::string& replacement)
{
    if (target == replacement) return;
    std::string::size_type startpos = 0;
    while (startpos!= std::string::npos)
    {
        startpos = src.find(target);
        if( startpos != std::string::npos )
        {
            src.replace(startpos,target.length(),replacement);
        }
    }
}

//std::string Log_Format(LPCSTR lpszFormat, ...)
//{
//    char szLog[1024]={0};
//    va_list ap;
//    va_start(ap, lpszFormat);
//    vsprintf(szLog, lpszFormat, ap);
//    va_end(ap);
//    return szLog;
//}

//#define RICH_FORMAT_LOG cout<<Log_Format
#define RICH_FORMAT_LOG(format, ...) do {} while (0);
//#define RICH_FORMAT_LOG cocos2d::log

CurlDownloader::CurlDownloader(void)
{
    m_pFile = NULL;
    m_bPause = false;
    m_bSupportMultiDown = false;
    m_bTerminate = false;
    m_ulFullFileSize = 0;
    m_ulDownFileSize = 0;
    m_strUrl = m_strDownloadPath = "";
    pthread_mutex_init(&m_mutexFile,NULL);
    m_strProxy = "";
    m_iHttpCode = 200;
    m_bRedirected = false;
    m_strOriginalUrl = "";
    m_curlCode = CURLE_OK;
    m_pInitCurl = NULL;
    pthread_mutex_init(&m_mutexCurl,NULL);
    //保证DownloadInit时m_mutexCurl要先于析构函数锁定，才能避免析构函数崩溃
    pthread_mutex_lock(&m_mutexCurl);
}

CurlDownloader::~CurlDownloader(void)
{
    Pause();
    pthread_mutex_lock(&m_mutexFile);
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
    pthread_mutex_unlock(&m_mutexFile);
    ClearThreadInfo();
    //文件互质变量释放
    pthread_mutex_destroy(&m_mutexFile);
    pthread_mutex_lock(&m_mutexCurl);
    if (m_pInitCurl)
    {
        curl_easy_cleanup(m_pInitCurl);
        m_pInitCurl = NULL;
    }
    pthread_mutex_unlock(&m_mutexCurl);
    pthread_mutex_destroy(&m_mutexCurl);
}

void CurlDownloader::ClearThreadInfo()
{
    std::vector<ThreadInfo*>::const_iterator ita = m_vecThrdInfo.begin();
    while (ita != m_vecThrdInfo.end())
    {
        ThreadInfo* pInfo = *ita++;
        if (pInfo)
        {
            if (pInfo->pCurl)
            {
                curl_easy_cleanup(pInfo->pCurl);
                pInfo->pCurl = NULL;
            }
            delete pInfo;
            pInfo = NULL;
        }
    }
    m_vecThrdInfo.clear();
}

bool CurlDownloader::Start(const std::string&strUrl, const std::string&strDownloadPath,
                           int iThreadCountX)
{
    assert(iThreadCountX);
    m_bPause = false;
    m_bTerminate = false;
    m_strUrl = strUrl;
    m_ulFullFileSize = 0;
    m_ulDownFileSize = 0;
    m_curlCode = CURLE_OK;
    m_iHttpCode = 200;
    //备份原始URL
    m_strOriginalUrl = strUrl;
    m_strDownloadPath = strDownloadPath;
    ClearThreadInfo();
    //删除本地文件
   // DeleteFileA(strDownloadPath.c_str());
    //FileUtils::getInstance()->removeFile(strDownloadPath);
	remove(strDownloadPath.c_str());
    std::string strTmpFile = strDownloadPath + DOWNLOAD_TMP_FILE_EXT;
    //通过临时文件判断是否已经下载过
    //if (!FileUtils::getInstance()->isFileExist(strTmpFile))
    if (access(strTmpFile.c_str(), 0) == -1)
    {
        //文件不存在，新的下载
        if (!DownloadInit())
        {
            return false;
        }
        //最优级线程个数
        int iCount = (int)ceil(1.0*m_ulFullFileSize/(MMBLOCK_SIZE));
        // 线程个数不能超过iThreadCountX
        const int iThreadCount = (m_bSupportMultiDown ? min(iCount, iThreadCountX) : 1);
        m_ulDownFileSize = 0;
        unsigned long ulFileSize = m_ulFullFileSize;
        //临时信息长度
        const int iTmpInfoLen = iThreadCount*3*sizeof(unsigned long) + sizeof(unsigned long);
        //写入临时数据
        const unsigned long ulTmpFileLen = ulFileSize + iTmpInfoLen;

        //创建临时文件
        m_pFile = fopen(strTmpFile.c_str(), "wb+");
        if (!m_pFile)
        {
            RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 临时文件打开失败, l", this);
            return false;
        }
        //文件定位至未尾倒数四字节起始位置, 将文件大小写入最后4字节中
  //   if (fseek(m_pFile, (int)-sizeof(unsigned long), SEEK_END)
  //          || 0 == fwrite(&ulFileSize, sizeof(unsigned long), 1, m_pFile))
        if (fseek(m_pFile, (int)(ulTmpFileLen-sizeof(unsigned long)), SEEK_SET)
            || 0 == fwrite(&ulFileSize, sizeof(unsigned long), 1, m_pFile))
        {
            RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 临时信息初始化写入失败", this);
            goto Loop;
        }
        //每个线程负责下载的数据库大小
        const unsigned long ulBlockSize = ulFileSize/iThreadCount;
        int off = - iTmpInfoLen;
        fseek(m_pFile, off, SEEK_END);
        //根据文件大小及线程个数，划分每个线程负责下载的数据区间
        for (int i = 0 ; i < iThreadCount; ++i)
        {
            ThreadInfo* pInfo = new ThreadInfo;
            pInfo->ulBegPos = i * ulBlockSize;
            if (i == iThreadCount - 1)
            {
                pInfo->ulBlockSize = ulFileSize - i * ulBlockSize;
            }
            else
            {
                pInfo->ulBlockSize = ulBlockSize;
            }
            m_vecThrdInfo.push_back(pInfo);
            //在文件尾再写入每个线程的下载数据信息：开始位置(4B)，负责下载的数据大小(4B)，
            //已经下载的大小(4B)
            fwrite(&pInfo->ulBegPos, sizeof(unsigned long), 1, m_pFile);
            fwrite(&pInfo->ulBlockSize, sizeof(unsigned long), 1, m_pFile);
            fwrite(&pInfo->ulRecvSize, sizeof(unsigned long), 1, m_pFile);
        }
        fflushex(m_pFile);
        
        for (int j = 0; j < iThreadCount; ++j)
        {
            ThreadInfo* pInfo = m_vecThrdInfo[j];
            //这里传入线程的参数既需要pInfo的地址，又需要m_pFile和m_mutexFile，所以需要传入
            //this指针及pInfo在m_vecThrdInfo中的索引，格式定义为:this+i字符串形式
            char* param = new char[32];
            memset(param, 0, 32);
           // memset(param,0,32);
            sprintf(param, "%p+%X", this, j);
            if (!CurlInit(param)
                || pthread_create(&pInfo->thrdId, NULL, DownloadFun, param))
            {
                RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 线程%d开启失败", this, j);
                goto Loop;
            }
        }
        return true;
    }
    else
    {
        //文件存在，解析临时文件，获取上一次未完成的信息，继续下载
        RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 根据上次的临时文件，进行断点续传", this);
        return ContinueDownloadByTmpFile();
    }
    
Loop:
    //暂停已经开始的线程
    Pause();
    FinalProc();
    return false;
}

bool CurlDownloader::Pause()
{
    m_bPause = true;
    bool bRet = true;
    std::vector<ThreadInfo*>::const_iterator ita = m_vecThrdInfo.begin();
    while (ita != m_vecThrdInfo.end())
    {
        ThreadInfo* pInfo = *ita++;
        if (pInfo && pInfo->ulRecvSize < pInfo->ulBlockSize)
        {
            //线程已经执行完就无需暂停
            if (pInfo->pCurl /*&& pInfo->thrdId.p*/ && pthread_kill(pInfo->thrdId, 0) != ESRCH
                && pthread_join(pInfo->thrdId, NULL))
            {
                RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 线程%d暂停失败", this,
                                (int)(ita - m_vecThrdInfo.begin()) - 1);
                bRet = false;
                //这里若暂停失败就有隐患
            }
        }
    }
    return bRet;
}

bool CurlDownloader::Resume()
{
    m_bPause = false;
    bool bRunFinish = true;
    //若线程正在执行或文件已经下载完毕，则直接退出
    if (CheckIsFinish(&bRunFinish) || !bRunFinish) return false;
    //未完成的线程再次开启
    std::vector<ThreadInfo*>::const_iterator ita = m_vecThrdInfo.begin();
    //
    while (ita != m_vecThrdInfo.end())
    {
        ThreadInfo* pInfo = *ita++;
        if (pInfo && pInfo->ulBlockSize > pInfo->ulRecvSize)
        {
            char* param = new char[32];
            memset(param, 0, 32);
            sprintf(param, "%p+%X", this, int(ita - m_vecThrdInfo.begin()) - 1);
            //没下载完，可能是因为暂停，也可能是因为接收失败
            if ((!pInfo->pCurl && !CurlInit(param))
                || pthread_create(&pInfo->thrdId, NULL, DownloadFun, param))
            {
                RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 线程%d还原失败", this,
                                (int)(ita - m_vecThrdInfo.begin()) - 1);
                //恢复原来状态
                Pause();
                m_bPause = true;
                return false;
            }
            else
            {
                //线程还原成功
            }
        }
    }
    return true;
}

bool CurlDownloader::Terminate()
{
    m_bTerminate = true;
    if (Pause())
    {
        pthread_mutex_lock(&m_mutexFile);
        if (m_pFile)
        {
            fclose(m_pFile);
            m_pFile = NULL;
        }
        pthread_mutex_unlock(&m_mutexFile);
        //删除临时文件及本地文件
        //FileUtils::getInstance()->removeFile(m_strDownloadPath);
        remove(m_strDownloadPath.c_str());
        std::string strTmpPath = m_strDownloadPath + DOWNLOAD_TMP_FILE_EXT;
        //FileUtils::getInstance()->removeFile(strTmpPath);
        remove(strTmpPath.c_str());
        //
        m_ulDownFileSize = 0;
        ClearThreadInfo();
        return true;
    }
    return false;
}

bool CurlDownloader::CurlInit(void* param)
{
    CurlDownloader* pDown = NULL;
    ThreadInfo* pInfo = NULL;
    int index = -1;
    ParseThreadParam(param, &pDown, &index);
    if (pDown && index != -1) pInfo = pDown->m_vecThrdInfo[index];
    if (pInfo)
    {
        //CURL* _curl = curl_easy_init();
        CURL* _curl = create_share_curl();
        if (!_curl) return false;
        // 增加代理设置
        CURLcode code;
        if (!pDown->m_strProxy.empty())
        {
            code = curl_easy_setopt(_curl, CURLOPT_PROXY, pDown->m_strProxy.c_str());
            if (code != CURLE_OK) goto Loop;
        }
        code = curl_easy_setopt(_curl, CURLOPT_URL, pDown->m_strUrl.c_str());
        if (code != CURLE_OK) goto Loop;
        // 支持SSL [15:21:22 6/21/2015 zhangfang]
        CheckSSL(_curl, pDown->m_strUrl);
        code = curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, &CurlDownloader::WriteData);
        if (code != CURLE_OK) goto Loop;
        code = curl_easy_setopt(_curl, CURLOPT_WRITEDATA, param);
        if (code != CURLE_OK) goto Loop;
        //重定向
        code = curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
        if (code != CURLE_OK) goto Loop;
        // 优化性能，防止超时崩溃
        code = curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
        if (code != CURLE_OK) goto Loop;
        // 当传输速度< 1 字节/秒 持续 5 秒时,该连接会终止，用这个限制接收超时
        code = curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
        if (code != CURLE_OK) goto Loop;
        code = curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, /*5L*/30L);
        if (code != CURLE_OK) goto Loop;
        // 只能限制连接超时，因为下载时间不可控
        code = curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT, /*15L*/MM_TIMEOUT);
        if (code != CURLE_OK) goto Loop;
        pInfo->pCurl = _curl;
        pInfo->iTryTimes = 0;
        //线程初始化完毕!
        return true;
    Loop:
        curl_easy_cleanup(_curl);
        return false;
    }
    return false;
}

void* CurlDownloader::DownloadFun(void* param)
{
    CurlDownloader* pDown = NULL;
    ThreadInfo* pInfo = NULL;
    int index = -1;
    long lCode = 0;
    ParseThreadParam(param, &pDown, &index);
    if (pDown && index != -1) pInfo = pDown->m_vecThrdInfo[index];
    if (pInfo)
    {
        unsigned long ulBegPos, ulUnRecvSize, ulCurRecvSize;
        CURLcode code;
        bool bRet;
      //  const int iOnceMaxRecvSize = MMBLOCK_SIZE;
        while (!pDown->m_bPause)
        {
            // 重试机制
        ReTry:
            //分段下载
            if (pInfo->ulRecvSize < pInfo->ulBlockSize)
            {
                //未接收的数据大小
                ulUnRecvSize = pInfo->ulBlockSize - pInfo->ulRecvSize;
                //本次要接收的数据大小
                ulCurRecvSize = ulUnRecvSize;
                //确定起始位置
                ulBegPos = pInfo->ulBegPos + pInfo->ulRecvSize;
                bRet = false;
                // 对于不支持多线程下载的，若设置Range，有时会接收不到数据
                if (pDown->m_bSupportMultiDown)
                {
                    char* range = new char[32];
                    memset(range, 0, 32);
                    // 多请求一字节，防止下载时少最后一字节现象
					if(index == pDown->m_vecThrdInfo.size() - 1)
						sprintf(range, "%lu-%lu", ulBegPos, ulBegPos + ulCurRecvSize - 1);
					else
						sprintf(range, "%lu-%lu", ulBegPos, ulBegPos + ulCurRecvSize/* - 1*/);
					
					RICH_FORMAT_LOG("线程:%d,%s",index,range);
                    code = curl_easy_setopt(pInfo->pCurl, CURLOPT_RANGE, range);
                }
                else
                {
                    code = CURLE_OK;
                }
                if (code == CURLE_OK)
                {
                    code = curl_easy_perform(pInfo->pCurl);
                    if (code == CURLE_OK)
                    {
                        // 有时返回CURLE_OK，但却没接收到数据
                        bRet = (ulUnRecvSize > pInfo->ulBlockSize - pInfo->ulRecvSize);
                    }
                    // 返回CURLE_OK，没接收到数据，也需要重试
                    //else
                    if (!bRet)
                    {
                        // 若下载失败是由于暂停或中止，则不必重试
                        if (!pDown->m_bPause && !pDown->m_bTerminate)
                        {
                            // 获取错误码
                            curl_easy_getinfo(pInfo->pCurl, CURLINFO_RESPONSE_CODE, &lCode);
                            pDown->m_iHttpCode = lCode;
                            pDown->m_curlCode = code;
                            if (pInfo->iTryTimes < MAX_RETRY_TIMES)
                            {
                                // 重新初始化 重试次数要备份
                                int tryTimes = pInfo->iTryTimes;
                                curl_easy_cleanup(pInfo->pCurl);
                                pInfo->pCurl = NULL;
                                if (pDown->CurlInit(param)/*1*/)
                                {
                                    // 修正已经重试的次数
                                    pInfo->iTryTimes = tryTimes;
                                    // 重试时间：一秒，两秒，四秒
                                    float iVal = pow(2.0, pInfo->iTryTimes);
                                    sleep(iVal);
                                    pInfo->iTryTimes++;
                                    // 对于单线程下载，重试要从0开始
                                    if (!pDown->m_bSupportMultiDown)
                                    {
                                        pInfo->ulRecvSize = 0;
                                        pDown->m_ulDownFileSize = 0;
                                    }
                                    //重试时的下载进度
                                    float fPercent =
                                    100.0*(pDown->m_ulDownFileSize)/(pDown->m_ulFullFileSize);
                                    RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 下载进度:%f%%, 线程%d下载数据失败，lastHttpCode: %ld, CURLcode: %d, 尝试第%d次重试",
                                                    pDown, fPercent, index, lCode, code,
                                                    pInfo->iTryTimes);
                                    
                                    goto ReTry;
                                }
                                else
                                {
                                    RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 线程%d初始化失败，无法尝试重试", pDown, index);
                                }
                            }
                            else
                            {
                                RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 线程%d重试下载失败",
                                                pDown, index);
                            }
                        }
                    }
                }
                //
                if (!bRet)
                {
                    RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 线程%d接收数据失败，错误码：CURLCode=%d", pDown, index, code);
                    break;
                }
            }
            else
            {
                //此线程数据已经接收完毕
                //if (pInfo->iTryTimes)
                {
                    RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 线程%d接收数据完毕，重试次数：%d", pDown, index, pInfo->iTryTimes);
                }
                break;
            }
        }
        
        if (1)
        {
            curl_easy_cleanup(pInfo->pCurl);
            pInfo->pCurl = NULL;
        }
    }
    return NULL;
}


bool CurlDownloader::ContinueDownloadByTmpFile()
{
    std::string strTmpPath = m_strDownloadPath + DOWNLOAD_TMP_FILE_EXT;
    //以读写二进制方式打开文件
    m_pFile = fopen(strTmpPath.c_str(), "rb+");
    //下载初始化，可用来检测文件是否有更新
    if (!m_pFile)
    {
        pthread_mutex_unlock(&m_mutexCurl);
    }
    if (m_pFile && DownloadInit())
    {
        //定位到文件未尾，先读取临时文件总长度
        fseek(m_pFile, 0, SEEK_END);
        long long ulTmpFileSize = ftell(m_pFile);
        //再获取要下载的文件长度，跟初始化获取到的长度比较，若不同，可能是资源有更新，需要重新下载
        unsigned long ulFileSize = 0;
        if (fseek(m_pFile, (int)-sizeof(unsigned long), SEEK_END) == 0
            && fread(&ulFileSize, sizeof(unsigned long), 1, m_pFile)
            && ulFileSize == m_ulFullFileSize
            && ulFileSize < ulTmpFileSize)
        {
            long ulTmp = ulTmpFileSize - ulFileSize - sizeof(unsigned long);
            //计算上一次的线程个数
            int iThreadCount = (int)ulTmp/(sizeof(unsigned long)*3);
            m_ulDownFileSize = 0;
            //定位至下载信息处，获取上一次的下载信息
            if (0 == fseek(m_pFile, ulFileSize, SEEK_SET))
            {
                unsigned long ulBegPos, ulBlockSize, ulRecvSize;
                // 当前分块信息是否有效
                bool bInfoValid;
                for (int i = 0; i < iThreadCount; ++i)
                {
                    bInfoValid = true;
                    if (m_bSupportMultiDown)
                    {
                        //支持多线程下载，才可能断点续传
                        fread(&ulBegPos, sizeof(unsigned long), 1, m_pFile);
                        fread(&ulBlockSize, sizeof(unsigned long), 1, m_pFile);
                        fread(&ulRecvSize, sizeof(unsigned long), 1, m_pFile);
                        if (ulBegPos == -1 || ulBlockSize == -1 || ulRecvSize == -1
                            || ulBegPos >= ulFileSize || ulBlockSize > ulFileSize
                            || ulRecvSize > ulFileSize || ulRecvSize > ulBlockSize
                            || ulBlockSize == 0)
                        {
                            bInfoValid = false;
                        }
                        
                        if (i == 0 && ulBegPos != 0)
                        {
                            bInfoValid = false;
                        }
                        
                    }
                    else
                    {
                        //单线程不支持断点续传，直接重新开始
                        RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 单线程不支持断点续传，直接重新开始", this);
                        ulBegPos = 0;
                        ulBlockSize = ulFileSize;
                        ulRecvSize = 0;
                    }
                    //
                    // 检测断点信息是否有效
                    if (i)
                    {
                        ThreadInfo* pLastInfo = m_vecThrdInfo[i - 1];
                        if (pLastInfo->ulBegPos + pLastInfo->ulBlockSize != ulBegPos)
                        {
                            bInfoValid = false;
                        }
                    }
                    
                    ThreadInfo* pInfo = new ThreadInfo;
                    if (bInfoValid)
                    {
                        //此分块信息有效
                        pInfo->ulBegPos = ulBegPos;
                        pInfo->ulBlockSize = ulBlockSize;
                        pInfo->ulRecvSize = ulRecvSize;
                        m_ulDownFileSize += ulRecvSize;
                    }
                    else
                    {
                        //此分块信息无效，弃之，重新下载此块
                        RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 断点续传第%d块信息有误，从新下载此块数据", this, i);
                        pInfo->ulBegPos = i * (ulFileSize/iThreadCount);
                        if (i == iThreadCount - 1)
                        {
                            pInfo->ulBlockSize = ulFileSize - i * (ulFileSize/iThreadCount);
                        }
                        else
                        {
                            pInfo->ulBlockSize = ulFileSize/iThreadCount;
                        }
                    }
                    m_vecThrdInfo.push_back(pInfo);
                }
                
                RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 断点续传开始，上一次已下载百分比%f%%", this, m_ulDownFileSize*100.0/m_ulFullFileSize);
                //
                for (int j = 0; j < iThreadCount; ++j)
                {
                    ThreadInfo* pInfo = m_vecThrdInfo[j];
                    if (pInfo->ulRecvSize < pInfo->ulBlockSize)
                    {
                        //此子线程上一次未下载完，重新开始下载
                        //这里传入线程的参数既需要pInfo的地址，又需要m_pFile和m_mutexFile，所以需要传入
                        //this指针及pInfo在m_vecThrdInfo中的索引，格式定义为:this+i字符串形式
                        char* param = new char[32];
                        memset(param, 0, 32);
                        sprintf(param, "%p+%X", this, j);
                        if (!CurlInit(param)
                            || pthread_create(&pInfo->thrdId, NULL, DownloadFun, param))
                        {
                            //暂停已经开始的下载
                            Pause();
                            fclose(m_pFile);
                            m_pFile = NULL;
                            return false;
                        }
                    }
                }
                //
                return true;
            }
        }
    }
    
    RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 临时文件有问题，重新开始下载", this);
    //临时文件有问题，删除，重新开始下载
    pthread_mutex_lock(&m_mutexFile);
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
    pthread_mutex_unlock(&m_mutexFile);
    //若删除不掉会死锁 解决删除不掉临时文件的情况
    
    if (access(strTmpPath.c_str(), 0) != -1 && !remove(strTmpPath.c_str()))
    {
        char ch[16]={0};
        int flag = 0;
        std::string strPath;
        do
        {
            memset(ch, 0, sizeof(ch));
            sprintf(ch, "~bk%d", flag++);
            strPath = m_strDownloadPath + std::string(ch);
        } while (access(strPath.c_str(), 0) != -1);
        m_strDownloadPath = strPath;
        RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 临时文件被占用无法删除，更改下载目标文件名为%s", this, strPath.c_str());
    }
    return Start(m_strUrl, m_strDownloadPath);
}

CURL *CurlDownloader::create_share_curl()
{
    CURL *curl_handle = curl_easy_init();
    //curl_easy_setopt(curl_handle, CURLOPT_SHARE, sharedns_handle);
    //curl_easy_setopt(curl_handle, CURLOPT_DNS_CACHE_TIMEOUT, 60 * 5);
    return curl_handle;
}

bool CurlDownloader::DownloadInit()
{

    struct timeval now,last;
    gettimeofday(&last, nullptr);
    
    RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p DownloadInit begin", this);
    //确定文件长度
    if (CheckFileLength())
    {
        //确定是否支持分片传输
        if (CheckIsSupportRange())
        {
            if (m_bSupportMultiDown)
            {
                //进一步确认是否支持
                if (!CheckIsSupportRangeEx())
                {
                    pthread_mutex_unlock(&m_mutexCurl);
                    return false;
                }
            }
        }
        else
        {
            pthread_mutex_unlock(&m_mutexCurl);
            return false;
        }
    }
    else
    {
        pthread_mutex_unlock(&m_mutexCurl);
        return false;
    }
    //
    gettimeofday(&now, nullptr);

    RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p DownloadInit end, IsMutilDonwload: %d, FileSize: %ldB, Take time=%f", this, (m_bSupportMultiDown ? 1 : 0), m_ulFullFileSize,(now.tv_sec - last.tv_sec) + (now.tv_usec - last.tv_usec) / 1000000.0f);
    pthread_mutex_unlock(&m_mutexCurl);
    //
    // 0KB文件不再支持下载，当404报
    if (m_ulFullFileSize == 0)
    {
        m_iHttpCode = 404;
    }
    return m_ulFullFileSize != 0;
}

//解析参数
void CurlDownloader::ParseThreadParam(void* param, CurlDownloader** ppDown, int* pIndex)
{
    if (param)
    {
        std::string strTmp((const char*)param);
        //解析参数
        std::string strParam1, strParam2;
        std::string::size_type pos = strTmp.find('+');
        if (pos != std::string::npos)
        {
            strParam1 = strTmp.substr(0, pos);
            strParam2 = strTmp.substr(pos + 1, strTmp.length() - pos + 1);
            CurlDownloader* pDown = (CurlDownloader*)strtoul(strParam1.c_str(), NULL, 16);
            if (ppDown) *ppDown = pDown;
            int index = (int)strtoul(strParam2.c_str(), NULL, 16);
            if (pDown && index < pDown->m_vecThrdInfo.size())
            {
                if (pIndex) *pIndex = index;
            }
        }
    }
}

size_t CurlDownloader::HeaderInfo(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::string* pHead = (std::string*)userdata;
    if (pHead)
    {
        pHead->append(std::string((char*)ptr, nmemb));
    }
    return size*nmemb;
}

size_t CurlDownloader::WriteData(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    //判断是否支持多线程分片上传
    if (userdata)
    {
        std::string strData((const char*)userdata);
        if (strData.find(RANGE_TEST_FLAG) == 0)
        {
            const int len = strlen(RANGE_TEST_FLAG);
            strData = strData.substr(len, strData.size() - len);
            int* pRecvSize = (int*)strtoul(strData.c_str(), NULL, 16);
            if (pRecvSize)
            {
                *pRecvSize += size*nmemb;
                if (*pRecvSize > g_iRangeTestSize)
                {
                    //此时已经确定不支持多线程下载了，直接返回
                    return size*nmemb - 1;
                }
            }
            return size*nmemb;
        }
    }
    CurlDownloader* pDown = NULL;
    ThreadInfo* pInfo = NULL;
    int index = -1;
    ParseThreadParam(userdata, &pDown, &index);
    if (pDown && index != -1) pInfo = pDown->m_vecThrdInfo[index];
    //
    size_t writeLen = 0;
    if (pInfo)
    {
        //若中止，则直接返回
        if (pDown->m_bTerminate)
        {
            return size*nmemb - 1;
        }
        pthread_mutex_lock(&pDown->m_mutexFile);
        if (pInfo->ulRecvSize < pInfo->ulBlockSize)
        {
            if (0 == fseek(pDown->m_pFile, pInfo->ulBegPos + pInfo->ulRecvSize, SEEK_SET))
            {
                unsigned long ulUnRecvSize = pInfo->ulBlockSize - pInfo->ulRecvSize;
                size_t toWriteCount = (ulUnRecvSize > size * nmemb ? nmemb : ulUnRecvSize/size);
                writeLen = fwrite(ptr, size, toWriteCount, pDown->m_pFile);
                pInfo->ulRecvSize += writeLen*size;
            }
            else
            {
                RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, _fseeki64 error!",pDown);
            }
        }
        else
        {
            //线程数据接收完
        }
		
        //
        pDown->m_ulDownFileSize += writeLen;
        pthread_mutex_unlock(&pDown->m_mutexFile);
        // 为优化断点续传改为即时更新临时信息
        pDown->UpdateDownloadInfoInTmpFile(&index);
        
        //如果多线程暂停，立即返回，单线程不存在暂停
        if (pDown->m_bPause/* && pDown->m_bSupportMultiDown*/)
        {
            // 单线程就让其暂停，恢复时即重新开始下载
            if (!pDown->m_bSupportMultiDown)
            {
                pInfo->ulRecvSize = 0;
                pDown->m_ulDownFileSize = 0;
            }
            //通过返回值使curl_easy_peform返回
            return size*nmemb - 1;
        }
    }
    return /*writeLen*/size*nmemb;
}

bool CurlDownloader::CheckIsFinish(bool* pbThrdRunFinish)
{
    //各下载线程是否执行完
    bool bRunFinish = true;
    //文件是否下载完
    bool bDownFinish = true;
    pthread_mutex_lock(&m_mutexFile);
    std::vector<ThreadInfo*>::const_iterator ita = m_vecThrdInfo.begin();
    while (ita != m_vecThrdInfo.end())
    {
        ThreadInfo* pInfo = *ita++;
        if (pInfo)
        {
            if (bDownFinish)
                bDownFinish = (pInfo->ulBlockSize == pInfo->ulRecvSize);
            // 有时pCurl已经为空，但pthread_kill却返回0
            if (bRunFinish)
                bRunFinish = (!pInfo->pCurl /*|| !pInfo->thrdId.p*/
                              || pthread_kill(pInfo->thrdId, 0) == ESRCH);
            if (!bDownFinish && !bRunFinish) break;
        }
    }
    pthread_mutex_unlock(&m_mutexFile);
    // 若下载完就当成运行也完
	if (pbThrdRunFinish) *pbThrdRunFinish = (bDownFinish ? true : bRunFinish);
    return bDownFinish && !m_vecThrdInfo.empty();
}

void CurlDownloader::UpdateDownloadInfoInTmpFile(int* pIndex)
{
    pthread_mutex_lock(&m_mutexFile);
    if (m_pFile)
    {
        int off =(int)( - sizeof(unsigned long) - m_vecThrdInfo.size()*sizeof(unsigned long)*3);
        int iBeg, iEnd;
        if (pIndex)
        {
            //只更新此指定线程块的信息
            iBeg = *pIndex;
            iEnd = iBeg + 1;
            off += (*pIndex)*sizeof(unsigned long)*3;
        }
        else
        {
            //更新所有
            iBeg = 0;
            iEnd = (int)m_vecThrdInfo.size();
        }
        if (0 == fseek(m_pFile, off, SEEK_END))
        {
            ThreadInfo* pInfo = NULL;
            for (int i = iBeg; i < iEnd; ++i)
            {
                pInfo = m_vecThrdInfo[i];
                fwrite(&pInfo->ulBegPos, sizeof(unsigned long), 1, m_pFile);
                fwrite(&pInfo->ulBlockSize, sizeof(unsigned long), 1, m_pFile);
                fwrite(&pInfo->ulRecvSize, sizeof(unsigned long), 1, m_pFile);
            }
        }
    }
    fflushex(m_pFile);
    pthread_mutex_unlock(&m_mutexFile);
}

bool CurlDownloader::FinalProc()
{
    bool bRet = false;
    //判断是否下载完成
    bool bRunFinish = false;
    bool bFileDownFinish = CheckIsFinish(&bRunFinish);
    if (bRunFinish)
    {
        //若线程执行完，则关闭临时文件
        pthread_mutex_lock(&m_mutexFile);
        if (m_pFile)
        {
            fclose(m_pFile);
            m_pFile = NULL;
        }
        pthread_mutex_unlock(&m_mutexFile);
        //若文件下载完成，则修改临时文件长度
        if (bFileDownFinish)
        {
            std::string strTmpPath = m_strDownloadPath + DOWNLOAD_TMP_FILE_EXT;
            {
                //这段我读懂了，就是删除文件后面那部分然后重命名文件
                struct stat file_info;
                if(stat(strTmpPath.c_str(),&file_info) == 0)
                {
                    int off = (int)(file_info.st_size - sizeof(unsigned long) - m_vecThrdInfo.size()*sizeof(unsigned long)*3);
                    if(truncate(strTmpPath.c_str(),off) == 0)
                        bRet = true;
                }
                
                if (bRet)
                {
                    int pos = (int)m_strDownloadPath.rfind("~bk");
                    if (pos != std::string::npos)
                    {
                        m_strDownloadPath = m_strDownloadPath.substr(0, pos);
                        RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 下载目标文件名曾因断点续传时临时文件被占用而更改过，现在还原为%s", this, m_strDownloadPath.c_str());
                    }
                    //若文件下载完成，则重命名
                    bRet = (0 == rename(strTmpPath.c_str(), m_strDownloadPath.c_str()));
                    if (!bRet)
                    {
                        RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, rename Fail",
                                        this);
                    }
                }
            }
        }
    }
    return bRet;
}

void CurlDownloader::GetProgress(unsigned long* pTotalFileSize, unsigned long* pDownSize)
{
    if (pTotalFileSize) *pTotalFileSize = m_ulFullFileSize;
    if (pDownSize) *pDownSize = m_ulDownFileSize;
}

bool CurlDownloader::IsMutliDownload()
{
    return m_bSupportMultiDown;
}

bool CurlDownloader::IsRedirected()
{
    return m_bRedirected;
}

bool CurlDownloader::WaitForFinish()
{
    std::vector<ThreadInfo*>::const_iterator ita = m_vecThrdInfo.begin();
    while (ita != m_vecThrdInfo.end())
    {
        ThreadInfo* pInfo = *ita++;
        if (pInfo && pInfo->pCurl ) pthread_join(pInfo->thrdId, NULL);
    }
    bool bRet = FinalProc();
    return bRet;
}

CurlDownState CurlDownloader::GetCurlDownState()
{
    if (m_bPause)
    {
        return DOWN_PAUSE;
    }
    else if (m_bTerminate)
    {
        return DOWN_TERMINATE;
    }
    return DOWN_PROGRESS;
}

void CurlDownloader::SetProxy(const std::string& strProxy)
{
    m_strProxy = strProxy;
}

int CurlDownloader::GetLastHttpCode()
{
    return (int)m_iHttpCode;
}

CURLcode CurlDownloader::GetLastCurlCode()
{
    return m_curlCode;
}

void CurlDownloader::Init()
{
    /*CURLcode code = */curl_global_init(CURL_GLOBAL_ALL);
    if (!sharedns_handle)
    {
        sharedns_handle = curl_share_init();
        curl_share_setopt(sharedns_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);  
    }
}

void CurlDownloader::Uninit()
{
    if (sharedns_handle)
    {
        curl_share_cleanup(sharedns_handle);
        sharedns_handle = NULL;
    }
    curl_global_cleanup();
}

void CurlDownloader::CheckSSL(CURL* pCurl, const std::string& strUrl)
{
    //不喜欢https
    if (!pCurl || strUrl.empty()) return;
    //    static std::string strRootDir;
    //    if (strRootDir.empty())
    //    {
    //        char path[MAX_PATH]={0};
    //        GetModuleFileNameA(NULL, path, MAX_PATH);
    //        strRootDir = std::string(path);
    //        StringReplace(strRootDir, "/", "\\");
    //        int pos = (int)strRootDir.rfind('\\');
    //        strRootDir = strRootDir.substr(0, pos);
    //    }
    //
    std::string strTmp = strUrl.substr(0, 5);
    //transform(strTmp.begin(), strTmp.end(), strTmp.begin(), tolower);
    if (strTmp == "https")
    {
        curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0);
        
        //        std::string strCertPath = strRootDir + "\\pcapk.mmarket.com.crt";
        //        std::string strKeyPath = strRootDir + "\\pcapk.mmarket.com.key";
        //        curl_easy_setopt(pCurl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
        //        curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0);
        //        curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 1);
        //        curl_easy_setopt(pCurl, CURLOPT_SSLCERT, strCertPath.c_str());
        //        curl_easy_setopt(pCurl, CURLOPT_SSLKEY, strKeyPath.c_str());
    }
}

std::string CurlDownloader::GetRealUrl()
{
    return m_strUrl;
}

int CurlDownloader::GetThreadCount()
{
    return (int)m_vecThrdInfo.size();
}

bool CurlDownloader::CheckFileLength()
{
    struct timeval now,last;
    gettimeofday(&last, nullptr);
    double downloadFileLenth = -1; 
    ///////////////////////////////////////////
    // 获取下载文件长度为0要重试 
    int iTryTimes = 0;
    std::string strTmp;
ReTry0:
    std::string strUrl = m_strUrl;
    CURL *curl = create_share_curl(); 
ReTry1:
    if (m_bPause)
    {
        RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 遭到暂停", this);
        //pthread_mutex_unlock(&m_mutexCurl);
        return false;
    }
    if (!m_strProxy.empty())
    {
        safeCurlSetopt(curl, CURLOPT_PROXY, m_strProxy.c_str());
    }
    //兼顾重定向-强行用GET方法
    safeCurlSetopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    safeCurlSetopt(curl, CURLOPT_URL, strUrl.c_str());
    // 支持SSL [15:21:22 6/21/2015 zhangfang]
    CheckSSL(curl, strUrl);
    safeCurlSetopt(curl, CURLOPT_HEADER, 1);
    safeCurlSetopt(curl, CURLOPT_NOBODY, 1);
    // 整个请求超时控制
    safeCurlSetopt(curl, CURLOPT_TIMEOUT, /*15L*/MM_TIMEOUT);
    // 优化性能，防止超时崩溃 
    safeCurlSetopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    long lCode = 0;
    if (m_bPause)
    {
        RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 遭到暂停", this);
        //pthread_mutex_unlock(&m_mutexCurl);
        return false;
    }
    CURLcode code;
    try {
        code = curl_easy_perform(curl);
    } catch (...) {
        return false;
    }
    
    if (code == CURLE_OK)  
    {  	
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lCode);
        if (lCode == 301 || lCode == 302)
        {
            //遇到重定向，获取重定向后的url
            char *redirecturl = {0};
            curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &redirecturl);
            //myLog(coutx<<"遇到重定向，RedirectUrl: "<<redicturl<<"\n";)
            RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 遇到重定向，RedirectUrl: %s",
                            this, redirecturl);
            strUrl = redirecturl;
            m_bRedirected = true;
            goto ReTry1;
        }
        else if (lCode >= 200 && lCode < 300)
        {
            if (lCode != 200)
            {
                RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, HttpCode: %ld", this, lCode);
            }
            curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth); 
        }
    }
    else
    {
        RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 获取文件长度失败, CURLcode=%d", 
                        this, code);
    }
    if (m_pInitCurl != curl)
    {
        curl_easy_cleanup(curl);
        curl = NULL;
    }
    //
    m_curlCode = code;
    m_iHttpCode = lCode;
    //
    // 0字节文件大小也要重试
    if (downloadFileLenth == -1 || downloadFileLenth == 0)
    {
        if (lCode == 404)
        {
            //资源不存在，就不重试了
            //pthread_mutex_unlock(&m_mutexCurl);
            return false;
        }
        // 重试获取文件长度
        if (iTryTimes < MAX_RETRY_TIMES)
        {
            // 重试时间：一秒，两秒，四秒 
            float iVal = pow(2.0, iTryTimes);
            sleep(iVal);
            iTryTimes++;
            RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 获取文件长度失败, lastHttpCode:%ld, CURLcode:%d, 尝试第%d次重试", this, lCode, code, iTryTimes);
            goto ReTry0;
        }
        else
        {
            RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 重试获取文件长度失败, LastHttpCode:%ld", this, lCode);
            //RICHLOG<<"重试获取文件长度失败";
            gettimeofday(&now, nullptr);
            RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, end, Take time=%f", this,
                            (now.tv_sec - last.tv_sec) + (now.tv_usec - last.tv_usec) / 1000000.0f);
            //pthread_mutex_unlock(&m_mutexCurl);
            return false;
        }
    }
    
    if (iTryTimes)
    {
        RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 获取文件长度成功，重试次数：%d", 
                        this, iTryTimes);
    }
    
    // url重置
    m_strUrl = strUrl;
    /////////////////////////////////////////
    //获取要下载的文件大小
    m_ulFullFileSize = (unsigned long)downloadFileLenth; 
    
    return true;
}

bool CurlDownloader::CheckIsSupportRange()
{
    int iTryTimes = 0;
    std::string strUrl = m_strUrl;
    // 判断是否支持断点续传 
    // 文件小于最小分块，即使判断支持多线程，最后还是开一个线程，就没必要判断
    // 因为多线程影响到暂停，恢复，所以需要无条件判断
    if (m_ulFullFileSize/* > MMBLOCK_SIZE*/)
    {
        // 判断是否支持多线程也重试 
        iTryTimes = 0;
    ReTry2:
        if (m_bPause)
        {
            RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 遭到暂停", this);
            //pthread_mutex_unlock(&m_mutexCurl);
            return false;
        }
        //curl = curl_easy_init(); 
        CURL* curl = create_share_curl();
        if (!m_strProxy.empty())
        {
            safeCurlSetopt(curl, CURLOPT_PROXY, m_strProxy.c_str());
        }		
        safeCurlSetopt(curl, CURLOPT_URL, m_strUrl.c_str());  
        // 支持SSL [15:21:22 6/21/2015 zhangfang]
        CheckSSL(curl, strUrl);
        safeCurlSetopt(curl, CURLOPT_HEADER, 1); 
        safeCurlSetopt(curl, CURLOPT_NOBODY, 1);
        // 通过头部信息判断是否支持断点续传
        std::string strHeader;
        safeCurlSetopt(curl, CURLOPT_HEADERDATA, &strHeader);
        safeCurlSetopt(curl, CURLOPT_HEADERFUNCTION, &CurlDownloader::HeaderInfo);
        safeCurlSetopt(curl, CURLOPT_RANGE, "0-");
        // 整个请求超时控制
        safeCurlSetopt(curl, CURLOPT_TIMEOUT, /*15L*/MM_TIMEOUT);
        // 优化性能，防止超时崩溃 
        safeCurlSetopt(curl, CURLOPT_NOSIGNAL, 1L);
        CURLcode code = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl = NULL;
        if (code == CURLE_OK)
        {
            m_bSupportMultiDown = (/*strHeader.find("Accept-Ranges: bytes") != std::string::npos
                                    || */strHeader.find("Content-Range: bytes") != std::string::npos);
        }
        else
        {
            // 重试判断是否支持多线程 
            if (iTryTimes < MAX_RETRY_TIMES)
            {
                // 重试时间：一秒，两秒，四秒 
                float iVal = pow(2.0, iTryTimes);
                sleep(iVal);
                iTryTimes++;
                RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 判断是否支持多线程失败, CURLcode:%d, 尝试第%d次重试", this, code, iTryTimes);
                goto ReTry2;
            }
            else
            {
                RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 判断是否支持多线程失败，默认采用单线程下载", this);
            }
        }
    }
    else
    {
        //RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 文件大小<最小分块5M, 勿需判断是否支持多线程，直接采用单线程下载", this);
    }
    return true;
}

bool CurlDownloader::CheckIsSupportRangeEx()
{
    if (m_ulFullFileSize)
    {
        //判断是否支持多线程分片下载
        if (m_bPause)
        {
            RICH_FORMAT_LOG("CurlDownloader Obj Ptr: 0X%p, 遭到暂停", this);
            return false;
        }
        CURL* curl = create_share_curl(); 
        if (!m_strProxy.empty())
        {
            safeCurlSetopt(curl, CURLOPT_PROXY, m_strProxy.c_str());
        }
        safeCurlSetopt(curl, CURLOPT_URL, m_strUrl.c_str()); 
        safeCurlSetopt(curl, CURLOPT_WRITEFUNCTION, &CurlDownloader::WriteData); 
        //
        int iRecvSize = 0;
        char* param = new char[32];
        memset(param, 0, 32);
        sprintf(param, "%s%p", RANGE_TEST_FLAG, &iRecvSize);
        //
        safeCurlSetopt(curl, CURLOPT_WRITEDATA, param); 
        //重定向
        safeCurlSetopt(curl, CURLOPT_FOLLOWLOCATION, 1); 
        safeCurlSetopt(curl, CURLOPT_NOSIGNAL, 1L);
        //尝试下载一小段字节的数据
        g_iRangeTestSize = 
        m_ulFullFileSize > RANGE_TEST_RECV_SIZE ? RANGE_TEST_RECV_SIZE : (int)m_ulFullFileSize/2;
        char range[32] = {0};
        sprintf(range, "1-%d", g_iRangeTestSize);
        safeCurlSetopt(curl, CURLOPT_RANGE, range); 
        // 当传输速度< 1 字节/秒 持续 5 秒时,该连接会终止，用这个限制接收超时 
        safeCurlSetopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L);  
        safeCurlSetopt(curl, CURLOPT_LOW_SPEED_TIME, 5L); 
        // 只能限制连接超时，因为下载时间不可控
        safeCurlSetopt(curl, CURLOPT_CONNECTTIMEOUT, MM_TIMEOUT);
        CURLcode code = curl_easy_perform(curl);
        if (CURLE_OK == code)
        {
            m_bSupportMultiDown = true;
        }
        else
        {
            m_bSupportMultiDown = false;
        }
        curl_easy_cleanup(curl);
        curl = NULL;
        delete[] param;
        param = NULL;
    }
    else
    {
        //此文件太小，直接用单线程
        m_bSupportMultiDown = false;
    }
    return true;
}
