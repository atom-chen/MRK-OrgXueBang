//
//  UILogin.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#ifndef UILogin_hpp
#define UILogin_hpp

#include "stdafx.h"

class UILogin : public Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(UILogin);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)override;
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)override;
    virtual void editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, cocos2d::ui::EditBoxDelegate::EditBoxEndAction action)override;
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox)override;
private:
    UILogin();
    ~UILogin();
private:
    void initUI();
    void btnClickHandle(Ref* pSender);
    void onRevMsgDoHandle(Ref* ref);
private:
    std::unordered_map<string, Widget*> mapWidget;
    
    cocos2d::ui::EditBox* ebAccount;
    cocos2d::ui::EditBox* ebPassword;
};

#endif /* UILogin_hpp */
