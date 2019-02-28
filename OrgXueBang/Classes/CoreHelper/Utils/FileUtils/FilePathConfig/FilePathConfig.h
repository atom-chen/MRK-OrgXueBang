#ifndef __FILE_PATH_CONFIG_H__
#define __FILE_PATH_CONFIG_H__

/*
 文件路径管理
 by：高超
 */

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
using namespace std;

class CFilePathConfig
{
public:
    /*单例*/
    static CFilePathConfig* getInstance();
    /*初始化函数*/
    virtual bool init();
    /*销毁*/
    static void destroyInstance();
    void free();
    
    /*添加查找路径*/
    void addSearchPath(const std::string & path, const bool front=false);
    /*删除查找路径*/
    void removeSearchPath(const std::string & path);
    //得到文件绝对路径
    string getFullPathForFilename(string file);
private:
    void removeSearchPathFromVector(const std::string & path);
    void ReFresh();
    std::vector<std::string> m_basePath;
    std::vector<std::string> m_AddPath;
    
    std::map<std::string,int> m_AddPathCnt;
};

#endif // __FILE_PATH_CONFIG_H__
