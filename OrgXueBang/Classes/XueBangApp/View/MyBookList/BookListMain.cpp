//
//  BookListMain.cpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/4/9.
//
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "UiBase.h"
#include "UIDef.hpp"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
#include "BookListCell.hpp"
#include "BookListViews.hpp"
#include "BookListManager.hpp"
//#include "MainPage.h"


#include "BookListMain.hpp"
#include "TipManager.hpp"

BookListMain::BookListMain()
:m_status(BookListStatus::none)
{
    BookListManager::getInstance()->clearData();
    talkingInterface::pageBegin("我的书单");
}
BookListMain::~BookListMain()
{
    BookListManager::getInstance()->clearData();
    BookListManager::delInstance();
    talkingInterface::pageEnd("我的书单");
    switch (m_status) {
        case BookListStatus::collect:
            talkingInterface::pageEnd("我的书单收藏");
            break;
        case BookListStatus::finish:
            talkingInterface::pageEnd("我的书单已读");
            break;
        default:
            talkingInterface::pageEnd("我的书单在读");
            break;
    }
}




bool BookListMain::init()
{
    if (!Layer::init())
    {
        return false;
    }
    this->setName("BookListMain");
    m_winSize = Director::getInstance()->getWinSize();
    
    //UI
    _initUI();

    
    //初始化是reading状态
    _changeStatus(BookListStatus::reading);
    
    //三个数据全要
    BookListManager::getInstance()->requestFinishData();
    BookListManager::getInstance()->requestReadingData();
    BookListManager::getInstance()->requestCollectData();
    
    
    return true;
}

void BookListMain::onEnter()
{
    Layer::onEnter();
}



//此处放到_initUI里, 因为看书退出不要再走一遍 onEnter
void BookListMain::_initUI()
{
    this->removeAllChildren();
    m_mapUI.clear();
    
    
    auto node = CSLoader::createNodeWithVisibleSize("View/MyBookList/booklistMain.csb");
    this->addChild(node);
    auto root = dynamic_cast<Widget*>(node->getChildByName("Root"));
    

    auto btn_back = dynamic_cast<Widget*>(root->getChildByName("title")->getChildByName("panel_top")->getChildByName("btn_back"));
    UiBase::addClickEventListener(btn_back, [=](Ref* pSender){
        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
        this->removeFromParent();
    });
    
    auto title_name = dynamic_cast<Text*>(root->getChildByName("title")->getChildByName("panel_top")->getChildByName("title_name"));
    title_name->setString("我的书单");
    
    
    
    auto reading = dynamic_cast<Button*>(Helper::seekWidgetByName(root, "reading"));
    UiBase::addClickEventListener(reading, [&](Ref* pSender){
        log("reading");
        _changeStatus(BookListStatus::reading);
        talkingInterface::traceEvent("我的书单", "在读");
        int count = (int)BookListManager::getInstance()->getReadingData().size();
        if (count <= 0) {
            TipManager::getInstance()->showMsgTips("无在读书本信息！");
        }
    });
    

    
    auto collect = dynamic_cast<Button*>(Helper::seekWidgetByName(root, "collect"));
    UiBase::addClickEventListener(collect, [&](Ref* pSender){
        log("collect");
        _changeStatus(BookListStatus::collect);
        talkingInterface::traceEvent("我的书单", "收藏");
        int count = (int)BookListManager::getInstance()->getCollectData().size();
        if (count <= 0) {
            TipManager::getInstance()->showMsgTips("无收藏书本信息！");
        }
    });
    
    
    auto finish = dynamic_cast<Button*>(Helper::seekWidgetByName(root, "finish"));
    UiBase::addClickEventListener(finish, [&](Ref* pSender){
        log("finish");
        _changeStatus(BookListStatus::finish);
        talkingInterface::traceEvent("我的书单", "已读");
        int count = (int)BookListManager::getInstance()->getFinishData().size();
        if (count <= 0) {
            TipManager::getInstance()->showMsgTips("无已读书本信息！");
        }
    });
    

    auto table = Helper::seekWidgetByName(root, "tableview");
    auto listviews = BookListViews::create(table->getContentSize());
    table->addChild(listviews);
    BookListManager::getInstance()->setListViews(listviews);
    
        
    m_mapUI["reading"] = reading;
    m_mapUI["collect"] = collect;
    m_mapUI["finish"] = finish;
    m_mapUI["views"] = listviews;
}


void BookListMain::_changeStatus(BookListStatus status)
{
    if (m_status == status)
    {
        return;
    }
    switch (m_status) {
        case BookListStatus::collect:
            talkingInterface::pageEnd("我的书单收藏");
            break;
        case BookListStatus::finish:
            talkingInterface::pageEnd("我的书单已读");
            break;
        default:
            talkingInterface::pageEnd("我的书单在读");
            break;
    }
    m_status = status;
    BookListManager::getInstance()->setStatus(m_status);
    
    switch (m_status) {
        case BookListStatus::collect:
            talkingInterface::pageBegin("我的书单收藏");
            break;
        case BookListStatus::finish:
            talkingInterface::pageBegin("我的书单已读");
            break;
        default:
            talkingInterface::pageBegin("我的书单在读");
            break;
    }
    
    //UI
    auto reading = dynamic_cast<Button*>(m_mapUI["reading"]);
    auto collect = dynamic_cast<Button*>(m_mapUI["collect"]);
    auto finish = dynamic_cast<Button*>(m_mapUI["finish"]);
    
    
    reading->loadTextureNormal("Image/SubImg/MyBookList/booklist_reading2.png", Widget::TextureResType::PLIST);
    collect->loadTextureNormal("Image/SubImg/MyBookList/booklist_collect2.png",  Widget::TextureResType::PLIST);
    finish->loadTextureNormal("Image/SubImg/MyBookList/booklist_finish2.png",  Widget::TextureResType::PLIST);
    reading->getChildByName("select")->setVisible(false);
    collect->getChildByName("select")->setVisible(false);
    finish->getChildByName("select")->setVisible(false);

    if (status == BookListStatus::reading){
        
        reading->loadTextureNormal("Image/SubImg/MyBookList/booklist_reading1.png", Widget::TextureResType::PLIST);
        reading->getChildByName("select")->setVisible(true);
        
    }  else if (status == BookListStatus::collect){
        
        collect->loadTextureNormal("Image/SubImg/MyBookList/booklist_collect1.png",  Widget::TextureResType::PLIST);
        collect->getChildByName("select")->setVisible(true);
        
    }  else if (status == BookListStatus::finish){
        
        finish->loadTextureNormal("Image/SubImg/MyBookList/booklist_finish1.png",  Widget::TextureResType::PLIST);
        finish->getChildByName("select")->setVisible(true);
    }
    
    //tableview
    auto views = dynamic_cast<BookListViews*>(m_mapUI["views"]);
    views->showView(status);
}
