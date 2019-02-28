//
//  UILoading.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/16.
//

#include "UILoading.hpp"
#include "HttpManager.h"

bool UILoading::init()
{
    if(!Layer::init()){
        return false;
    }
    this->setName("UILoading");
    initUI();
    return true;
}

void UILoading::initUI()
{
    auto mUI = CSLoader::createNode("View/ToolView/UILoading.csb");
    
    this->addChild(mUI);
    
    vector<string> vecNodeName = {
        "panel_popup","panel_black"
    };
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
    
    m_frog = nullptr;
    createFrogLater(10);
}
void UILoading::showLoader(float minShowTime)
{
    auto node = SceneManager::getInstance()->getCurScene();
    if (node == nullptr) {
        return;
    }
    node->runAction(CallFunc::create([minShowTime](){
        auto node = SceneManager::getInstance()->getCurScene();
        if (node == nullptr) {
            return;
        }
        auto loader= (UILoading*)node->getChildByName("UILoading");
        if (!loader) {
            loader = UILoading::create();
            node->addChild(loader, 200);
        }
        loader->setVisible(true);
        auto action = loader->getActionByTag(100);
        if (action) {
            loader->stopAction(action);
        }
        action = Sequence::create(DelayTime::create(minShowTime), CallFunc::create([](){
            UILoading::HideLoader();
        }), NULL);
        action->setTag(100);
        loader->runAction(action);
        
        loader->createFrogLater(10);
        //        CCLOG("UILoading show !");
    }
                                     ));
    
}

void UILoading::HideLoader()
{
    auto node = SceneManager::getInstance()->getCurScene();
    if (node == nullptr) {
        return;
    }
    node->runAction(Sequence::create(DelayTime::create(0.01f),
                                     CallFunc::create([](){
        auto node = SceneManager::getInstance()->getCurScene();
        if (node == nullptr) {
            return;
        }
        auto loader= node->getChildByName("UILoading");
        if (loader && HttpManager::getInstance()->getCurHttpNum() == 0) {
            auto action = loader->getActionByTag(100);
            if (action) {
                return;
            }
            loader->setVisible(false);
            if (((UILoading*)loader)->m_frog) {
                ((UILoading*)loader)->m_frog->setVisible(false);
            }
        }
    }), NULL));
}

void UILoading::showFrog()
{
    showLoader();
    auto node = SceneManager::getInstance()->getCurScene();
    auto loader= node->getChildByName("UILoading");
    if (loader) {
        ((UILoading*)loader)->createFrog();
    }
}

void UILoading::createFrog()
{
    mapWidget["panel_black"]->setVisible(true);
    
    auto skeleton_frog = SkeletonAnimation::createWithJsonFile("spine/frog.json", "spine/frog.atlas", 0.5f);
    skeleton_frog->setAnimation(0, "animation", true);
    skeleton_frog->setTimeScale(0.5);
    auto panelSize = mapWidget["panel_popup"]->getContentSize();
    skeleton_frog->setPosition(Vec2(panelSize.width / 2, panelSize.height / 2));
    //skeleton_frog->setScale(0.5);
    mapWidget["panel_popup"]->addChild(skeleton_frog);
    
    m_frog = skeleton_frog;
    
    this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3), CallFunc::create([=](){
//        if (HttpManager::getInstance()->getCurHttpNum() < 1 && Reador::getInstance()->isInfoOk()) {
        if (HttpManager::getInstance()->getCurHttpNum() < 1) {
            this->removeFromParent();
            return;
        }
    }), NULL)));
}

bool UILoading::isShow()
{
    auto node = SceneManager::getInstance()->getCurScene();
    if (node == nullptr) {
        return false;
    }
    auto loader= node->getChildByName("UILoading");
    if (loader) {
        if (loader->isVisible()) {
            return true;
        }
    }
    return false;
}

void UILoading::createFrogLater(float delay)
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
