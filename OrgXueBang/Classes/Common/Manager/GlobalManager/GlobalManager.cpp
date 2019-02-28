//
//  GlobalManager.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#include "GlobalManager.hpp"

#include "MessageManager.hpp"
#include "CrossPlatform.hpp"
#include "CoreHelperStdafx.h"
#include "FileLog.h"
#include "UINetAlert.hpp"
#include "SceneManager.hpp"

static GlobalManager* instance = nullptr;
GlobalManager* GlobalManager::getInstance()
{
    if(instance == nullptr){
        instance = new GlobalManager();
    }
    return instance;
}
void GlobalManager::destroyInstance()
{
    delete instance;
    instance = nullptr;
}

GlobalManager::GlobalManager()
{
}
GlobalManager::~GlobalManager()
{
}

void GlobalManager::endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json)
{
    eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
    if (nOp == eHttpType::uploadLog) {
        CFileLog::UnLock();
    }
}
void GlobalManager::addRequest(cocos2d::network::HttpRequest *request)
{
    request->retain();
    long n = (long)request->getUserData();
    if (n < 3) {
        n++;
        request->setUserData((void *)n);
        HttpManager::getInstance()->HttpsendRequest(request);
        CCLOG("retry n = %ld", n);
    } else {
        auto dlg = UINetAlert::create();
        request->setUserData((void *)0);
        dlg->addRequest(request);
//        UiBase::getInstance()->getRunningScene()->addChild(dlg,1000000);
        SceneManager::getInstance()->getCurScene()->addChild(dlg,1000000);
    }
}
