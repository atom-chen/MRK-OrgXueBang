//
//  BaseScene.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/11.
//
//

#include "BaseScene.hpp"
#include "SceneManager.hpp"

BaseScene::BaseScene()
{
    
}
BaseScene::~BaseScene()
{
    
}
void BaseScene::onEnter()
{
    Scene::onEnter();
}

void BaseScene::onExit()
{
    Scene::onExit();
}

bool BaseScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto node = Layout::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    node->setName(DESIGN_APP_WINDOW_NAME);
    this->addChild(node, 1);
    setFixWindow();
    return true;
}

void BaseScene::setFixWindow()
{
    //    scheduleOnce([=](float dt){
    auto node = this->getChildByName(DESIGN_APP_WINDOW_NAME);
    
    node->setContentSize(Size(DESIGN_RESOLUTION_WIDTH, DESIGN_RESOLUTION_HEIGHT));
    node->setScale(1.0);
    SceneManager::getInstance()->m_curSceneSize = node->getContentSize();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    SceneManager::getInstance()->m_fullSceneSize = visibleSize;
    if (visibleSize.height > DESIGN_RESOLUTION_HEIGHT) {
        float offy = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
        SceneManager::getInstance()->m_topY = DESIGN_RESOLUTION_HEIGHT + offy;
        SceneManager::getInstance()->m_bottomY = -offy;
        //#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        //        if (visibleSize.height == 1624) { //针对 iphone x 上下要留出空间
        //            Game::sharedGame()->m_topY -= 88;
        //            Game::sharedGame()->m_bottomY += 68;
        //        }
        //#endif
    } else {
        SceneManager::getInstance()->m_topY = DESIGN_RESOLUTION_HEIGHT;
        SceneManager::getInstance()->m_bottomY = 0;
    }
//    UiBase::getInstance()->setVisibleHeight(DESIGN_RESOLUTION_HEIGHT);
    //    }, 0.01, "setFixWindow");
}

void BaseScene::setStretchWindow()
{
    //    scheduleOnce([=](float dt){
    auto node = this->getChildByName(DESIGN_APP_WINDOW_NAME);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    node->setContentSize(Size(DESIGN_RESOLUTION_WIDTH, visibleSize.height / visibleSize.width * DESIGN_RESOLUTION_WIDTH));
    node->setScale(visibleSize.width / DESIGN_RESOLUTION_WIDTH);
    SceneManager::getInstance()->m_curSceneSize = node->getContentSize();
    SceneManager::getInstance()->m_fullSceneSize = node->getContentSize();
//    UiBase::getInstance()->setVisibleHeight(node->getContentSize().height);
    //    }, 0.01, "setStretchWindow");
}
