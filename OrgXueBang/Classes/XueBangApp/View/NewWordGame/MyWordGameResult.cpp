//
//  MyWordGameResult.cpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/20.
//

#include "MyWordGameResult.hpp"
#include "GameMacro.hpp"
#include "MyWordGame.hpp"
#include "ui/CocosGUI.h"
#include "UiBase.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "UserManager.hpp"
#include "TipManager.hpp"

#define NEWWORD_UNITHEIGHT 81

MyWordGameResult::MyWordGameResult():
m_ui(nullptr),
m_item(nullptr),
m_tableView(nullptr),
m_callback(nullptr)
{
}

MyWordGameResult *MyWordGameResult::create(Node* parent, void *data)
{
    talkingInterface::pageBegin("生词游戏结果");
    
    auto node = parent->getChildByName("MyWordGameResult");
    if (node) {
        return (MyWordGameResult *)node->getChildByName("MyWordGameResult");
    }
    
    MyWordGameResult *ret = new (std::nothrow) MyWordGameResult();
    if (ret && ret->init(data)) {
        parent->addChild(ret->m_ui);
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

MyWordGameResult::~MyWordGameResult()
{
    m_item->release();
    talkingInterface::pageEnd("生词游戏结果");
}

bool MyWordGameResult::OnBaseKeyBack()
{
    this->m_ui->removeFromParent();
    return true;
}

bool MyWordGameResult::init(void *data)
{
    m_ui = CSLoader::createNodeWithVisibleSize("View/NewWordGame/layer_newWordGameResult.csb");
    UiBase::resizeBackImg(m_ui);
    this->setName("MyWordGameResult");
    m_ui->addChild(this, -1);
    m_ui->setName("MyWordGameResult");
    
    initData(data);
    
    Layout* panel_center = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME));
//    m_item = dynamic_cast<Widget*>(CSLoader::createNode("ui/newWordGame/node_wordgame_resultItem.csb")->getChildByName("panel_word"));
    m_item = dynamic_cast<Widget*>(CSLoader::createNode("View/NewWordBook/node_myWordItem.csb")->getChildByName("panel_word"));
    m_item->retain();
    
    m_tableView = new (std::nothrow) TableView();
    m_tableView->initWithViewSize(panel_center->getContentSize());
    m_tableView->autorelease();
    m_tableView->setDataSource(this);
    m_tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    panel_center->addChild(m_tableView);
    
    auto txt_correct_rate = dynamic_cast<Text*>(panel_center->getChildByName("txt_correct_rate"));
    string ratestr = StringUtils::format("正确率：%d", m_data->_correctRate).append("%");
    txt_correct_rate->setString(ratestr);

    auto txt_used_time = dynamic_cast<Text*>(panel_center->getChildByName("txt_used_time"));
    string timeStr = changeToTimeStr(m_data->_usedTime);
    txt_used_time->setString(StringUtils::format("用时%s", timeStr.c_str()));

    auto no_upgrade_bg = panel_center->getChildByName("no_upgrade_bg");
    if (m_data->_upgradWordsList.size() > 0) {
        no_upgrade_bg->setVisible(false);
    }else {
        no_upgrade_bg->setOpacity(0);
        no_upgrade_bg->setVisible(true);
        no_upgrade_bg->setScale(0.01);
        auto s1 = ScaleTo::create(0.12f, 1);
        auto f1 = FadeTo::create(0.12f, 255);
        no_upgrade_bg->runAction(Spawn::create(s1, f1, NULL));
    }
    
    auto btn_replay = dynamic_cast<Button*>(m_ui->getChildByName(BOTTOM_PANEL_NAME)->getChildByName("btn_replay"));
    UiBase::addClickEventListener(btn_replay, [=](Ref* pSender){
        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
        getDataFromSever();
        talkingInterface::traceEvent("生词游戏结果", "再来一次");
    });

    auto btn_return = dynamic_cast<Button*>(m_ui->getChildByName(BOTTOM_PANEL_NAME)->getChildByName("btn_return"));
    UiBase::addClickEventListener(btn_return, [=](Ref* pSender){
        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
        SoundManager::getInstance()->pauseBackSound();
        if (m_callback) {
            m_callback(2, nullptr);
        }
        talkingInterface::traceEvent("生词游戏结果", "返回生词本");
        this->m_ui->removeFromParent();
    });
    
    return true;
}

