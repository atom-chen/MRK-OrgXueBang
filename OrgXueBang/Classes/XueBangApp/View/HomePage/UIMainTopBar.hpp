//
//  UIMainTopBar.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/15.
//

#ifndef UIMainTopBar_hpp
#define UIMainTopBar_hpp

#include "stdafx.h"

class UIMainTopBar : public Node
{
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
    static UIMainTopBar* create(Node* nodeRoot);
    
    UIMainTopBar(Node* nodeRoot);
    ~UIMainTopBar();
private:
    void initUI();
    void initUIData();
    void btnClickHandle(Ref* pSender);
    void onUpdateHeadIcon(Ref* ref);
private:
    std::unordered_map<string, Widget*> mapWidget;
    Node* csbRoot;
};

#endif /* UIMainTopBar_hpp */
