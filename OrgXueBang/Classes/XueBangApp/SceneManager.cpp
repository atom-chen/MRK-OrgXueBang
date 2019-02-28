//
//  SceneManager.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#include "SceneManager.hpp"

#include "LogoScene.hpp"
#include "MainScene.hpp"
#include "BookScene.hpp"

#include "UILogin.hpp"
#include "baseIdfa.h"

SceneManager::SceneManager()
:m_gameState(eStateNone)
,m_curScene(nullptr)
//,m_designSize(Size(768,1024))
{
    m_fullSceneSize = Director::getInstance()->getVisibleSize();
    
}
SceneManager::~SceneManager()
{
    
}

SceneManager* SceneManager::getInstance()
{
    static SceneManager instance;
    return &instance;
}


void SceneManager::pauseGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Director::getInstance()->pause();
#else
    Director::getInstance()->stopAnimation();
#endif
}

void SceneManager::resumeGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Director::getInstance()->resume();
#else
    Director::getInstance()->startAnimation();
#endif
}

void SceneManager::replaceScene(Scene *scene)
{
    Director::getInstance()->replaceScene(scene);
}

void SceneManager::exitGame()
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

Scene* SceneManager::getScene(eGameState state)
{
    Scene* pScene = NULL;
    switch(state){
        case eStateLogo:
            pScene = LogoScene::create();
            break;
        case eStateBook:
            pScene = BookScene::create();
            break;
        case eStateMain:
            pScene = MainScene::create();
            break;
        default:
            break;
    }
    m_curScene = pScene;
    return pScene;
}

void SceneManager::startGame()
{
//    string logoFile = FileUtils::getInstance()->getWritablePath() + "logo.txt";
//    string ver = FileUtils::getInstance()->getStringFromFile(logoFile);
//    string app = "";
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
//    app = CBaseIdfa::getInstance()->getversion();
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    app = CBaseIdfa::getInstance()->getversion();
//#endif
//
////#if COCOS2D_DEBUG > 0
////    ver = "";
////#endif
//
//    if (ver == app) {
        setGameState(eStateMain);
//    } else {
//        setGameState(eStateLogo);
//        FileUtils::getInstance()->writeStringToFile(app, logoFile);
//    }
}

void SceneManager::showUILogin()
{
    setGameState(eStateMain);
    auto layer = UILogin::create();
    m_curScene->addChild(layer);
}
void SceneManager::setGameState(eGameState state)
{
//    CFileLog::LogSaveFormat("jump to scene from %d to %d",m_gameState,state);
    if(state == m_gameState)
    {
        return;
    }
    switch (state) {
        case eStateLogo:
        {
            auto scene = LogoScene::create();
            m_curScene = scene;
            Director::getInstance()->runWithScene(scene);
        }
            break;
        case eStateMain:
        {
            if (m_gameState == eStateLogo || m_gameState == eStateNone) {
                MainScene* scene = (MainScene*)getScene(eStateMain);
                m_curScene = scene;
                m_mainScene = scene;
                replaceScene(scene);
            } else {
                Director::getInstance()->popScene();
                m_curScene = m_mainScene;
            }
        }
            break;
        case eStateBook:
        {
            BookScene* scene = (BookScene*)getScene(eStateBook);
            m_curScene = scene;
            if (m_gameState == eStateMain) {
                Director::getInstance()->pushScene(scene);
            } else {
                replaceScene(scene);
            }
        }
            break;
            
        default:
            break;
    }
    m_gameState = state;
}
