//
//  HttpManager.cpp
//  ReadingFriends
//
//  Created by liuwei on 16/7/6.
//
//

#include "HttpManager.h"
#include "StringUtil.hpp"
#include "FileLog.h"
#include "md5code.h"
#include "systemInfo.hpp"
#include "GlobalManager.hpp"
#include "SceneManager.hpp"
#include "UserManager.hpp"


//#include "NetRetryManager.hpp"
//#include "TipLoader.hpp"
//#include "TipManager.hpp"

HttpDataDelegate::HttpDataDelegate()
{
    HttpManager::getInstance()->registerDataDelegate(this);
}

HttpDataDelegate::~HttpDataDelegate()
{
    HttpManager::getInstance()->unregisterDataDelegate(this);
}

HttpManager::HttpManager()
{
    m_pUploadFile = new UploadFile;
    m_curHttpNum = 0;
}

HttpManager::~HttpManager()
{
    delete m_pUploadFile;
}

HttpManager* HttpManager::m_pHttpManager = NULL;
HttpManager* HttpManager::getInstance()
{
    if(!m_pHttpManager)
    {
        m_pHttpManager = new HttpManager;
        HttpClient::getInstance()->setTimeoutForConnect(10);
    }
    return m_pHttpManager;
}

void HttpManager::HttpsendRequest(HttpRequest::Type type,std::string url, eHttpType MyType, const unordered_map<string, string>& values)
{
    std::string strDat = "";
    if (!values.empty())
    {
        for (auto it : values)
        {
            strDat += it.first;
            strDat += '=';
            strDat += it.second;
            strDat += '&';
        }
        strDat = strDat.substr(0, strDat.length()-1);
    }

    HttpsendRequest(type,url,MyType,strDat);
}

void HttpManager::HttpsendRequest(HttpRequest::Type type,std::string url, eHttpType MyType, const std::string& data)
{
    HttpRequest* request = new HttpRequest();
    request->setRequestType(type);
//    CCLOG("\nhttp url = %s", url.c_str());
//    CCLOG("http data = %s\n", data.c_str());
    if (HttpRequest::Type::GET == type)
    {
        if (data.length())
        {
            url += '?';
            url += data;
        }
        request->setUrl(url.c_str());
    }
    else
    {
        request->setUrl(url.c_str());
        if (data.length())
            request->setRequestData(data.c_str(), data.length());
    }
    
    //CFileLog::LogSaveFormat("http req type=%d,%s,%s",(int)MyType,url.c_str(),data.c_str());
    
    //日志相关，不展示青蛙Loader
    if (MyType < eHttpType::mallluckdraw || MyType > eHttpType::postevent_end) {
//        //TipLoader::showLoader();
        m_curHttpNum ++;
//        CCLOG("-------show----------type = %d, num = %d", MyType, m_curHttpNum);
        if (m_curHttpNum < 1) {
            m_curHttpNum = 0;
        }
    }
    //add by hyp
    std::vector<std::string> headers = request->getHeaders();
    std::vector<std::string> commonHeaders = convertHeaderDataToVector(_getCommonHeaders());
    headers.insert(headers.end(), commonHeaders.begin(), commonHeaders.end());
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(HttpManager::_onHttpRequestCompleted, this));
    request->setTag(StringUtils::format("%d", MyType).c_str());//
    HttpClient::getInstance()->send(request);
    request->release();
}

void HttpManager::HttpUploadRequest(eHttpType MyType, const stuUpload& upload)
{
    stuUpload myUpload = upload;
    
    //callback
    auto func = [&, MyType](std::string strHeader, std::string strResult){
        
        log("head = %s", strHeader.c_str());
        log("type = %d\n %s\n\n\n", MyType, strResult.c_str());
        
        for (auto it = m_mapDataDelegate.begin(); it != m_mapDataDelegate.end(); it++)
        {
            if (it->second)
            {
                it->first->doWithHttpData(MyType, true, strResult);
            }
        }
    };
    m_pUploadFile->uploadFile(func, myUpload);
}

