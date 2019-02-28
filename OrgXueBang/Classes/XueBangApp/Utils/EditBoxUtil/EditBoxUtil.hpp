//
//  EditBoxUtil.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/2/1.
//

#ifndef EditBoxUtil_hpp
#define EditBoxUtil_hpp

#include "cocos2d.h"
USING_NS_CC;

#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

class EditBoxUtil : public cocos2d::Ref
{
public:
    static cocos2d::ui::EditBox* createEditBox(cocos2d::ui::TextField* oldTf,cocos2d::ui::EditBoxDelegate* pDelegate = nullptr);
    
};

#endif /* EditBoxUtil_hpp */
