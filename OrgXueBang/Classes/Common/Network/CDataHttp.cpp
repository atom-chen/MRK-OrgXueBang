//
//  CDataHttp.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#include "CDataHttp.hpp"
#include "NetworkService.hpp"
#include "CCBuffer.h"
#include "FileSave.h"
#include "HttpManager.h"
#include "SceneManager.hpp"

#define MYDEF_SESSION_FILE  "session.save"

CDataHttpDelegate::~CDataHttpDelegate()
{
    CDataHttp::getInstance()->unregeditCallBack(this);
}

CDataHttpDelegate::CDataHttpDelegate()
{
    CDataHttp::getInstance()->regeditCallBack(this);
}

static CDataHttp* g_dataHttp = nullptr;
CDataHttp* CDataHttp::getInstance()
{
    if (g_dataHttp == nullptr) {
        g_dataHttp = new CDataHttp;
        g_dataHttp->init();
    }
    return g_dataHttp;
}

void CDataHttp::destroyInstance()
{
    if (g_dataHttp) {
        g_dataHttp->free();
        delete g_dataHttp;
        g_dataHttp = nullptr;
    }
}

/*初始化函数*/
bool CDataHttp::init()
{
    return true;
}

void CDataHttp::free()
{
}
bool CDataHttp::HttpsendRequest(HttpRequest::Type type,std::string url, eHttpType MyType, const std::string& data)
{
    if (GetHttpOpState(MyType) == eHttpStateBase::eHttpStateBase_send)
        return false;
    SetHttpOpState(MyType,eHttpStateBase::eHttpStateBase_send);
    HttpManager::getInstance()->HttpsendRequest(type,url,MyType, data);
    return true;
}
bool CDataHttp::HttpsendRequest(HttpRequest::Type type, eHttpType MyType, const std::string& data)
{
    if (GetHttpOpState(MyType) == eHttpStateBase::eHttpStateBase_send)
        return false;
    SetHttpOpState(MyType,eHttpStateBase::eHttpStateBase_send);
    HttpManager::getInstance()->HttpsendRequest(type,NetworkService::getInstance()->getServiceUrl(MyType),MyType, data);
    return true;
}
bool CDataHttp::HttpsendRequest(HttpRequest::Type type,std::string url, eHttpType MyType, const unordered_map<string, string>& values)
{
    if (GetHttpOpState(MyType) == eHttpStateBase::eHttpStateBase_send)
        return false;
    SetHttpOpState(MyType,eHttpStateBase::eHttpStateBase_send);
    HttpManager::getInstance()->HttpsendRequest(type,url,MyType, values);
    return true;
}


bool CDataHttp::HttpUploadRequest(eHttpType MyType, const stuUpload& upload)
{
    if (GetHttpOpState(MyType) == eHttpStateBase::eHttpStateBase_send)
        return false;
    SetHttpOpState(MyType,eHttpStateBase::eHttpStateBase_send);
    HttpManager::getInstance()->HttpUploadRequest(MyType, upload);
    return true;
}

void CDataHttp::TryAddHttpState(eHttpType op)
{
    if (m_mapHttpState.find(op) == m_mapHttpState.end())
    {
        stuMyHttpOpState s ;
        s.m_eHttpStateBase = eHttpStateBase::eHttpStateBase_none;
        s.m_strNetMsg = "NULL";
        m_mapHttpState[op] = s;
    }
}

eHttpStateBase CDataHttp::GetHttpOpState(eHttpType op)
{
    TryAddHttpState(op);
    return m_mapHttpState[op].m_eHttpStateBase;
}

eHttpStateBase CDataHttp::SetHttpOpState(eHttpType op,eHttpStateBase state)
{
    TryAddHttpState(op);
    m_mapHttpState[op].m_eHttpStateBase = state;
    return m_mapHttpState[op].m_eHttpStateBase;
}

const char* CDataHttp::GetHttpOpMsg(eHttpType op)
{
    TryAddHttpState(op);
    return m_mapHttpState[op].m_strNetMsg.c_str();
}

const char* CDataHttp::SetHttpOpMsg(eHttpType op,const char* pMsg)
{
    TryAddHttpState(op);
    m_mapHttpState[op].m_strNetMsg = pMsg;
    return m_mapHttpState[op].m_strNetMsg.c_str();
}

void  CDataHttp::regeditCallBack(CDataHttpDelegate *p)
{
    m_mapRegedit[p] = true;
}

void  CDataHttp::unregeditCallBack(CDataHttpDelegate *p)
{
    if (m_mapRegedit.find(p) != m_mapRegedit.end())
        m_mapRegedit[p] = false;
}

