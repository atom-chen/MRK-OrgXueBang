//
//  TipLayer.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/7.
//
//

#include "TipLayer.hpp"
#include "TipManager.hpp"

TipLayer::TipLayer()
{
    m_actionType = eNone;
}

TipLayer::~TipLayer()
{
    
}

void TipLayer::onEnter()
{
    BaseLayer::onEnter();
}

void TipLayer::onExit()
{
    BaseLayer::onExit();
}

void TipLayer::openTip()
{
    inAction(m_actionType);
}

void TipLayer::closeTip()
{
    outAction(m_actionType);
}

void TipLayer::inAction(TipLayer::eActionType action)
{
    switch(m_actionType){
        case eNone:
            // do nothing
            break;
        case eCenterScale:
            inActionCenterScale();
            break;
        case eTopDown:
            inActionTopDown();
            break;
        default:
            break;
    }
}

void TipLayer::outAction(TipLayer::eActionType action)
{
    switch(m_actionType){
        case eNone:
            removeSelfCallFunc();
            break;
        case eCenterScale:
            outActionCenterScale();
            break;
        case eTopDown:
            outActionTopDown();
            break;
        default:
            outActionCenterScale();
            break;
    }
    
}

void TipLayer::removeSelfCallFunc()
{
    TipManager::getInstance()->removeTip(this);
}

void TipLayer::inActionCenterScale()
{
    Widget* panel_popup = (Widget*)m_rootWidget->getChildByName("panel_popup");
    
    panel_popup->setScale(0.001f);
    
    EaseIn* scale1 = EaseIn::create(ScaleTo::create(0.3f,1.00f),0.3f);
    
    ScaleTo* scale2 = ScaleTo::create(0.22f,1.0f);
    panel_popup->runAction(Sequence::create(scale1, scale2, DelayTime::create(0.1f),
                                                CallFunc::create(CC_CALLBACK_0(TipLayer::inActionCallFunc,this)),nullptr));
}

void TipLayer::outActionCenterScale()
{
    Widget* panel_popup = (Widget*)m_rootWidget->getChildByName("panel_popup");
    float scaleFactor = panel_popup->getScale();
    
    ScaleTo* scale1 = ScaleTo::create(0.1f,1.01 * scaleFactor);
    EaseOut* scale2 = EaseOut::create(ScaleTo::create(0.35f,0.85f),0.35f);
    
    panel_popup->runAction(Sequence::create(
                                     scale1,scale2,CallFunc::create(CC_CALLBACK_0(TipLayer::removeSelfCallFunc, this))
                                     ,nullptr
                                     )
                    );
}

void TipLayer::inActionTopDown()
{
    Widget* panel_popup = (Widget*)m_rootWidget->getChildByName("panel_popup");
    panel_popup->runAction(Sequence::create(
                                     EaseElasticOut::create(MoveBy::create(0.5f,Point(0,-SCREEN_HEIGHT)),1.5f)
                                     ,CallFunc::create(CC_CALLBACK_0(TipLayer::inActionCallFunc,this))
                                     ,nullptr
                                     ));
}

void TipLayer::outActionTopDown()
{
    Widget* panel_popup = (Widget*)m_rootWidget->getChildByName("panel_popup");
    panel_popup->runAction(
                    Sequence::create(
                                     EaseElasticIn::create(MoveBy::create(0.5f,Point(0, SCREEN_HEIGHT)),1.5f)
                                     ,CallFunc::create(CC_CALLBACK_0(TipLayer::removeSelfCallFunc, this))
                                     ,nullptr)
                    );
}


void TipLayer::inActionCallFunc()
{
    
}

void TipLayer::onKeyBackClick(){
    closeTip();
}
