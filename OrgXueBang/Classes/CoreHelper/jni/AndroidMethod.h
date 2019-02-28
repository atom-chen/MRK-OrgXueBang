/********************************************************************
	调用java接口
*********************************************************************/

#ifndef __AndroidMethod_H__
#define __AndroidMethod_H__


#include "cocos2d.h"
using namespace cocos2d;



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#ifdef __cplusplus
extern "C" {
#endif
    
#define MYJNICALL_TYPE_DEVICEINFO   10000
#define MYJNICALL_TYPE_NETINFO      11000
#define MYJNICALL_TYPE_RECORDINFO   12000
#define MYJNICALL_TYPE_PLAYOVER		12001
#define MYJNICALL_TYPE_INSTALLAPK   13000
#define MYJNICALL_TYPE_APKPATH      13001
#define MYJNICALL_TYPE_PAY_WX       14000
#define MYJNICALL_TYPE_PAY_ALI      14001
#define MYJNICALL_TYPE_SHARE        15000
#define MYJNICALL_TYPE_SHARE_LINK   15001
#define MYJNICALL_TYPE_SAVE_IMAGE   16000
#define MYJNICALL_TYPE_OPEN_URL     17000
#define MYJNICALL_TYPE_SCANQR       18000
#define MYJNICALL_TYPE_APPRAISE     19000
#define MYJNICALL_TYPE_AUDIO        19001

#define MYJNICALL_TYPE_AVAILABLE_MEM     20000
#define MYJNICALL_TYPE_USED_MEM     21000
    
#define MYJNICALL_TYPE_TALKINGDATA_PAGEBEGIN     22000
#define MYJNICALL_TYPE_TALKINGDATA_PAGEEND     23000
#define MYJNICALL_TYPE_TALKINGDATA_EVENT     24000
#define MYJNICALL_TYPE_JUMPVIDEO    25000
    
#define MYJNICALL_TYPE_MULTI_TOUCH_ENABLE    26000
    
#define MYJNICALL_TYPE_JUMPSEER    27000
    
#define MYJNICALL_TYPE_VIDEO_CREATE         50000
#define MYJNICALL_TYPE_VIDEO_REMOVE         50001
#define MYJNICALL_TYPE_VIDEO_SETFULLRECT    50002
#define MYJNICALL_TYPE_VIDEO_FULLSCREEN     50003
#define MYJNICALL_TYPE_VIDEO_ORGSCREEN      50004
#define MYJNICALL_TYPE_VIDEO_PLAY           50005
#define MYJNICALL_TYPE_VIDEO_PAUSE          50006
#define MYJNICALL_TYPE_VIDEO_ISPLAYING      50007
#define MYJNICALL_TYPE_VIDEO_CURTIME        50008
#define MYJNICALL_TYPE_VIDEO_SUMTIME        50009
#define MYJNICALL_TYPE_VIDEO_SEEKTIME       50010
#define MYJNICALL_TYPE_VIDEO_ISEXIST        50011
#define MYJNICALL_TYPE_VIDEO_CALLBACK       50012
#define MYJNICALL_TYPE_VIDEO_SETURL         50013
#define MYJNICALL_TYPE_VIDEO_OFFSET_WINDOW  50014
#define MYJNICALL_TYPE_VIDEO_SHOW_WINDOW    50015

#define MYJNICALL_TYPE_PUSHMSG_REGISTER     55000    //push sdk register
#define MYJNICALL_TYPE_PUSHMSG_RECEIVED     55001    //push sdk received msg
#define MYJNICALL_TYPE_PUSHMSG_CLICK        55002    //push sdk push click

#define MYJNICALL_TYPE_RECORD_MODE          60001
    
//android 检测键盘是否弹出
#define MYJNICALL_TYPE_CHECK_KEYBOARD_SHOW  90000
    
    jstring Java_com_readingmate_book_AppActivity_JavaCallCpp(JNIEnv * env,
                                                          jclass thiz,
                                                          jint nType,
                                                          jstring str);
    
    const char* CppCallJava(int nType,const std::string& str);
    
//    void CppCallShareLink(int type,const std::string& imagePath,const std::string& url,const std::string& title,const std::string& desc);
    
#ifdef __cplusplus
}
#endif
#endif


#endif /* defined(__AndroidMethod_H__) */
