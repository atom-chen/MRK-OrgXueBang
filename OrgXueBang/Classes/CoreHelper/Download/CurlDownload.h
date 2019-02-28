#pragma once


#include "curl/curl.h"

//采用curl/pthreads库实现多线程下载 [zhangfang]

//#include "curl/curl.h"
//#include "../../cocos2d/external/curl/include/ios/curl/curl.h"
//#include "pthread.h"
//#include <string>
//#include <vector>


//多线程下载每次传输的数据块的大小
#define MMBLOCK_SIZE 1024*1024*5
//下载临时文件后缀
#define DOWNLOAD_TMP_FILE_EXT ".dltmp"
//超时时间
#define MM_TIMEOUT 60L
//失败重试次数限制
#define MAX_RETRY_TIMES 2
//检测是否支持多线程分片传输的字符串
#define RANGE_TEST_FLAG "RangeTest"
//用于检测是否支持多线程下载接收大小
#define RANGE_TEST_RECV_SIZE 1024


using namespace std;

//线程信息
typedef struct _tThreadInfo{
    unsigned long ulBegPos;  //下载起始位置
    unsigned long ulBlockSize; //本线程负责下载的数据大小
    unsigned long ulRecvSize; //本线程已经接收到的数据大小
    CURL* pCurl;
    pthread_t thrdId;
    int iTryTimes;  //失败已经重试次数
    _tThreadInfo()
    {
        ulBegPos = ulBlockSize = ulRecvSize = 0;
        pCurl = NULL;
        //
        //thrdId.p = NULL;
        //thrdId.x = 0;
        iTryTimes = 0;
    }
}ThreadInfo;

//下载状态
enum CurlDownState{DOWN_PROGRESS, DOWN_PAUSE, DOWN_TERMINATE};
//线程数
#define MAX_DOWN_THREAD_COUNT 3

class CurlDownloader
{
public:
    CurlDownloader(void);
    ~CurlDownloader(void);
    //开始下载
    bool Start(const std::string&strUrl, const std::string&strDownloadPath,
               int iThreadCountX = MAX_DOWN_THREAD_COUNT);
    //暂停
    bool Pause();
    //恢复下载，支持断点续传
    bool Resume();
    //终止
    bool Terminate();
    //阻塞等待下载完
    bool WaitForFinish();
    //获取下载进度
    void GetProgress(unsigned long* pTotalFileSize, unsigned long* pDownSize);
    //是否是多线程下载
    bool IsMutliDownload();
    //
    bool IsRedirected();
    //检测是否完成下载
    bool CheckIsFinish(bool* pbThrdRunFinish);
    //获取当前状态
    CurlDownState GetCurlDownState();
    //设置代理
    void SetProxy(const std::string& strProxy);
    //获取错误码
    int GetLastHttpCode();
    //获取CURL错误码
    CURLcode GetLastCurlCode();
    //
    std::string GetRealUrl();
    int GetThreadCount();
    //全局初始化
    static void Init();
    static void Uninit();
protected:
    //下载初始化
    bool DownloadInit();
    //curl初始化
    bool CurlInit(void* param);
    //下载函数
    static void* DownloadFun(void* param);
    // 头信息
    static size_t HeaderInfo(char *ptr, size_t size, size_t nmemb, void *userdata);
    //写数据
    static size_t WriteData(char *ptr, size_t size, size_t nmemb, void *userdata);
    //解析线程参数
    static void ParseThreadParam(void* param, CurlDownloader** ppDown, int* pIndex);
    //通过临时文件进行断点续传
    bool ContinueDownloadByTmpFile();
    //最后处理
    bool FinalProc();
    //更新临时文件中指定线程块下载信息
    void UpdateDownloadInfoInTmpFile(int* pIndex);
    //清理线程信息
    void ClearThreadInfo();
    //检测SSL
    static void CheckSSL(CURL* pCurl, const std::string& strUrl);
    //检测文件长度
    bool CheckFileLength();
    //检测是否支持分片传输
    bool CheckIsSupportRange();
    bool CheckIsSupportRangeEx();
private:
    // 创建curl对象
    CURL *create_share_curl();
    //文件
    FILE* m_pFile;
    //用于控制写文件的互斥变量
    pthread_mutex_t m_mutexFile;
    //用于保存每个线程的信息的数组
    std::vector<ThreadInfo*> m_vecThrdInfo;
    //下载地址及本地保存路径
    std::string m_strUrl, m_strDownloadPath;
    //代理
    std::string m_strProxy;
    //是否暂停
    bool m_bPause;
    //是否中止下载
    bool m_bTerminate;
    //是否支持多线程下载
    bool m_bSupportMultiDown;
    //要下载的文件大小
    unsigned long m_ulFullFileSize;
    //已经下载的文件大小
    unsigned long m_ulDownFileSize;
    //HTTP错误码
    long m_iHttpCode;
    //是否重定向过，重定向之前的URL
    bool m_bRedirected;
    std::string m_strOriginalUrl;
    //
    CURLcode m_curlCode;
    // 用于解决广西复杂网络环境的CURL
    CURL* m_pInitCurl;
    pthread_mutex_t m_mutexCurl;
    // 共享dns处理对象
    static CURLSH* sharedns_handle;
};


//CurlDownloader::Init();
//CurlDownloader down;
////开始下载，支持断点续传
//bool bRet = down.Start("http://url.52z.com/down/%CD%BC%C6%AC%B8%C4%D0%A1%B9%A4%BE%DF@13_117981.exe", "c:\\pictool.exe");
////在此过程中可获取进度，可暂停，还原，中止
////........................
////等待下载执行完
//bRet = down.WaitForFinish();
////
//CurlDownloader::Uninit();
//system("pause");
//return 0;
