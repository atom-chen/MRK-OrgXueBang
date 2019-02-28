

#ifndef BookReadUserData_hpp
#define BookReadUserData_hpp

#include "stdafx.h"
#include "CurlDownload.h"
#include "BaseDownload.h"
#include "OneDownload.h"
#include "CDataHttp.hpp"
#include "MyUnZip.h"

class CBookDownData
{
public:
    int m_errorNum;
    std::string m_savePath;
    std::string m_downPath;
    std::string m_downFile;
    std::string m_bookID;
    std::string m_md5;
    std::string m_url;
    COneDownload*  m_oneDownload;
    CMyUnZip* m_myUnZip;
};

class CBookDownManager : public Ref
{
public:
    static CBookDownManager* getInstance();
    static void destroyInstance();
    // static const stuJson& getStuJson();
public:
    CBookDownManager();
    ~CBookDownManager();
    bool init();
public:
    
    void checkDownUnZipList(float dt);
    
    bool checkBookRes(std::string url,std::string bookID,std::string md5);
    void addDownList(std::string url,std::string bookID,std::string md5);
    string removeBookRes(std::string bookID);
    CBookDownData* getBookDownData(std::string bookID);
private:
    std::map<std::string,CBookDownData> m_mapBookData;
    
};

#endif /* BookReadUserData_hpp */

