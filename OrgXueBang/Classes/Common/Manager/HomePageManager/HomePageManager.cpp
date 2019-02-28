//
//  HomePageManager.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/23.
//

#include "HomePageManager.hpp"
#include "UserManager.hpp"
#include "CoreHelperStdafx.h"
#include "EventNameDefine.hpp"

HomePageManager* HomePageManager::instance = nullptr;

HomePageManager* HomePageManager::getInstance()
{
    if(instance == nullptr)
    {
        instance = new HomePageManager();
    }
    return instance;
}
void HomePageManager::destroyInstance()
{
    delete instance;
    instance = nullptr;
}

HomePageManager::HomePageManager():forceReq(false),curReadingWeek(""),curReadingBookID("")
{
    init();
}
HomePageManager::~HomePageManager()
{
    
}
void HomePageManager::init()
{
    mHomePageModel = new HomePageModel();
}

HomePageModel* HomePageManager::getHomePageModel()
{
    return mHomePageModel;
}

bool HomePageManager::reqHomePageBookList(bool reqNextPage,bool bforceReq)
{
    forceReq = bforceReq;
    int pageCount = mHomePageModel->getHomePageBookListEnt()->_PageCount;
    int afterIndex = mHomePageModel->getHomePageBookListEnt()->_AfterPageIndex;
    int frontIndex = mHomePageModel->getHomePageBookListEnt()->_FrontPageIndex;
    
    if(forceReq || pageCount == 0){
        reqHomePageBookList(1);
    }else{
        if(reqNextPage){
            afterIndex ++;
            if(afterIndex > pageCount){
                return false;
            }else{
                if(mHomePageModel->isHadLocalData(afterIndex)){
                    return false;
                }else{
                    mHomePageModel->getHomePageBookListEnt()->_AfterPageIndex = afterIndex;
                    reqHomePageBookList(afterIndex);
                }
            }
        }else{
            frontIndex --;
            if(frontIndex < 1){
                return false;
            }else{
                if(mHomePageModel->isHadLocalData(frontIndex)){
                    return false;
                }else{
                    mHomePageModel->getHomePageBookListEnt()->_FrontPageIndex = frontIndex;
                    reqHomePageBookList(frontIndex);
                }
            }
        }
    }
    return true;//请求成功
}

void HomePageManager::reqHomePageBookList(int curPageIndex,int pageLimitNum)
{
    pageLimitNum = 10;
    
    unordered_map<string, string>values;
    unordered_map<string, string>heads;
    values["page_index"] = StringUtils::format("%d",curPageIndex);
    values["page_limit"] = StringUtils::format("%d",pageLimitNum);
    
    CDataHttp::getInstance()->HttpGetNew(eHttpType::homePageBookList, heads, values);
}

void HomePageManager::endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json)
{
    eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
    if (nOp == eHttpType::homePageBookList){
        if (eHttpStateBase::eHttpStateBase_Rok == e) {
            std::string return_code = json.getValue("return_code");
            if (return_code != "6000") {
                std::string return_msg = json.getValue("return_msg");
                return;
            }
            if(forceReq){
                mHomePageModel->clearData();
            }
            forceReq = false;
            mHomePageModel->parseHomePageBookList(json);
            
            MsgParam* param = new MsgParam();
            MessageManager::getInstance()->postNotification((int)nOp,(Ref*)param);
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr){
            string tipError = CDataHttp::getInstance()->GetHttpOpMsg(nOp);
            MsgParam* param = new MsgParam(tipError);
            MessageManager::getInstance()->postNotification((int)nOp,(Ref*)param);
        }
    }
}

void HomePageManager::setBookReaded(string bookID)
{
    mHomePageModel->setBookReaded(bookID);
    
    MsgParam* param = new MsgParam();
    vector<string> vecStr;
    vecStr.push_back(curReadingWeek);
    vecStr.push_back(curReadingBookID);
    param->setVecTemp(&vecStr);
    MessageManager::getInstance()->postNotification(NSEventNameDef::ReadingEnd,(Ref*)param);
}

void HomePageManager::setReadingBookInfo(string curWeek,string curBookID)
{
    curReadingWeek = curWeek;
    curReadingBookID = curBookID;
}