string MyWordGameResult::changeToTimeStr(long ltime)
{
    int hour = (int)ltime / 3600;
    int min = (int)(ltime % 3600)/60;
    int sec = (int)(ltime % 60);
    
    if (hour > 0) {
        return StringUtils::format("%d小时%d分%02d秒", hour, min, sec);
    }
    return StringUtils::format("%d分%02d秒", min, sec);
}

void MyWordGameResult::initData(void *data)
{
    if (data) {
        m_data = (GameResultData*)data;
    }
}

void MyWordGameResult::reloadData()
{
    if (this->m_tableView) {
        this->m_tableView->reloadData();
    }
}

void MyWordGameResult::starAnimation(int idx, int level, Node* star1, Node* star2, Node* star3)
{
    auto f1 = FadeTo::create(0.24, 255);
    auto s1 = ScaleTo::create(0.24, 3.0f);
    auto s2 = ScaleTo::create(0.12, 1.0f);
    
    auto call = CallFunc::create([=](){
        SoundManager::getInstance()->playSoundEffect("music/snd_qstart.mp3");
    });
    float dltime = idx * 0.1f + 0.5f;
    if (level == 1) {
        star1->setOpacity(0);
        star1->setScale(0.01);
        
        
        star1->runAction(Sequence::create(DelayTime::create(dltime), call, Spawn::create(f1->clone(), s1->clone(), NULL), s2->clone(), NULL));
        
        star2->setVisible(false);
        star3->setVisible(false);
    }else if (level == 2) {
        star2->setOpacity(0);
        star2->setScale(0.01);
        star2->runAction(Sequence::create(DelayTime::create(dltime), call, Spawn::create(f1->clone(), s1->clone(), NULL), s2->clone(), NULL));
        
        star3->setVisible(false);
    }else if (level == 3) {
        star3->setOpacity(0);
        star3->setScale(0.01);
        star3->runAction(Sequence::create(DelayTime::create(dltime), call, Spawn::create(f1->clone(), s1->clone(), NULL), s2->clone(), NULL));
    }else {
        star1->setVisible(false);
        star2->setVisible(false);
        star3->setVisible(false);
    }
}

TableViewCell* MyWordGameResult::tableCellAtIndex(TableView *table, ssize_t idx)
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
        text_word->setString(m_data->_upgradWordsList[idx]->_word);
        
        auto star1 = dynamic_cast<ImageView*>(item1->getChildByName("img_star1"));
        auto star2 = dynamic_cast<ImageView*>(item1->getChildByName("img_star2"));
        auto star3 = dynamic_cast<ImageView*>(item1->getChildByName("img_star3"));
        starAnimation((int)idx, m_data->_upgradWordsList[idx]->_skillLevel, star1, star2, star3);
        
        auto Image_bg = dynamic_cast<ImageView*>(item1->getChildByName("Image_1"));
        Image_bg->setSwallowTouches(false);
    }else {
        Widget* item1 = (Widget*)cell->getChildByTag((int)idx + 1000);
        auto text_word = dynamic_cast<Text*>(item1->getChildByName("text_word"));
        text_word->setString(m_data->_upgradWordsList[idx]->_word);
    }
    return cell;
}

Size MyWordGameResult::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return m_item->getContentSize();
}

ssize_t MyWordGameResult::numberOfCellsInTableView(TableView *table)
{
    return m_data->_upgradWordsList.size();
}

void MyWordGameResult::getDataFromSever()
{
    HttpDataToString dataStr;
    dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
    CDataHttp::getInstance()->HttpGet(eHttpType::getNewWordsGameRule, dataStr.getString());
}

void MyWordGameResult::endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json)
{
    if (nOp == eHttpType::getNewWordsGameRule) {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e) {
            stuJson returnData = json.getJson("return_data");
            MyWordGameData* data = new MyWordGameData();
            
            int spend_gold_coin = atoi(returnData.getValue("spend_gold_coin").c_str());
            
            stuJson returnMonsterPoint = returnData.getJson("monster_point");
            int total_point = atoi(returnMonsterPoint.getValue("total_point").c_str());
            int point = atoi(returnMonsterPoint.getValue("point").c_str());
            
            int monstersCount = returnData.GetArraySize("monsters");
            for (int i = 0; i < monstersCount; i++) {
                stuJson one = returnData.GetArrayJson("monsters", i);
                string monsterStr = StringUtils::format("monster%d", atoi(one.getValue("id").c_str()));
                data->_monsterNameList.push_back(monsterStr);
            }
            
            data->_payGem = spend_gold_coin;
            data->_curEggProc= point;
            data->_totalEggProc= total_point;
            if (m_callback) {
                m_callback(1, data);
            }
            this->m_ui->removeFromParent();
        }
    }
}
