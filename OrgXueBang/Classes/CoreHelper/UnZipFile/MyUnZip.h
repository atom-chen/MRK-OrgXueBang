//
//  MyLabel.hpp
//  HelloWorldDemo
//
//  Created by 高超 on 16/7/1.
//
//

/*
文件解压
 by：高超
 */

#ifndef MyUNZIP_h
#define MyUNZIP_h

enum eMyUnZipStatus
{
	run,
	error,
	ok,
};

class CMyUnZip
{
public:
	//获取进度
	CMyUnZip(std::string file);
	~CMyUnZip();
	void GetProgress(unsigned long& nTotalFile, unsigned long& nCurFile);
	eMyUnZipStatus getStatus();
	std::string basename(const std::string& path) const;
	void ThreadUnZip();
    std::string& getOutDir(){return m_strOutDir; };
private:
	eMyUnZipStatus m_eStatus;
	std::mutex m_Mutex;
	std::string m_strFile;
    std::string m_strOutDir;
	FileUtils* _fileUtils;
	unsigned long m_nTotalFile;
	unsigned long m_nCurFile;
	std::thread m_tRun;
};


#endif
