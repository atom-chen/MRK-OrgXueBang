//
//  MsgParam.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#ifndef MsgParam_hpp
#define MsgParam_hpp

#include <stdio.h>
#include <string>
using namespace std;
#include "cocos2d.h"

class MsgParam
{
private:
    enum eRES{
        RES_OK = 0,//消息
        RES_ERROR,
    };
public:
    MsgParam();
    ~MsgParam();
    
    MsgParam(string errTip);
    
    bool getIsResOK();
    
    string getErrorTip();
    void setErrorTip(string errTip);
    
    void setRefData(cocos2d::Ref* data);
    cocos2d::Ref* getRefData();
    void setVecTemp(vector<string>* vecStr);
    vector<string> getVecTemp();
private:
    eRES resFlag;//返回结果
    string errorTip;//错误提示
    
    vector<string>* vecStrTemp;
    
    cocos2d::Ref* _data;//扩展用
    int _type;//扩展用
};


#endif /* MsgParam_hpp */
