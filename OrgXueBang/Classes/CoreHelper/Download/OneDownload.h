
/*
 下载
 by：高超
 */

#ifndef __ONE_DOWNLOAD_h
#define __ONE_DOWNLOAD_h

class COneDownload
{
public:
    
    COneDownload(const std::string& url,const std::string savaPath,const std::string& md5 = "null");
    ~COneDownload(void);

    /*取消的任务，使任务进入等待取消，完全取消后才能del 否则del卡住不管*/
    eBaseDownloadStatus     cannel();
    /*获取任务状态*/
    eBaseDownloadStatus     getStatus();
	/*获取进度，只在run状态是正确的，其他状态看心情*/
	void    getProgress(unsigned long& nTotalFileSize, unsigned long& nDownSize);
private:
    
	/*线程*/
	void Runthread();
	/*ontimer*/
	void MyDownloadTimer();
	/*任务出错*/
	void     error();

	/*所有任务*/
    stuBaseDownload m_stuDownload;
	std::mutex m_MutexDownload;
	std::thread m_tDown;
	bool		m_bRunThread;
    unsigned long m_nTotalFileSize;
    unsigned long m_nDownSize;
};

#endif
