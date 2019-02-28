//
//  BookPageContentBG.cpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/11/2.
//

#include "BookPageContentBG.hpp"

BookPageContentBG::BookPageContentBG()
{
    
}
BookPageContentBG::~BookPageContentBG()
{
    
}
void BookPageContentBG::onEnter()
{
    Layer::onEnter();
}
void BookPageContentBG::onExit()
{
    Layer::onExit();
}
bool BookPageContentBG::init()
{
    if (!Layer::init())
    {
        return false;
    }
    initUI();
    return true;
}

void BookPageContentBG::initUI()
{
    auto mUI = CSLoader::createNode("View/Reading/BookPageContentBG.csb");
    mUI->setAnchorPoint(Vec2(0.5,0.5));
    mUI->setPosition(Vec2::ZERO);
    this->addChild(mUI);
}
