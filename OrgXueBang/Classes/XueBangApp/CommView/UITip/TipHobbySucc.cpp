//
//  TipHobbySucc.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/15.
//
//  for:笔记成功后的弹窗==对勾+成功

#include "TipHobbySucc.hpp"

TipHobbySucc::TipHobbySucc()
{
    
}

TipHobbySucc::~TipHobbySucc()
{
    
}

void TipHobbySucc::onEnter()
{
    TipLayer::onEnter();
}

void TipHobbySucc::onExit()
{
    TipLayer::onExit();
}

bool TipHobbySucc::init()
{
    if (TipLayer::init())
    {
        this->load("ui/login/layer_tip_hobby.csb");
        return true;
    }
    
    return false;
}

void TipHobbySucc::addEvents()
{
    
}

void TipHobbySucc::refreshUIView()
{
    
}

bool TipHobbySucc::OnBaseKeyBack()
{
    this->closeTip();
    return true;
}