void HttpManager::_onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
    if (!response)
    {
        return;
    }
    CCLOG("url = %s", response->getHttpRequest()->getUrl());
    if (response->getHttpRequest()->getRequestType() == HttpRequest::Type::POST) {
        string postString = "";
        postString.insert(0, response->getHttpRequest()->getRequestData(), response->getHttpRequest()->getRequestDataSize());
        CCLOG("post data = %s", postString.c_str());
    }
    
    long statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code:%ld , tag = %s", statusCode, response->getHttpRequest()->getTag());
    log("response: %s", statusString);
    int iCommand = atoi(response->getHttpRequest()->getTag());
    std::string strContent = "";
    
    //日志相关，不展示青蛙Loader
    eHttpType MyType = (eHttpType)iCommand;
    if (MyType < eHttpType::mallluckdraw || MyType > eHttpType::postevent_end) {
        m_curHttpNum --;
//        CCLOG("-------hide----------type = %d, num = %d", MyType, m_curHttpNum);
        if (m_curHttpNum <= 0) {
            m_curHttpNum = 0;
            ////TipLoader::HideLoader();
        }
    }
    
    if (!response->isSucceed())
    {
        CFileLog::LogFormat("http res %s",response->getHttpRequest()->getUrl());
        CFileLog::LogFormat("http response failed error buffer:%s", response->getErrorBuffer());
        
//        auto dlg = NetRetryDlg::create(UiBase::getInstance()->getRunningScene());
//        dlg->addRequest(response->getHttpRequest());
        
        //自动翻页读书，被网络异常中断
        if (SceneManager::getInstance()->getGameState() == eStateBook) {
//            if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
//            {
//                BookDataManager::getInstance()->setReadMode(eReadNone);
//                __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
//            }
        }
        else
        {
//            NetRetryManager::addRequest(response->getHttpRequest());
            GlobalManager::addRequest(response->getHttpRequest());
        }
    }
    else{
        std::vector<char> *buffer = response->getResponseData();
        strContent.assign(buffer->begin(), buffer->end());
        CFileLog::LogFormat("http res type = %d len:%d \n%s\n", iCommand,(int)(strContent.length()), strContent.c_str());
    }
    for (auto it = m_mapDataDelegate.begin(); it != m_mapDataDelegate.end(); it++) {
        if (it->second) {
            it->first->doWithHttpData((eHttpType)iCommand, response->isSucceed(),strContent);
        }
    }
}


void HttpManager::unregisterDataDelegate(HttpDataDelegate* pDelegate)
{
    if (m_mapDataDelegate.find(pDelegate) != m_mapDataDelegate.end()) {
        m_mapDataDelegate[pDelegate] = false;
    }
}

void HttpManager::registerDataDelegate(HttpDataDelegate* pDelegate)
{
    m_mapDataDelegate[pDelegate] = true;
}

void HttpManager::getMd5str(std::string& md5str)
{
    md5str += "9BA963B040B8006617D4B91C74274093";
    CMd5Code md5;
    md5.GenerateMD5((unsigned char*)md5str.c_str(), (int)md5str.length());
    md5str = md5.ToString();
    
    std::transform(md5str.begin(), md5str.end(), md5str.begin(), ::toupper);
}

HttpDataToString::HttpDataToString():m_str(""),m_md5str(""),m_valueStr("")
{
    
}

void HttpDataToString::addData(const std::string &key, const std::string &value, bool bUrlEncode)
{
    if (bUrlEncode) {
        m_str += StringUtils::format("%s=%s&", key.c_str(), StringUtil::UrlEncode(value).c_str());
    } else {
        m_str += StringUtils::format("%s=%s&", key.c_str(), value.c_str());
    }
    m_valueStr += value;
}

std::string& HttpDataToString::getString()
{
    m_md5str = m_valueStr;
    HttpManager::getMd5str(m_md5str);
    m_md5str = m_str + "signcode=" + m_md5str;
    return m_md5str;
}

void HttpManager::HttpsendRequest(cocos2d::network::HttpRequest *request)
{
    eHttpType MyType = (eHttpType)atoi(request->getTag());
    //日志相关，不展示青蛙Loader
    if (MyType < eHttpType::mallluckdraw || MyType > eHttpType::postevent_end) {
        //TipLoader::showLoader();
        m_curHttpNum ++;
//        CCLOG("-------show----------type = %d, num = %d", MyType, m_curHttpNum);
        if (m_curHttpNum < 1) {
            m_curHttpNum = 0;
        }
    }
    HttpClient::getInstance()->send(request);
    request->release();
}

