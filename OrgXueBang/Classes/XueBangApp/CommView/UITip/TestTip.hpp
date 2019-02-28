//
//  TestTip.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/8.
//
//

#ifndef TestTip_hpp
#define TestTip_hpp

#include "TipLayer.hpp"

class TestTip : public TipLayer
{
    enum ButtonType
    {
        eBtnClose = 0,
        eBtnSure,
        eBtnCancle,
    };
public:
    TestTip();
    virtual ~TestTip();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void addEvents() override;
    
    CREATE_FUNC(TestTip);
    
    virtual void refreshUIView() override;
    
    void touchEvent(Ref* sender, Widget::TouchEventType type);
    
private:
    Button* btn_close;
};

#endif /* TestTip_hpp */
