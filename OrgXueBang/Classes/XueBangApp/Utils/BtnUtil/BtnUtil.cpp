//
//  BtnUtil.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/16.
//

#include "BtnUtil.hpp"

Vec2 BtnUtil::s_lastPos = Vec2(0,0);
void BtnUtil::addClickEventListener(Widget* node, const Widget::ccWidgetClickCallback &callback)
{
    if(!node){
        return;
    }
    
    int actionTag = 9999;
    float scale = node->getScale();
    node->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType eventType){
        Node* unit = (Node*)pSender;
        if (eventType == Widget::TouchEventType::BEGAN) {
            s_lastPos = unit->getParent()->convertToWorldSpace(unit->getPosition());
            unit->stopActionByTag(actionTag);
            auto a = EaseElasticOut::create(ScaleTo::create(0.9f, scale * 0.9), 0.22f);
            a->setTag(actionTag);
            unit->runAction(a);
        } else if (eventType == Widget::TouchEventType::MOVED) {
            Vec2 newPos = unit->getParent()->convertToWorldSpace(unit->getPosition());
            if (fabs(newPos.y - s_lastPos.y) >=  50.0f || fabs(newPos.x - s_lastPos.x) >=  50.0f) {
                unit->stopActionByTag(actionTag);
                auto a = EaseElasticOut::create(ScaleTo::create(0.9f, scale * 1.0), 0.22f);
                a->setTag(actionTag);
                unit->runAction(a);
            }
        } else if (eventType == Widget::TouchEventType::ENDED) {
            unit->stopActionByTag(actionTag);
            auto a = EaseElasticOut::create(ScaleTo::create(0.9f, scale * 1.0), 0.22f);
            a->setTag(actionTag);
            unit->runAction(a);
            
            Vec2 newPos = unit->getParent()->convertToWorldSpace(unit->getPosition());
            if (abs(s_lastPos.x - newPos.x) < 50 && abs(s_lastPos.y - newPos.y) < 50) {
                callback(pSender);
            }
        } else {
            unit->stopActionByTag(actionTag);
            auto a = EaseElasticOut::create(ScaleTo::create(0.9f, scale * 1.0), 0.22f);
            a->setTag(actionTag);
            unit->runAction(a);
        }
    });
}
