
#include "stdafx.h"
#include "unzip/unzip.h"
#include "MyUnZip.h"

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

CMyUnZip::CMyUnZip(std::string file)
{
	m_eStatus = eMyUnZipStatus::run;
	
	//启动一个线程，开始解压，
	//进度
	//锁
	//其他？
    _fileUtils = FileUtils::getInstance();
    m_strFile = file;
	
	m_nTotalFile = 1;
	m_nCurFile = 0;
    m_strOutDir = "null";
	m_tRun = std::thread(&CMyUnZip::ThreadUnZip,this);
}

CMyUnZip::~CMyUnZip()
{
	//结束线程
	m_tRun.join();
}

void CMyUnZip::GetProgress(unsigned long& nTotalFile, unsigned long& nCurFile)
{
	m_Mutex.lock();
	nTotalFile = m_nTotalFile;
	nCurFile = m_nCurFile;
	m_Mutex.unlock();
}

eMyUnZipStatus CMyUnZip::getStatus()
{
	eMyUnZipStatus r;
	m_Mutex.lock();
	r = m_eStatus;
	m_Mutex.unlock();
	return r;
}

std::string CMyUnZip::basename(const std::string& path) const
{
	size_t found = path.find_last_of("/\\");
	
	if (std::string::npos != found)
	{
		return path.substr(0, found);
	}
	else
	{
		return path;
	}
}

