//
//  BaseScene.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/11.
//
//

#ifndef BaseScene_hpp
#define BaseScene_hpp

#include "PreIncludes.hpp"
#include "ConstantStdafx.hpp"

class BaseScene : public Scene
{
public:
    BaseScene();
    virtual ~BaseScene();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
    
    void setFixWindow();
    void setStretchWindow();
};

#endif /* BaseScene_hpp */
