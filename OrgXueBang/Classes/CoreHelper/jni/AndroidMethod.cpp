#include "stdafx.h"
#include "AndroidMethod.h"
#include "Recorder.h"
//#include "../../../android_common/AndroidPay.h"
//#include "VodInterface.hpp"
//#include "PushManager.hpp"
//#include "ThirdPartyPay.hpp"
#include "SoundInterface.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

#define MYJNI_CLASS  "com/readingmate/book/AppActivity"

#ifdef __cplusplus
extern "C" {
#endif
    
    jstring Java_com_readingmate_book_AppActivity_JavaCallCpp(JNIEnv * env,
                                                             jclass thiz,
                                                             jint nType,
                                                             jstring str)
    {
        const char* ccResp = env->GetStringUTFChars(str, NULL);
        cocos2d::log("nType = %d,ccResp = %s", (int)nType, ccResp);
        const char *r = "type no define";
        
        switch (nType)
        {
            case MYJNICALL_TYPE_DEVICEINFO:
                cocos2d::log("MYJNICALL_TYPE_DEVICEINFO is cpp call java");
                r = "OK";
                break;
            case MYJNICALL_TYPE_RECORDINFO:
                cocos2d::log("callback record");
                Recorder::getInstance()->stopRecordCallBack(std::string(ccResp));
                r = "OK";
                break;
            case MYJNICALL_TYPE_PLAYOVER:
                cocos2d::log("callback play over");
                Recorder::getInstance()->stopPlayRecordCallBack(std::string(ccResp));
                r = "OK";
                break;
            case MYJNICALL_TYPE_PAY_WX:
                cocos2d::log("callback pay");
                //ThirdPartyPay::getInstance()->handlePayResult(std::string(ccResp),2);
                r = "OK";
                break;
            case MYJNICALL_TYPE_PAY_ALI:
                cocos2d::log("callback pay");
                //ThirdPartyPay::getInstance()->handlePayResult(std::string(ccResp),1);
                r = "OK";
                break;
            case MYJNICALL_TYPE_SCANQR:
                cocos2d::log("callback scanqr");
                //ComInterface::scanQRCallBack(std::string(ccResp));
                r = "OK";
                break;
            case MYJNICALL_TYPE_JUMPSEER:
                cocos2d::log("callback seer to open book");
//                Director::getInstance()->getRunningScene()->scheduleOnce([=](float t)
//                 {
//                     ComInterface::openBook(std::string(ccResp));
//                 }, 0.1, "SeerJumpToBook");
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                    //ComInterface::openBook(std::string(ccResp));
                });
                
                r = "OK";
                break;
            case MYJNICALL_TYPE_VIDEO_CALLBACK:
                //VodInterface::callBack(std::string(ccResp));
                r = "OK";
                break;
            case MYJNICALL_TYPE_PUSHMSG_REGISTER: {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                    //PushManager::instance()->setPushRegisterId(std::string(ccResp));
                });
            }
                break;
            case MYJNICALL_TYPE_PUSHMSG_RECEIVED: {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                    //PushManager::instance()->receivePushMsg(std::string(ccResp));
                });
            }
                break;
            case MYJNICALL_TYPE_PUSHMSG_CLICK: {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                    //PushManager::instance()->sendPushClickMsgIdForAndroid(std::string(ccResp));
                });
            }
                break;
            case MYJNICALL_TYPE_AUDIO: {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                    SoundInterface::runFinishCallBack(atoi(ccResp));
                });
            }
                break;
            default:
                
                break;
        }
        return (env)->NewStringUTF(r);
    }
    
    
    
    
    
    
    const char* CppCallJava(int nType, const std::string& str)
    {
        const char* value=NULL;
        
        JniMethodInfo jmi;
        bool isHave = JniHelper::getStaticMethodInfo(jmi,
                                                     MYJNI_CLASS,
                                                     "CppCallJava",
                                                     "(ILjava/lang/String;)Ljava/lang/String;");
        if(!isHave)
            return value;
        jobject  data = NULL;
        jstring  p = jmi.env->NewStringUTF(str.c_str());
        data = jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID,nType,p);
        value=jmi.env->GetStringUTFChars((jstring)data, NULL);
        jmi.env->DeleteLocalRef(jmi.classID);
        return value;
    }
    
//    void CppCallShareLink(int type,const std::string& imagePath,const std::string& url,const std::string& title,const std::string& desc)
//    {
//        JniMethodInfo jmi;
//        bool isHave = JniHelper::getStaticMethodInfo(jmi,
//                                                     MYJNI_CLASS,
//                                                     "CppCallShareLink",
//                                                     "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
//        if(!isHave)
//            return;
//        cocos2d::log("-------- CppCallShareLink function find !---------");
////        jobject  data = NULL;
//        jstring  p1 = jmi.env->NewStringUTF(imagePath.c_str());
//        jstring  p2 = jmi.env->NewStringUTF(url.c_str());
//        jstring  p3 = jmi.env->NewStringUTF(title.c_str());
//        jstring  p4 = jmi.env->NewStringUTF(desc.c_str());
//        jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID,type,p1,p2,p3,p4);
//        jmi.env->DeleteLocalRef(jmi.classID);
//    }
#ifdef __cplusplus
}
#endif

#endif
