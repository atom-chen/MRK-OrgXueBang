//
//  UILogin.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#include "UILogin.hpp"
#include "UserManager.hpp"
#include "MessageManager.hpp"
#include "HttpTag.h"
#include "UIMainPage.hpp"

#include "CoreHelperStdafx.h"
#include "FileSaveMap.h"
#include "EditBoxUtil.hpp"

void UILogin::onEnter()
{
    Layer::onEnter();
    MessageManager::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(UILogin::onRevMsgDoHandle),(int)eHttpType::login);
}
void UILogin::onExit()
{
    Layer::onExit();
    
    MessageManager::getInstance()->removeObserver(this);
    
    auto glView = Director::getInstance()->getOpenGLView();
    if (glView)
    {
        glView->setIMEKeyboardState(false);
    }
}
UILogin::UILogin()
{
    
}
UILogin::~UILogin()
{
    CCLOG("UILogin::~UILogin()====");
}
bool UILogin::init()
{
    if(!Layer::init()){
        return false;
    }
    this->setName("UILogin");
    initUI();
    return true;
}

void UILogin::initUI()
{
//    auto mUI = CSLoader::createNode("View/CommView/Login.csb");
    auto mUI = CSLoader::createNodeWithVisibleSize("View/CommView/Login.csb");
    
    this->addChild(mUI);
    
    vector<string> vecBtnName = {"btnLogin",};
    vector<string> vecNodeName = {
        "tfAccount","tfPwd","txtErrorTip",
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UILogin::btnClickHandle, this));
        mapWidget[name] = btn;
    }
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
    ((Text*)mapWidget["txtErrorTip"])->setString("");
    
    ((TextField*)mapWidget["tfAccount"])->setPlaceHolderColor(Color4B(0x5f, 0x5f, 0x5f, 80));
    ((TextField*)mapWidget["tfAccount"])->setTextColor(Color4B(51, 51, 51, 255));
    ebAccount = EditBoxUtil::createEditBox((TextField*)mapWidget["tfAccount"], this);
    ebAccount->setFontColor(Color4B(51, 51, 51, 255));
    ebAccount->setInputMode(EditBox::InputMode::SINGLE_LINE);
    ebAccount->setReturnType(EditBox::KeyboardReturnType::DONE);
    
    
    ((TextField*)mapWidget["tfPwd"])->setPlaceHolderColor(Color4B(0x5f, 0x5f, 0x5f, 80));
    ((TextField*)mapWidget["tfPwd"])->setTextColor(Color4B(51, 51, 51, 255));
    ebPassword = EditBoxUtil::createEditBox((TextField*)mapWidget["tfPwd"], this);
    ebPassword->setFontColor(Color4B(51, 51, 51, 255));

    ebPassword->setInputMode(EditBox::InputMode::SINGLE_LINE);
    ebPassword->setReturnType(EditBox::KeyboardReturnType::DONE);
    
    string la = CFileSaveMap::getInstance()->readFromFile("LoginAccount");
    ebAccount->setText(la.c_str());
}

void UILogin::btnClickHandle(Ref* pSender)
{
    SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    if(name == "btnLogin")
    {
        string tip = "";
        string account = ebAccount->getText();
        string pwd = ebPassword->getText();
        StringUtil::trim(account);
        StringUtil::trim(pwd);
        if(account.length() == 0){
            tip = "账号不能为空！";
            ((Text*)mapWidget["txtErrorTip"])->setString(tip);
            return;
        }
        if(pwd.length() == 0){
            tip = "密码不能为空！";
            ((Text*)mapWidget["txtErrorTip"])->setString(tip);
            return;
        }
        
//        if(true){
//            account = "13581857877";
//            pwd = "1234";
//            pwd = "123456";
//        }
        ((Text*)mapWidget["txtErrorTip"])->setString(tip);
        UserManager::getInstance()->reqLogin(account,pwd);
    }
}
void UILogin::onRevMsgDoHandle(Ref* ref)
{
    MsgParam* param = (MsgParam*)ref;
    if(param->getIsResOK()){
        printf("UILogin::onRevMsgDoHandle::");
        
        string account = ebAccount->getText();
        StringUtil::trim(account);
        CFileSaveMap::getInstance()->writeValueFile("LoginAccount",account);
        
        Director::getInstance()->getRunningScene()->removeAllChildren();
        auto layerMain = UIMainPage::create();
        Director::getInstance()->getRunningScene()->addChild(layerMain);
    }else{
        string tip = param->getErrorTip();
        ((Text*)mapWidget["txtErrorTip"])->setString(tip);
    }
}

void UILogin::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    CCLOG("editBox %p DidBegin !", editBox);
}

void UILogin::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    CCLOG("editBox %p DidEnd !", editBox);
}

void UILogin::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, cocos2d::ui::EditBoxDelegate::EditBoxEndAction action)
{
    CCLOG("editBox %p DidEnd with action %d!", editBox, action);
}

void UILogin::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    CCLOG("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void UILogin::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    CCLOG("editBox %p was returned !==%s",editBox,editBox->getText());
    
    if (ebAccount == editBox)
    {
    }
    else if (ebPassword == editBox)
    {
//        btnClickHandle(mapWidget["btnLogin"]);
    }
}
