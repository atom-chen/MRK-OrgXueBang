//
//  GameMacro.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/10.
//
//

#ifndef GameMacro_hpp
#define GameMacro_hpp

//#include "PreIncludes.hpp"

//设计分辨率
#define DESIGN_RESOLUTION_WIDTH     750
#define DESIGN_RESOLUTION_HEIGHT    1334
#define DESIGN_APP_WINDOW_NAME     "appWindow"
#define DESIGN_BG_WINDOW_NAME       "bgWindow"


//分辨率参考768*1024适配
#define BASE_WND_WIDTH      768     //单位窗口宽
#define BASE_WND_HEIGHT     1024    //单位窗口基线高
#define BASE_WND_CENTER_X   512     //单位窗口框中点
#define BASE_WND_CENTER_Y   384     //单位窗口高中点

//屏幕宽高宏
#define SCREEN_HEIGHT       (Director::getInstance()->getOpenGLView()->getFrameSize().height)
#define SCREEN_WIDTH        (Director::getInstance()->getOpenGLView()->getFrameSize().width)
#define SCREEN_SIZE         Director::getInstance()->getOpenGLView()->getFrameSize()

//屏幕的纵横比
#define RATIO_SCREEN        (SCREEN_HEIGHT/SCREEN_WIDTH)

//适配2d资源尺寸用
#define SIZE_SCALE_2D       (Director::getInstance()->getOpenGLView()->getFrameSize().height/Game::sharedGame()->getDesignSize().height)

//适配全屏放大2d资源尺寸用
#define SIZE_SCALE_2D_Y     (Director::getInstance()->getOpenGLView()->getFrameSize().height/Game::sharedGame()->getDesignSize().height)
#define SIZE_SCALE_2D_X     (Director::getInstance()->getOpenGLView()->getFrameSize().width/Game::sharedGame()->getDesignSize().width)

#define WND_CENTER_X        (SCREEN_WIDTH/2)
#define WND_CENTER_Y        (SCREEN_HEIGHT/2)
#define WINDOW_CENTER       Vec2(WND_CENTER_X,WND_CENTER_Y)

//计算UI的中心位置 mzg
#define UI_CENTER_X         (Game::sharedGame()->getDesignSize().width/2)
#define UI_CENTER_Y         (Game::sharedGame()->getDesignSize().height/2)
#define UI_CENTER           Vec2(UI_CENTER_X,UI_CENTER_Y)


#if defined(WIN32) && defined(_WINDOWS)
#define DebugLine           CCLOG("%s. %d", __FUNCTION__, __LINE__);
#else
#define DebugLine           CCLOG("%s. %d", __PRETTY_FUNCTION__, __LINE__);
#endif

#define SLog(args)          CCLOG("===YY===%s===%d===%s===",__FUNCTION__,__LINE__,args);
#define ILog(argi)          CCLOG("===YY===%s===%d===%d===",__FUNCTION__,__LINE__,argi);
#define FLog(argf)          CCLOG("===YY===%s===%d===%f===",__FUNCTION__,__LINE__,argf);
#define SFLog(args,argf)    CCLOG("===YY===%s===%s===%f===",__FUNCTION__,args,argf);
#define SLLog(args,argl)    CCLOG("===YY===%s===%s===%ld===",__FUNCTION__,args,argl);
#define SILog(args,argi)    CCLOG("===YY===%s===%s===%d===",__FUNCTION__,args,argi);
#define SSLog(args1,args2)  CCLOG("===YY===%s===%s===%s===",__FUNCTION__,args1,args2);

//打印下载Log的开关
#define DOWNLOAD_LOG 0

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define SoundName(str)  (str ".ogg")
#define EffectName(str)	(str ".ogg")
#else
#define SoundName(str)  (str ".mp3")
#define EffectName(str)	(str ".mp3")
#endif

#define MY_DESIGNSIZE_W 1536.0f//
#define MY_DESIGNSIZE_H 2173.0f

//#define MY_DESIGNSIZE_W 2480.0f//
//#define MY_DESIGNSIZE_H 3508.0f
//////////////////////////////////////////////////////////////////////////
//基础数据类型
//typedef	signed char     int8;
//typedef	unsigned char   uint8;
//typedef	short           int16;
//typedef	unsigned short  uint16;
//typedef	int             int32;
//typedef	unsigned int    uint32;
//typedef	long            int64;
//typedef	unsigned long   uint64;
//内存操作
#define MEMSET          memset
#define MEMCPY          memcpy
#define MEMMOVE         memmove

#define		MALLOC(s)		malloc(s)
#define		FREE(p)			free(p)
#define		FREEIF(p)		if (p) { FREE((void*)p); (p) = 0; }

//获取和设置一个值的宏
#define MYDEF_GETSETVALUE(type,name)    \
type m_##name;\
void set##name(type value)\
{\
m_##name = value;\
}\
type get##name()\
{\
return m_##name;\
}\
type* getAddress##name()\
{\
return &m_##name;\
}


//获取和设置数组的宏
#define MYDEF_GETSETVALUEARRAY(type,name,len)    \
type m_##name[len];\
bool set##name(const type* value,int nLen)\
{\
if(nLen < len)\
{\
memset(m_##name,0,sizeof(type)*len);\
memcpy(m_##name,value,nLen);\
return true;\
}\
return false;\
}\
type* get##name()\
{\
return m_##name;\
}


//////////////////////////////////
//Z轴

#define MYDEF_LAYER_Z_GUIDE         5  //松鼠
#define MYDEF_LAYER_Z_UI            10  //UI
#define MYDEF_LAYER_Z_WIN           20  //胜利界面
#define MYDEF_LAYER_Z_PAUSE         30  //暂停界面
#define MYDEF_LAYER_Z_LOGIN         100 //登录
#define MYDEF_LAYER_Z_REGEDIT       101 //注册

#define MYDEF_LAYER_Z_STORE         201 //商店 // by hyp
#define MYDEF_LAYER_Z_PROMPT        205 //提示面板//add by hyp
#define MYDEF_LAYER_Z_WAIT          500 //等待

#define BTN_NAME_BACK   "com_return.png"

#define RECORD_SPX0_WAV1    0

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define RECORD_FORMAT   "wav"
#else
#define RECORD_FORMAT   "spx"
#endif

#define QUESTION_ID "1"
#define QUESTION_TITLE "Which page did you find very interesting? Please choose it from the book gallery or draw your own picture about this book."
#define QUESTION_CN "请选择书中你最喜欢的一页，或拍照上传你画的图来表达读后感，添加到阅读笔记里吧！"

#define BOTTOM_BTN_MOVE_DELAY 0.2
#define BOTTOM_BTN_FUNC_DELAY 0.1

#endif /* GameMacro_hpp */
