//
//  TestTip.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/8.
//
//

#include "TestTip.hpp"

TestTip::TestTip()
{
    
}

TestTip::~TestTip()
{
    
}

void TestTip::onEnter()
{
    TipLayer::onEnter();
}

void TestTip::onExit()
{
    TipLayer::onExit();
}

bool TestTip::init()
{
    if (TipLayer::init())
    {
//        this->load(UI_layer_tip);
        return true;
    }
    
    return false;
}

void TestTip::addEvents()
{
    //关联基本Panel
    Widget* panel_root = (Widget*)getRootWidget();
    Widget* panel_left = (Widget*)panel_root->getChildByName("panel_left");
    Widget* panel_right = (Widget*)panel_root->getChildByName("panel_right");
    Widget* panel_top = (Widget*)panel_root->getChildByName("panel_top");
    Widget* panel_bottom = (Widget*)panel_root->getChildByName("panel_bottom");
    Widget* panel_center = (Widget*)panel_root->getChildByName("panel_center");
    Widget* panel_popup = (Widget*)panel_root->getChildByName("panel_popup");
    
    //隐藏无用Panel
    panel_top->setVisible(false);
    panel_left->setVisible(false);
    panel_right->setVisible(false);
    panel_bottom->setVisible(false);
    panel_center->setVisible(false);
    panel_popup->setVisible(true);
    
    Button* btn_close = (Button*)panel_popup->getChildByName("btn_close");
    btn_close->setActionTag(eBtnClose);
    btn_close->addTouchEventListener(CC_CALLBACK_2(TestTip::touchEvent, this));
    
}

void TestTip::touchEvent(Ref *sender, Widget::TouchEventType type)
{
    CLICK_FILTER(sender, type);
    ButtonType btnType = (ButtonType)(dynamic_cast<Widget*>(sender))->getActionTag();
    switch (btnType)
    {
        case eBtnClose:
            closeTip();
            break;
            
        default:
            break;
    }
}

void TestTip::refreshUIView()
{
    
}
