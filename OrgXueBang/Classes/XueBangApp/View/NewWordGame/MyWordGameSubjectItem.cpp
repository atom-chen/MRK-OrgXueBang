//
//  MyWordGameSubjectItem.cpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/17.
//

#include "MyWordGameSubjectItem.hpp"
#include "GameMacro.hpp"
#include "MyWordGame.hpp"

#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
using namespace cocos2d::ui;

MyWordGameSubjectItem::MyWordGameSubjectItem():
m_clicked(false),
m_rightId(0),
m_rightDescStr(""),
m_tableView(nullptr),
m_item(nullptr),
m_callback(nullptr)
{
    m_subjectList.clear();
}

void MyWordGameSubjectItem::createTable(Node* parent)
{
    m_item = dynamic_cast<Widget*>(CSLoader::createNode("View/NewWordGame/node_wordgame_board.csb")->getChildByName("panel_subjectItem"));
    m_item->retain();
    
    m_tableView = new (std::nothrow) TableView();
    m_tableView->initWithViewSize(parent->getContentSize());
    m_tableView->autorelease();
    m_tableView->setDataSource(this);
    m_tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    parent->addChild(m_tableView);
}

TableViewCell* MyWordGameSubjectItem::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto cell = table->cellAtIndex(idx);
    if (!cell) {
        cell = TableViewCell::create();
        
        Widget* item1 = m_item->clone();
        item1->setTag((int)idx + 1000);
        item1->setSwallowTouches(false);
        item1->setPosition(Vec2(0, 0));
        cell->addChild(item1);
        
        auto btn_subject = dynamic_cast<Button*>(item1->getChildByName("btn_subject"));
        btn_subject->setTitleText(m_subjectList[idx]->_desc);
        btn_subject->setSwallowTouches(false);
        UiBase::addClickEventListener(btn_subject, [=](Ref* pSender){
            if (m_clicked) {
                return;
            }
            m_clicked = true;
            
//            if (m_subjectList[idx]->_bRight) {
//                CCLOG("touch id = %d ===== right ======", (int)idx);
//            }else {
//                CCLOG("touch id = %d ===== wrong!!! ======", (int)idx);
//            }
            if ( m_callback ) {
                m_callback(m_rightId, m_rightDescStr, m_subjectList[idx]->_bRight);
            }
        });
    }else {
        Widget* item1 = (Widget*)cell->getChildByTag((int)idx + 1000);
        auto btn_subject = dynamic_cast<Button*>(item1->getChildByName("btn_subject"));
        btn_subject->setTitleText(m_subjectList[idx]->_desc);
    }
    return cell;
}

Size MyWordGameSubjectItem::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return m_item->getContentSize();
}

ssize_t MyWordGameSubjectItem::numberOfCellsInTableView(TableView *table)
{
    return m_subjectList.size();
}

MyWordGameSubjectItem::~MyWordGameSubjectItem()
{
    m_subjectList.clear();
    
    m_item->release();
}

void MyWordGameSubjectItem::reloadData()
{
    if (this->m_tableView) {
        this->m_tableView->reloadData();
    }
}

void MyWordGameSubjectItem::setUnitData(std::vector<GameSubject*>& subjectList)
{
    m_subjectList = subjectList;
    for (int i = 0; i < m_subjectList.size(); i++) {
        if (m_subjectList[i]->_bRight) {
            m_rightId = m_subjectList[i]->_id;
            m_rightDescStr = m_subjectList[i]->_desc;
        }
    }
    m_clicked = false;
}
