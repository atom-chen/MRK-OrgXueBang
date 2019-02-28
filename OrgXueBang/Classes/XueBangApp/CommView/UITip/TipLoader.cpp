//
//  TipLoader.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/14.
//
//

#include "TipLoader.hpp"

TipLoader::TipLoader()
{
//    m_bDel = false;
}

TipLoader::~TipLoader()
{
}

void TipLoader::onEnter()
{
    BaseLayer::onEnter();
//    _touchListener = EventListenerTouchOneByOne::create();
//    _touchListener->setSwallowTouches(true);
//
//    _touchListener->onTouchBegan = CC_CALLBACK_2(TipLoader::onTouchBegan, this);
//    _touchListener->onTouchMoved = CC_CALLBACK_2(TipLoader::onTouchMoved, this);
//    _touchListener->onTouchEnded = CC_CALLBACK_2(TipLoader::onTouchEnded, this);
//
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_touchListener, -2);
}

void TipLoader::onExit()
{
//    Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
    BaseLayer::onExit();
}

bool TipLoader::init()
{
    if (BaseLayer::init())
    {
//        this->load(UI_layer_loader);
        this->setName("TipLoader");
        return true;
    }
    
    return false;
}

void TipLoader::addEvents()
{
    //关联基本Panel
    Widget* panel_root = (Widget*)getRootWidget();
    Widget* panel_left = (Widget*)panel_root->getChildByName("panel_left");
    Widget* panel_right = (Widget*)panel_root->getChildByName("panel_right");
    Widget* panel_top = (Widget*)panel_root->getChildByName("panel_top");
    Widget* panel_bottom = (Widget*)panel_root->getChildByName("panel_bottom");
    Widget* panel_center = (Widget*)panel_root->getChildByName("panel_center");
    Widget* panel_popup = (Widget*)panel_root->getChildByName("panel_popup");
    
    //隐藏无用Panel
    panel_top->setVisible(false);
    panel_left->setVisible(false);
    panel_right->setVisible(false);
    panel_bottom->setVisible(false);
    panel_center->setVisible(false);
    panel_popup->setVisible(true);
    
    
//    panel_root->getChildByName("panel_black")->setVisible(true);
    m_frog = nullptr;
//    this->runAction(Sequence::create(DelayTime::create(10), CallFunc::create([=](){
//        if (m_frog) {
//            return;
//        }
//
//        createFrog();
//    }), NULL));
    createFrogLater(10);

}


void TipLoader::refreshUIView()
{
    
}

void TipLoader::showLoader(float minShowTime)
{
    auto node = UiBase::getInstance()->getRunningScene();
    if (node == nullptr) {
        return;
    }
    node->runAction(CallFunc::create([minShowTime](){
        auto node = UiBase::getInstance()->getRunningScene();
        if (node == nullptr) {
            return;
        }
        auto loader= (TipLoader*)node->getChildByName("TipLoader");
        if (!loader) {
            loader = TipLoader::create();
            node->addChild(loader, 200);
        }
        loader->setVisible(true);
        auto action = loader->getActionByTag(100);
        if (action) {
            loader->stopAction(action);
        }
        action = Sequence::create(DelayTime::create(minShowTime), CallFunc::create([](){
            TipLoader::HideLoader();
        }), NULL);
        action->setTag(100);
        loader->runAction(action);
        
        loader->createFrogLater(10);
//        CCLOG("tiploader show !");
    }
                                     ));
    
}

void TipLoader::HideLoader()
{
    auto node = UiBase::getInstance()->getRunningScene();
    if (node == nullptr) {
        return;
    }
    node->runAction(Sequence::create(DelayTime::create(0.01f),
                                     CallFunc::create([](){
        auto node = UiBase::getInstance()->getRunningScene();
        if (node == nullptr) {
            return;
        }
        auto loader= node->getChildByName("TipLoader");
//        if (loader && Reador::getInstance()->isInfoOk()) {
//            loader->removeFromParent();
//        }
//        if (loader && HttpManager::getInstance()->getCurHttpNum() == 0) {
            auto action = loader->getActionByTag(100);
            if (action) {
                return;
            }
            loader->setVisible(false);
            if (((TipLoader*)loader)->m_frog) {
                ((TipLoader*)loader)->m_frog->setVisible(false);
            }
//            CCLOG("tiploader hide !");
//        }
    }), NULL));

}

void TipLoader::showFrog()
{
    showLoader();
    auto node = UiBase::getInstance()->getRunningScene();
    auto loader= node->getChildByName("TipLoader");
    if (loader) {
        ((TipLoader*)loader)->createFrog();
    }

}

void TipLoader::createFrog()
{
    Widget* panel_root = (Widget*)getRootWidget();
    Widget* panel_popup = (Widget*)panel_root->getChildByName("panel_popup");
    
    panel_root->getChildByName("panel_black")->setVisible(true);
    
    auto skeleton_frog = SkeletonAnimation::createWithJsonFile("spine/frog.json", "spine/frog.atlas", 0.5f);
    skeleton_frog->setAnimation(0, "animation", true);
    skeleton_frog->setTimeScale(0.5);
    auto panelSize = panel_popup->getContentSize();
    skeleton_frog->setPosition(Vec2(panelSize.width / 2, panelSize.height / 2));
    //skeleton_frog->setScale(0.5);
    panel_popup->addChild(skeleton_frog);
    
    m_frog = skeleton_frog;
    
    this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3), CallFunc::create([=](){
//        if (HttpManager::getInstance()->getCurHttpNum() < 1 && Reador::getInstance()->isInfoOk()) {
//            this->removeFromParent();
//            return;
//        }
    }), NULL)));
}

bool TipLoader::isShow()
{
    auto node = UiBase::getInstance()->getRunningScene();
    if (node == nullptr) {
        return false;
    }
    auto loader= node->getChildByName("TipLoader");
    if (loader) {
        if (loader->isVisible()) {
            return true;
        }
    }
    return false;
}

void TipLoader::createFrogLater(float delay)
{
    this->stopActionByTag(222);
    
    auto action = Sequence::create(DelayTime::create(10), CallFunc::create([=](){
        if (m_frog) {
            return;
        }
        
        createFrog();
    }), NULL);
    action->setTag(222);
    
    this->runAction(action);
}


