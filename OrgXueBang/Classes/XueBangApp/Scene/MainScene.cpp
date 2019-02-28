//
//  MainScene.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#include "MainScene.hpp"

#include "UILogin.hpp"
#include "UIUserInfo.hpp"
#include "UIMainPage.hpp"
#include "UserManager.hpp"

void MainScene::onEnter()
{
    Scene::onEnter();
}

void MainScene::onExit()
{
    Scene::onExit();
}

bool MainScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    Node* layer;
    if(UserManager::getInstance()->getUserModel()->isLogined()){
        layer = UIMainPage::create();
    }else{
        layer = UILogin::create();
    }
    this->addChild(layer);
    return true;
}
