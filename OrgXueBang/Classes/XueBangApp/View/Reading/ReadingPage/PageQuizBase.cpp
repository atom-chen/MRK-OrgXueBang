//
//  PageQuizBase.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#include "PageQuizBase.hpp"
#include "TipManager.hpp"


PageQuizBase::PageQuizBase()
:m_quizType(eQuizNone)
{
    
}

PageQuizBase::~PageQuizBase()
{
    
}

void PageQuizBase::onEnter()
{
    PageBase::onEnter();
}

void PageQuizBase::onExit()
{
    PageBase::onExit();
}

PageQuizBase* PageQuizBase::create(Widget* pageUI)
{
    PageQuizBase *ret = new (std::nothrow) PageQuizBase();
    if (ret && ret->init(pageUI))
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

bool PageQuizBase::init(Widget* pageUI)
{
    if (!PageBase::init())
    {
        return false;
    }
    
    this->load(pageUI);
    
    return true;
}

void PageQuizBase::addEvents()
{
    
}

void PageQuizBase::update(float dt)
{
    
}

void PageQuizBase::loadPage(WholeBook &wholeBook, int index)
{
    PageBase::loadPage(wholeBook, index);
}

void PageQuizBase::freePage()
{
    PageBase::freePage();
}

int PageQuizBase::playAudio(std::function<void (int, const std::string &)> finishCall)
{
    return PageBase::playAudio(finishCall);
}

void PageQuizBase::stopAudio()
{
    PageBase::stopAudio();
}

void PageQuizBase::setAutoPlay()
{
    PageBase::setAutoPlay();
}
