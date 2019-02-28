#include "LogoScene.hpp"

#include "stdafx.h"
#include "LogoLayerV2.hpp"

void LogoScene::onEnter()
{
//    BaseScene::onEnter();
    Scene::onEnter();
}

void LogoScene::onExit()
{
//    BaseScene::onExit();
    Scene::onExit();
}

bool LogoScene::init()
{
    if (!Scene::init())
//    if (!BaseScene::init())
    {
        return false;
    }
    
    auto layer = LogoLayerV2::create();
    this->addChild(layer);
    
    return true;
}
