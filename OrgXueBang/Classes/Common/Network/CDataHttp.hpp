//
//  CDataHttp.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#ifndef CDataHttp_hpp
#define CDataHttp_hpp

#include "HttpTag.h"
#include "BaseLoad.h"
#include "HttpManager.h"
#include "UploadFile.h"
/*
 用户数据模块
 by：高超
 */

class CDataHttpDelegate
{
public:
    CDataHttpDelegate();
    virtual ~CDataHttpDelegate();
    virtual void endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json) {};
};

////////////////////////////////
//HTTP的状态
enum class eHttpStateBase
{
    eHttpStateBase_none     = 0,
    eHttpStateBase_send        ,   //发送
    eHttpStateBase_err        ,   //错误
    eHttpStateBase_Rok        ,   //返回成功
    eHttpStateBase_Rerr        ,   //返回错误
};

//每次操作的状态，每个操作只能存在一次
typedef struct stuMyHttpOpState
{
    eHttpStateBase m_eHttpStateBase;
    std::string m_strNetMsg;
} stuMyHttpOpState;
////////////////////////////////

class CDataHttp:public Ref,public HttpDataDelegate
{
public:
    /*单例*/
    static CDataHttp* getInstance();
    /*销毁*/
    static void destroyInstance();
    
    bool init();
    void free();
public:
    /*命令回调*/
    virtual void doWithHttpData(eHttpType type, bool success, std::string& strData);
    /*联网状态相关*/
    std::map<eHttpType,stuMyHttpOpState> m_mapHttpState;
    void            TryAddHttpState(eHttpType op);
    eHttpStateBase    GetHttpOpState(eHttpType op);
    eHttpStateBase    SetHttpOpState(eHttpType op,eHttpStateBase state);
    const char*        GetHttpOpMsg(eHttpType op);
    const char*        SetHttpOpMsg(eHttpType op,const char* pMsg);
    
    bool            HttpPost(eHttpType op,const char* strUrl,const unordered_map<string, string>& values)
    {return HttpsendRequest(HttpRequest::Type::POST,strUrl,op,values);};
    bool            HttpPost(eHttpType op,const char* strUrl, std::string& str)
    {return HttpsendRequest(HttpRequest::Type::POST,strUrl,op,str);};
    bool            HttpPost(eHttpType op, std::string& str)
    {return HttpsendRequest(HttpRequest::Type::POST,op,str);};
    bool            HttpGet(eHttpType op,const char* strUrl,const unordered_map<string, string>& values)
    {return HttpsendRequest(HttpRequest::Type::GET,strUrl,op,values);};
    bool            HttpGet(eHttpType op,const char* strUrl, std::string& str)
    {return HttpsendRequest(HttpRequest::Type::GET,strUrl,op,str);};
    bool            HttpGet(eHttpType op, std::string& str)
    {return HttpsendRequest(HttpRequest::Type::GET,op,str);};
    bool            HttpPut(eHttpType op,const char* strUrl,const unordered_map<string, string>& values)
    {return HttpsendRequest(HttpRequest::Type::PUT,strUrl,op,values);};
    bool            HttpPut(eHttpType op,const char* strUrl, std::string& str)
    {return HttpsendRequest(HttpRequest::Type::PUT,strUrl,op,str);};
    
    bool            HttpGetNew(eHttpType op, const unordered_map<string, string>& heads, const unordered_map<string, string>& values)
    {return HttpGetRequest(op,heads, values);};
    bool            HttpPostNew(eHttpType op, const unordered_map<string, string>& heads, const std::string& str)
    {return HttpPostRequest(op,heads,str);};
    
    bool HttpsendRequest(HttpRequest::Type type,std::string url, eHttpType MyType, const std::string& data);
    bool HttpsendRequest(HttpRequest::Type type,eHttpType MyType, const std::string& data);
    bool HttpsendRequest(HttpRequest::Type type,std::string url, eHttpType MyType, const unordered_map<string, string>& values);
    
    bool HttpUploadRequest(eHttpType MyType, const stuUpload& upload);
    
    bool HttpPostRequest(eHttpType MyType, const unordered_map<string, string>& heads, const std::string& data);
    bool HttpGetRequest(eHttpType MyType, const unordered_map<string, string>& heads, const unordered_map<string, string> &values);
    
    void            checkErrorCode(eHttpType e,std::string nCode);
    void            regeditCallBack(CDataHttpDelegate *p);
    void            unregeditCallBack(CDataHttpDelegate *p);
    
    
    void            parseCookie(const std::vector<char>* headData);
    std::string     getCookie();
private:
    stuJson m_stujson;
    std::map<CDataHttpDelegate*,bool> m_mapRegedit;
};

#endif /* CDataHttp_hpp */
