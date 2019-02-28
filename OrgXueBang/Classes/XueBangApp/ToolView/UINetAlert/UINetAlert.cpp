//
//  UINetAlert.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/16.
//

#include "UINetAlert.hpp"
#include "HttpManager.h"

bool UINetAlert::init()
{
    if(!Layer::init()){
        return false;
    }
    initUI();
    return true;
}

void UINetAlert::initUI()
{
    auto mUI = CSLoader::createNode("View/ToolView/UINetAlert.csb");
    
    this->addChild(mUI);
    
    vector<string> vecBtnName = {"img_close","img_known"};
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
//        btn->addClickEventListener(CC_CALLBACK_1(UINetAlert::btnClickHandle, this));
        BtnUtil::addClickEventListener(btn, CC_CALLBACK_1(UINetAlert::btnClickHandle, this));
        mapWidget[name] = btn;
    }
    m_requestList.clear();
}

void UINetAlert::btnClickHandle(Ref* pSender)
{
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    if(name == "img_close")
    {
        this->removeFromParent();
    }
    else if(name == "img_known")
    {
        for (auto request : m_requestList) {
            HttpManager::getInstance()->HttpsendRequest(request);
        }
        __NotificationCenter::getInstance()->postNotification(NSEventNameDef::ReloadOnePage, nullptr);
        this->removeFromParent();
    }
}

void UINetAlert::addRequest(cocos2d::network::HttpRequest *request)
{
    request->retain();
    m_requestList.push_back(request);
}
