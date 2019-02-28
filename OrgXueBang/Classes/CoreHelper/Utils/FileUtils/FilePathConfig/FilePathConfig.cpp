//#include "stdafx.h"

#include "FilePathConfig.h"
#include "platform/CCFileUtils.h"
USING_NS_CC;


static CFilePathConfig* g_CFilePathConfig = NULL;

/*单例*/
CFilePathConfig* CFilePathConfig::getInstance()
{
    if (g_CFilePathConfig == NULL) {
        g_CFilePathConfig = new CFilePathConfig();
        g_CFilePathConfig->init();
    }
   // NotificationCenter::getInstance()
    return g_CFilePathConfig;
}

/*初始化函数*/
bool CFilePathConfig::init()
{
    m_basePath = FileUtils::getInstance()->getSearchPaths();
    
    //添加其他的资源目录
    //m_AddPath
    m_AddPath.clear();
    m_AddPath.push_back("config");
    m_AddPath.push_back("Default");
    m_AddPath.push_back("fonts");
    m_AddPath.push_back("img");
    m_AddPath.push_back("music");
    m_AddPath.push_back("particle");
    m_AddPath.push_back("res");
    m_AddPath.push_back("shaders");
    m_AddPath.push_back("spine");
    m_AddPath.push_back("ui");
    m_AddPath.push_back("aaimage");
    m_AddPath.push_back("aaimage/mainpage");
    m_AddPath.push_back("aaimage/mainbook");
    m_AddPath.push_back("aaimage/machine");
    ReFresh();
    return true;
}

void CFilePathConfig::destroyInstance()
{
    //释放
    if (g_CFilePathConfig) {
        g_CFilePathConfig->free();
        delete  g_CFilePathConfig;
    }
    g_CFilePathConfig = NULL;
}

void CFilePathConfig::free()
{
    //
}

/*添加查找路径*/
void CFilePathConfig::addSearchPath(const std::string & path, const bool front)
{
    if (m_AddPathCnt.find(path) != m_AddPathCnt.end()) {
        m_AddPathCnt[path] ++;
    }
    else
    {
        m_AddPathCnt[path] = 1;
    }

    removeSearchPathFromVector(path);
    if (front) {
        m_AddPath.insert(m_AddPath.begin(),path);
    } else {
        m_AddPath.push_back(path);
    }
    ReFresh();

}

/*删除查找路径*/
void CFilePathConfig::removeSearchPath(const std::string & path)
{
    if (m_AddPathCnt.find(path) != m_AddPathCnt.end()) {
        m_AddPathCnt[path] --;
        if (m_AddPathCnt[path] <= 0) {
            m_AddPathCnt[path] = 0;
            removeSearchPathFromVector(path);
            ReFresh();
        }
    }
}

void CFilePathConfig::removeSearchPathFromVector(const std::string & path)
{
    for (auto i = m_AddPath.begin() ; i != m_AddPath.end(); i++) {
        if (*i == path) {
            m_AddPath.erase(i);
            //ReFresh();
            return;
        }
    }
}

void CFilePathConfig::ReFresh()
{
    std::vector<std::string> path;
    path.clear();
    std::string writePath = FileUtils::getInstance()->getWritablePath();
    path.push_back(writePath);
    
    for (int i = 0 ; i < m_AddPath.size(); i++) {
        path.push_back(writePath + m_AddPath[i]);
        path.push_back(m_AddPath[i]);
    }
    path.insert(path.end(), m_basePath.begin(), m_basePath.end());
    FileUtils::getInstance()->setSearchPaths(path);
}

//得到文件绝对路径
string CFilePathConfig::getFullPathForFilename(string file)
{
    if (file.length() < 1) {
        return "-1";
    }
    //判断音频文件是否存在
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(file);
    if (!FileUtils::getInstance()->isFileExist(filePath)) {
        std::string writePath = FileUtils::getInstance()->getWritablePath();
        filePath = writePath+file;
        if (!FileUtils::getInstance()->isFileExist(filePath)) {
            return "-1";//不存在文件
        }
    }
    return filePath;
}
