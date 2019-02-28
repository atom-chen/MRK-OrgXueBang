//
//  UIReadingFinish.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/25.
//

#include "UIReadingFinish.hpp"
#include "EventNameDefine.hpp"

UIReadingFinish::UIReadingFinish()
{
    MessageManager::getInstance()->addObserver(this, CC_CALLMESSAGE_SELECTOR(UIReadingFinish::updateUIData), NSEventNameDef::ReadingEndShowAnim);
    talkingInterface::pageBegin("阅读完成阅读页");
}
UIReadingFinish::~UIReadingFinish()
{
    MessageManager::getInstance()->removeObserver(this);
    talkingInterface::pageEnd("阅读完成阅读页");
}
void UIReadingFinish::onEnter()
{
    PageBase::onEnter();
}
void UIReadingFinish::onExit()
{
    PageBase::onExit();
}
bool UIReadingFinish::init()
{
    if (!PageBase::init())
    {
        return false;
    }
    //    initUI();
    return true;
}

UIReadingFinish* UIReadingFinish::create(Widget* pageUI)
{
    UIReadingFinish *ret = new (std::nothrow) UIReadingFinish();
    if (ret && ret->init(pageUI))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool UIReadingFinish::init(Widget* pageUIs)
{
    auto pageUI = (Widget*)CSLoader::createNode("View/Reading/UIReadingFinish.csb");
    //    this->addChild(mUI);
    
    if (!PageBase::init())
    {
        return false;
    }
    this->load(pageUI);
    return true;
}

void UIReadingFinish::addEvents()
{
    initUI();
}

void UIReadingFinish::initUI()
{
    //    auto mUI = CSLoader::createNode("ui/ReadKing/LayerUIReadingFinish.csb");
    //    m_pageWidget->addChild(mUI);
    //    UiBase::getInstance()->getRunningScene()->addChild(mUI);
    
    auto mUI = m_pageWidget;
    
    vector<string> vecBtnName = {"imgReRead","imgGoMain",};
    vector<string> vecNodeName = {
        "imgBG","PanelCenter","PanelBtn","nodeAnim",
    };
    
    for (int i=0; i < vecBtnName.size(); i++) {
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), vecBtnName[i]);
        BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(UIReadingFinish::btnClickHandle, this));
        mapWidget[vecBtnName[i]] = btn;
    }
    for (string name : vecNodeName) {
        Widget* btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
    mapWidget["nodeAnim"] = (Widget*)mapWidget["PanelCenter"]->getChildByName("nodeAnim");
//    initAnim();
}

void UIReadingFinish::btnClickHandle(Ref* pSender)
{
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    if(name == "imgReRead")
    {
        //        this->removeFromParent();
//        msg_param_readking* param = new msg_param_readking();
//        param->_type = msg_param_readking::eTYPE::RK_READING_BACK;
//        MessageManager::getInstance()->postNotification(MSG_RK_DO_HANDLE_DATA,(Ref*)param);
       
//        SceneManager::getInstance()->setGameState(eStateBook);
        MessageManager::getInstance()->postNotification(NSEventNameDef::ReadingAgain);
        talkingInterface::traceEvent("原版阅读报告按钮", "重新阅读");
    }
    else if(name == "imgGoMain")
    {
        SceneManager::getInstance()->setGameState(eStateMain);
    }
}

void UIReadingFinish::updateUIData(Ref* data)
{
    initAnim();
}
void UIReadingFinish::initAnim()
{
    SkeletonAnimation* skeletonAnim = nullptr;
    if (mapWidget["nodeAnim"]->getChildByName("skeletonAnim")) {
        skeletonAnim = dynamic_cast<SkeletonAnimation*>(mapWidget["nodeAnim"]->getChildByName("skeletonAnim"));
    }else {
        skeletonAnim = SkeletonAnimation::createWithJsonFile("spine/chui.json", "spine/chui.atlas", 1.0f);
//        skeletonAnim->setPosition(Vec2(m_pageWidget->getContentSize().width*0.5, m_pageWidget->getContentSize().height*0.5));
        skeletonAnim->setScale(0.5);
        skeletonAnim->setName("skeletonAnim");
        mapWidget["nodeAnim"]->addChild(skeletonAnim);
    }
    skeletonAnim->clearTracks();
    skeletonAnim->setAnimation(0, "animation", false);
}
