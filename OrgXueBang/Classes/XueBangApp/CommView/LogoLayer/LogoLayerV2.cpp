//
//  LogoLayerV2.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#include "LogoLayerV2.hpp"

#include "SceneManager.hpp"

bool LogoLayerV2::init()
{
    if(!Layer::init()){
        return false;
    }
    initUI();
    return true;
}

void LogoLayerV2::initUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    string csb = "View/CommView/LogoPad.csb";
    if (visibleSize.width < 850) {
        csb = "View/CommView/Logo.csb";
    }
    
//    auto mUI = CSLoader::createNode(csb);
    auto mUI = CSLoader::createNodeWithVisibleSize(csb);
    
    this->addChild(mUI);
    
    vector<string> vecBtnName = {};
    vector<string> vecNodeName = {
        "pageview_start",
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(LogoLayerV2::btnClickHandle, this));
        mapWidget[name] = btn;
    }
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
    doPageView();
}

void LogoLayerV2::btnClickHandle(Ref* pSender)
{
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    if(name == "img_start")
    {
        jumpNext();
    }
}

void LogoLayerV2::doPageView()
{
    auto pageview_start = dynamic_cast<PageView*>(mapWidget["pageview_start"]);
    pageview_start->setIndicatorEnabled(true);
    pageview_start->setIndicatorIndexNodesColor(Color3B(55, 55, 55));
    pageview_start->setIndicatorSelectedIndexColor(Color3B(200, 0, 0));
    pageview_start->setIndicatorPosition(Vec2(pageview_start->getContentSize().width / 2, 20));
    pageview_start->setIndicatorIndexNodesScale(0.5);
    //    pageview_start->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    
    auto items = pageview_start->getItems();
    for (int i = 0; i < items.size(); i++) {
        Widget* img_start = dynamic_cast<Widget*>(items.at(i)->getChildByName("img_start"));
        img_start->addClickEventListener(CC_CALLBACK_1(LogoLayerV2::btnClickHandle, this));
    }
    auto node = items.at(items.size() - 1);
    
    m_layer = Layer::create();
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    
    listener->onTouchBegan = [=](Touch* touch, Event* event)
    {
        if (pageview_start->getCurrentPageIndex() + 1 >= pageview_start->getItems().size()) {
            m_bEnd = true;
        } else {
            m_bEnd = false;
        }
        Vec2 touchPos = touch->getLocation();
        this->m_touchPosOld.set(touchPos);
        
        return true;
    };
    listener->onTouchMoved = [this](Touch* touch, Event* event)
    {
        Vec2 touchPos = touch->getLocation();
        this->m_touchPosNew.set(touchPos);
    };
    listener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        Vec2 touchPos = touch->getLocation();
        this->m_touchPosNew.set(touchPos);
        if (m_bEnd) {
            this->checkEnd();
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_layer);
    this->addChild(m_layer, 10000);
}
void LogoLayerV2::checkEnd()
{
    if (abs(m_touchPosNew.x - m_touchPosOld.x) < abs(m_touchPosNew.y - m_touchPosOld.y)) {
        return;
    }
    if (m_touchPosNew.x + 100 > m_touchPosOld.x) {
        return;
    }
    
    auto pageview_start = dynamic_cast<PageView*>(mapWidget["pageview_start"]);
    if (pageview_start->getCurrentPageIndex() + 1 >= pageview_start->getItems().size()) {
        jumpNext();
    } ;
}
void LogoLayerV2::jumpNext()
{
    log("<#double#>");
    SceneManager::getInstance()->setGameState(eStateMain);
}
