//

#include "stdafx.h"
#include "BaseUnZipFile.h"
#include "CCBuffer.h"

#define MYDEF_UNZIP_FILE "unzipfile.save"

static CBaskUnZip* g_CBaskUnZip = nullptr;

CBaskUnZip* CBaskUnZip::getInstance()
{
    if (g_CBaskUnZip == nullptr) {
        g_CBaskUnZip = new CBaskUnZip;
		g_CBaskUnZip->autorelease();
		g_CBaskUnZip->retain();
		g_CBaskUnZip->init();
    }
    return g_CBaskUnZip;
}

void CBaskUnZip::destroyInstance()
{
    if (g_CBaskUnZip) {
		g_CBaskUnZip->free();
		g_CBaskUnZip->release();
		//delete g_CBaskUnZip;
        g_CBaskUnZip = nullptr;
    }
}

CBaskUnZip::CBaskUnZip(void)
{

}

CBaskUnZip::~CBaskUnZip(void)
{

}

void CBaskUnZip::init()
{
	m_stuUnZipMap.clear();
    load();
	Director::getInstance()->getScheduler()->schedule(schedule_selector(CBaskUnZip::MyUnZipTimer),this,0.5f,false);
}

void CBaskUnZip::free()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(CBaskUnZip::MyUnZipTimer),this);
    save();
	//释放资源
	m_stuUnZipMap.clear();
}

void	CBaskUnZip::save()
{
    auto pBuff =  CCBuffer::create();
    
    //个数
    int nNum = (int)m_stuUnZipMap.size();
    pBuff->writeInt(nNum);
    
    for (auto it = m_stuUnZipMap.begin() ; it != m_stuUnZipMap.end(); it++)
    {
        pBuff->writeLengthAndString(it->second.m_strKey.c_str());
        pBuff->writeInt((int)(it->second.m_eStatusCur));
        pBuff->writeLengthAndString(it->second.m_strOutDir.c_str());
    }
    Data d ;
    d.copy((unsigned char *)pBuff->data(), pBuff->length());
    std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_UNZIP_FILE;
    FileUtils::getInstance()->writeDataToFile(d,path);
}

void	CBaskUnZip::load()
{
    //return;
    //不需要加锁
    //加载所有的
    m_stuUnZipMap.clear();
    
    std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_UNZIP_FILE;
    Data d = FileUtils::getInstance()->getDataFromFile(path);
    if (d.getSize())
    {
        auto pBuff =  CCBuffer::create((const char*)d.getBytes(),(unsigned int)d.getSize());
        //个数
        int nNum = pBuff->readInt();
        for (int i = 0 ; i < nNum; i++) {

            stuBaseUnZip stu;
            stu.init(pBuff->readLengthAndString());
            stu.m_eStatusCur = (eBaseUnZipStatus)pBuff->readInt();
            stu.m_strOutDir = pBuff->readLengthAndString();
            //存档只有暂停.完成和错误
            switch (stu.m_eStatusCur) {
                case eBaseUnZipStatus::none://,			//不存在的任务
                case eBaseUnZipStatus::wait://,			//默认的开始是等待状态，准备开始,继续任务也会进入等待状态，只能继续处于pause的waitpause的不可以。
                case eBaseUnZipStatus::run://,			//进行中(前置状态 wait)
                    stu.m_eStatusCur = eBaseUnZipStatus::wait;
                    break;
                case eBaseUnZipStatus::finish://,			//完事（前置状态run）
                case eBaseUnZipStatus::error://,			//错误（各种状态都能错误，需要先删除再开始）
                    break;
            }

            m_stuUnZipMap[stu.m_strKey] = stu;
        }
    }
}

