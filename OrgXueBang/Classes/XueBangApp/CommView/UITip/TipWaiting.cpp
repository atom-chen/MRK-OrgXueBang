//
//  TipWaiting.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/14.
//
//

#include "TipWaiting.hpp"

static TipWaiting* pLoader = nullptr;


TipWaiting::TipWaiting()
{
    
}

TipWaiting::~TipWaiting()
{
    pLoader = nullptr;
}

void TipWaiting::onEnter()
{
    TipLayer::onEnter();
}

void TipWaiting::onExit()
{
    TipLayer::onExit();
}

bool TipWaiting::init()
{
    if (TipLayer::init())
    {
//        this->load(UI_layer_loader);
        return true;
    }
    
    return false;
}

void TipWaiting::addEvents()
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
    
//    ImageView* img_loader = (ImageView*)panel_popup->getChildByName("img_loader");
//    
//    img_loader->runAction(RepeatForever::create(RotateBy::create(1.0, 360.f)));
    
    this->runAction(Sequence::create(DelayTime::create(0.0), CallFunc::create([=](){
        panel_root->getChildByName("panel_black")->setVisible(true);
        
        auto anim_turnpage = SkeletonAnimation::createWithJsonFile("spine/pageturn.json", "spine/pageturn.atlas", 1.0f);
        anim_turnpage->setAnimation(0, "animation", true);
        //anim_turnpage->setTimeScale(0.5);
        auto panelSize = panel_popup->getContentSize();
        anim_turnpage->setPosition(Vec2(panelSize.width / 2, panelSize.height / 2));
        //anim_turnpage->setScale(0.5);
        panel_popup->addChild(anim_turnpage);
        
    }), NULL));
}

void TipWaiting::refreshUIView()
{
    
}
