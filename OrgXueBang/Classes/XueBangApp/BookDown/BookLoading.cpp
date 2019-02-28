//
//  BookLoading.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//


#include "HttpTag.h"
#include "UIMainPage.hpp"
#include "BookDownManager.h"
#include "CoreHelperStdafx.h"
#include "BookLoading.hpp"
#include "MessageManager.hpp"



BookLoading::BookLoading()
{
    MessageManager::addObserver(this,CC_CALLMESSAGE_SELECTOR(BookLoading::loadingResult),MSG_BOOK_DOWN_RESULT);
    MessageManager::addObserver(this,CC_CALLMESSAGE_SELECTOR(BookLoading::updateProgress),MSG_BOOK_DOWN_PROGRESS);
}
BookLoading::~BookLoading()
{
     MessageManager::removeObserver(this);
}
BookLoading *BookLoading::create(std::string bookID)
{
    BookLoading *ret = new (std::nothrow) BookLoading();
    ret->m_bookID = bookID;
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}
void BookLoading::onEnter()
{
    Layer::onEnter();
}
void BookLoading::onExit()
{
    Layer::onExit();
}

bool BookLoading::init()
{
    if(!Layer::init()){
        return false;
    }
    this->setName("BookLoading");
    initUI();
    return true;
}

void BookLoading::initUI()
{
    auto mUI = CSLoader::createNode("View/CommView/BookLoading.csb");
    this->addChild(mUI);
    
    vector<string> vecNodeName = {
        "LoadingSlider"
    };
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
}

void BookLoading::updateProgress(Ref* ref)
{
    MessageParam* param = (MessageParam*)ref;
    if (m_bookID!=param->paramStr.at(0)) {
        return;
    }
    float progress = param->paramFloat.at(0);
    //刷新进度条
    Slider* slider =  (Slider*)mapWidget["LoadingSlider"];
    float value = slider->getPercent()>(int)(progress*100)?slider->getPercent():(int)(progress*100);
    slider->setPercent(value);
    
    CCLOG("updateProgress = %f", progress);
}
 void BookLoading::loadingResult(Ref* ref)
{
    MessageParam* param = (MessageParam*)ref;
    if (m_bookID!=param->paramStr.at(0)) {
        return;
    }
    int result = param->paramInt.at(0);
    Slider* slider =  (Slider*)mapWidget["LoadingSlider"];
    slider->setPercent(result?100:slider->getPercent());
    
    CCLOG("loadingResult = %d", result);
}
