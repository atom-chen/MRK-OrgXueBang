//
//  UIReadingFinish.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/25.
//

#ifndef UIReadingFinish_hpp
#define UIReadingFinish_hpp

#include "PageBase.hpp"
#include "stdafx.h"

class UIReadingFinish : public PageBase//BaseLayer
{
public:
    UIReadingFinish();
    virtual ~UIReadingFinish();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
    virtual bool init(Widget* pageUI);
    
    //    CREATE_FUNC(UIReadingFinish);
    static UIReadingFinish* create(Widget* pageUI = nullptr);
    
    virtual void addEvents() override;
private:
    void initUI();
    void btnClickHandle(Ref* pSender);
    void updateUIData(Ref* data);
    void initAnim();
private:
    std::unordered_map<string, Widget*> mapWidget;
};

#endif /* UIReadingFinish_hpp */
