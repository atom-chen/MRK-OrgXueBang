//
//  BehaviorManager.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/2/22.
//
//

#include "BehaviorManager.hpp"
#include "TipManager.hpp"
#include "UserManager.hpp"

BehaviorManager::BehaviorManager()
{
    event_id = "";
    bReady = false;
    iPostCount = 0;
}

BehaviorManager::~BehaviorManager()
{
    
}

BehaviorManager* BehaviorManager::getInstance()
{
    static BehaviorManager instance;
    return &instance;
}


void BehaviorManager::endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json)
{
    eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
    
    if (nOp >= eHttpType::postevent && nOp <= eHttpType::postevent_end)
    {
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            std::string return_code = json.getValue("return_code");
            if (return_code != "6000") {
                std::string return_msg = json.getValue("return_msg");
                TipManager::getInstance()->showTipWithString(return_msg.c_str());
                return;
            }
            
            
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            std::string return_msg = json.getValue("return_msg");
            std::string return_code = json.getValue("return_code");
            TipManager::getInstance()->showTipWithString(return_msg.c_str());
        }
        else//网络异常
        {
            
        }
    }
    else if (nOp == eHttpType::postevent_openbook)
    {
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            std::string return_code = json.getValue("return_code");
            if (return_code != "6000") {
                std::string return_msg = json.getValue("return_msg");
                TipManager::getInstance()->showTipWithString(return_msg.c_str());
                return;
            }
            
            if (!bReady) {
                bReady = true;
                stuJson returnData = json.getJson("return_data");
                event_id = returnData.getValue("event_id");
                if(event_id == "-1") event_id = "";
            }
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            std::string return_msg = json.getValue("return_msg");
            std::string return_code = json.getValue("return_code");
            TipManager::getInstance()->showTipWithString(return_msg.c_str());
        }
        else//网络异常
        {
            
        }
    }
    
    
}

void BehaviorManager::postEvent(const string  type, const string log, eHttpType netOp)
{
    HttpDataToString dataStr;
    dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
    dataStr.addData("type", type);//course/page/login||textbook/textpage
    dataStr.addData("log", log);
    dataStr.addData("event_id", event_id);
    int tmp;
    if(netOp == eHttpType::postevent_openbook
       || netOp == eHttpType::postevent_closebook)
    {
        tmp = (int)netOp;
    }
    else
    {
        tmp = (int)netOp + iPostCount;
    }
    CDataHttp::getInstance()->HttpPost((eHttpType)tmp , dataStr.getString());
    iPostCount++;//反正日志只是记录，并不需要特异性回调
}
