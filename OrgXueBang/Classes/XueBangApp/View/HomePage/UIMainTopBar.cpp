//
//  UIMainTopBar.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/15.
//

#include "UIMainTopBar.hpp"
#include "UIUserInfo.hpp"
#include "UserManager.hpp"

#include "UINetAlert.hpp"
#include "UILoading.hpp"
#include "MyWordReport.hpp"
#include "MyWord.hpp"
#include "BookListMain.hpp"
#include "HomePageManager.hpp"
#include "TipManager.hpp"

UIMainTopBar::UIMainTopBar(Node* nodeRoot)
{
    csbRoot = nodeRoot;
    MessageManager::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(UIMainTopBar::onUpdateHeadIcon), NSEventNameDef::UpdateHeadIcon);
}

UIMainTopBar::~UIMainTopBar()
{
    MessageManager::getInstance()->removeObserver(this);
}

UIMainTopBar* UIMainTopBar::create(Node* nodeRoot)
{
    UIMainTopBar *pRet = new(std::nothrow) UIMainTopBar(nodeRoot);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}
void UIMainTopBar::onEnter()
{
    Node::onEnter();
}
void UIMainTopBar::onExit()
{
    Node::onExit();
}

bool UIMainTopBar::init()
{
    if(!Node::init()){
        return false;
    }
    initUI();
    initUIData();
    return true;
}

void UIMainTopBar::initUI()
{
//    auto mUI = CSLoader::createNode("View/HomePage/NodeTopBar.csb");
//    this->addChild(mUI);
    
    auto mUI = csbRoot;
    
    vector<string> vecBtnName = {"btnWord","btnBookList","imgHead"};
    vector<string> vecNodeName = {
        "txtName","txtClass",
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UIMainTopBar::btnClickHandle, this));
        mapWidget[name] = btn;
    }
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
}

void UIMainTopBar::btnClickHandle(Ref* pSender)
{
    SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    if(name == "btnWord")
    {
        MyWord::create(this);
//        MyWordReport::create(this);
    }
    else if(name == "btnBookList")
    {
//        auto layer = UINetAlert::create();
//        this->addChild(layer);
        
        
//        auto dlg = UINetAlert::create();
//        request->setUserData((void *)0);
//        dlg->addRequest(request);
//        UiBase::getInstance()->getRunningScene()->addChild(dlg,1000000);
        
//        UILoading::showFrog();
        talkingInterface::traceEvent("首页", "我的书单");
        this->addChild(BookListMain::create());
    }
    else if(name == "imgHead")
    {
        auto layer = UIUserInfo::create();
        this->addChild(layer);
//        HomePageManager::getInstance()->reqHomePageBookList(false);
//        TipManager::getInstance()->openTip(eTipWaiting);
    }
}

void UIMainTopBar::initUIData()
{
    onUpdateHeadIcon(nullptr);
}
void UIMainTopBar::onUpdateHeadIcon(Ref* ref)
{
    EntUser* user = UserManager::getInstance()->getUserEnt();
    ((Text*)mapWidget["txtName"])->setString(user->_UserName);
    ((Text*)mapWidget["txtClass"])->setString(user->_GradeName + user->_ClassName);
    
    string fileName = "Image/SubImg/Commom/HeadIcon/" + user->_HeadIcon;
    ((ImageView*)mapWidget["imgHead"])->loadTexture(fileName,Widget::TextureResType::PLIST);
}
