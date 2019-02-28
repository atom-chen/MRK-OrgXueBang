//
//  BtnUtil.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/16.
//

#ifndef BtnUtil_hpp
#define BtnUtil_hpp

#include "cocos2d.h"
USING_NS_CC;

#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

class BtnUtil : public cocos2d::Ref
{
public:
    static void addClickEventListener(cocos2d::ui::Widget* node, const cocos2d::ui::Widget::ccWidgetClickCallback& callback);
    
private:
    static Vec2 s_lastPos;
};


#endif /* BtnUtil_hpp */
