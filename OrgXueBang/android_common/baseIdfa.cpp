//
//  Recorder.cpp
//  ReadingFriends
//
//  Created by Summer on 16/7/20.
//
//
#include "stdafx.h"
#include "baseIdfa.h"
//#include "CbaseIdfa.h"
#include "AndroidMethod.h"
#include "assist.h"

CBaseIdfa* CBaseIdfa::m_pBashIdfa = nullptr;

CBaseIdfa::CBaseIdfa()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_stuJson.init();
//CBaseLoad::LoadJsonString(stuJson& stuJson, std::string strData)
    string info = CppCallJava(MYJNICALL_TYPE_DEVICEINFO,"null");
    CCLOG("info = %s", info.c_str());
    CBaseLoad::LoadJsonString(m_stuJson, info);
#endif
}

CBaseIdfa::~CBaseIdfa()
{
}

//bool CBaseIdfa::setDeviceInfo(const char* info)
//{
//    cocos2d::log("%s",info);
//    return CBaseLoad::LoadJsonString(m_stuJson,info);
//}

CBaseIdfa* CBaseIdfa::getInstance()
{
    if (m_pBashIdfa == nullptr)
    {
        m_pBashIdfa = new CBaseIdfa;
    }
    return m_pBashIdfa;
}

const char* CBaseIdfa::getIdfaStr()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return m_stuJson.getValue("Idfa").c_str();
#endif
    return "0000";
}

const char* CBaseIdfa::getIdfvStr()
{
    return getIdfaStr();
}

const char* CBaseIdfa::getSystemVersion()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return m_stuJson.getValue("SystemVersion").c_str();
#endif
    return "123123";
}

const char* CBaseIdfa::getmodel()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return m_stuJson.getValue("model").c_str();
#endif
    return "123123";
}

const char* CBaseIdfa::getversion()//"]//|否|版本|
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return m_stuJson.getValue("version").c_str();
#endif
    return "123123";
}

const char* CBaseIdfa::getcarrier()//""]//|否|运营商|
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return m_stuJson.getValue("carrier").c_str();
#endif
    return "123123";
}

const char* CBaseIdfa::getplatform()//""]//|否|ios或者android|
{
    return "android";
}

const char* CBaseIdfa::getplatform_id()//""]//|否|分发平台id|
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    return "android_cn";
    return GetXmlString("appChannelID");
#endif
    return "0";
}

const char* CBaseIdfa::getbundle_id()//""]//|否|包id|
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return m_stuJson.getValue("bundleid").c_str();
#endif
    return "123123";
}

const char* CBaseIdfa::getDeviceToken()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return m_stuJson.getValue("DeviceToken").c_str();
#endif
    return "123123";
}

void CBaseIdfa::setDeviceToken(std::string token)
{

}

//const char* CBaseIdfa::getOthers(std::string key)
//{
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    return "null";
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    return m_stuJson.getValue(key).c_str();
//#endif
//    return "123123";
//}

//void CBaseIdfa::goToUpdateApp()
//{
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    stuJson stuJ;
//    CBaseLoad::LoadJson(stuJ,"config/ConfigData.json");
//    return CCBaseIdfa::getInstance()->goToUpdateApp(stuJ.getJson("versionUpdata").getValue("appstore"));
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    CLayerDownLoad::downLoadApk();
//#endif
//}

void CBaseIdfa::makeDeviceInfo()
{
    char buf[1024];
    sprintf(buf, "{\"device\" : \"%s\",\"system\" : \"%s\",\"model\" : \"%s\",\"version\" : \"%s\",\"carrier\" : \"%s\",\"platform\" : \"%s\",\"channel\" : \"%s\"}", getIdfaStr(), getSystemVersion(), getmodel(), getversion(), getcarrier(), getplatform(), getplatform_id());
    m_strDeviceInfo = buf;
}

const char* CBaseIdfa::getDeviceInfo()
{
    if (m_strDeviceInfo.length() == 0) {
        makeDeviceInfo();
    }
    return m_strDeviceInfo.c_str();
}

float CBaseIdfa::availableMemory()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    string info = CppCallJava(MYJNICALL_TYPE_AVAILABLE_MEM,"null");
    float am = atof(info.c_str());
    return am;
#endif
    return 0;
}

float CBaseIdfa::usedMemory()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    string info = CppCallJava(MYJNICALL_TYPE_USED_MEM,"null");
    float am = atof(info.c_str());
    return am;
#endif
    return 0;
}

void CBaseIdfa::setMultipleTouchEnabled(bool bEnabled)
{
    if (bEnabled)
    {
        CppCallJava(MYJNICALL_TYPE_MULTI_TOUCH_ENABLE,"true");
    } 
    else 
    {
        CppCallJava(MYJNICALL_TYPE_MULTI_TOUCH_ENABLE,"false");
    }
}

const char* CBaseIdfa::getImei()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return m_stuJson.getValue("imei").c_str();
#endif
    return "";
}

const char* CBaseIdfa::getwifiMac()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return m_stuJson.getValue("wifiMac").c_str();
#endif
    return "";
}

const char* CBaseIdfa::getbtMac()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return m_stuJson.getValue("btmac").c_str();
#endif
    return "";
}
