//
//  UISetting.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#include "UISetting.hpp"
#include "FacadeManager.hpp"
#include "MessageManager.hpp"
#include "HttpTag.h"
#include "talkingInterface.h"

#include "CoreHelperStdafx.h"
#include "SceneManager.hpp"
#include "UIAboutUs.hpp"
#include "UIAgreement.hpp"
#include "UILogin.hpp"

void UISetting::onEnter()
{
    Layer::onEnter();
}
void UISetting::onExit()
{
    Layer::onExit();
}

bool UISetting::init()
{
    if(!Layer::init()){
        return false;
    }
    initUI();
    return true;
}

void UISetting::initUI()
{
    auto mUI = CSLoader::createNodeWithVisibleSize("View/CommView/Setting.csb");
    
    this->addChild(mUI);
    
    vector<string> vecBtnName = {"FileNodeTopNav","itemClearCache","itemAboutUs","itemAgreement",
        "btnLogout","btnOK","btnCancle"};
    vector<string> vecNodeName = {
        "txtCacheSize","PanelPopup"
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        
        if(name == "FileNodeTopNav"){
            auto node = mUI->getChildByName(name)->getChildByName("ImgBG");
            btn = dynamic_cast<Button*>(node->getChildByName("btnBack"));
            
            ((Text*)node->getChildByName("txtTitle"))->setString("系统设置");
            
            BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UISetting::btnClickHandle, this));
            mapWidget["btnBack"] = btn;
        }else{
            BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UISetting::btnClickHandle, this));
            mapWidget[name] = btn;
        }
    }
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
    mapWidget["itemAgreement"]->setVisible(false);
    
    mapWidget["imgBG"] = (Widget*)mUI->getChildByName("FileNodeBG")->getChildByName("imgBG");
    auto screenSize = SceneManager::getInstance()->m_fullSceneSize;
    mapWidget["imgBG"]->setContentSize(screenSize);
    Helper::doLayout(mapWidget["imgBG"]);
    
    FileUtil::fileSizeTotal = 0l;
    string filePath = FileUtils::getInstance()->getWritablePath();
    FileUtil::getDirSize(filePath.c_str(),0);
    ((Text*)mapWidget["txtCacheSize"])->setString(StringUtils::format("%ldmb",FileUtil::fileSizeTotal/1024/1024));
    
}

void UISetting::btnClickHandle(Ref* pSender)
{
    SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    if(name == "btnBack")
    {
        this->removeFromParentAndCleanup(true);
    }
    else if(name == "itemClearCache"){
        mapWidget["PanelPopup"]->setVisible(true);
    }
    else if(name == "itemAboutUs"){
        auto layer = UIAboutUs::create();
        this->addChild(layer);
    }
    else if(name == "itemAgreement"){
        auto layer = UIAgreement::create();
        this->addChild(layer);
    }
    else if(name == "btnLogout"){
        FacadeManager::getInstance()->getUserManager()->doLogout();
        
        Director::getInstance()->getRunningScene()->removeAllChildren();
        auto layer = UILogin::create();
        Director::getInstance()->getRunningScene()->addChild(layer);
    }
    else if(name == "btnOK"){
        string filePath = FileUtils::getInstance()->getWritablePath();
        FileUtil::clearDir(filePath.c_str());
        
        ((Text*)mapWidget["txtCacheSize"])->setString("0mb");
        mapWidget["PanelPopup"]->setVisible(false);
        
        DownImg::createDir();
        FileUtils::getInstance()->purgeCachedEntries();
        SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
        AudioEngine::uncacheAll();
        talkingInterface::traceEvent("系统设置清除缓存", "");
    }
    else if(name == "btnCancle"){
        mapWidget["PanelPopup"]->setVisible(false);
    }
}
