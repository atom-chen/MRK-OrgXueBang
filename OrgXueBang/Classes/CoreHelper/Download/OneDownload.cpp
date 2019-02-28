//

#include "stdafx.h"
#include "BaseDownload.h"
#include "OneDownload.h"
#include "CurlDownload.h"
#include "FileLog.h"

COneDownload::COneDownload(const std::string& url,const std::string savaPath,const std::string& md5)
{

    cocos2d::log("start");
    m_stuDownload.init(url,savaPath,md5);
    m_bRunThread = true;
    m_nTotalFileSize = 1;
    m_nDownSize = 0;
    
    std::string strTmpPath = m_stuDownload.m_strSavePath + DOWNLOAD_TMP_FILE_EXT;
    remove(strTmpPath.c_str());
    remove((m_stuDownload.m_strSavePath).c_str());
    
    m_tDown = std::thread(&COneDownload::Runthread,this);
}

COneDownload::~COneDownload(void)
{
    auto r = eBaseDownloadStatus::none;
    m_MutexDownload.lock();
    r = m_stuDownload.m_eStatusCur;
    m_MutexDownload.unlock();
    if (r != eBaseDownloadStatus::finish && r != eBaseDownloadStatus::error) {
        //正常结束，我不删文件
        cannel();
        while(getStatus() != eBaseDownloadStatus::error)
            ;
        
    }
    
	m_tDown.join();
}

void COneDownload::Runthread()
{
	bool bRun = true;
	while (bRun)
	{
		MyDownloadTimer();
        m_MutexDownload.lock();
        bRun = m_bRunThread;
        m_MutexDownload.unlock();
        if (bRun) {
            sleep(1);	//1S
        }
	}
}

