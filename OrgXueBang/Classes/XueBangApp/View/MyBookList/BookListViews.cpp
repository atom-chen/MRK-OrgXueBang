//
//  BookListView.cpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/4/13.
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
#include "BookListManager.hpp"


#include "BookListViews.hpp"

const float NEWBOOK_CELL_COUNT = 3.5;

BookListViews::BookListViews()
{
    
}

BookListViews::~BookListViews()
{
    
}

BookListViews* BookListViews::create(cocos2d::Size size)
{
    BookListViews *pRet = new(std::nothrow) BookListViews();
    if (pRet && pRet->init(size))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}


bool BookListViews::init(cocos2d::Size size)
{
    if (!Node::init())
    {
        return false;
    }
    
    m_size = size;
    
    
    //三个代理
    auto reading = BookListReading::create();
    auto finish = BookListFinish::create();
    auto collect = BookListCollect::create();
    this->addChild(reading);
    this->addChild(finish);
    this->addChild(collect);
    
    //三个tableview
    auto t1 = TableView::create(reading, m_size);
    t1->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    t1->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    t1->setDelegate(reading);
    this->addChild(t1);
    t1->setVisible(false);
    t1->reloadData();
    
    
    auto t2 = TableView::create(finish, m_size);
    t2->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    t2->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    t2->setDelegate(finish);
    this->addChild(t2);
    t2->setVisible(false);
    t2->reloadData();
    
    
    auto t3 = TableView::create(collect, m_size);
    t3->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    t3->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    t3->setDelegate(collect);
    this->addChild(t3);
    t3->setVisible(false);
    t3->reloadData();
    
    m_mapViews[BookListStatus::reading] = t1;
    m_mapViews[BookListStatus::finish] = t2;
    m_mapViews[BookListStatus::collect] = t3;
    
    
    return true;
}



void BookListViews::onEnter()
{
    Node::onEnter();
}


Size BookListViews::getTableViewSize()
{
    return m_size;
}


void BookListViews::showView(BookListStatus status)
{
    for (auto & it : m_mapViews)
    {
        it.second->setVisible(false);
    }
    if (m_mapViews.find(status) != m_mapViews.end())
    {
        m_mapViews[status]->setVisible(true);
    }
}

void BookListViews::refreshView(BookListStatus status)
{
    if (m_mapViews.find(status) != m_mapViews.end())
    {
        auto tableview = dynamic_cast<TableView*>( m_mapViews[status]);

        Vec2 oldOff = tableview->getContentOffset();
        Size oldSize = tableview->getContentSize();

        tableview->reloadData();

        Size newSize = tableview->getContentSize();
        oldOff.y -= newSize.height - oldSize.height;
        tableview->setContentOffset(oldOff);
    }
}






///BookListReading
void BookListReading::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    if (view->getContentOffset().y > 50)
    {
        BookListManager::getInstance()->requestReadingData();
    }
}

void BookListReading::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{
}

void BookListReading::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
}

cocos2d::Size BookListReading::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    auto size = dynamic_cast<BookListViews*>(this->getParent())->getTableViewSize();
    return Size(size.width, size.height/NEWBOOK_CELL_COUNT);
}

cocos2d::extension::TableViewCell* BookListReading::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = BookListCell::create();
        cell->setContentSize(tableCellSizeForIndex(table,idx));
    }
    

    auto data = BookListManager::getInstance()->getReadingData();
    dynamic_cast<BookListCell*>(cell)->updateCell(data[idx]);
    
    return cell;
}

ssize_t BookListReading::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return BookListManager::getInstance()->getReadingData().size();
}





/// BookListFinish
void BookListFinish::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    if (view->getContentOffset().y > 50)
    {
        BookListManager::getInstance()->requestFinishData();
    }
}

void BookListFinish::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{
}

void BookListFinish::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
}

cocos2d::Size BookListFinish::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    auto size = dynamic_cast<BookListViews*>(this->getParent())->getTableViewSize();
    return Size(size.width, size.height/NEWBOOK_CELL_COUNT);
}

cocos2d::extension::TableViewCell* BookListFinish::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = BookListCell::create();
        cell->setContentSize(tableCellSizeForIndex(table,idx));
    }

    
    auto data = BookListManager::getInstance()->getFinishData();
    dynamic_cast<BookListCell*>(cell)->updateCell(data[idx]);
    
    return cell;
}

ssize_t BookListFinish::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return BookListManager::getInstance()->getFinishData().size();
}




///BookListCollect
void BookListCollect::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    if (view->getContentOffset().y > 50)
    {
        BookListManager::getInstance()->requestCollectData();
    }
}

void BookListCollect::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{
}

void BookListCollect::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
}

cocos2d::Size BookListCollect::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    auto size = dynamic_cast<BookListViews*>(this->getParent())->getTableViewSize();
    return Size(size.width, size.height/NEWBOOK_CELL_COUNT);
}

cocos2d::extension::TableViewCell* BookListCollect::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = BookListCell::create();
        cell->setContentSize(tableCellSizeForIndex(table,idx));
    }

    
    auto data = BookListManager::getInstance()->getCollectData();
    dynamic_cast<BookListCell*>(cell)->updateCell(data[idx]);
    
    return cell;
}

ssize_t BookListCollect::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return BookListManager::getInstance()->getCollectData().size();
}
