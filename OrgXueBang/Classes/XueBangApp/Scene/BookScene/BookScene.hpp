//
//  BookScene.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/11.
//
//

#ifndef BookScene_hpp
#define BookScene_hpp

#include "SceneManager.hpp"
#include "BaseScene.hpp"
#include "BaseKeyEvent.h"

class BookScene : public BaseScene,public BaseKeyListener
{
public:
    BookScene();
    virtual ~BookScene();
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual bool init() override;
    
    CREATE_FUNC(BookScene);
    
    virtual bool OnBaseKeyBack() override;
private:
    
};

#endif /* BookScene_hpp */
