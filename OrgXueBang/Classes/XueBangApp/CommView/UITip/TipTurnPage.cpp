//
//  TipTurnPage.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/15.
//
//

#include "TipTurnPage.hpp"

TipTurnPage::TipTurnPage()
{
    
}

TipTurnPage::~TipTurnPage()
{
    
}

void TipTurnPage::onEnter()
{
    TipLayer::onEnter();
}

void TipTurnPage::onExit()
{
    TipLayer::onExit();
}

bool TipTurnPage::init()
{
    if (TipLayer::init())
    {
//        this->load(UI_layer_tip_turnpage);
        return true;
    }
    
    return false;
}

void TipTurnPage::addEvents()
{
    Widget* panel_dark = static_cast<Widget*>(Helper::seekWidgetByName(m_rootWidget, "panel_dark"));
    auto anim_turnpage = SkeletonAnimation::createWithJsonFile("spine/pageturn.json", "spine/pageturn.atlas",1.0f);
    anim_turnpage->setAnimation(0, "animation", true);
    auto panelSize = panel_dark->getContentSize();
    anim_turnpage->setPosition(Vec2(panelSize.width / 2, panelSize.height / 2));
    panel_dark->addChild(anim_turnpage);
}

void TipTurnPage::refreshUIView()
{
    
}
