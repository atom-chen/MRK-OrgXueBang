//
//  FacadeManager.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/29.
//

#include "FacadeManager.hpp"

FacadeManager* FacadeManager::instance = nullptr;
FacadeManager* FacadeManager::getInstance()
{
    if(instance == nullptr){
        instance = new FacadeManager();
    }
    return instance;
}
void FacadeManager::destroyInstance()
{
    delete instance;
    instance = nullptr;
}
FacadeManager::FacadeManager()
{
    
}
FacadeManager::~FacadeManager()
{
    HomePageManager::destroyInstance();
    UserManager::destroyInstance();
    GlobalManager::destroyInstance();
    Reador::destroyInstance();
}
HomePageManager* FacadeManager::getHomePageManager()
{
    return HomePageManager::getInstance();
}
UserManager* FacadeManager::getUserManager()
{
    return UserManager::getInstance();
}

GlobalManager* FacadeManager::getGlobalManager()
{
    return GlobalManager::getInstance();
}

Reador* FacadeManager::getReadorManager()
{
    return Reador::getInstance();
}
