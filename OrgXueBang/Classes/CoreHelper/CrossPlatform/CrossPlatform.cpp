//
//  CrossPlatform.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#include "CrossPlatform.hpp"

#include "baseIdfa.h"

CrossPlatform* CrossPlatform::instance = nullptr;

CrossPlatform::CrossPlatform()
{
}
CrossPlatform::~CrossPlatform()
{
}
CrossPlatform* CrossPlatform::getInstance()
{
    if(instance == nullptr){
        instance = new CrossPlatform();
    }
    return instance;
}

string CrossPlatform::getDeviceInfo()
{
    string deviceInfo = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    deviceInfo = CBaseIdfa::getInstance()->getDeviceInfo();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    deviceInfo = CBaseIdfa::getInstance()->getDeviceInfo();
#endif
    return deviceInfo;
}

string CrossPlatform::getVersion()
{
    string appVersion = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    appVersion = CBaseIdfa::getInstance()->getversion();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    appVersion = CBaseIdfa::getInstance()->getversion();
#endif
    return appVersion;
}
