//
//  MyWordTableByProficiency.cpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/12.
//

#include "GameMacro.hpp"
#include "MyWordTableByProficiency.hpp"
#include "MyWord.hpp"
#include "MyWordOneWord.hpp"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
using namespace cocos2d::ui;

#define WORD_PAGELIMIT 20

#define NEWWORD_UNITHEIGHT 81

void MyWordTableByProficiency::createTable(Node* parent)
{
    m_item = dynamic_cast<Widget*>(CSLoader::createNode("View/NewWordBook/node_myWordItem.csb")->getChildByName("panel_word"));
    m_item->retain();
    
    m_tableView = new (std::nothrow) TableView();
    m_tableView->initWithViewSize(parent->getContentSize());
    m_tableView->autorelease();
    m_tableView->setDataSource(this);
    m_tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    parent->addChild(m_tableView);
    
    auto panel_empty = m_tableView->getParent()->getChildByName("panel_empty");
    panel_empty->setVisible(true);
}

TableViewCell* MyWordTableByProficiency::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto cell = table->cellAtIndex(idx);
    if (!cell) {
        cell = TableViewCell::create();
        
        Widget* item1 = m_item->clone();
        item1->setTag((int)idx + 1000);
        item1->setSwallowTouches(false);
        item1->setPosition(Vec2(750.0f * 0.42f, NEWWORD_UNITHEIGHT * 0.5f - 5));
        cell->addChild(item1);
        
        auto text_word = dynamic_cast<Text*>(item1->getChildByName("text_word"));
        text_word->setString(m_wordList[idx]->word);
        
        auto star1 = dynamic_cast<ImageView*>(item1->getChildByName("img_star1"));
        auto star2 = dynamic_cast<ImageView*>(item1->getChildByName("img_star2"));
        auto star3 = dynamic_cast<ImageView*>(item1->getChildByName("img_star3"));
        if (m_wordList[idx]->level <= 0) {
            star1->setVisible(false);
            star2->setVisible(false);
            star3->setVisible(false);
        }else if (m_wordList[idx]->level <= 1) {
            star2->setVisible(false);
            star3->setVisible(false);
        }else if (m_wordList[idx]->level <= 2) {
            star3->setVisible(false);
        }
        auto Image_bg = dynamic_cast<ImageView*>(item1->getChildByName("Image_1"));
        Image_bg->setTag((int)idx);
        Image_bg->setSwallowTouches(false);
        Image_bg->addTouchEventListener(CC_CALLBACK_2(MyWordTableByProficiency::touchEvent, this));
    }else {
        Widget* item1 = (Widget*)cell->getChildByTag((int)idx + 1000);
        auto text_word = dynamic_cast<Text*>(item1->getChildByName("text_word"));
        text_word->setString(m_wordList[idx]->word);
        
        auto star1 = dynamic_cast<ImageView*>(item1->getChildByName("img_star1"));
        auto star2 = dynamic_cast<ImageView*>(item1->getChildByName("img_star2"));
        auto star3 = dynamic_cast<ImageView*>(item1->getChildByName("img_star3"));
        if (m_wordList[idx]->level <= 0) {
            star1->setVisible(false);
            star2->setVisible(false);
            star3->setVisible(false);
        }else if (m_wordList[idx]->level <= 1) {
            star2->setVisible(false);
            star3->setVisible(false);
        }else if (m_wordList[idx]->level <= 2) {
            star3->setVisible(false);
        }
    }
    return cell;
}

void MyWordTableByProficiency::nodeSelectAnimation(Node* node, bool bSelect)
{
    int tag = 999;
    if (!node) return;
    
    node->stopActionByTag(tag);
    if (bSelect) {
        auto action = EaseElasticOut::create(ScaleTo::create(0.9f, 0.96), 0.22f);
        action->setTag(tag);
        node->runAction(action);
    }else {
        auto action = EaseElasticOut::create(ScaleTo::create(0.9f, 1.0), 0.22f);
        action->setTag(tag);
        node->runAction(action);
    }
}

void MyWordTableByProficiency::touchEvent(Ref* pSender, Widget::TouchEventType type)
{
    auto item = (Node*)pSender;
    if (type == Widget::TouchEventType::BEGAN) {
        this->m_clickPos = item->convertToWorldSpace(Vec2(0, 0));
        nodeSelectAnimation(item->getParent(), true);
    }else if (type == Widget::TouchEventType::MOVED) {
        Vec2 touchPos = item->convertToWorldSpace(Vec2(0, 0)); //item->getParent()->convertTouchToNodeSpace(item);
        if (fabs(touchPos.y - m_clickPos.y) >=  50.0f) {
            nodeSelectAnimation(item->getParent(), false);
        }
    }else if (type == Widget::TouchEventType::ENDED) {
        Vec2 newPos = item->convertToWorldSpace(Vec2(0, 0));
        
        if (std::abs(newPos.x - this->m_clickPos.x) < 50 && std::abs(newPos.y - this->m_clickPos.y) < 50) {
            int idx = item->getTag();
//            Node* parentNode = UiBase::getInstance()->getRunningScene();
            Node* parentNode = SceneManager::getInstance()->getCurScene();
            MyWordOneWord::create(parentNode, m_wordList[idx]->word, m_wordList[idx]->audio);
        }
        nodeSelectAnimation(item->getParent(), false);
    }else if (type == Widget::TouchEventType::CANCELED) {
        nodeSelectAnimation(item->getParent(), false);
    }
}

Size MyWordTableByProficiency::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(NEWWORD_UNITHEIGHT, NEWWORD_UNITHEIGHT);
}

ssize_t MyWordTableByProficiency::numberOfCellsInTableView(TableView *table)
{
    return m_wordList.size();
}

MyWordTableByProficiency::~MyWordTableByProficiency()
{
    m_wordList.clear();
    
    m_item->release();
}

void MyWordTableByProficiency::reloadData()
{
    if (this->m_tableView) {
        this->m_tableView->reloadData();
    }
}

void MyWordTableByProficiency::setUnitData(std::vector<wordStruct*>& wordList)
{
    m_wordList = wordList;
    if (m_wordList.size() > 0) {
        auto panel_empty = m_tableView->getParent()->getChildByName("panel_empty");
        panel_empty->setVisible(false);
    }
}
