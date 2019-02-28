//
//  UIUserInfo.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#include "UIUserInfo.hpp"
#include "UserManager.hpp"
#include "MessageManager.hpp"
#include "HttpTag.h"

#include "CoreHelperStdafx.h"
#include "SceneManager.hpp"
#include "UIModifyHead.hpp"
#include "UISetting.hpp"

void UIUserInfo::onEnter()
{
    Layer::onEnter();
    MessageManager::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(UIUserInfo::onUpdateHeadIcon), NSEventNameDef::UpdateHeadIcon);
}
void UIUserInfo::onExit()
{
    Layer::onExit();
    MessageManager::getInstance()->removeObserver(this);
}

bool UIUserInfo::init()
{
    if(!Layer::init()){
        return false;
    }
    initUI();
    initUIData();
    return true;
}

void UIUserInfo::initUI()
{
    auto mUI = CSLoader::createNodeWithVisibleSize("View/CommView/UserInfo.csb");
    
    this->addChild(mUI);
    
    vector<string> vecBtnName = {"btnSetting","imgHead"};
    vector<string> vecNodeName = {
        "txtName","txtValidate","itemInfo","lvInfo","FileNodeTopNav",
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        if(name == "btnSetting"){
            btn = (Widget*)mUI->getChildByName("FileNodeTopNav")->getChildByName(name);
        }
        BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UIUserInfo::btnClickHandle, this));
        mapWidget[name] = btn;
    }
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
        
        if(name == "itemInfo"){
            btn->removeFromParent();
            btn->retain();
        }
        else if(name == "lvInfo"){
            ListView* lv = static_cast<cocos2d::ui::ListView*>(btn);
            lv->removeAllItems();
            lv->setScrollBarEnabled(false);
            lv->setPropagateTouchEvents(true);
            lv->setSwallowTouches(false);
            lv->setItemsMargin(15);
        }
        else if(name == "FileNodeTopNav"){
            auto node = mUI->getChildByName(name)->getChildByName("ImgBG");
            auto btn = dynamic_cast<Button*>(node->getChildByName("btnBack"));
            btn->addClickEventListener(std::bind(&UIUserInfo::btnClickHandle,this,std::placeholders::_1));
            mapWidget["btnBack"] = btn;
            
            ((Text*)node->getChildByName("txtTitle"))->setString("个人信息");
        }
    }
    
    mapWidget["imgBG"] = (Widget*)mUI->getChildByName("FileNodeBG")->getChildByName("imgBG");
    auto screenSize = SceneManager::getInstance()->m_fullSceneSize;
    mapWidget["imgBG"]->setContentSize(screenSize);
    Helper::doLayout(mapWidget["imgBG"]);
}

void UIUserInfo::initUIData()
{
    EntUser* user = UserManager::getInstance()->getUserEnt();
    ((Text*)mapWidget["txtName"])->setString(user->_UserName);
    
    Date* tmp_date = Date::date();
    tmp_date->setTime(user->_ExpireTime);
    int tmp_day = tmp_date->getMday();
    int tmp_month = tmp_date->getMonth()+1;
    int tmp_year = tmp_date->getYear()+1900;
    string strDate = "";
    if(tmp_year < 2017)
    {
        strDate = "账号有效期至 2017-12-12";
    }else{
        strDate = StringUtils::format("%s%04d-%02d-%02d", "账号有效期至 ", tmp_year, tmp_month, tmp_day);
    }
    ((Text*)mapWidget["txtValidate"])->setString(strDate);
    
    onUpdateHeadIcon(nullptr);
    doAllListCell();
}
void UIUserInfo::doAllListCell()
{
    EntUser* user = UserManager::getInstance()->getUserEnt();
    
    map<string,string> mapInfo;
    mapInfo["学校名称："] = user->_SchoolName;
    mapInfo["任课教师："] = user->_TeacherName;
    mapInfo["我的年级："] = user->_GradeName;
    mapInfo["我的班级："] = user->_ClassName;
    
    ListView* lv = static_cast<cocos2d::ui::ListView*>(mapWidget["lvInfo"]);
    lv->removeAllItems();
    for(auto info : mapInfo){
        auto item = mapWidget["itemInfo"]->clone();
        ((Text*)item->getChildByName("txtKey"))->setString(info.first.c_str());
        ((Text*)item->getChildByName("txtValue"))->setString(info.second.c_str());
        lv->pushBackCustomItem(item);
    }
    
}
void UIUserInfo::btnClickHandle(Ref* pSender)
{
    SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    if(name == "btnBack")
    {
        this->removeFromParent();
    }
    else if(name == "btnSetting")
    {
        auto layer = UISetting::create();
        this->addChild(layer);
    }
    else if(name == "imgHead")
    {
        auto layer = UIModifyHead::create();
        this->addChild(layer);
    }
}
void UIUserInfo::onUpdateHeadIcon(Ref* ref)
{
    EntUser* user = UserManager::getInstance()->getUserEnt();
    
    string fileName = "Image/SubImg/Commom/HeadIcon/" + user->_HeadIcon;
    ((ImageView*)mapWidget["imgHead"])->loadTexture(fileName,Widget::TextureResType::PLIST);
}
