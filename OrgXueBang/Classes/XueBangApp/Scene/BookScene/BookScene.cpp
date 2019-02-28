//
//  BookScene.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/11.
//
//

#include "BookScene.hpp"
#include "TipManager.hpp"
#include "BookBody.hpp"
#include "BookListManager.hpp"
#include "ComDlg.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "NetworkReachabilityManager.h"
static NetworkReachabilityStatus s_netStatus;
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "AndroidMethod.h"
static string s_netStatus;
#endif

BookScene::BookScene()
{
    
}

BookScene::~BookScene()
{
    
}

void BookScene::onEnter()
{
    BaseScene::onEnter();
    //scheduleUpdate();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    NetworkRecahabilityManager::shareManager()->startMonitoring();
    this->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        auto newStatus = NetworkRecahabilityManager::shareManager()->getCurrentStatus();
        if (newStatus != s_netStatus) {
            if (newStatus != NetworkReachabilityStatus::ReachableViaWiFi && newStatus != NetworkReachabilityStatus::ReachableViaWWAN) {
                return;
            }
            s_netStatus = newStatus;
            if (newStatus == NetworkReachabilityStatus::ReachableViaWWAN) {
                if (NetworkRecahabilityManager::shareManager()->isReachableViaWWAN())
                {
                    auto dlg = ComDlg::create();
                    dlg->setStringAndBtnType("      当前未连接Wi-Fi,是否继续用流量阅读？");
                    this->addChild(dlg);
                }
            }
        }
    }), DelayTime::create(5), NULL)));
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    this->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        string info = CppCallJava(MYJNICALL_TYPE_NETINFO, "");
//        CCLOG("ret = %s", info.c_str());
        stuJson retJson;
        retJson.init();
        CBaseLoad::LoadJsonString(retJson, info);
        auto newStatus = retJson.getValue("net");
        if (newStatus != s_netStatus) {
            if (newStatus != "2" && newStatus != "1") {
                return;
            }
            s_netStatus = newStatus;
            if (newStatus == "1") {
                auto dlg = ComDlg::create();
                dlg->setStringAndBtnType("      当前未连接Wi-Fi,是否继续用流量阅读？");
                this->addChild(dlg);
            }
        }
    }), DelayTime::create(5), NULL)));
#endif
    
}

void BookScene::onExit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    NetworkRecahabilityManager::shareManager()->stopMonitoring();
#endif
    BaseScene::onExit();
    
    if (BookListManager::instance()) {
        BookListManager::instance()->reloadListView();
    }
}

bool BookScene::init()
{
    if (!BaseScene::init())
    {
        return false;
    }
    
    auto node = this->getChildByName(DESIGN_APP_WINDOW_NAME);
    setFixWindow();
    
    BookBody* body = BookBody::create();
    node->addChild(body);
    
    return true;
}

bool BookScene::OnBaseKeyBack()
{
    SLog("返回键退出")
    return true;
}
