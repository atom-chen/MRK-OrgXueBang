//
//  UploadFile.h
//  ReadingFriends
//
//  Created by liuwei on 16/7/6.
//
//

#ifndef UploadFile_h
#define UploadFile_h

#include <vector>
#include <string>
#include <map>
#include <functional>
using namespace std;

#include "cocos2d.h"

struct KVPair
{
    std::string m_strKey;
    std::string m_strValue;
    
    KVPair(std::string key, std::string value)
    {
        m_strKey = key;
        m_strValue = value;
    }
};

typedef vector<KVPair*> KVPairVec;

struct stuFileInfo
{
    std::string m_strFileName;
    std::string m_strFileCopyName;
    std::string m_strFileContentType;
};

struct stuUpload
{
    //data
    std::string m_strUrl;
    vector<stuFileInfo> m_vecFiles;
    std::map<std::string,std::string> m_mapHeadValue;
    KVPairVec* m_mapPostValue;
    KVPairVec* m_mapGetValue;
    
    
    
    stuUpload(std::string url,
              vector<stuFileInfo> vecFiles,
              std::map<std::string,std::string> head = {},
              KVPairVec* post = new KVPairVec(),
              KVPairVec* get = new KVPairVec()
              )
    :m_strUrl(url),
    m_vecFiles(vecFiles)
    {
        m_mapHeadValue = head;
        m_mapPostValue = post;
        m_mapGetValue  = get;
    }
    
    
    //callback
    std::function<void(double)>  m_callBackProgress;
    std::function<void()>        m_callBackFinsih;
    std::vector<char>            m_vecResponseData;
    std::vector<char>            m_vecHeadData;
};

typedef std::function<void(std::string)> uploadFinishCallback;
typedef std::function<void(std::string, std::string)> uploadResponse;
class UploadFile : public cocos2d::Ref
{
public:
    UploadFile();
    ~UploadFile();
public:
    /* 上传文件 */
    void uploadFile(uploadResponse callBack, const stuUpload& upload);
    
    
    /* 设置上传进度 */
    void    setUploadProgress(double dProgress);
    
    /* 获取上传进度 */
    double  getUploadProgress();
    
    
private:
    /* 线程上传函数 */
    void _threadUpload(stuUpload* pUploadData);
    
private:
    std::mutex m_mutexProgress;
    double m_dProgress;
};

#endif /* UploadFile_h */
