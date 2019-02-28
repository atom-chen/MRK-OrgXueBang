//
//  MsgParam.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#include "MsgParam.hpp"

MsgParam::MsgParam():resFlag(RES_OK)
,errorTip("")
,_type(0)
{
    
}
MsgParam::~MsgParam()
{
    
}

MsgParam::MsgParam(string errTip)
{
    resFlag = RES_ERROR;
    errorTip = errTip;
}

string MsgParam::getErrorTip()
{
    return errorTip;
}

bool MsgParam::getIsResOK()
{
    return (resFlag == RES_OK);
}

void MsgParam::setRefData(cocos2d::Ref* data)
{
    _data = data;
}
cocos2d::Ref* MsgParam::getRefData()
{
    return _data;
}
void MsgParam::setVecTemp(vector<string>* vecStr)
{
    vecStrTemp = vecStr;
}
vector<string> MsgParam::getVecTemp()
{
    return *vecStrTemp;
}
