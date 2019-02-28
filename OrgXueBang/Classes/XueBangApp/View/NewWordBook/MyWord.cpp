//
//  MyWord.cpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/8.
//

#include "MyWord.hpp"
#include "GameMacro.hpp"
#include "UiBase.h"
#include "MyWordTableByLetter.hpp"
#include "MyWordTableByProficiency.hpp"
#include "MyWordReport.hpp"
#include "MyWordGame.hpp"
#include "BaseScene.hpp"
#include "UserManager.hpp"

MyWord::MyWord():
wordTableLetterX(0.0f),
wordTableProficiencyX(0.0f),
m_skeleton_btnPlay(nullptr),
m_wordTableLetter(nullptr),
m_wordTableProficiency(nullptr)
{
}

MyWord *MyWord::create(Node* parent, bool bDefShowLetter)
{
//    auto scene = (BaseScene*)Game::sharedGame()->getCurScene();
//    scene->setFixWindow();
    
    talkingInterface::pageBegin("生词本");
    
    auto node = parent->getChildByName("MyWord");
    if (node) {
        CCLOG("============MyWord::create===return");
        return (MyWord *)node->getChildByName("MyWord");
    }
    
    MyWord *ret = new (std::nothrow) MyWord();
    if (ret && ret->init(bDefShowLetter)) {
        CCLOG("============MyWord::create===m_ui");
        parent->addChild(ret->m_ui);
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

void MyWord::onExit()
{
//    if (Game::sharedGame()->getGameState() == eStateMain) {
//        auto scene = (BaseScene*)Game::sharedGame()->getCurScene();
//        scene->setStretchWindow();
//    }
    Node::onExit();
}

MyWord::~MyWord()
{
    CCLOG("============~MyWord");
    delete m_wordTableLetter;
    delete m_wordTableProficiency;
    
//    if (m_skeleton_btnPlay) {
//        m_skeleton_btnPlay->clearTracks();
//        m_skeleton_btnPlay->removeFromParent();
//        m_skeleton_btnPlay = nullptr;
//    }
    
    talkingInterface::pageEnd("生词本");
}

bool MyWord::OnBaseKeyBack()
{
    if (m_ui) {
        m_ui->removeFromParent();
        m_ui = nullptr;
    }
    return true;
}

bool MyWord::init(bool bDefShowLetter)
{
    m_ui = CSLoader::createNodeWithVisibleSize("View/NewWordBook/UINewWordBook.csb");
    UiBase::resizeBackImg(m_ui);
    this->setName("MyWord");
    m_ui->addChild(this, -1);
    
    m_ui->setName("MyWord");
    
    m_bDefaultShowLetter = bDefShowLetter;
    
    auto img_back = dynamic_cast<Widget*>(m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("node_userTitle")->getChildByName("panel_top")->getChildByName("btn_back"));
    UiBase::addClickEventListener(img_back, [=](Ref* pSender){
        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
        if (m_ui) {
            m_ui->removeFromParent();
            m_ui = nullptr;
        }
    });
    auto title_name = dynamic_cast<Text*>(m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("node_userTitle")->getChildByName("panel_top")->getChildByName("title_name"));
    title_name->setString("生词本");
    
    auto btn_wordLetter = dynamic_cast<Button*>(m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("btn_sortLetter"));
    btn_wordLetter->setBright(true);
    btn_wordLetter->setTouchEnabled(false);
    UiBase::addClickEventListener(btn_wordLetter, [=](Ref* pSender){
        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
        
        this->showWordLetterTable();
        talkingInterface::traceEvent("生词本", "字母排序");
    });
    auto btn_wordProficiency = dynamic_cast<Button*>(m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("btn_srorProficiency"));
    UiBase::addClickEventListener(btn_wordProficiency, [=](Ref* pSender){
        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
        
        this->showWordProficiencyTable();
        talkingInterface::traceEvent("生词本", "熟练度排序");
    });
    
    createWordLetterTable();
    createWordProficiencyTable();
    
    auto btn_game = dynamic_cast<Button*>(m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("btn_playGame"));
    if (!m_skeleton_btnPlay) {
        m_skeleton_btnPlay = SkeletonAnimation::createWithJsonFile("spine/war_menu.json", "spine/war_menu.atlas", 1.0f);
        m_skeleton_btnPlay->setPosition(Vec2(btn_game->getContentSize().width * 0.5, btn_game->getContentSize().height * 0.5));
        btn_game->addChild(m_skeleton_btnPlay, -1);
    }
    m_skeleton_btnPlay->setAnimation(0, "menu", true);
    
    UiBase::addClickEventListener(btn_game, [=](Ref* pSender){
        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
        getDataFromSever(2);
        talkingInterface::traceEvent("生词本", "生词游戏");
    });
    
    auto btn_report = dynamic_cast<Widget*>(m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("btn_report"));
    UiBase::addClickEventListener(btn_report, [=](Ref* pSender){
        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
        MyWordReport::create(m_ui->getParent());
        talkingInterface::traceEvent("生词本", "生词报告");
    });
    
    getDataFromSever(1);
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_ui);
    
    return true;
}

void MyWord::onEnter()
{
    Node::onEnter();
}

void MyWord::createWordLetterTable()
{
    Layout* panel_wordLetter = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("panel_wordLetter"));
    panel_wordLetter->setVisible(true);
    Layout* panel_table = dynamic_cast<Layout*>(panel_wordLetter->getChildByName("panel_table"));
    auto table = new MyWordTableByLetter();
    table->createTable(panel_table);
    
    MyWordTableByLetter::stuWordUnit* unit[26];
    for (int i = 0; i < 26; i++) {
        unit[i] = new MyWordTableByLetter::stuWordUnit;
        unit[i]->unit = StringUtils::format("%c", ('A' + i));
        unit[i]->wordList.clear();
        unit[i]->bHasData = false;
    }
    table->setUnits(unit, 26);
    m_wordTableLetter = table;
    
    Layout* panel_tableChild = dynamic_cast<Layout*>(panel_table->getChildByName("panel_table_child"));
    wordTableLetterX = panel_tableChild->getPositionX();
}

