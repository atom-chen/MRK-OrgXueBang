//
//  UIModifyHead.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#include "UIModifyHead.hpp"
#include "UserManager.hpp"
#include "MessageManager.hpp"
#include "stdafx.h"
#include "SceneManager.hpp"

void UIModifyHead::onEnter()
{
    iTarget = 1;
    Layer::onEnter();
}
void UIModifyHead::onExit()
{
    Layer::onExit();
    UserManager::getInstance()->reqHeadIcon(StringUtils::format("face%d.png",iTarget));
    
    EntUser* user = UserManager::getInstance()->getUserEnt();
    user->_HeadIcon = StringUtils::format("face%d.png",iTarget);
    
    MessageManager::getInstance()->postNotification(NSEventNameDef::UpdateHeadIcon);
}

bool UIModifyHead::init()
{
    if(!Layer::init()){
        return false;
    }
    initUI();
    return true;
}

void UIModifyHead::initUI()
{
    auto mUI = CSLoader::createNodeWithVisibleSize("View/CommView/ModifyHead.csb");
    
    this->addChild(mUI);
    
    vector<string> vecBtnName = {"FileNodeTopNav",};
    vector<string> vecNodeName = {
        "txtName","txtValidate","imgHead",
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        
        if(name == "FileNodeTopNav"){
            auto node = mUI->getChildByName(name)->getChildByName("ImgBG");
            btn = dynamic_cast<Button*>(node->getChildByName("btnBack"));
            BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UIModifyHead::btnClickHandle, this));
            mapWidget["btnBack"] = btn;
            
            ((Text*)node->getChildByName("txtTitle"))->setString("更换头像");
        }
    }
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
    
    for(int i=1; i<=6; i++){
        Button* btnHead = static_cast<cocos2d::ui::Button*>(Helper::seekWidgetByName(static_cast<Widget*>(mUI), StringUtils::format("btnHead%d",i)));
        btnHead->setActionTag(i);
        btnHead->addTouchEventListener(CC_CALLBACK_2(UIModifyHead::touchEvent, this));
        
        mapWidget[StringUtils::format("imgFace%d",i)] = ((Widget*)btnHead->getChildByName("imgHead"));
        mapWidget[StringUtils::format("imgSelected%d",i)] = (Widget*)btnHead->getChildByName("imgSelected");
    }
    
    
    mapWidget["imgBG"] = (Widget*)mUI->getChildByName("FileNodeBG")->getChildByName("imgBG");
    auto screenSize = SceneManager::getInstance()->m_fullSceneSize;
    mapWidget["imgBG"]->setContentSize(screenSize);
    Helper::doLayout(mapWidget["imgBG"]);
    
    EntUser* user = UserManager::getInstance()->getUserEnt();
    const char index = user->_HeadIcon.at(4);
    iTarget = atoi(&index);
    if(iTarget > 6 || iTarget < 1){
        iTarget = 1;
    }
    selectedItam(iTarget);
}


void UIModifyHead::selectedItam(int indexTarget)
{
    auto res = ((ImageView*)mapWidget[StringUtils::format("imgFace%d",indexTarget)])->getRenderFile();
    ((ImageView*)mapWidget["imgHead"])->loadTexture(res.file,(Widget::TextureResType)res.type);
    
    string fileName = "Image/SubImg/ModifyHead/img_grade_select1.png";
    ((ImageView*)mapWidget[StringUtils::format("imgSelected%d",indexTarget)])->loadTexture(fileName,Widget::TextureResType::PLIST);
}

void UIModifyHead::touchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    CLICK_FILTER(sender, type);
    int btnType = (dynamic_cast<Widget*>(sender))->getActionTag();
    if(btnType == iTarget){
        return;
    }
    SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
    string fileName = "Image/SubImg/ModifyHead/img_grade_select0.png";
    ((ImageView*)mapWidget[StringUtils::format("imgSelected%d",iTarget)])->loadTexture(fileName,Widget::TextureResType::PLIST);
    
    iTarget = btnType;
    selectedItam(iTarget);
}
void UIModifyHead::btnClickHandle(Ref* pSender)
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
void UIModifyHead::onRevMsgDoHandle(Ref* ref)
{
}
