//

#include "stdafx.h"
#include "CurlDownload.h"
#include "BaseDownload.h"
#include "CCBuffer.h"
#include "FileLog.h"


#define MYDEF_DOWNLOAD_FILE  "file_downloadcache.save"

static CBaskDownload* g_CBaskDownload = nullptr;

CBaskDownload* CBaskDownload::getInstance()
{
    if (g_CBaskDownload == nullptr) {
        g_CBaskDownload = new CBaskDownload;
		g_CBaskDownload->init();
    }
    return g_CBaskDownload;
}

void CBaskDownload::destroyInstance()
{
    if (g_CBaskDownload) {
		g_CBaskDownload->free();
		delete g_CBaskDownload;
        g_CBaskDownload = nullptr;
    }
}

CBaskDownload::CBaskDownload(void)
{
	CurlDownloader::Init();
}

CBaskDownload::~CBaskDownload(void)
{
	CurlDownloader::Uninit();
}

void CBaskDownload::init()
{
    m_strPath = FileUtils::getInstance()->getWritablePath();
	m_stuDownloadMap.clear();
	//加载
	load();
	//启动线程
	m_bRunThread = true;
	m_tDown = std::thread(&CBaskDownload::Runthread,this);
}

void CBaskDownload::free()
{
	m_MutexDownloadMap.lock();
	//保存进度
	save();
	//释放资源
	m_stuDownloadMap.clear();
	m_bRunThread = false;
	m_MutexDownloadMap.unlock();
	//结束线程
	m_tDown.join();
}

void	CBaskDownload::save()
{
	//不需要加锁
	//存档只有暂停.完成和错误
	
	//return;

    auto pBuff =  new CCBuffer() ;
	
	//个数
	int nNum = (int)m_stuDownloadMap.size();
	pBuff->writeInt(nNum);
	
	for (auto it = m_stuDownloadMap.begin() ; it != m_stuDownloadMap.end(); it++)
	{
		//key
		//path
		//md5
		//status

		pBuff->writeLengthAndString(it->second.m_strKey.c_str());
		pBuff->writeLengthAndString(it->second.m_strSavePath.c_str());
		pBuff->writeLengthAndString(it->second.m_strMd5.c_str());
		
		pBuff->writeInt((int)(it->second.m_eStatusCur));
		pBuff->writeInt((int)(it->second.m_nTotalFileSize));
		pBuff->writeInt((int)(it->second.m_nDownSize));

	}
	Data d ;
	d.copy((unsigned char *)pBuff->data(), pBuff->length());
	std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_DOWNLOAD_FILE;
	FileUtils::getInstance()->writeDataToFile(d,path);
    delete pBuff;
}

void	CBaskDownload::load()
{
	//return;
	//不需要加锁
	//加载所有的
	
	//
	std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_DOWNLOAD_FILE;
	Data d = FileUtils::getInstance()->getDataFromFile(path);
	if (d.getSize())
	{
        auto pBuff =  new CCBuffer((const char*)d.getBytes(),(unsigned int)d.getSize());
		//个数
		int nNum = pBuff->readInt();
		for (int i = 0 ; i < nNum; i++) {
			//key
			//path
			//md5
			//status
			stuBaseDownload stu;
			stu.m_strKey = pBuff->readLengthAndString();
			stu.m_strSavePath = pBuff->readLengthAndString();
			stu.m_strMd5 = pBuff->readLengthAndString();
			stu.m_eStatusCur = (eBaseDownloadStatus)pBuff->readInt();
			//存档只有暂停.完成和错误
			switch (stu.m_eStatusCur) {
				case eBaseDownloadStatus::none:			//不存在的任务
					break;
				case eBaseDownloadStatus::wait://,			//默认的开始是等待状态，准备开始,继续任务也会进入等待状态，只能继续处于pause的waitpause的不可以。
				case eBaseDownloadStatus::run://,			//进行中(前置状态 wait)
				case eBaseDownloadStatus::waitpause://,		//准备暂停(前置状态run,wait)
					stu.m_eStatusCur = eBaseDownloadStatus::puase;
					break;
				case eBaseDownloadStatus::puase://,			//暂停(前置状态waitpause)
					break;
				case eBaseDownloadStatus::finish://,			//完事（前置状态run）
					break;
				case eBaseDownloadStatus::waitcancel://,		//被取消，可以理解为删除中,等待删除删除，我将删除下载的文件
					//stu.m_eStatusCur = eBaseDownloadStatus::error;
					break;
				case eBaseDownloadStatus::error://,			//错误（各种状态都能错误，需要先删除再开始）
					break;
				default:
					break;
			}
			
			stu.m_pDownloader = nullptr;
			stu.m_nTotalFileSize = pBuff->readInt();
			stu.m_nDownSize = pBuff->readInt();
			m_stuDownloadMap[stu.m_strKey] = stu;
		}
        delete pBuff;
	}
}