void CMyUnZip::ThreadUnZip()
{
	
	std::string zip = m_strFile;
	//解压。。。。解压。。。。。解压。。。。
	// Find root path for zip file
	size_t pos = zip.find_last_of("/\\");
	if (pos == std::string::npos)
	{
		CCLOG("CMyUnZip : no root path specified for zip file %s\n", zip.c_str());
		m_Mutex.lock();
		m_eStatus = eMyUnZipStatus::error;
		m_Mutex.unlock();
		return ;
	}
	const std::string rootPath = zip.substr(0, pos+1);
	cocos2d::log("CMyUnZip:rootPath:%s",rootPath.c_str());
	// Open the zip file
	unzFile zipfile = unzOpen(FileUtils::getInstance()->getSuitableFOpen(zip).c_str());
	if (! zipfile)
	{
		CCLOG("CMyUnZip : can not open downloaded zip file %s\n", zip.c_str());
		m_Mutex.lock();
		m_eStatus = eMyUnZipStatus::error;
		m_Mutex.unlock();
		return ;
	}
	
	// Get info about the zip file
	unz_global_info global_info;
	if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
	{
		CCLOG("CMyUnZip : can not read file global info of %s\n", zip.c_str());
		unzClose(zipfile);
		m_Mutex.lock();
		m_eStatus = eMyUnZipStatus::error;
		m_Mutex.unlock();
		return ;
	}
	
	
	// Buffer to hold data read from the zip file
	char readBuffer[BUFFER_SIZE];
	// Loop to extract all files.
	uLong i;
	for (i = 0; i < global_info.number_entry; ++i)
	{
		m_Mutex.lock();
		m_nTotalFile = global_info.number_entry;
		m_nCurFile = i;
		cocos2d::log("CMyUnZip:file:%ld,%ld",m_nCurFile,m_nTotalFile);
		m_Mutex.unlock();
		// Get info about current file.
		unz_file_info fileInfo;
		char fileName[MAX_FILENAME];
		if (unzGetCurrentFileInfo(zipfile,
								  &fileInfo,
								  fileName,
								  MAX_FILENAME,
								  NULL,
								  0,
								  NULL,
								  0) != UNZ_OK)
		{
			CCLOG("CMyUnZip : can not read compressed file info\n");
			unzClose(zipfile);
			m_Mutex.lock();
			m_eStatus = eMyUnZipStatus::error;
			m_Mutex.unlock();
			return ;
		}
		cocos2d::log("CMyUnZip:fileName:%s",fileName);
		const std::string fullPath = rootPath + fileName;
		
		// Check if this entry is a directory or a file.
		const size_t filenameLength = strlen(fileName);
		if (fileName[filenameLength-1] == '/')
		{
			//There are not directory entry in some case.
			//So we need to create directory when decompressing file entry
			if ( !_fileUtils->createDirectory(basename(fullPath)) )
			{
				// Failed to create directory
				CCLOG("CMyUnZip : can not create directory %s\n", fullPath.c_str());
				unzClose(zipfile);
				m_Mutex.lock();
				m_eStatus = eMyUnZipStatus::error;
				m_Mutex.unlock();
				return ;
			}
            if (i == 0 ) {//文件夹目录
                m_strOutDir = fileName;
                
                for (int i = 0; i <  filenameLength ; i++) {
                    if (fileName[i] == '/')
                    {
                        std::string strfile = fileName;
                        m_strOutDir = strfile.substr(0, i+1);
                        break;
                    }
                }
                
                
            }
		}
		else
		{
            //找到这个文件的文件夹然后创建他
            for (int i = (int)filenameLength-1; i>0; i--) {
                if (fileName[i] == '/')
                {
                    std::string strfile = fileName;
                    auto subpath = strfile.substr(0, i+1);
                    if ( !_fileUtils->createDirectory(basename(rootPath + subpath)) )
                    {
                        // Failed to create directory
                        CCLOG("CMyUnZip :2 can not create directory %s\n", fullPath.c_str());
                        unzClose(zipfile);
                        m_Mutex.lock();
                        m_eStatus = eMyUnZipStatus::error;
                        m_Mutex.unlock();
                        
                    }
                    break;
                }
            }
            if (m_strOutDir == "null") {
                for (int i = 0; i <  filenameLength ; i++) {
                    if (fileName[i] == '/')
                    {
                        std::string strfile = fileName;
                        m_strOutDir = strfile.substr(0, i+1);
                        break;
                    }
                }
            }
			// Entry is a file, so extract it.
			// Open current file.
			if (unzOpenCurrentFile(zipfile) != UNZ_OK)
			{
				CCLOG("CMyUnZip : can not extract file %s\n", fileName);
				unzClose(zipfile);
				m_Mutex.lock();
				m_eStatus = eMyUnZipStatus::error;
				m_Mutex.unlock();
				return ;
			}
			
			// Create a file to store current file.
			FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), "wb");
			if (!out)
			{
				CCLOG("CMyUnZip : can not create decompress destination file %s\n", fullPath.c_str());
				unzCloseCurrentFile(zipfile);
				unzClose(zipfile);
				m_Mutex.lock();
				m_eStatus = eMyUnZipStatus::error;
				m_Mutex.unlock();
				return ;
			}
			
			// Write current file content to destinate file.
			int error = UNZ_OK;
			do
			{
				error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
				if (error < 0)
				{
					CCLOG("CMyUnZip : can not read zip file %s, error code is %d\n", fileName, error);
					fclose(out);
					unzCloseCurrentFile(zipfile);
					unzClose(zipfile);
					m_Mutex.lock();
					m_eStatus = eMyUnZipStatus::error;
					m_Mutex.unlock();
					return ;
				}
				
				if (error > 0)
				{
					fwrite(readBuffer, error, 1, out);
				}
			} while(error > 0);
			
			fclose(out);
		}
		
		unzCloseCurrentFile(zipfile);
		
		// Goto next entry listed in the zip file.
		if ((i+1) < global_info.number_entry)
		{
			if (unzGoToNextFile(zipfile) != UNZ_OK)
			{
				CCLOG("CMyUnZip : can not read next file for decompressing\n");
				unzClose(zipfile);
				m_Mutex.lock();
				m_eStatus = eMyUnZipStatus::error;
				m_Mutex.unlock();
				return ;
			}
		}
	}
	unzClose(zipfile);

	
	m_Mutex.lock();
	m_eStatus = eMyUnZipStatus::ok;
	m_Mutex.unlock();
}
