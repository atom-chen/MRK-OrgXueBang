



#include "BookDownManager.h"
#include "BookLoading.hpp"
#include "FileSaveMap.h"
#include "MessageManager.hpp"
#include "FilePathConfig.h"


#define MYDEF_BOOK_DOWN_DIR "ReadBookDownload/"

CBookDownManager* g_CBookDownManager = nullptr;

CBookDownManager* CBookDownManager::getInstance()
{
    if(g_CBookDownManager == nullptr)
    {
        g_CBookDownManager  = new CBookDownManager();
        g_CBookDownManager->init();
    }
    return g_CBookDownManager;
}

void CBookDownManager::destroyInstance()
{
    if(g_CBookDownManager != nullptr)
        delete g_CBookDownManager;
    g_CBookDownManager = nullptr;
}

CBookDownManager::~CBookDownManager()
{
     m_mapBookData.clear();
}


CBookDownManager::CBookDownManager()
{
    //创建个文件夹
    std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_BOOK_DOWN_DIR;
    auto pFileUtils = FileUtils::getInstance();
    if(pFileUtils->isDirectoryExist(path) == false)
    {
        pFileUtils->createDirectory(path);
        while(!pFileUtils->isDirectoryExist(path))
            ;
    }
    CFilePathConfig::getInstance()->addSearchPath(MYDEF_BOOK_DOWN_DIR);
    m_mapBookData.clear();
}

bool CBookDownManager::init()
{
    Director::getInstance()->getScheduler()->schedule(schedule_selector(CBookDownManager::checkDownUnZipList), this, 0.1f, false);
    return true;
}

void CBookDownManager::checkDownUnZipList(float dt)
{
    std::map<std::string,bool> bookList; //标记结果
    for (auto iter = m_mapBookData.begin();iter != m_mapBookData.end();iter++) {
        CBookDownData * bookDownData = &iter->second;
        if (bookDownData->m_oneDownload!=nullptr) {
            switch (bookDownData->m_oneDownload->getStatus()) {
                case eBaseDownloadStatus::finish:
                    if (bookDownData->m_myUnZip==nullptr)
                    {
                        bookDownData->m_myUnZip = new CMyUnZip(bookDownData->m_downFile);
                    }
                    break;
                case eBaseDownloadStatus::error:
                    bookDownData->m_errorNum++;
                    if (bookDownData->m_errorNum<5) {
                        bookDownData->m_oneDownload = new COneDownload(bookDownData->m_url,bookDownData->m_downFile,bookDownData->m_md5);
                    }else
                    {
                        bookList[bookDownData->m_bookID] = false;
                    }
                    break;
                case eBaseDownloadStatus::run:
                {
                    unsigned long nTotalFileSize; unsigned long nDownSize;
                    bookDownData->m_oneDownload->getProgress(nTotalFileSize,nDownSize);
                    float progress =  nDownSize/nTotalFileSize;
                    MessageParam param;
                    param.paramStr.push_back(iter->first);
                    param.paramFloat.push_back(progress);
                    MessageManager::postNotification(MSG_BOOK_DOWN_PROGRESS,&param);
                }
                    break;
                default:
                    break;
            }
        }
        if (bookDownData->m_myUnZip!=nullptr) {
            switch (bookDownData->m_myUnZip->getStatus()) {
                case eMyUnZipStatus::ok:
                {
                    FileUtils::getInstance()->removeFile(bookDownData->m_downFile);
                    string outPath = bookDownData->m_myUnZip->getOutDir();
                    //标记已下载
                    bookList[bookDownData->m_bookID] = true;
                    CFileSaveMap::getInstance()->writeValueFile(bookDownData->m_bookID,bookDownData->m_md5);
                }
                    break;
                case eMyUnZipStatus::error:
                    FileUtils::getInstance()->removeFile(bookDownData->m_downFile);
                    bookList[bookDownData->m_bookID] = false;
                    break;
                default:
                    break;
            }
        }
    }
    //通知UI  从列表中删除
    for (auto iter = bookList.begin();iter != bookList.end();iter++) {
        MessageParam param;
        param.paramStr.push_back(iter->first);
        param.paramInt.push_back(iter->second);
        MessageManager::postNotification(MSG_BOOK_DOWN_RESULT,&param);
        delete(m_mapBookData[iter->first].m_oneDownload);
        delete(m_mapBookData[iter->first].m_myUnZip);
        m_mapBookData.erase(iter->first);
    }
    
}

bool CBookDownManager::checkBookRes(std::string url,std::string bookID,std::string md5)
{
    if (CFileSaveMap::getInstance()->readFromFile(bookID)==md5) {
        return true;//md5存在表示已下载 直接去读书
    }
    //加入下载列表
    addDownList(url,bookID,md5);
    //loading UI
    auto scene =  Director::getInstance()->getRunningScene();
    auto loading = BookLoading::create(bookID);
    scene->addChild(loading);
    return false;
}

void CBookDownManager::addDownList(std::string url,std::string bookID,std::string md5)
{
    if (m_mapBookData.find(bookID) != m_mapBookData.end()) {
        return;
    }
    //清空目录
    string filePath = removeBookRes(bookID);
    FileUtils::getInstance()->createDirectory(filePath);
    
    vector<string> strVec;
    StringUtil::splitString(url.c_str(),'/',strVec);
    string fileName = filePath + strVec.at(strVec.size()-1);
    //加入下载列表 开始下载
    CBookDownData bookDownData;
    bookDownData.m_md5 = md5;
    bookDownData.m_url = url;
    bookDownData.m_downPath = filePath;
    bookDownData.m_downFile = fileName;
    bookDownData.m_bookID = bookID;
    bookDownData.m_myUnZip = nullptr;
    bookDownData.m_oneDownload = new COneDownload(url,fileName,md5);
    m_mapBookData[bookID] = bookDownData;
}

CBookDownData* CBookDownManager::getBookDownData(std::string bookID)
{
    if (m_mapBookData.find(bookID) == m_mapBookData.end()) {
        return nullptr;
    }
    return &m_mapBookData[bookID];
}
string CBookDownManager::removeBookRes(std::string bookID)
{
    //清空目录
    string filePath = FileUtils::getInstance()->getWritablePath() + MYDEF_BOOK_DOWN_DIR + bookID + "/";
    FileUtils::getInstance()->removeDirectory(filePath);
    CFileSaveMap::getInstance()->eraseValueFile(bookID);
    return filePath;
}

