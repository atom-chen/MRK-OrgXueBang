//
//  MyWordTableByLetter.cpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/12.
//

#include "MyWordTableByLetter.hpp"
#include "GameMacro.hpp"
#include "MyWordTableByLetterChild.hpp"
#include "MyWord.hpp"

#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
using namespace cocos2d::ui;

#define NEWWORD_UNITWIDTH 80
#define NEWWORD_UNITHEIGHT 84

MyWordTableByLetter::MyWordTableByLetter():
m_showUnit(0),
m_bTouched(true),
m_tableView(nullptr),
m_item(nullptr)
{
    
}
void MyWordTableByLetter::createTable(Node* parent)
{
    m_item = dynamic_cast<Widget*>(CSLoader::createNode("View/NewWordBook/node_myWordUnit.csb")->getChildByName("panel_unit"));
    m_item->retain();
    
    m_tableView = new (std::nothrow) TableView();
    m_tableView->initWithViewSize(parent->getContentSize());
    m_tableView->autorelease();
    m_tableView->setDataSource(this);
    m_tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    m_tableView->setCascadeOpacityEnabled(true);
    parent->addChild(m_tableView);
    
    auto panel_empty = m_tableView->getParent()->getParent()->getChildByName("panel_empty");
    panel_empty->setVisible(false);
    
    Layout* panel_tableChild = dynamic_cast<Layout*>(parent->getChildByName("panel_table_child"));
    auto tableChild = new MyWordTableByLetterChild();
    tableChild->createTable(panel_tableChild);
    m_wordTableChild = tableChild;
}

MyWordTableByLetter::~MyWordTableByLetter()
{
    for (int i = 0; i < m_unitList.size(); i++) {
        m_unitList[i]->wordList.clear();
        delete m_unitList[i];
    }
    m_unitList.clear();
    m_item->release();
}

void MyWordTableByLetter::setTableTouched(bool b)
{
    m_bTouched = b;
    if (this->m_tableView) {
        this->m_tableView->setTouchEnabled(b);
    }
    
    auto panel_empty = m_tableView->getParent()->getParent()->getChildByName("panel_empty");
    if (!m_bTouched) {
        panel_empty->setVisible(false);
        setCellSelectVisible(m_tableView, m_showUnit, false);
    }else {
        setCellSelectVisible(m_tableView, m_showUnit, true);
        if (nullptr == m_wordTableChild || m_unitList[m_showUnit]->wordList.size() <= 0) {
            panel_empty->setOpacity(0);
            panel_empty->setScale(0.01f);
            panel_empty->setVisible(true);
            auto action1 = ScaleTo::create(0.15, 1.0f);
            auto action2 = FadeTo::create(0.15, 255);
            auto actionSeq = Spawn::create(action1, action2, NULL);
            panel_empty->runAction(actionSeq);
        }else {
            panel_empty->setVisible(false);
        }
    }
}

void MyWordTableByLetter::setUnits(stuWordUnit* sheets[], int n)
{
    for (int i = 0; i < n; i++) {
        m_unitList.push_back(sheets[i]);
    }
}

void MyWordTableByLetter::setCellSelectVisible(TableView *table, int cellIdx, bool isVisible)
{
    auto cell = table->cellAtIndex(cellIdx);
    if ( cell ) {
        Widget* item = (Widget*)cell->getChildByTag(cellIdx + 1000);
        auto img_select = dynamic_cast<ImageView*>(item->getChildByName("img_select"));
        if (isVisible) {
            img_select->setVisible(true);
        }else {
            img_select->setVisible(false);
        }
    }
}

TableViewCell* MyWordTableByLetter::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto cell = table->cellAtIndex(idx);
    if (!cell) {
        cell = TableViewCell::create();
        
        Widget* item1 = m_item->clone();
        item1->setTag((int)idx + 1000);
        item1->setSwallowTouches(false);
        item1->setPosition(Vec2(0, 0));
        cell->addChild(item1);
        
        std::string wordImgStr = StringUtils::format("Image/SubImg/NewWordBook/mywordV2_%s.png", m_unitList[idx]->unit.c_str());
        auto img_word = dynamic_cast<ImageView*>(item1->getChildByName("img_bar"));
        img_word->ignoreContentAdaptWithSize(true);
        img_word->loadTexture(wordImgStr, Widget::TextureResType::PLIST);
        auto img_select = dynamic_cast<ImageView*>(item1->getChildByName("img_select"));
        if (m_showUnit != idx) {
            img_select->setVisible(false);
        }

        item1->setSwallowTouches(false);
        static Vec2 unitPos;
        item1->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType eventType){
            if (!m_bTouched) return;
            
            Node* unit = (Node*)pSender;
            if (eventType == Widget::TouchEventType::BEGAN) {
                unitPos = unit->convertToWorldSpace(Vec2(0, 0));
            } else if (eventType == Widget::TouchEventType::ENDED) {
                Vec2 newPos = unit->convertToWorldSpace(Vec2(0, 0));
                if (std::abs(newPos.x - unitPos.x) < 50 && std::abs(newPos.y - unitPos.y) < 50) {
                    //去掉old cell的选中状态
                    setCellSelectVisible(table, m_showUnit, false);
                    int n = unit->getTag() - 1000;
                    m_showUnit = n;
                    this->onClickUnit(n);
                    setCellSelectVisible(table, m_showUnit, true);
                }
            }
        });

    }else {
        Widget* item1 = (Widget*)cell->getChildByTag((int)idx + 1000);
        auto img_select = dynamic_cast<ImageView*>(item1->getChildByName("img_select"));
        if (m_showUnit != idx) {
            img_select->setVisible(false);
        }
    }
    
    return cell;
}

Size MyWordTableByLetter::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(NEWWORD_UNITWIDTH, NEWWORD_UNITHEIGHT);
}

ssize_t MyWordTableByLetter::numberOfCellsInTableView(TableView *table)
{
    return m_unitList.size();
}

void MyWordTableByLetter::onClickUnit(int n)
{
    auto panel_empty = m_tableView->getParent()->getParent()->getChildByName("panel_empty");
    if (nullptr == m_wordTableChild || m_unitList[n]->wordList.size() <= 0) {
        panel_empty->setOpacity(0);
        panel_empty->setScale(0.01f);
        panel_empty->setVisible(true);
        auto action1 = ScaleTo::create(0.15, 1.0f);
        auto action2 = FadeTo::create(0.15, 255);
        auto actionSeq = Spawn::create(action1, action2, NULL);
        panel_empty->runAction(actionSeq);
    }else {
        panel_empty->setVisible(false);
    }
    
    m_wordTableChild->setUnitData(m_unitList[n]->wordList);
}

void MyWordTableByLetter::reloadData()
{
    if (this->m_tableView) {
        this->m_tableView->reloadData();
    }
}

void MyWordTableByLetter::setUnitData(std::vector<wordStruct*> wordList, int unitId)
{
    if (unitId >= 0 && wordList.size() > 0) {
        m_unitList[unitId]->bHasData = true;
        m_unitList[unitId]->wordList = wordList;
    }
}

void MyWordTableByLetter::showDefaultUnit()
{
    m_showUnit = 0;
    onClickUnit(m_showUnit);
    reloadData();
}
