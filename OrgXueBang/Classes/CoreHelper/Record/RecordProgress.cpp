//
//  RecordProgress.cpp
//  ReadingFriends
//
//  Created by liuwei on 16/12/15.
//
//

#include "stdafx.h"
#include "RecordProgress.h"
using namespace cocos2d;

#define TAG_GRAY    1132
#define TAG_LIGHT   1133

void CRecordProgress::startRecordProgress(cocos2d::Node* button, const stuJson& json, float scale)
{
    if (!button)
    {
        return;
    }
    
    std::string strGrey = json.getValue("grey_png");
    std::string strLight = json.getValue("light_png");
    auto parent = button->getParent();
    
    
    auto grey = Sprite::create(strGrey);
    if (grey)
    {
        grey->setPosition(button->getPosition());
        grey->setScale(scale);
        parent->addChild(grey, 1, TAG_GRAY);
    }
    
    auto light = Sprite::create(strLight);
    if (light)
    {
        ProgressTimer* timer = ProgressTimer::create(light);
        timer->setType(ProgressTimer::Type::RADIAL);
        timer->setPosition(button->getPosition());
        timer->setScale(scale);
        parent->addChild(timer, 2, TAG_LIGHT);
    }
    

    button->setLocalZOrder(3);
}


void CRecordProgress::inRecordProgress(cocos2d::Node* button, float t)
{
    if (!button)
    {
        return;
    }
    
    auto timer = dynamic_cast<ProgressTimer*>(button->getParent()->getChildByTag(TAG_LIGHT));
    if (timer)
    {
        timer->setPercentage(100*t);
    }
    
}

void CRecordProgress::endRecordProgress(cocos2d::Node* button)
{
    if (!button)
    {
        return;
    }
    
    button->setLocalZOrder(0);
    
    auto parent = button->getParent();
    if (parent->getChildByTag(TAG_GRAY))
    {
        parent->removeChildByTag(TAG_GRAY);
    }
    if (parent->getChildByTag(TAG_LIGHT))
    {
        parent->removeChildByTag(TAG_LIGHT);
    }
}