void MyWord::createWordProficiencyTable()
{
    Layout* panel_wordProficiency = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("panel_wordProficiency"));
    panel_wordProficiency->setVisible(true);
    
    for(auto it : panel_wordProficiency->getChildren()) {
        it->setLocalZOrder(10);
    }
    
    auto table = new MyWordTableByProficiency();
    table->createTable(panel_wordProficiency);
    
    m_wordTableProficiency = table;
    
    wordTableProficiencyX = panel_wordProficiency->getPositionX();
}

void MyWord::showWordLetterTable(bool firstEnter)
{
    if (!m_ui) return;
    
    auto panel_center = (Layout*)m_ui->getChildByName(CENTER_PANEL_NAME);
    auto btn_wordLetter = dynamic_cast<Button*>(panel_center->getChildByName("btn_sortLetter"));
    auto btn_wordProficiency = dynamic_cast<Button*>(panel_center->getChildByName("btn_srorProficiency"));
    
    Layout* panel_shade = dynamic_cast<Layout*>(panel_center->getChildByName("panel_shade"));
    auto f1 = FadeTo::create(0.12f, 0);
    panel_shade->runAction(f1);
    
    //Letter
    Layout* panel_wordLetter = dynamic_cast<Layout*>(panel_center->getChildByName("panel_wordLetter"));
    Layout* panel_wordLetter_table = dynamic_cast<Layout*>(panel_wordLetter->getChildByName("panel_table"));
    Layout* panel_wordLetter_tableChild = dynamic_cast<Layout*>(panel_wordLetter_table->getChildByName("panel_table_child"));

    //Proficiency
    Layout* panel_wordProficiency = dynamic_cast<Layout*>(panel_center->getChildByName("panel_wordProficiency"));
    
    btn_wordLetter->setTouchEnabled(false);
    btn_wordProficiency->setTouchEnabled(true);

    btn_wordLetter->setHighlighted(true);
    btn_wordProficiency->setHighlighted(false);

    //touched
    m_wordTableLetter->setTableTouched(true);
    
    m_bDefaultShowLetter = true;
    
    //action
    float panelWidth = Director::getInstance()->getWinSize().width;//m_ui->getContentSize().width;
    if (!firstEnter) {
        auto move1 = MoveTo::create(0.25, Vec2(2 * panelWidth, panel_wordProficiency->getPositionY()));
        auto action1 = EaseSineOut::create(move1);
        panel_wordProficiency->stopAllActions();
        panel_wordProficiency->runAction(action1);
    }else {
        panel_wordProficiency->setPositionX(2 * panelWidth);
    }
    
    auto m1 = MoveTo::create(0.25, Vec2(wordTableLetterX, panel_wordLetter_tableChild->getPositionY()));
    panel_wordLetter_tableChild->stopAllActions();
    panel_wordLetter_tableChild->runAction(EaseSineIn::create(m1));
}

