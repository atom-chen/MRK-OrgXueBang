#ifndef __LogoScene_SCENE_H__
#define __LogoScene_SCENE_H__

#include "PreIncludes.hpp"
#include "BaseScene.hpp"

class LogoScene : public Scene //: public BaseScene
{
    
public:
    CREATE_FUNC(LogoScene);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
};

#endif // __StartScene_SCENE_H__
