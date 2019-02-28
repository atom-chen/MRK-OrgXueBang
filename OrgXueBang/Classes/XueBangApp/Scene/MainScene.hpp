//
//  MainScene.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "PreIncludes.hpp"
#include "BaseScene.hpp"

class MainScene : public Scene //: public BaseScene
{
public:
    CREATE_FUNC(MainScene);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
};

#endif /* MainScene_hpp */
