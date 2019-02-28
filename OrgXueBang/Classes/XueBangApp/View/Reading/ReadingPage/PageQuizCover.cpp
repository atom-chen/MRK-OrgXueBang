//
//  PageQuizCover.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#include "PageQuizCover.hpp"
//#include "BookBody.hpp"
#include "FileLog.h"


PageQuizCover::PageQuizCover()
:m_bCanNotify(true)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("img/transit.plist");
    DownImg::getInstance()->registCallback(this);
    m_pageSize = Director::getInstance()->getVisibleSize();
}

PageQuizCover::~PageQuizCover()
{
    DownImg::getInstance()->unregistCallback(this);
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("img/transit.plist");
}

void PageQuizCover::onEnter()
{
    PageBase::onEnter();
}

void PageQuizCover::onExit()
{
    PageBase::onExit();
}

PageQuizCover* PageQuizCover::create()
{
    PageQuizCover *ret = new (std::nothrow) PageQuizCover();
    
    auto mUI = CSLoader::createNode("View/Reading/BookCover.csb");
    Widget* pageUI = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_quiz_start");
    pageUI->removeFromParent();
    pageUI->retain();
    
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

PageQuizCover* PageQuizCover::create(Widget* pageUI)
{
    PageQuizCover *ret = new (std::nothrow) PageQuizCover();
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

bool PageQuizCover::init(Widget* pageUI)
{
    if (!PageBase::init())
    {
        return false;
    }
    
    this->load(pageUI);
    
    return true;
}

void PageQuizCover::addEvents()
{
    txt_content = (Text*)m_pageWidget->getChildByName("txt_content");
    
    Button* btn_left = (Button*)m_pageWidget->getChildByName("btn_left");
    btn_left->setActionTag(eBtnLeft);
    btn_left->addTouchEventListener(CC_CALLBACK_2(PageQuizCover::touchEvent, this));
    
    Button* btn_right = (Button*)m_pageWidget->getChildByName("btn_right");
    btn_right->setActionTag(eBtnRight);
    btn_right->addTouchEventListener(CC_CALLBACK_2(PageQuizCover::touchEvent, this));
    
    SkeletonAnimation* skeleton_msg = (SkeletonAnimation*)m_pageWidget->getChildByTag(eSklMsg);
    if (!skeleton_msg) {
        skeleton_msg = spine::SkeletonAnimation::createWithJsonFile("spine/quiz.json", "spine/quiz.atlas" ,1.02f);
        skeleton_msg->setTag(eSklMsg);
        //skeleton_msg->setAnimation(0, "quiz", false);
        //skeleton_msg->setTimeScale(0.5);
        skeleton_msg->setPosition(m_pageWidget->getContentSize()/2);
        m_pageWidget->addChild(skeleton_msg, -1);
    }
}

void PageQuizCover::touchEvent(Ref *sender, Widget::TouchEventType type)
{
    CLICK_FILTER(sender, type);
    ButtonType btnType = (ButtonType)(dynamic_cast<Widget*>(sender))->getActionTag();
    CFileLog::LogSaveFormat("===PageQuizCover::touchEvent==%d", (int)btnType);
    switch (btnType)
    {
        case eBtnBack:
        {
            
        }
            break;
        case eBtnLeft:
        {
//            BookBody::turnPreviousPage();
        }
            break;
        case eBtnRight:
        {
//            BookBody::turnNextPage();
        }
            break;

        default:
            break;
    }
}

void PageQuizCover::update(float dt)
{
    
}

void PageQuizCover::loadPage(WholeBook &wholeBook, int index)
{
    PageBase::loadPage(wholeBook, index);
    OnePage* onePage = wholeBook.m_bookPages->at(index);
    txt_content->setString(onePage->m_pageTitle);
}

void PageQuizCover::freePage()
{
    PageBase::freePage();
}

void PageQuizCover::endDownload(string& url, string& file, int iTag, eBookResType resType)
{
#if DOWNLOAD_LOG
    SILog("下载结束", iTag)
    SSLog("下载文件", file.c_str())
    SILog("文件类型", resType)
#endif
    if (m_pageIndex == iTag && resType == eBookResOth) {
        
    }
}

void PageQuizCover::onTurnOut()
{
    PageBase::onTurnOut();
    SkeletonAnimation* skeleton_msg = (SkeletonAnimation*)m_pageWidget->getChildByTag(eSklMsg);
    if (skeleton_msg) {
        skeleton_msg->clearTracks();
    }
    
}


void PageQuizCover::btnBack()
{
    m_bCanNotify = false;
}

void PageQuizCover::onTurnIn()
{
    PageBase::onTurnIn();
    
    SkeletonAnimation* skeleton_msg = (SkeletonAnimation*)m_pageWidget->getChildByTag(eSklMsg);
    skeleton_msg->runAction(Sequence::create(DelayTime::create(0.5),
                                           CallFunc::create([=](){
        skeleton_msg->clearTracks();
        skeleton_msg->setAnimation(0, "quiz", false);
        skeleton_msg->setCompleteListener([=] (spTrackEntry* entry){
            if (m_bCanNotify)
            {
                __NotificationCenter::getInstance()->postNotification(MSG_TURN_PAGE, (Ref*)(intptr_t)(m_pageIndex + 1));
            }
            else
            {
                m_bCanNotify = true;
            }
        });
        SoundManager::getInstance()->playSound("music/snd_qstart.mp3");
    }), NULL));
}
