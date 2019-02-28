//
//  FacadeManager.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/29.
//

#ifndef FacadeManager_hpp
#define FacadeManager_hpp

#include "HomePageManager.hpp"
#include "UserManager.hpp"
#include "GlobalManager.hpp"
#include "Reador.hpp"

class FacadeManager : public cocos2d::Ref
{
public:
    static FacadeManager* getInstance();
    static void destroyInstance();
    HomePageManager* getHomePageManager();
    UserManager* getUserManager();
    GlobalManager* getGlobalManager();
    Reador* getReadorManager();
private:
    FacadeManager();
    ~FacadeManager();
private:
    static FacadeManager* instance;
};


#endif /* FacadeManager_hpp */
