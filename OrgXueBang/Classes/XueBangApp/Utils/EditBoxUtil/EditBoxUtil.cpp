//
//  EditBoxUtil.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/2/1.
//

#include "EditBoxUtil.hpp"


EditBox* EditBoxUtil::createEditBox(cocos2d::ui::TextField* oldTf,cocos2d::ui::EditBoxDelegate* pDelegate)
{
    if (oldTf == nullptr) {
        return nullptr;
    }
    cocos2d::ui::Scale9Sprite* sacel9SprY = cocos2d::ui::Scale9Sprite::create("findBtn.png");
    auto nativeKB = cocos2d::ui::EditBox::create(oldTf->getContentSize(), sacel9SprY);
    
    nativeKB->setAnchorPoint(oldTf->getAnchorPoint());
    nativeKB->setPosition(oldTf->getPosition());
    
    nativeKB->setFontName(oldTf->getFontName().c_str());
    nativeKB->setFontSize(oldTf->getFontSize());
    nativeKB->setFontColor(oldTf->getTextColor());
    
    nativeKB->setPlaceholderFont(oldTf->getFontName().c_str(), oldTf->getFontSize());
    nativeKB->setPlaceHolder(oldTf->getPlaceHolder().c_str());
    nativeKB->setPlaceholderFontColor(oldTf->getPlaceHolderColor());
    if(oldTf->getMaxLength() > 0){
        nativeKB->setMaxLength(oldTf->getMaxLength());
    }
    if (oldTf->isPasswordEnabled()) {
        nativeKB->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    }
    
    oldTf->setVisible(false);
    oldTf->getParent()->addChild(nativeKB);
    if(pDelegate){
        nativeKB->setDelegate(pDelegate);
    }
    
    return nativeKB;
}