void CBaskUnZip::MyUnZipTimer(float dt)
{//下载线程
	////////////////////////////////////////////////////////////////////////////////////////
    //完成正在运行的
    bool bNeedSave = false;
	int nCurRun = 0;
	for (auto it = m_stuUnZipMap.begin(); it != m_stuUnZipMap.end(); it++)
	{
		//运行的是否完成
		if (it->second.m_eStatusCur == eBaseUnZipStatus::run)
		{
			nCurRun++;
			if (it->second.m_pUnZip && it->second.m_pUnZip->getStatus() != eMyUnZipStatus::run)
			{
				cocos2d::log("%s:finish",it->second.m_strKey.c_str());
				if (it->second.m_pUnZip->getStatus() == eMyUnZipStatus::ok)
					it->second.setStatus(eBaseUnZipStatus::finish);
				else
					it->second.setStatus(eBaseUnZipStatus::error);
                it->second.m_strOutDir = it->second.m_pUnZip->getOutDir();
				delete it->second.m_pUnZip;
				it->second.m_pUnZip = nullptr;
                bNeedSave = true;
			}
		}
	}
	//继续等待运行的
	for (auto it = m_stuUnZipMap.begin(); it != m_stuUnZipMap.end() && MYDEF_UNZIP_TASK_ALL > nCurRun; it++)
	{//开启wait
		//运行的是否完成
		if (it->second.m_eStatusCur == eBaseUnZipStatus::wait)
		{
			if (it->second.m_pUnZip == nullptr)
                it->second.m_pUnZip = new CMyUnZip(FileUtils::getInstance()->getWritablePath() + it->second.m_strKey);
			it->second.setStatus(eBaseUnZipStatus::run);
			cocos2d::log("%s:run",it->second.m_strKey.c_str());
			nCurRun++;
		}
	}
    if (bNeedSave) {
        save();
    }
}

int CBaskUnZip::start(const std::string& key)
{
    //开始任务,
    //查找石是否存在项目
	int r = -1;
    auto it = m_stuUnZipMap.find(key);
    if (it == m_stuUnZipMap.end())
    {
        //需要新建
		stuBaseUnZip stu;
		stu.init(key);
		m_stuUnZipMap[key] = stu;
		r =  0;
		cocos2d::log("%s:wait",stu.m_strKey.c_str());
    }
	else if(it->second.m_eStatusCur == eBaseUnZipStatus::error)
	{//错误课重新开始
		it->second.m_eStatusCur = eBaseUnZipStatus::wait;
		cocos2d::log("%s:restart to  wait",it->second.m_strKey.c_str());
	}
    return r;
}

void CBaskUnZip::remove(const std::string& key)
{
    auto it = m_stuUnZipMap.find(key);
    if (it != m_stuUnZipMap.end())
        m_stuUnZipMap.erase(it);
}

/**/
eBaseUnZipStatus     CBaskUnZip::getStatus(const std::string& key)
{
	auto r = eBaseUnZipStatus::none;
	auto it = m_stuUnZipMap.find(key);
	if (it != m_stuUnZipMap.end())
	{
		r = it->second.m_eStatusCur;
	}
    return r;
}

const std::string  CBaskUnZip::getOutDir(const std::string& key)
{
    std::string r = "null/";
    auto it = m_stuUnZipMap.find(key);
    if (it != m_stuUnZipMap.end())
    {
        return it->second.m_strOutDir;
    }
    return r;
}

void    CBaskUnZip::getProgress(const std::string& key,unsigned long& nTotalFileSize, unsigned long& nCurFile)
{
	auto it = m_stuUnZipMap.find(key);
	if (it != m_stuUnZipMap.end() && it->second.m_pUnZip)
	{
		it->second.m_pUnZip->GetProgress(nTotalFileSize, nCurFile);
	}
}

/*获取所有任务*/
void	CBaskUnZip::getAllUnZip(std::vector<std::string>& taskArray )
{
	taskArray.clear();
	for (auto it = m_stuUnZipMap.begin(); it != m_stuUnZipMap.end(); it++)
	{
		taskArray.push_back(it->second.m_strKey);
	}
}

/*设置回调*/
void	CBaskUnZip::setStatusCall(std::string key,CMyUnZipCallback pCall)
{
	auto it = m_stuUnZipMap.find(key);
	if (it != m_stuUnZipMap.end() && it->second.m_pUnZip)
	{
		it->second.m_pCallBack = pCall;
	}
}
