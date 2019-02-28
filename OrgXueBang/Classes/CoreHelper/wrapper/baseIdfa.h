
//  idfa

#ifndef __BASE_IDFA_H__
#define __BASE_IDFA_H__

#include "BaseLoad.h"


class CBaseIdfa
{
    
public:
    static CBaseIdfa* getInstance();
    void setDeviceToken(std::string token);

public:
    /** 获取idfa */
    const char* getIdfaStr();
    const char* getIdfvStr();
    /*版本*/
    const char* getSystemVersion();
    const char* getmodel();
    const char* getversion();//"]//|否|版本|
    const char* getcarrier();//""]//|否|运营商|
    const char* getplatform();//""]//|否|ios或者android|
    const char* getplatform_id();//""]//|否|分发平台id|
    const char* getbundle_id();//""]//|否|包id|
    const char* getDeviceToken();

    void makeDeviceInfo();
    const char* getDeviceInfo();
    
    float availableMemory();
    float usedMemory();
    
    void setMultipleTouchEnabled(bool bEnabled);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    const char* getTrueIdfaStr();
#endif
private:
    CBaseIdfa();
    ~CBaseIdfa();
    static CBaseIdfa* m_pBashIdfa;
    std::string m_strDeviceToken;
    std::string m_strDeviceInfo;
    
//    NSString* deviceModelName(NSString* deviceModel);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    stuJson m_stuJson;
public:
    const char* getImei();
    const char* getwifiMac();
    const char* getbtMac();
#endif
};


#endif /* Recorder_hpp */
