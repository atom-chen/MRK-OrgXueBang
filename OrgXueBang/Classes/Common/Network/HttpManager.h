//
//  HttpManager.h
//  ReadingFriends
//
//  Created by liuwei on 16/7/6.
//
//

#ifndef HttpManager_h
#define HttpManager_h

#include "PreIncludes.hpp"
#include "UploadFile.h"
#include "HttpTag.h"

class HttpDataToString
{
    std::string m_str;
    std::string m_valueStr;
    std::string m_md5str;
public:
    HttpDataToString();
    void addData(const std::string& key, const std::string& value, bool bUrlEncode = false);
    std::string& getString();
};


class HttpDataDelegate
{
public:
    HttpDataDelegate();
    virtual ~HttpDataDelegate();
    virtual void doWithHttpData(eHttpType type, bool success, std::string& strData) = 0;
};


class HttpManager : public Ref
{
protected:
    HttpManager();
    ~HttpManager();
public:
    static HttpManager* getInstance();
    static void getMd5str(std::string& md5str);
private:
    static HttpManager* m_pHttpManager;
    
public:
    void HttpsendRequest(HttpRequest::Type type,std::string url, eHttpType MyType, const std::string& data);
    void HttpsendRequest(HttpRequest::Type type,std::string url, eHttpType MyType, const unordered_map<string, string>& values);
    void HttpUploadRequest(eHttpType MyType, const stuUpload& upload);
    
    void HttpsendRequest(HttpRequest* request);
    
    void HttpPostRequest(std::string url, eHttpType MyType, const unordered_map<string, string>& heads, const std::string& data);
    void HttpGetRequest(std::string url, eHttpType MyType, const unordered_map<string, string>& heads, const unordered_map<string, string>& values);
public:
    /* 反注请求返回信息 */
    void unregisterDataDelegate(HttpDataDelegate* pDelegate);
    /* 注请求返回信息 */
    void registerDataDelegate(HttpDataDelegate* pDelegate);
    int getCurHttpNum(){return m_curHttpNum;};
private:
    /* 请求返回函数 */
    void _onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
    /* 获取公共headers */
    std::map<std::string, std::string> _getCommonHeaders();
private:
    std::map<HttpDataDelegate*,bool> m_mapDataDelegate;
    UploadFile* m_pUploadFile;
    int m_curHttpNum;
    
private:
    /* 转化header数据 */
    std::vector<std::string> convertHeaderDataToVector(const std::map<std::string, std::string>& value);
};

#endif /* HttpManager_h */