void MyWord::showWordProficiencyTable(bool firstEnter)
{
    if (!m_ui) return;
    
    auto panel_center = (Layout*)m_ui->getChildByName(CENTER_PANEL_NAME);
    auto btn_wordLetter = dynamic_cast<Button*>(panel_center->getChildByName("btn_sortLetter"));
    auto btn_wordProficiency = dynamic_cast<Button*>(panel_center->getChildByName("btn_srorProficiency"));
    
    Layout* panel_shade = dynamic_cast<Layout*>(panel_center->getChildByName("panel_shade"));
    panel_shade->setOpacity(0);
    auto f1 = FadeTo::create(0.12f, 255);
    panel_shade->runAction(f1);
    
    //Letter
    Layout* panel_wordLetter = dynamic_cast<Layout*>(panel_center->getChildByName("panel_wordLetter"));
    Layout* panel_wordLetter_table = dynamic_cast<Layout*>(panel_wordLetter->getChildByName("panel_table"));
    Layout* panel_wordLetter_tableChild = dynamic_cast<Layout*>(panel_wordLetter_table->getChildByName("panel_table_child"));

    //Proficiency
    Layout* panel_wordProficiency = dynamic_cast<Layout*>(panel_center->getChildByName("panel_wordProficiency"));
    
    btn_wordLetter->setTouchEnabled(true);
    btn_wordProficiency->setTouchEnabled(false);
    
    btn_wordLetter->setHighlighted(false);
    btn_wordProficiency->setHighlighted(true);

    //touched
    m_wordTableLetter->setTableTouched(false);
    
    m_bDefaultShowLetter = false;

    m_wordTableProficiency->reloadData();

    //aciton
    float panelWidth = Director::getInstance()->getWinSize().width;//m_ui->getContentSize().width;
    if (!firstEnter) {
        auto move1 = MoveTo::create(0.25, Vec2(-2 * panelWidth, panel_wordLetter_tableChild->getPositionY()));
        auto action1 = EaseSineOut::create(move1);
        panel_wordLetter_tableChild->stopAllActions();
        panel_wordLetter_tableChild->runAction(action1);
    }else {
        panel_wordLetter_tableChild->setPositionX(-2 * panelWidth);
    }
    
    auto m1 = MoveTo::create(0.25, Vec2(wordTableProficiencyX, panel_wordProficiency->getPositionY()));
    panel_wordProficiency->stopAllActions();
    panel_wordProficiency->runAction(EaseSineIn::create(m1));
}

void MyWord::getDataFromSever(int type)
{
    HttpDataToString dataStr;
    dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
    if (type == 1) {
        CDataHttp::getInstance()->HttpGet(eHttpType::getNewWordsV2, dataStr.getString());
    }else if(type == 2) {
        CDataHttp::getInstance()->HttpGet(eHttpType::getNewWordsGameRule, dataStr.getString());
    }
}

void MyWord::endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json)
{
    if (nOp == eHttpType::getNewWordsV2) {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e) {
            std::vector<stuWord*> totalWordList;
            totalWordList.clear();
            
            stuJson returnData = json.getJson("return_data");
            string audioPath = returnData.getValue("word_audio");
            string coverPath = returnData.getValue("word_cover");
            stuJson returnWordList = returnData.getJson("new_words");
            
            int wordCount = returnWordList.GetArraySize("word_list");
            for (int i = 0; i < wordCount; i++) {
                stuJson one = returnWordList.GetArrayJson("word_list", i);
                
                stuWord* item = new stuWord;
                item->id = atoi(one.getValue("id").c_str());
                item->word = one.getValue("word");
                item->image = one.getValue("image");
                item->audio = audioPath + one.getValue("audio");
                item->desc = one.getValue("description_short");
                item->level = atoi(one.getValue("skill_level").c_str());
                item->right_gameTimes = atoi(one.getValue("right_game_times").c_str());
                item->total_gameTimes = atoi(one.getValue("total_game_times").c_str());
                totalWordList.push_back(item);
            }

            //字母排序
            std::sort(totalWordList.begin(), totalWordList.end(), [](stuWord *a, stuWord *b) -> bool{
                return a->word < b->word;
            });
            int addWordCount = 0;
            for (int i = 0; i < 26; i++) {
                std::vector<stuWord*> oneWordList;
                oneWordList.clear();
                string unit = StringUtils::format("%c", ('a' + i));
                
                for (int j = 0; j < wordCount; j++) {
                    stuWord* item = totalWordList[j];
                    string leftStr = Helper::getSubStringOfUTF8String(item->word, 0, 1);
                    std::transform(leftStr.begin(), leftStr.end(), leftStr.begin(), ::tolower);
                    
                    if (unit == leftStr) {
                        oneWordList.push_back(item);
                        addWordCount++;
                    }
                }
//                if ((int)oneWordList.size() > 0) {
                    m_wordTableLetter->setUnitData(oneWordList, i);
//                }
                if (addWordCount >= wordCount) {
                    break;
                }
            }
            if (m_bDefaultShowLetter) {
                showWordLetterTable(true);
            }
            if (m_wordTableLetter) {
                m_wordTableLetter->showDefaultUnit();
            }

            //熟练度排序 从低到高排序
            std::sort(totalWordList.begin(), totalWordList.end(), [](stuWord *a, stuWord *b) -> bool{
                return a->level < b->level;
            });
            if (m_wordTableProficiency) {
                m_wordTableProficiency->setUnitData(totalWordList);
            }
            if (!m_bDefaultShowLetter) {
                showWordProficiencyTable(true);
            }
        }
    }else if (nOp == eHttpType::getNewWordsGameRule) {
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

            std::function<void()> exitCallBack = this->getOnExitCallback();

            MyWordGame::create(m_ui->getParent(), data, m_bDefaultShowLetter);
            if (m_ui) {
                m_ui->removeFromParent();
                m_ui = nullptr;
            }
        }
    }
}
