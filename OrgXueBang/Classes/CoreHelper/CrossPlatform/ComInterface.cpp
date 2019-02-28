//
//  ComInterface.cpp
//  ReadingFriends
//
//  Created by metrics001 on 16/10/20.
//
//
#include "stdafx.h"
#include "ComInterface.h"
#include "SceneManager.hpp"
#include "Reador.hpp"
#include "AndroidMethod.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "AppraiseIOS.h"
#include "AuthorizeIOS.h"
//#include "XiheIOS.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "json.h"
#endif

void ComInterface::showAppraise()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AppraiseIOS::showAppraise();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CppCallJava(MYJNICALL_TYPE_APPRAISE,"");
#endif
    
}

bool ComInterface::checkAuthorize(eAuthorizeType type)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    switch (type) {
        case eAuthorizeType_1:
            return AuthorizeIOS::checkPhoto();
            break;
        case eAuthorizeType_2:
            return AuthorizeIOS::checkMicrophone();
            break;
        case eAuthorizeType_3:
            return AuthorizeIOS::checkPush();
            break;
        case eAuthorizeType_4:
            return AuthorizeIOS::checkNetwork();
            break;
        default:
            return false;
            break;
    }
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return true;
#endif
    return false;
}

float ComInterface::getBright()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return AuthorizeIOS::getBright();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
#endif
    return 0.5f;
}
void  ComInterface::setBright(float value)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AuthorizeIOS::setBright(value);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
#endif
}
//打开网址
void ComInterface::openUrl(string urlStr)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AuthorizeIOS::openUrl(urlStr);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CppCallJava(MYJNICALL_TYPE_OPEN_URL,urlStr.c_str());
#endif
}
//扫描二维码
void ComInterface::scanQR()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CppCallJava(MYJNICALL_TYPE_SCANQR,"");
#endif
}
//扫描二维码回调
void ComInterface::scanQRCallBack(string urlStr)
{
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//        CLayerLogin::scanCardCallback(urlStr);
//    });
#endif
}

void ComInterface::showXiheVedio(string strPhone,string strName)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    XiheIOS::showVedio(strPhone, strName);
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    Json::Value root;
//    root["phone"] = strPhone;
//    root["name"] = strName;
//    string str = root.toStyledString();
//    CppCallJava(MYJNICALL_TYPE_JUMPSEER, str);
#endif
}

void ComInterface::openBook(string bookname)
{
    Reador::getInstance()->setCurrentBook(bookname, "0");
    SceneManager::getInstance()->setGameState(eStateBook);
}

void ComInterface::hideStatusbar(bool bHide)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AuthorizeIOS::hideStatusbar(bHide);
#else
    
#endif
    UserDefault::getInstance()->setBoolForKey("hide_statusBar", bHide);
}

