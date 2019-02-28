//
//  UIModifyHead.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#ifndef UIModifyHead_hpp
#define UIModifyHead_hpp

#include "stdafx.h"

class UIModifyHead : public Layer , public BaseKeyListenerDoRemove
{
public:
    CREATE_FUNC(UIModifyHead);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
    void touchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
private:
    void initUI();
    void btnClickHandle(Ref* pSender);
    void onRevMsgDoHandle(Ref* ref);
    void selectedItam(int indexTarget);
private:
    std::unordered_map<string, Widget*> mapWidget;
    int iTarget;
};

#endif /* Login_hpp */
