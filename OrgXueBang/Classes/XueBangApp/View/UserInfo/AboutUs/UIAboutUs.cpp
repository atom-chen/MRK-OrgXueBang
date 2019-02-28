//
//  UIAboutUs.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#include "UIAboutUs.hpp"
#include "UserManager.hpp"
#include "MessageManager.hpp"
#include "HttpTag.h"

#include "CoreHelperStdafx.h"
#include "SceneManager.hpp"
#include "CrossPlatform.hpp"

void UIAboutUs::onEnter()
{
    Layer::onEnter();
}
void UIAboutUs::onExit()
{
    Layer::onExit();
}

bool UIAboutUs::init()
{
    if(!Layer::init()){
        return false;
    }
    initUI();
    return true;
}

void UIAboutUs::initUI()
{
    auto mUI = CSLoader::createNodeWithVisibleSize("View/CommView/AboutUs.csb");
    
    this->addChild(mUI);
    
    vector<string> vecBtnName = {"FileNodeTopNav",};
    vector<string> vecNodeName = {
        "txtVersion",
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        
        if(name == "FileNodeTopNav"){
            auto node = mUI->getChildByName(name)->getChildByName("ImgBG");
            btn = dynamic_cast<Button*>(node->getChildByName("btnBack"));
            
            ((Text*)node->getChildByName("txtTitle"))->setString("关于我们");
            
            BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UIAboutUs::btnClickHandle, this));
            mapWidget["btnBack"] = btn;
        }else{
            BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UIAboutUs::btnClickHandle, this));
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
    
    string strVersion = "当前版本：V" + CrossPlatform::getInstance()->getVersion();
    ((Text*)mapWidget["txtVersion"])->setString(strVersion);
}

void UIAboutUs::btnClickHandle(Ref* pSender)
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