void CDataHttp::parseCookie(const std::vector<char>* headData)
{
    std::string strHead = "";
    strHead.assign(headData->begin(), headData->end());
    log("\n\nhead %s\n\n", strHead.c_str());

    std::string strFind = "Set-Cookie: ";
    auto pos1 = strHead.find(strFind);
    auto pos2 = strHead.find("; Path=/");
    if (pos1 == std::string::npos || pos2 == std::string::npos)
    {
        return;
    }

    std::string strCookie = strHead.substr(pos1+strFind.length(), pos2 - pos1 - strFind.length());
    auto pBuff =  CCBuffer::create();
    pBuff->writeLengthAndString(strCookie.c_str());
    CFileSave::EncryptSave(MYDEF_SESSION_FILE,(unsigned char *)pBuff->data(),pBuff->length(), MYDEF_SESSION_FILE);
}



std::string CDataHttp::getCookie()
{
    std::string strCookie = "";
    Data d;
    if (eFileSaveResult::ok == CFileSave::EncryptLoad(MYDEF_SESSION_FILE, d, MYDEF_SESSION_FILE))
    {
        auto pBuff =  CCBuffer::create((const char*)d.getBytes(),(unsigned int)d.getSize());
        strCookie = pBuff->readLengthAndString();
    }
    return strCookie;
}

bool CDataHttp::HttpPostRequest(eHttpType MyType, const unordered_map<string, string>& heads, const std::string& data)
{
    if (GetHttpOpState(MyType) == eHttpStateBase::eHttpStateBase_send)
        return false;
    SetHttpOpState(MyType,eHttpStateBase::eHttpStateBase_send);
    HttpManager::getInstance()->HttpPostRequest(NetworkService::getInstance()->getServiceUrl(MyType),MyType, heads, data);
    return true;
}

bool CDataHttp::HttpGetRequest(eHttpType MyType, const unordered_map<string, string>& heads, const unordered_map<string, string> &values)
{
    if (GetHttpOpState(MyType) == eHttpStateBase::eHttpStateBase_send)
        return false;
    SetHttpOpState(MyType,eHttpStateBase::eHttpStateBase_send);
    HttpManager::getInstance()->HttpGetRequest(NetworkService::getInstance()->getServiceUrl(MyType),MyType, heads, values);
    return true;
}

//联网操作回调
void CDataHttp::doWithHttpData(eHttpType type, bool success, std::string& strData)
{
    m_stujson.init();
    if (success)
    {
        CBaseLoad::LoadJsonString(m_stujson, strData);
        if(m_stujson.isHasJson("return_code"))
        {
            string return_code = m_stujson.getValue("return_code");
            if ( return_code == "6000")
            {
                SetHttpOpState(type,eHttpStateBase::eHttpStateBase_Rok);
                SetHttpOpMsg(type, "OK");
            }
            else
            {
                SetHttpOpState(type,eHttpStateBase::eHttpStateBase_Rerr);
                SetHttpOpMsg(type, m_stujson.getValue("return_msg").c_str());
                if (return_code == "6022")//token已失效
                {
                    if (type != eHttpType::postdevicetoken) {
//                        Reador::getInstance()->set2Guest();
//                        Reador::getInstance();
//                        Login::create(UiBase::getInstance()->getRunningScene());
//                        Login::setBack2Main();
                        SceneManager::getInstance()->showUILogin();
                    }
                    return;
                }
                else if (return_code == "6030")//版本需更新
                {
//                    auto dlg = ComDlg::create(UiBase::getInstance()->getRunningScene());
//                    dlg->setStringAndBtnType("      " + m_stujson.getValue("return_msg"));
//                    dlg->setCallback([](const string& s){
//                        CCLOG("return = %s", s.c_str());
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//                        Application::getInstance()->openURL("https://itunes.apple.com/cn/app/id1202078045");
//#endif
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//                        Application::getInstance()->openURL("http://dblinks.metricschina.com/");
//#endif
//
////                        Game::sharedGame()->exitGame();
//                    });
//                    return;
                }
            }
        }
        else
        {
            success = false;
            SetHttpOpState(type,eHttpStateBase::eHttpStateBase_err);
        }
    }
    else
    {
        SetHttpOpState(type,eHttpStateBase::eHttpStateBase_err);
    }
    
    //公共数据解析
//    Reador::getInstance()->endWithHttpData(type, success, m_stujson);
//    //add by hyp  主页数据
//    MachineManager::getInstance()->endWithHttpData(type, success, m_stujson);
//    MainPageData::getInstance()->endWithHttpData(type, success, m_stujson);
//    MainBookData::getInstance()->endWithHttpData(type, success, m_stujson);
//    StoreManager::getInstance()->endWithHttpData(type, success, m_stujson);
        
    for (auto it = m_mapRegedit.begin(); it != m_mapRegedit.end(); it++) {
        if (it->second) {
            it->first->endWithHttpData(type,success,m_stujson);
        }
    }
}