void CBaskDownload::Runthread()
{
	bool bRun = true;
	while (bRun)
	{
		m_MutexDownloadMap.lock();
		bRun = m_bRunThread;
		m_MutexDownloadMap.unlock();
		MyDownloadTimer();
		sleep(1);	//1S
        //cocos2d::log("1111");
	}
}

void CBaskDownload::MyDownloadTimer()
{//下载线程
	m_MutexDownloadMap.lock();
	bool bNeedSave = false;
	////////////////////////////////////////////////////////////////////////////////////////
	//暂停需要暂停的
	for (auto it = m_stuDownloadMap.begin(); it != m_stuDownloadMap.end(); it++)
	{
		if (it->second.m_eStatusCur == eBaseDownloadStatus::waitpause)
		{
			if (it->second.m_pDownloader)
			{
				delete it->second.m_pDownloader;
				it->second.m_pDownloader = nullptr;
			}
			cocos2d::log("%s:pause",it->second.m_strKey.c_str());
			it->second.setStatus(eBaseDownloadStatus::puase);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
    //完成正在运行的
	int nCurRun = 0;
	for (auto it = m_stuDownloadMap.begin(); it != m_stuDownloadMap.end(); it++)
	{
		//运行的是否完成
		if (it->second.m_eStatusCur == eBaseDownloadStatus::run)
		{
			nCurRun++;
			if (it->second.m_pDownloader) {
				//只有运行的
				bool bRunFinish = true;
				//获取下进度
				it->second.m_pDownloader->GetProgress(&(it->second.m_nTotalFileSize), &(it->second.m_nDownSize));
				//cocos2d::log("%p,%ld,%ld",it->second.m_pDownloader,it->second.m_nTotalFileSize,it->second.m_nDownSize);
                
                bool bDownFinish = it->second.m_pDownloader->CheckIsFinish(&bRunFinish);
                
                if (bRunFinish)
                {
                    //等待结束
                    it->second.m_pDownloader->WaitForFinish();
                    //释放内存
                    delete it->second.m_pDownloader;
                    it->second.m_pDownloader = nullptr;
                    
                    //若线程已经结束或文件已经下载完毕，则直等待结束
                    if (it->second.m_nDownSize >= it->second.m_nTotalFileSize && bDownFinish)
                    {
                        //md5检测
                        bool bOk = false;
                        Data d = FileUtils::getInstance()->getDataFromFile(m_strPath + it->second.m_strSavePath);
                        if (d.getSize())
                        {
                            CMd5Code md5;
                            md5.GenerateMD5(d.getBytes(),(int)d.getSize());
                            cocos2d::log("%s,%s",md5.ToString().c_str(),it->second.m_strMd5.c_str());
                            if (md5.ToString() == it->second.m_strMd5 || it->second.m_strMd5.length() != 32)
                                bOk = true;
                            else
                                CFileLog::LogSaveFormat("%s,%s",md5.ToString().c_str(),it->second.m_strMd5.c_str());
                        }
                        if (bOk)
                        {//下载完成
                            cocos2d::log("%s:finish",it->second.m_strKey.c_str());
                            it->second.setStatus(eBaseDownloadStatus::finish);
                        }
                        else
                        {//下载失败
                            CFileLog::LogSaveFormat("%s:error",it->second.m_strKey.c_str());
                            cocos2d::log("%s:error",it->second.m_strKey.c_str());
                            it->second.setStatus(eBaseDownloadStatus::error);
                        }
                        
                        bNeedSave = true;
                    }
                    else
                    {
                        cocos2d::log("%s:error_f",it->second.m_strKey.c_str());
                        it->second.setStatus(eBaseDownloadStatus::error);
                    }
                }
			}
			else
			{
				//到这就扯了，不可能的，
                CFileLog::LogSaveFormat("*****CBaskDownload::MyDownloadTimer************m_pDownloader==null********");
				//直接err
				error(it->second.m_strKey);
			}
		}
	}
	//继续等待运行的
	for (auto it = m_stuDownloadMap.begin(); it != m_stuDownloadMap.end() && MYDEF_DOWNLOAD_TASK_ALL > nCurRun; it++)
	{//开启wait
		//运行的是否完成
		if (it->second.m_eStatusCur == eBaseDownloadStatus::wait)
		{
			if (it->second.m_pDownloader == nullptr)
				it->second.m_pDownloader = new CurlDownloader;
			it->second.setStatus(eBaseDownloadStatus::run);
            cocos2d::log("%s:11run",it->second.m_strKey.c_str());
			if( it->second.m_pDownloader->Start(it->second.m_strKey.c_str(),m_strPath + it->second.m_strSavePath))
            {
                cocos2d::log("%s:s:run",it->second.m_strKey.c_str());
            }
            else
            {
                CFileLog::LogSaveFormat("%s:s:run_error",it->second.m_strKey.c_str());
                it->second.setStatus(eBaseDownloadStatus::error);
            }
			
			nCurRun++;
            break;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	//删除需要删除的
	bool bDel = false;
	do
	{
		bDel = false;
		for (auto it = m_stuDownloadMap.begin(); it != m_stuDownloadMap.end(); it++)
		{
			if (it->second.m_eStatusCur == eBaseDownloadStatus::waitcancel)
			{
				if (it->second.m_pDownloader)
				{
					it->second.m_pDownloader->Terminate();
					delete it->second.m_pDownloader;
				}
				std::string strTmpPath = m_strPath + it->second.m_strSavePath + DOWNLOAD_TMP_FILE_EXT;
				//FileUtils::getInstance()->removeFile(strTmpPath);
				remove(strTmpPath.c_str());
				remove((m_strPath + it->second.m_strSavePath).c_str());
				cocos2d::log("%s:del",it->second.m_strKey.c_str());
				m_stuDownloadMap.erase(it);
				bDel = true;
				bNeedSave = true;
				break;
			}
		}
	}while (bDel);
	
	if (bNeedSave)
		save();

	m_MutexDownloadMap.unlock();
	//结束。sleep
}

int CBaskDownload::start(const std::string& key,const std::string savaPath,const std::string& md5)
{
    //开始任务,
    //查找石是否存在项目
	int r = -1;
    cocos2d::log("start");
	m_MutexDownloadMap.lock();
    auto it = m_stuDownloadMap.find(key);
    if (it == m_stuDownloadMap.end())
    {
        //需要新建
		stuBaseDownload stu;
		stu.init(key,savaPath,md5);
		m_stuDownloadMap[key] = stu;
		r =  0;
		cocos2d::log("%s:wait",stu.m_strKey.c_str());
    }
	m_MutexDownloadMap.unlock();
    cocos2d::log("start1");
    return r;
}

/*暂停任务*/
eBaseDownloadStatus CBaskDownload::pause(const std::string& key)
{
    cocos2d::log("pause");
	auto r = eBaseDownloadStatus::none;
	m_MutexDownloadMap.lock();
	auto it = m_stuDownloadMap.find(key);
	if (it != m_stuDownloadMap.end() &&
		(it->second.m_eStatusCur == eBaseDownloadStatus::run||it->second.m_eStatusCur == eBaseDownloadStatus::wait))
	{//运行和run可暂停
		it->second.setStatus(eBaseDownloadStatus::waitpause);
		r = eBaseDownloadStatus::waitpause;
		cocos2d::log("%s:waitpause",it->second.m_strKey.c_str());
	}
	m_MutexDownloadMap.unlock();
    cocos2d::log("pause1");
	return r;
}

/*继续任务，只对暂停的有效*/
eBaseDownloadStatus     CBaskDownload::resumes(const std::string& key)
{
    cocos2d::log("resumes");
	auto r = eBaseDownloadStatus::none;
	m_MutexDownloadMap.lock();
	auto it = m_stuDownloadMap.find(key);
	if (it != m_stuDownloadMap.end() && it->second.m_eStatusCur == eBaseDownloadStatus::puase)
	{
		it->second.setStatus(eBaseDownloadStatus::wait);
		r = eBaseDownloadStatus::wait;
		cocos2d::log("%s:wait",it->second.m_strKey.c_str());
	}
	m_MutexDownloadMap.unlock();
    cocos2d::log("resumes1");
	return r;
}

/*取消的任务，取消后可再次开始*/
eBaseDownloadStatus     CBaskDownload::cannel(const std::string& key)
{
    cocos2d::log("cannel");
	auto r = eBaseDownloadStatus::none;
	m_MutexDownloadMap.lock();
	auto it = m_stuDownloadMap.find(key);
	if (it != m_stuDownloadMap.end())
	{//所有任务都可以等待取消
		cocos2d::log("%s:waitcancel",it->second.m_strKey.c_str());
		it->second.setStatus(eBaseDownloadStatus::waitcancel);
		r = eBaseDownloadStatus::waitcancel;
	}
	m_MutexDownloadMap.unlock();
    cocos2d::log("cannel1");
	return r;
}

/**/
eBaseDownloadStatus     CBaskDownload::getStatus(const std::string& key)
{

    if (m_MutexDownloadMap.try_lock())
    {
        auto r = eBaseDownloadStatus::none;
        
        //m_MutexDownloadMap.lock();
        auto it = m_stuDownloadMap.find(key);
        if (it != m_stuDownloadMap.end())
        {
            r = it->second.m_eStatusCur;
        }
        m_MutexDownloadMap.unlock();
       // cocos2d::log("getStatus1");
        return r;
    }
  //  cocos2d::log("getStatus2");
    return eBaseDownloadStatus::lock;
}

void    CBaskDownload::getProgress(const std::string& key,unsigned long& nTotalFileSize, unsigned long& nDownSize)
{

   // cocos2d::log("getProgress");

    nTotalFileSize = 1;
    nDownSize = 0;
    if (m_MutexDownloadMap.try_lock())
    {
        auto it = m_stuDownloadMap.find(key);
        if (it != m_stuDownloadMap.end())
        {
            nTotalFileSize = it->second.m_nTotalFileSize;
            nDownSize = it->second.m_nDownSize;
        }
        m_MutexDownloadMap.unlock();
    }

}

void     CBaskDownload::error(const std::string& key)
{
//    cocos2d::log("error");
	//这个不需要锁
	auto it = m_stuDownloadMap.find(key);
	if (it != m_stuDownloadMap.end())
	{
		if (it->second.m_pDownloader) {
			delete it->second.m_pDownloader;
			it->second.m_pDownloader = nullptr;
		}
		cocos2d::log("%s:error",it->second.m_strKey.c_str());
		it->second.setStatus(eBaseDownloadStatus::error);
	}
//    cocos2d::log("error1");
}

/*获取所有任务*/
void	CBaskDownload::getAllDownload(std::vector<std::string>& taskArray )
{
//    cocos2d::log("getAllDownload1");
	m_MutexDownloadMap.lock();
	taskArray.clear();
	for (auto it = m_stuDownloadMap.begin(); it != m_stuDownloadMap.end(); it++)
	{
		taskArray.push_back(it->second.m_strKey);
	}
	m_MutexDownloadMap.unlock();
//    cocos2d::log("getAllDownload2");
}