void COneDownload::MyDownloadTimer()
{//下载线程
	m_MutexDownload.lock();

    switch(m_stuDownload.m_eStatusCur)
    {
    case   eBaseDownloadStatus::none://,			//不存在的任务
        
        break;
    case   eBaseDownloadStatus::wait://,			//
        if (m_stuDownload.m_pDownloader == nullptr)
            m_stuDownload.m_pDownloader = new CurlDownloader;
        m_stuDownload.setStatus(eBaseDownloadStatus::run);
        cocos2d::log("%s:11run",m_stuDownload.m_strKey.c_str());
        if( m_stuDownload.m_pDownloader->Start(m_stuDownload.m_strKey.c_str(),m_stuDownload.m_strSavePath))
        {
            cocos2d::log("%s:s:run",m_stuDownload.m_strKey.c_str());
        }
        else
        {
            CFileLog::LogSaveFormat("%s:s:run_error",m_stuDownload.m_strKey.c_str());
            m_stuDownload.setStatus(eBaseDownloadStatus::waitcancel);
        }
        break;
    case   eBaseDownloadStatus::run://,			//进行中(前置状态 wait)
        
        if (m_stuDownload.m_pDownloader) {
            //只有运行的
            bool bRunFinish = true;
            //获取下进度
            m_stuDownload.m_pDownloader->GetProgress(&(m_stuDownload.m_nTotalFileSize), &(m_stuDownload.m_nDownSize));

            bool bDownFinish = m_stuDownload.m_pDownloader->CheckIsFinish(&bRunFinish);
            
            if (bRunFinish)
            {
                //等待结束
                m_stuDownload.m_pDownloader->WaitForFinish();
                //释放内存
                delete m_stuDownload.m_pDownloader;
                m_stuDownload.m_pDownloader = nullptr;
                
                //若线程已经结束或文件已经下载完毕，则直等待结束
                if (m_stuDownload.m_nDownSize >= m_stuDownload.m_nTotalFileSize && bDownFinish)
                {
                    //md5检测
                    bool bOk = false;
                    if (m_stuDownload.m_strMd5 == "null") {
                        bOk = true;
                    }
                    else
                    {
                        Data d = FileUtils::getInstance()->getDataFromFile( m_stuDownload.m_strSavePath);
                        if (d.getSize())
                        {
                            CMd5Code md5;
                            md5.GenerateMD5(d.getBytes(),(int)d.getSize());
                            cocos2d::log("%s,%s",md5.ToString().c_str(),m_stuDownload.m_strMd5.c_str());
                            if (md5.ToString() == m_stuDownload.m_strMd5 || m_stuDownload.m_strMd5.length() != 32)
                                bOk = true;
                            else
                                CFileLog::LogSaveFormat("%s,%s",md5.ToString().c_str(),m_stuDownload.m_strMd5.c_str());
                        }
                    }
                    
                    if (bOk)
                    {//下载完成
                        cocos2d::log("%s:finish",m_stuDownload.m_strKey.c_str());
                        m_stuDownload.setStatus(eBaseDownloadStatus::finish);
                        m_bRunThread = false;
                    }
                    else
                    {//下载失败
                        cocos2d::log("%s:error",m_stuDownload.m_strKey.c_str());
                        m_stuDownload.setStatus(eBaseDownloadStatus::waitcancel);
                    }
                }
                else
                {
                    CFileLog::LogSaveFormat("%s:error_f",m_stuDownload.m_strKey.c_str());
                    m_stuDownload.setStatus(eBaseDownloadStatus::waitcancel);
                }
            }
        }
        else
        {
            //到这就扯了，不可能的，
            CFileLog::LogSaveFormat("*****CBaskDownload::MyDownloadTimer************m_pDownloader==null********");
            //直接err
            m_stuDownload.setStatus(eBaseDownloadStatus::waitcancel);
        }
        
        break;
    case   eBaseDownloadStatus::waitpause://,		//准备暂停(前置状态run,wait)
        break;
    case   eBaseDownloadStatus::puase://,			//暂停(前置状态waitpause)
        break;
    case   eBaseDownloadStatus::finish://,			//完事（前置状态run）
        m_bRunThread = false;
        break;
    case   eBaseDownloadStatus::waitcancel://,		//被取消
        {
            if (m_stuDownload.m_pDownloader)
            {
                m_stuDownload.m_pDownloader->Terminate();
                delete m_stuDownload.m_pDownloader;
            }
            m_stuDownload.m_pDownloader = nullptr;
            std::string strTmpPath = m_stuDownload.m_strSavePath + DOWNLOAD_TMP_FILE_EXT;
            remove(strTmpPath.c_str());
            remove(( m_stuDownload.m_strSavePath).c_str());
            cocos2d::log("%s:del",m_stuDownload.m_strKey.c_str());
            m_stuDownload.setStatus(eBaseDownloadStatus::error);
            
            m_bRunThread = false;
        }
        break;
    case   eBaseDownloadStatus::error://,			//错误
        m_bRunThread = false;
        break;
    case   eBaseDownloadStatus::lock://,           //被锁状态
        break;
    }

	m_MutexDownload.unlock();
	//结束。sleep
}

/*取消的任务*/
eBaseDownloadStatus     COneDownload::cannel()
{
	auto r = eBaseDownloadStatus::none;
	m_MutexDownload.lock();
    cocos2d::log("%s:waitcancel",m_stuDownload.m_strKey.c_str());
    m_stuDownload.setStatus(eBaseDownloadStatus::waitcancel);
    r = eBaseDownloadStatus::waitcancel;
	m_MutexDownload.unlock();
    cocos2d::log("cannel1");
	return r;
}

/**/
eBaseDownloadStatus     COneDownload::getStatus()
{

    if (m_MutexDownload.try_lock())
    {
        auto r = m_stuDownload.m_eStatusCur;
        m_MutexDownload.unlock();
        return r;
    }
    return eBaseDownloadStatus::lock;
}

void    COneDownload::getProgress(unsigned long& nTotalFileSize, unsigned long& nDownSize)
{

    if (m_MutexDownload.try_lock())
    {
        m_nTotalFileSize = m_stuDownload.m_nTotalFileSize;
        m_nDownSize = m_stuDownload.m_nDownSize;
        m_MutexDownload.unlock();
    }
    nTotalFileSize = m_nTotalFileSize;
    nDownSize = m_nDownSize;
}