std::map<std::string, std::string> HttpManager::_getCommonHeaders()
{
    std::map<std::string, std::string> mapHeades;
    mapHeades["platformId"] = systemInfo::getInstance()->getPlatformId();
    mapHeades["idfa"] = systemInfo::getInstance()->getIdfa();
    mapHeades["idfv"] = systemInfo::getInstance()->getIdfv();
    mapHeades["deviceToken"] = systemInfo::getInstance()->getDeviceToken();
    mapHeades["imei"] = systemInfo::getInstance()->getImei();
    mapHeades["model"] = systemInfo::getInstance()->getModel();
    mapHeades["system"] = systemInfo::getInstance()->getSystem();
    mapHeades["version"] = systemInfo::getInstance()->getVersion();
    mapHeades["netType"] = systemInfo::getInstance()->getnetType();
    mapHeades["wifiMac"] = systemInfo::getInstance()->getwifiMac();
    mapHeades["btMac"] = systemInfo::getInstance()->getbtMac();
    mapHeades["carrier"] = systemInfo::getInstance()->getcarrier();
    mapHeades["appVersion"] = systemInfo::getInstance()->getappVersion();
    mapHeades["channelId"] = systemInfo::getInstance()->getchannelId();

    
    mapHeades["token"] = UserManager::getInstance()->getUserModel()->getUserToken();
//    mapHeades["userid"] = UserManager::getInstance()->getUserEnt()->_UserID;
    return mapHeades;
}

void HttpManager::HttpPostRequest(std::string url, eHttpType MyType, const unordered_map<string, string>& heads, const std::string& data)
{
    HttpRequest* request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    
    request->setUrl(url.c_str());
    if (data.length())
        request->setRequestData(data.c_str(), data.length());
    
    //CFileLog::LogSaveFormat("http req type=%d,%s,%s",(int)MyType,url.c_str(),data.c_str());
    
    //日志相关，不展示青蛙Loader
    if (MyType < eHttpType::mallluckdraw || MyType > eHttpType::postevent_end) {
        //TipLoader::showLoader();
        m_curHttpNum ++;
        //        CCLOG("-------show----------type = %d, num = %d", MyType, m_curHttpNum);
        if (m_curHttpNum < 1) {
            m_curHttpNum = 0;
        }
    }
    
    std::vector<std::string> headers =request->getHeaders();
    if (!heads.empty())
    {
        for (auto it : heads)
        {
            std::string strT = it.first + ":" + it.second;
            headers.push_back(strT);
        }
    }
    headers.push_back("Content-Type:application/json");
    std::vector<std::string> commonHeaders = convertHeaderDataToVector(_getCommonHeaders());
    headers.insert(headers.end(), commonHeaders.begin(), commonHeaders.end());
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(HttpManager::_onHttpRequestCompleted, this));
    request->setTag(StringUtils::format("%d", MyType).c_str());//
    HttpClient::getInstance()->send(request);
    request->release();
}

void HttpManager::HttpGetRequest(std::string url, eHttpType MyType, const unordered_map<string, string> &heads, const unordered_map<string, string> &values)
{
    HttpRequest* request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    
    std::string strDat = "";
    if (!values.empty())
    {
        for (auto it : values)
        {
            strDat += it.first;
            strDat += '=';
            strDat += it.second;
            strDat += '&';
        }
        strDat = strDat.substr(0, strDat.length()-1);
    }
    if (strDat.length())
    {
        url += '?';
        url += strDat;
    }
    request->setUrl(url.c_str());
    
    //CFileLog::LogSaveFormat("http req type=%d,%s,%s",(int)MyType,url.c_str(),strDat.c_str());
    
    //日志相关，不展示青蛙Loader
    if (MyType < eHttpType::mallluckdraw || MyType > eHttpType::postevent_end) {
        //TipLoader::showLoader();
        m_curHttpNum ++;
        //        CCLOG("-------show----------type = %d, num = %d", MyType, m_curHttpNum);
        if (m_curHttpNum < 1) {
            m_curHttpNum = 0;
        }
    }
    
    std::vector<std::string> headers =request->getHeaders();
    if (!heads.empty())
    {
        for (auto it : heads)
        {
            std::string strT = it.first + ":" + it.second;
            headers.push_back(strT);
        }
    }
    std::vector<std::string> commonHeaders = convertHeaderDataToVector(_getCommonHeaders());
    headers.insert(headers.end(), commonHeaders.begin(), commonHeaders.end());
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(HttpManager::_onHttpRequestCompleted, this));
    request->setTag(StringUtils::format("%d", MyType).c_str());//
    HttpClient::getInstance()->send(request);
    request->release();
}
/* 转化header数据 */
std::vector<std::string> HttpManager::convertHeaderDataToVector(const std::map<std::string, std::string>& value)
{
    std::vector<std::string> vecResult;
    
    for (auto& it : value)
    {
        std::string strT = it.first + ":" + it.second;
        vecResult.push_back(strT);
    }
    
    return vecResult;
}
