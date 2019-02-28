//
//  UIAgreement.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#include "UIAgreement.hpp"
#include "UserManager.hpp"
#include "MessageManager.hpp"
#include "HttpTag.h"

#include "CoreHelperStdafx.h"
#include "SceneManager.hpp"
#include "TipManager.hpp"

void UIAgreement::onEnter()
{
    Layer::onEnter();
}
void UIAgreement::onExit()
{
    Layer::onExit();
}

bool UIAgreement::init()
{
    if(!Layer::init()){
        return false;
    }
    initUI();
    return true;
}

void UIAgreement::initUI()
{
    auto mUI = CSLoader::createNodeWithVisibleSize("View/CommView/Agreement.csb");
    
    this->addChild(mUI);
    
    vector<string> vecBtnName = {"FileNodeTopNav",};
    vector<string> vecNodeName = {
        "PanelCenter",
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        
        if(name == "FileNodeTopNav"){
            auto node = mUI->getChildByName(name)->getChildByName("ImgBG");
            btn = dynamic_cast<Button*>(node->getChildByName("btnBack"));
            
            ((Text*)node->getChildByName("txtTitle"))->setString("服务协议");
            
            BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UIAgreement::btnClickHandle, this));
            mapWidget["btnBack"] = btn;
        }else{
            BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UIAgreement::btnClickHandle, this));
            mapWidget[name] = btn;
        }
    }
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
        
    mapWidget["imgBG"] = (Widget*)mUI->getChildByName("FileNodeBG")->getChildByName("imgBG");
    auto screenSize = SceneManager::getInstance()->m_fullSceneSize;
    mapWidget["imgBG"]->setContentSize(screenSize);
    Helper::doLayout(mapWidget["imgBG"]);
    
    
    m_webView = cocos2d::experimental::ui::WebView::create();
    m_webView->setScalesPageToFit(true);
    m_webView->setVisible(false);
    m_webView->setOnShouldStartLoading([=](cocos2d::experimental::ui::WebView *sender, const std::string &url){
        printf("setOnShouldStartLoading");
        return true;
    });
    m_webView->setOnDidFinishLoading([=](cocos2d::experimental::ui::WebView *sender, const std::string &url){
        printf("setOnDidFinishLoading");
        TipManager::getInstance()->removeLoading(mUI);
        m_webView->setVisible(true);
    });
    m_webView->setOnDidFailLoading([=](cocos2d::experimental::ui::WebView *sender, const std::string &url){
        printf("setOnDidFailLoading");
        TipManager::getInstance()->showMsgTips("网页开小差了，请稍候 !");
        TipManager::getInstance()->removeLoading(mUI);
    });
    m_webView->setContentSize(mapWidget["PanelCenter"]->getContentSize());
    m_webView->setPosition(mapWidget["PanelCenter"]->getContentSize()/2);
    mapWidget["PanelCenter"]->addChild(m_webView, -1);
    
    m_webView->loadURL("http://lbactivity.dubaner.com/html/xieyi.html");
    TipManager::getInstance()->showLoading(mUI);
}

void UIAgreement::btnClickHandle(Ref* pSender)
{
    SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    if(name == "btnBack")
    {
        this->removeFromParentAndCleanup(true);
    }
}
