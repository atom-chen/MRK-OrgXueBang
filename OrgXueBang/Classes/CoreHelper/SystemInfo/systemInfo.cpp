//
//  systemInfo.cpp
//  ReadingMate-mobile
//
//  Created by 丁晶 on 2018/11/23.
//

#include "stdafx.h"
#include "systemInfo.hpp"
#include "baseIdfa.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "NetworkReachabilityManager.h"
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "AndroidMethod.h"
#endif

systemInfo* systemInfo::m_pSystemInfo = nullptr;
systemInfo::systemInfo()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NetworkRecahabilityManager::shareManager()->startMonitoring();
#endif
}

systemInfo::~systemInfo()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NetworkRecahabilityManager::shareManager()->stopMonitoring();
#endif
}

systemInfo* systemInfo::getInstance()
{
    if (m_pSystemInfo == nullptr)
    {
        m_pSystemInfo = new systemInfo();
    }
    return m_pSystemInfo;
}

const string& systemInfo::getPlatformId()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    m_platformId = "1";
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    m_platformId = "2";
#endif
    return m_platformId;
}


const string& systemInfo::getIdfa()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    if (m_idfa.length() < 1) {
        m_idfa = CBaseIdfa::getInstance()->getTrueIdfaStr();
    }
    return m_idfa;
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (m_idfv.length() < 1) {
        m_idfv = CBaseIdfa::getInstance()->getIdfaStr();
    }
#endif
    return m_idfv;
}


const string& systemInfo::getIdfv()
{
    if (m_idfv.length() < 1) {
        m_idfv = CBaseIdfa::getInstance()->getIdfaStr();
    }
    return m_idfv;
}


const string& systemInfo::getDeviceToken()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    if (m_deviceToken.length() < 1) {
        m_deviceToken = CBaseIdfa::getInstance()->getDeviceToken();
    }
    return m_deviceToken;
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (m_idfv.length() < 1) {
        m_idfv = CBaseIdfa::getInstance()->getIdfaStr();
    }
#endif
    return m_idfv;
}


const string& systemInfo::getImei()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (m_imei.length() < 1) {
        m_imei = CBaseIdfa::getInstance()->getImei();
    }
#endif
    return m_imei;
}


const string& systemInfo::getModel()
{
    if (m_model.length() < 1) {
        m_model = CBaseIdfa::getInstance()->getmodel();
    }
    return m_model;
}


const string& systemInfo::getSystem()
{
    if (m_system.length() < 1) {
        m_system = CBaseIdfa::getInstance()->getplatform();
    }
    return m_system;
}


const string& systemInfo::getVersion()
{
    if (m_version.length() < 1) {
        m_version = CBaseIdfa::getInstance()->getSystemVersion();
    }
    return m_version;
}


const string& systemInfo::getnetType()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    auto newStatus = NetworkRecahabilityManager::shareManager()->getCurrentStatus();
    switch (newStatus) {
        case NetworkReachabilityStatus::ReachableViaWiFi:
            m_netType = "wifi";
            break;
        case NetworkReachabilityStatus::ReachableViaWWAN:
            m_netType = "4g";
            break;
        default:
            m_netType = "";
            break;
    }
    
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
        string info = CppCallJava(MYJNICALL_TYPE_NETINFO, "");
        //        CCLOG("ret = %s", info.c_str());
        stuJson retJson;
        retJson.init();
        CBaseLoad::LoadJsonString(retJson, info);
        auto newStatus = retJson.getValue("net");
    
        if (newStatus == "2") {
            m_netType = "wifi";
        }
        else if (newStatus == "1") {
            m_netType = "4g";
        }
        else {
            m_netType = "";
        }
    
#endif
    return m_netType;
}


const string& systemInfo::getwifiMac()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (m_wifiMac.length() < 1) {
        m_wifiMac = CBaseIdfa::getInstance()->getwifiMac();
    }
#endif
    return m_wifiMac;
}


const string& systemInfo::getbtMac()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (m_btMac.length() < 1) {
        m_btMac = CBaseIdfa::getInstance()->getbtMac();
    }
#endif
    return m_btMac;
}


const string& systemInfo::getcarrier()
{
    if (m_carrier.length() < 1) {
        m_carrier = CBaseIdfa::getInstance()->getcarrier();
    }
    return m_carrier;
}


const string& systemInfo::getappVersion()
{
    if (m_appVersion.length() < 1) {
        m_appVersion = CBaseIdfa::getInstance()->getversion();
    }
    return m_appVersion;
}


const string& systemInfo::getchannelId()
{
    if (m_channelId.length() < 1) {
        m_channelId = CBaseIdfa::getInstance()->getplatform_id();
    }
    return m_channelId;
}



