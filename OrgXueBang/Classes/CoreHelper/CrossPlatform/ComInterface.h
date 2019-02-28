//
//  ComInterface.hpp
//  ReadingFriends
//
//  Created by metrics001 on 16/10/20.
//
//

#ifndef ComInterface_hpp
#define ComInterface_hpp

#include <stdio.h>
using namespace std;
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
//检查权限种类
enum eAuthorizeType
{
    eAuthorizeType_1 = 1,//相册权限
    eAuthorizeType_2 = 2,//麦克风权限
    eAuthorizeType_3 = 3,//推送权限
    eAuthorizeType_4 = 4,//网络权限
};

class ComInterface {
public:
    //引导评价
    static void showAppraise();
    //授权检查
    static bool checkAuthorize(eAuthorizeType type);
    //屏幕亮度设置
    static float getBright();
    static void  setBright(float value);
    //打开网址
    static void  openUrl(string urlStr);
    //扫描二维码
    static void  scanQR();
    //扫描二维码回调
    static void  scanQRCallBack(string urlStr);
    
    //西禾接口
    static void showXiheVedio(string strPhone,string strName = "");
    
    static void openBook(string bookname);
    
    static void hideStatusbar(bool bHide);
};
#endif /* ComInterface_hpp */
