
/*
 下载调度类，单例
 by：高超
 */

#ifndef __BASE_DOWNLOAD_h
#define __BASE_DOWNLOAD_h

class CurlDownloader;

/*
 下载说明
 状态：	默认开始是等待状态，会自动进入运行状态，同时只能有N个任务下载，也就是所有任务进入run前 都是wait。
		暂停，需要暂停中的过度，过度状态不允许任何操作。这个回头再加
		未下载的完的任务，下次启动程序，都会进入暂停中，若想启动，只能调用start
 记录：  所有的记录都会保存，不管是不是完成的，除非调用cancel，就会将下载好的文件删除，

 key:	以下载地址记录唯一ID 下载前需要知道MD5 校验
 
 延迟：	所有的操作全部都是在非主线程中进行，设置之后不会立即执行，需要等待线程刷新，所以还需要回调么？不要了，需要显示的时候请getstatus
 */


//最多同时运行几个下载任务，其他的只能等待。宏随便改，越大线程越多，不一定会快。
#define MYDEF_DOWNLOAD_TASK_ALL	3

enum class eBaseDownloadStatus
{
	none,			//不存在的任务
	wait,			//默认的开始是等待状态，准备开始,继续任务也会进入等待状态，只能继续处于pause的waitpause的不可以。
	run,			//进行中(前置状态 wait)
	waitpause,		//准备暂停(前置状态run,wait)
	puase,			//暂停(前置状态waitpause)
	finish,			//完事（前置状态run）
	waitcancel,		//被取消，可以理解为删除中,等待删除删除，我将删除下载的文件
	error,			//错误（各种状态都能错误，需要先删除再开始）
    lock,           //被锁状态
  //  md5fail         //md5没有通过校验
};


//所有的下载在这里都应该能查到，旧的要有记录，下载路径和下载名。
//如果取消下载就删除记录
//完成的要有记录
typedef struct stuBaseDownload
{
    //ID
    std::string				m_strKey;
    std::string				m_strSavePath;
	std::string				m_strMd5;
    eBaseDownloadStatus		m_eStatusCur;
    CurlDownloader*			m_pDownloader;
	unsigned long			m_nTotalFileSize;
	unsigned long			m_nDownSize;
	void init(const std::string& key,const std::string savaPath,const std::string strMd5)
    {
        m_strKey = key;
        m_strSavePath = savaPath;
		m_strMd5 = strMd5;
        m_pDownloader = nullptr;
		m_eStatusCur = eBaseDownloadStatus::wait;
		m_nTotalFileSize = 1;
		m_nDownSize = 0;
    };
	void setStatus(eBaseDownloadStatus eStatus)
	{
		m_eStatusCur = eStatus;
	};
}stuBaseDownload;


class CBaskDownload
{
public:
    
    static CBaskDownload* getInstance();
    static void destroyInstance();

    CBaskDownload(void);
    ~CBaskDownload(void);
	void init();
	void free();
    /*开始任务
     返回-1代表存在运行任务
     返回0是OK*/
	int						start(const std::string& key,const std::string savaPath,const std::string& md5);
    /*暂停任务,wait run 可以暂停，不可暂停或者找不到返回 none*/
    eBaseDownloadStatus     pause(const std::string& key);
    /*继续任务，只对暂停的有效，只对暂停的有效，找不到或者不可继续返回none*/
    eBaseDownloadStatus     resumes(const std::string& key);
    /*取消的任务，使任务进入等待取消，完全取消后可再次重新开始*/
    eBaseDownloadStatus     cannel(const std::string& key);
    /*获取任务状态*/
    eBaseDownloadStatus     getStatus(const std::string& key);
	/*获取进度，只在run状态是正确的，其他状态看心情*/
	void    getProgress(const std::string& key,unsigned long& nTotalFileSize, unsigned long& nDownSize);
	/*获取所有任务名字*/
	void	getAllDownload(std::vector<std::string>& taskArray );
private:
	/*线程*/
	void Runthread();
	/*ontimer*/
	void MyDownloadTimer();
	/*任务出错*/
	void     error(const std::string& key);
	/*file*/
	void	save();
	void	load();
	/*所有任务*/
    std::map<std::string,stuBaseDownload> m_stuDownloadMap;
	std::mutex m_MutexDownloadMap;
	std::thread m_tDown;
	bool		m_bRunThread;
    std::string m_strPath;
};

#endif
