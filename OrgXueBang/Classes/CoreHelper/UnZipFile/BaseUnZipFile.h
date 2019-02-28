
/*
解压
 by：高超
 */

#ifndef __BASE_UNZIPFILE_h
#define __BASE_UNZIPFILE_h

#include "MyUnZip.h"
/*
 下载说明
 状态：	默认开始是等待状态，会自动进入运行状态，同时只能有N个任务解压，也就是所有任务进入run前 都是wait。
 
 延迟：	所有的操作全部都是在非主线程中进行，设置之后不会立即执行，需要等待线程刷新，所以还需要回调么？不要了，需要显示的时候请getstatus
 */


//最多同时解压
#define MYDEF_UNZIP_TASK_ALL	3

enum class eBaseUnZipStatus
{
	none,			//不存在的任务
	wait,			//默认的开始是等待状态，准备开始,继续任务也会进入等待状态，只能继续处于pause的waitpause的不可以。
	run,			//进行中(前置状态 wait)
	finish,			//完事（前置状态run）
	error,			//错误（各种状态都能错误，需要先删除再开始）
};

typedef std::function<void(std::string,eBaseUnZipStatus)> CMyUnZipCallback;

//所有的下载在这里都应该能查到，旧的要有记录，下载路径和下载名。
//如果取消下载就删除记录
//完成的要有记录
typedef struct stuBaseUnZip
{
    //ID
    std::string				m_strKey;
    //std::string				m_strSavePath;
    eBaseUnZipStatus		m_eStatusCur;
    CMyUnZip*				m_pUnZip;
	unsigned long			m_nTotalFile;
	unsigned long			m_nCurFile;
	CMyUnZipCallback		m_pCallBack;
    std::string				m_strOutDir;
	void init(const std::string& key)
    {
        m_strKey = key;
        //m_strSavePath = savaPath;
        m_pUnZip = nullptr;
		m_eStatusCur = eBaseUnZipStatus::wait;
		m_nTotalFile = 1;
		m_nCurFile = 0;
		m_pCallBack = nullptr;
        m_strOutDir = "null/";
    };
	void setStatus(eBaseUnZipStatus eStatus)
	{
		m_eStatusCur = eStatus;
		if(m_pCallBack)
			m_pCallBack(m_strKey,m_eStatusCur);
	};
}stuBaseUnZip;


class CBaskUnZip:public Ref
{
public:
    
    static CBaskUnZip* getInstance();
    static void destroyInstance();

    CBaskUnZip(void);
    ~CBaskUnZip(void);
	void init();
	void free();
    void	save();
    void	load();
    /*开始任务
     返回-1代表存在运行任务
     返回0是OK*/
	int                 start(const std::string& key);
    /*删除*/
    void                 remove(const std::string& key);
    /*获取任务状态*/
    eBaseUnZipStatus    getStatus(const std::string& key);
    /*获取存储路径*/
    const std::string   getOutDir(const std::string& key);
	/*获取进度，只在run状态是正确的，其他状态看心情*/
	void                getProgress(const std::string& key,unsigned long& nTotalFile, unsigned long& nCurFile);
	/*获取所有任务名字*/
	void                getAllUnZip(std::vector<std::string>& taskArray );
	/*设置回调*/
	void                setStatusCall(std::string key,CMyUnZipCallback pCall = nullptr);
private:
	/*ontimer*/
	void                MyUnZipTimer(float dt);
	/*所有任务*/
    std::map<std::string,stuBaseUnZip> m_stuUnZipMap;
};

#endif
