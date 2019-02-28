//
//  MyWordGame.cpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/15.
//

#include "MyWordGame.hpp"
#include "GameMacro.hpp"
#include "UserManager.hpp"
#include "UiBase.h"
#include "MyWordGameSubjectItem.hpp"
#include "TipManager.hpp"
#include "MyWordGameResult.hpp"
#include "DownImg.h"
//#include "GemStore.hpp"
#include "AlertDialog.hpp"
#include "BaseScene.hpp"
#include "MyWord.hpp"

#define WRONG_SUBJECT_NUMBER 3 //出错误题数

MyWordGame::MyWordGame():
m_panel_subjectX(0.0f),
m_gameStatus(eStatusNone),
m_isbeginTime(false),
b_canTouched(true),
m_skeleton_star(nullptr),
m_skeleton_egg(nullptr),
m_skeleton_senceLight(nullptr),
m_gameResultData(nullptr),
m_subjectItemLayer(nullptr)
{
    m_copySubjectList.clear();
    m_testSubject.clear();
}

MyWordGame *MyWordGame::create(Node* parent, void *data, bool bDefShowLetter)
{
//    auto scene = (BaseScene*)Game::sharedGame()->getCurScene();
//    scene->setFixWindow();
    
    talkingInterface::pageBegin("生词游戏");
    
    auto node = parent->getChildByName("MyWordGame");
    if (node) {
        return (MyWordGame *)node->getChildByName("MyWordGame");
    }
    
    MyWordGame *ret = new (std::nothrow) MyWordGame();
    if (ret && ret->init(data, bDefShowLetter)) {
        parent->addChild(ret->m_ui);
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

void MyWordGame::onExit()
{
    SoundManager::getInstance()->pauseBackSound();
    SoundManager::getInstance()->stopAllSound();
    
//    if (Game::sharedGame()->getGameState() == eStateMain) {
//        auto scene = (BaseScene*)Game::sharedGame()->getCurScene();
//        scene->setStretchWindow();
//    }
    Node::onExit();
}

MyWordGame::~MyWordGame()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(MyWordGame::updateMonsterZorder));
    delete m_subjectItemLayer;
    clearDataList();
    talkingInterface::pageEnd("生词游戏");
}

bool MyWordGame::OnBaseKeyBack()
{
//    this->m_ui->removeFromParent();
    touchBackCallFun();
    return true;
}

void MyWordGame::clearDataList()
{
    m_copySubjectList.clear();
    m_testSubject.clear();
    //-------------------------------

    for (int i = 0; i <  m_gamePlayData->_monsterList.size(); i++) {
        if (m_gamePlayData->_monsterList[i]->_monster) {
            m_gamePlayData->_monsterList[i]->_monster->clearTracks();
            m_gamePlayData->_monsterList[i]->_monster->removeFromParent();
            m_gamePlayData->_monsterList[i]->_monster = nullptr;
        }
        delete m_gamePlayData->_monsterList[i];
    }

    for (int i = 0; i <  m_gamePlayData->_subjectList.size(); i++) {
        delete m_gamePlayData->_subjectList[i];
    }
    m_gamePlayData->_monsterNameList.clear();
    m_gamePlayData->_monsterList.clear();
    m_gamePlayData->_subjectList.clear();
    m_gamePlayData->_rightWordVect.clear();
    m_gamePlayData->_wrongWordVect.clear();
    delete m_gamePlayData;
    //-------------------------------
    
    if (m_skeleton_star) {
        m_skeleton_star->clearTracks();
        m_skeleton_star->removeFromParent();
        m_skeleton_star = nullptr;
    }

    if (m_skeleton_egg) {
        m_skeleton_egg->clearTracks();
        m_skeleton_egg->removeFromParent();
        m_skeleton_egg = nullptr;
    }
    
    if (m_skeleton_senceLight) {
        m_skeleton_senceLight->clearTracks();
        m_skeleton_senceLight->removeFromParent();
        m_skeleton_senceLight = nullptr;
    }
    
    //-------------------------------
    if (m_gameResultData) {
        for (int i = 0; i < m_gameResultData->_upgradWordsList.size(); i++) {
            delete m_gameResultData->_upgradWordsList[i];
        }
        m_gameResultData->_upgradWordsList.clear();
        m_gameResultData = nullptr;
    }
    delete m_gameResultData;
}

bool MyWordGame::init(void *data, bool bDefShowLetter)
{
    m_ui = CSLoader::createNodeWithVisibleSize("View/NewWordGame/layer_newWordGame.csb");
    UiBase::resizeBackImg(m_ui);
    this->setName("MyWordGame");
    m_ui->addChild(this, -1);
    
    m_ui->setName("MyWordGame");
    
    m_bDefShowLetter = bDefShowLetter;
    initData(data);
    
    auto btn_back = dynamic_cast<Button*>(m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("btn_back"));
    UiBase::addClickEventListener(btn_back, [=](Ref* pSender){
        if (b_canTouched) {
            touchBackCallFun();
        }
    });
    
    setGameStatus(ePlayGameStatus::eStatusInit);

    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_ui);
    
    SoundManager::getInstance()->playBackSound("music/sleepbg.mp3", true);
    return true;
}

void MyWordGame::onEnter()
{
    Node::onEnter();
}

void MyWordGame::initData(void *data)
{
    if (!data) {
        return;
    }
    m_gamePlayData = (myWordGameData*)data;
    
    for (int i = 0; i <  m_gamePlayData->_monsterNameList.size(); i++) {
        monsterUnit* monsterItem = new monsterUnit();
        monsterItem->_name = m_gamePlayData->_monsterNameList[i];
        monsterItem->_monster = nullptr;
        m_gamePlayData->_monsterList.push_back(monsterItem);
    }
    
    m_gamePlayData->_curSubjectId = 0;
    m_gamePlayData->_curSubjectCount = (int)m_gamePlayData->_subjectList.size();
    m_gamePlayData->_rightSubject = 0;
    
    if (!m_gameResultData) {
        m_gameResultData = new GameResultData();
    }
    m_gameResultData->_upgradWordsList.clear();
}

void MyWordGame::touchBackCallFun()
{
    SoundManager::getInstance()->pauseBackSound();
    SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
    
    if(getGameStatus() == ePlayGameStatus::eStatusInit) {
        MyWord::create(m_ui->getParent(), m_bDefShowLetter);
        this->m_ui->removeFromParent();
    }else {
        auto dialog = AlertDialog::create("真的要退出此次游戏吗？");
        dialog->setOkCallBack([=](){
            getDataFromSever(0);
            SoundManager::getInstance()->playBackSound("music/sleepbg.mp3", true);
        });
        dialog->setCancelCallBack([=](){
            SoundManager::getInstance()->resumeBackSound();
        });
        m_ui->getParent()->addChild(dialog);
    }
}

MyWordGameSubjectItem* MyWordGame::createMyWordGameSubjectItemLayer()
{
    Layout* panel_subject = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("panel_subject"));
    panel_subject->setVisible(true);
    
    auto table = new MyWordGameSubjectItem();
    table->createTable(panel_subject);
    table->setCallBackFunc(CC_CALLBACK_3(MyWordGame::touchSubjectItemCallBack, this));
    return table;
}

void MyWordGame::touchSubjectItemCallBack(int wordId, string rightDesc, bool isRight)
{
    float panelWidth = m_ui->getContentSize().width;
    Layout* panel_center = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME));
    Layout* panel_subject = dynamic_cast<Layout*>(panel_center->getChildByName("panel_subject"));
    
    b_canTouched = false;
    if ( isRight ) {
        m_gamePlayData->_rightWordVect.push_back(wordId);
        
        SkeletonAnimation* skeleton_good = nullptr;
        if (m_ui->getChildByName("skeleton_good")) {
            skeleton_good = dynamic_cast<SkeletonAnimation*>(m_ui->getChildByName("skeleton_good"));
        }else {
            skeleton_good = SkeletonAnimation::createWithJsonFile("spine/good_feel.json", "spine/good_feel.atlas", 1.0f);
//            skeleton_good = SkeletonAnimation::createWithJsonFile("spine/good.json", "spine/good.atlas", 1.0f);
//            skeleton_good->setScale(0.35);
            skeleton_good->setPosition(Vec2(m_ui->getContentSize().width*0.5, m_ui->getContentSize().height*0.5));
            skeleton_good->setName("skeleton_good");
            m_ui->addChild(skeleton_good);
        }
        skeleton_good->clearTracks();
        skeleton_good->setAnimation(0, "good", false);
//        skeleton_good->setAnimation(0, "animation", false);
    
        for (int i = 0; i <  m_gamePlayData->_monsterList.size(); i++) {
            if (m_gamePlayData->_monsterList[i]->_monster) {
                setOneMonster(m_gamePlayData->_monsterList[i]->_monster, eMonsterStatus::win);
            }
        }
        SoundManager::getInstance()->playSoundEffect("music/quiz_right.mp3");
    }else {
        m_gamePlayData->_wrongWordVect.push_back(wordId);
        
        for (int i = 0; i <  m_gamePlayData->_monsterList.size(); i++) {
            if (m_gamePlayData->_monsterList[i]->_monster) {
                setOneMonster(m_gamePlayData->_monsterList[i]->_monster, eMonsterStatus::fail);
            }
        }
        SoundManager::getInstance()->playSoundEffect("music/quiz_wrong.mp3");
    }
    auto call1 = CallFunc::create([=](){
        auto txt_subject_prompt = dynamic_cast<Text*>(panel_subject->getChildByName("txt_subject_prompt"));
        txt_subject_prompt->setOpacity(0);
        txt_subject_prompt->setVisible(true);
        txt_subject_prompt->setString(rightDesc);
        txt_subject_prompt->setScale(0.01);
        auto s1 = ScaleTo::create(0.12f, 1);
        auto f1 = FadeTo::create(0.12f, 255);
        
        auto callfun1 = CallFunc::create([=](){
            auto m1 = MoveTo::create(0.3, Vec2(2 * panelWidth, panel_subject->getPositionY()));
            auto a1 = EaseSineIn::create(m1);
            panel_subject->runAction(a1);
        });
        
        auto callfun2 = CallFunc::create([=](){
            //下一道题目
            if (getGameStatus() > ePlayGameStatus::eStatusInit) {
                setGameStatus(ePlayGameStatus::eStatusStartEffect);
            }
            b_canTouched = true;
        });
        txt_subject_prompt->stopAllActions();
        txt_subject_prompt->runAction(Sequence::create(Spawn::create(s1, f1, NULL), DelayTime::create(1.5f), callfun1, DelayTime::create(0.5f), callfun2, NULL));

        if ( isRight ) {
            m_gamePlayData->_rightSubject++;
            updateDoSubjectInfo();
        }
    });
    panel_subject->stopAllActions();
    panel_subject->runAction(Sequence::create(call1, NULL));
}

void MyWordGame::setGameStatus(ePlayGameStatus Status)
{
    if(Status == m_gameStatus) {
        return;
    }
    
    switch (Status) {
        case ePlayGameStatus::eStatusInit: {
            statusInit();
        }
            break;
        case ePlayGameStatus::eStatusStart: {
            statusStartView();
        }
            break;
        case ePlayGameStatus::eStatusStartEffect: {
            statusStartEffect(m_copySubjectList);
        }
            break;
        case ePlayGameStatus::eStatusGame: {
            statusStartGame();
        }
            break;
        case ePlayGameStatus::eStatusEndEgg: {
            statusEndEgg();
        }
            break;
        case ePlayGameStatus::eStatusResult: {
            statusResult();
        }
            break;
        default:
            break;
    }
    m_gameStatus = Status;
}

void MyWordGame::createAnimation(Node* startNode, Node* targetNode)
{
    Vec2 gemIconPos = startNode->getParent()->convertToWorldSpace(startNode->getPosition());
    Vec2 targetPos = targetNode->getParent()->convertToWorldSpace(targetNode->getPosition());
    
    float baseTime = 1.0f;
    srand ((unsigned)time(nullptr));
    
    Vec2 pos1 = gemIconPos;
    Vec2 pos2 = Vec2(targetPos.x - 20.0f, targetPos.y - 4.0f);
    ccBezierConfig bezierConfig;
    bezierConfig.controlPoint_1 = pos1;
    int m = rand() % 300;
    int n = rand() % 100;
    bezierConfig.controlPoint_2 = Vec2(pos2.x + (m - 150.0f), pos2.y + (n + 100.0f));
    bezierConfig.endPosition = pos2;

    BezierTo* move = BezierTo::create(baseTime, bezierConfig);
    EaseExponentialOut* easeEO = EaseExponentialOut::create(move);
    EaseSineOut* easeSO = EaseSineOut::create(ScaleTo::create(baseTime * 0.3f, 1.5f));
    FadeTo* fade = FadeTo::create(baseTime * 0.3f, 0);

    startNode->runAction(Sequence::create(easeEO, Spawn::create(fade, easeSO, nullptr), nullptr));
}

void MyWordGame::updateEggProc()
{
    if (m_gamePlayData->_curEggProc > m_gamePlayData->_totalEggProc) {
        return;
    }
    Layout* panel_bottom = dynamic_cast<Layout*>(m_ui->getChildByName(BOTTOM_PANEL_NAME));
    auto txt_egg_proc = dynamic_cast<Text*>(panel_bottom->getChildByName("txt_egg_proc"));
    txt_egg_proc->setString(StringUtils::format("%d/%d", m_gamePlayData->_curEggProc, m_gamePlayData->_totalEggProc));
    
    auto txt_right_numb = dynamic_cast<Text*>(panel_bottom->getChildByName("txt_right_numb"));
    txt_right_numb->setString(StringUtils::format("+%d", m_gamePlayData->_rightSubject));
//    if(getGameStatus() < ePlayGameStatus::eStatusStart) {
//        txt_right_numb->setVisible(false);
//    }else {
//        txt_right_numb->setVisible(true);
//    }
}

void MyWordGame::updateDoSubjectInfo()
{
    auto subject_title = m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("subject_title");
    subject_title->setVisible(true);
    auto newWordGame_icon_proc = subject_title->getChildByName("newWordGame_icon_proc");
    auto txt_cur_subject = dynamic_cast<Text*>(newWordGame_icon_proc->getChildByName("txt_cur_subject"));
    txt_cur_subject->setString(StringUtils::format("%d/%d", m_gamePlayData->_curSubjectId, m_gamePlayData->_curSubjectCount));
    auto txt_right_number = dynamic_cast<Text*>(newWordGame_icon_proc->getChildByName("txt_right_number"));
    txt_right_number->setString(StringUtils::format("答对%d", m_gamePlayData->_rightSubject));
    
    Layout* panel_bottom = dynamic_cast<Layout*>(m_ui->getChildByName(BOTTOM_PANEL_NAME));
    auto txt_right_numb = dynamic_cast<Text*>(panel_bottom->getChildByName("txt_right_numb"));
    txt_right_numb->setString(StringUtils::format("+%d", m_gamePlayData->_rightSubject));
    if(!txt_right_numb->isVisible()) {
        txt_right_numb->setVisible(true);
    }
}

std::vector<GameSubject*> MyWordGame::getTestSubjectList(std::vector<GameSubject*> vector, std::vector<GameSubject*> srcVector)
{
    std::vector<GameSubject*> tmpSubjectList;
    tmpSubjectList.clear();
    m_testName.assign("");
    
    //取正确的题目
    int curIdx = random(0, (int)vector.size() - 1);
    auto iter = std::find(srcVector.begin(), srcVector.end(), vector[curIdx]);
    if (iter != srcVector.end()){
        srcVector.erase(iter);
    }
    
    vector[curIdx]->_bRight = true;
    m_testName = vector[curIdx]->_name;
    tmpSubjectList.push_back(vector[curIdx]);
    //主动删掉已做过的题目
    auto it = std::find(m_copySubjectList.begin(), m_copySubjectList.end(), vector[curIdx]);
    if (it != m_copySubjectList.end()){
        m_copySubjectList.erase(it);
    }
    
    //取随机错误的3道题
    std::vector<GameSubject*> errorSubjectList = getRandomSubject(WRONG_SUBJECT_NUMBER, srcVector);
    for (int i = 0; i < errorSubjectList.size(); i++) {
        errorSubjectList[i]->_bRight = false;
        tmpSubjectList.push_back(errorSubjectList[i]);
    }
    //打乱顺序
    random_shuffle(tmpSubjectList.begin(), tmpSubjectList.end());
    return tmpSubjectList;
}

std::vector<GameSubject*> MyWordGame::getRandomSubject(int randomCount, std::vector<GameSubject*> srcList)
{
    std::vector<GameSubject*> randomList;
    
    srand(unsigned(time(NULL)));
    random_shuffle(srcList.begin(), srcList.end());
    if (randomCount >= (int)srcList.size()) {
        randomList = srcList;
    }else {
        for (int i = 0; i < randomCount; i++) {
            randomList.push_back(srcList[i]);
        }
    }
    
    return randomList;
}

void MyWordGame::playAudio(string path)
{
    string strPathSnd = DownImg::getInstance()->getLocalFilePath(path.c_str());
    
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        SoundManager::getInstance()->VolumeBackSound(0.6f);
        SoundManager::getInstance()->playSound(strPathSnd);
    }
    else {
        TipManager::getInstance()->showMsgTips("声音文件下载中...");
    }
}

bool MyWordGame::haveEnoughGem(int spendGem)
{
    if (spendGem <= 0 ) return true;
    int coin = UserManager::getInstance()->getUserEnt()->_Coin;
    if (coin >= spendGem) {
        return true;
    }
    return false;
}

void MyWordGame::updateUserGemInfo()
{
    auto gemNode = m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("node_gem");
    auto txt_gem = dynamic_cast<Text*>(gemNode->getChildByName("btn_gem")->getChildByName("gem_number"));
    int coin = UserManager::getInstance()->getUserEnt()->_Coin;
    txt_gem->setString(StringUtils::format("%d",coin));
}

void MyWordGame::enterGemStore()
{
    talkingInterface::traceEvent("生词游戏", "宝石购买");
}

void MyWordGame::statusInit()
{
    //top panel
    auto subject_title = m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("subject_title");
    subject_title->setVisible(false);
    
    //user gem
    updateUserGemInfo();
    auto gemNode = m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("node_gem");
    auto btn_gem = dynamic_cast<Button*>(gemNode->getChildByName("btn_gem"));
    UiBase::addClickEventListener(btn_gem, [=](Ref* pSender){
//        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
//        enterGemStore();
    });
    
    //center panel
    Layout* panel_center = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME));
    Layout* panel_subject = dynamic_cast<Layout*>(panel_center->getChildByName("panel_subject"));
    panel_subject->setVisible(false);
    
    auto txt_game_end = panel_center->getChildByName("txt_game_end");
    txt_game_end->setVisible(false);
    
    Layout* panel_begin = dynamic_cast<Layout*>(panel_center->getChildByName("panel_begin"));
    panel_begin->setVisible(true);
    auto btn_play = dynamic_cast<Button*>(panel_begin->getChildByName("btn_wplayGame"));
    UiBase::addClickEventListener(btn_play, [=](Ref* pSender){
        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
        bool ret = haveEnoughGem(m_gamePlayData->_payGem);
        if (ret) {
            talkingInterface::traceEvent("生词游戏", "开始");
            getDataFromSever(1);
        }else {
//            auto dialog = AlertDialog::create("嗨！宝石不够了，需要购买一点吗？");
//            dialog->setOkCallBack(CC_CALLBACK_0(MyWordGame::enterGemStore, this));
            auto dialog = AlertDialog::create("嗨！宝石不够了，快去阅读书本获取吧～");
            dialog->setOkCallBack(CC_CALLBACK_0(MyWordGame::enterMainPage, this));
            m_ui->getParent()->addChild(dialog);
        }
    });
    Layout* panel_paygem = dynamic_cast<Layout*>(panel_begin->getChildByName("panel_paygem"));
    panel_paygem->setOpacity(0);
    panel_paygem->setVisible(true);
    auto f1 = FadeTo::create(0.15f, 255);
    panel_paygem->runAction(f1);

    auto icon_gem = dynamic_cast<Sprite*>(panel_paygem->getChildByName("icon_gem"));
    auto txt_pay_gems = dynamic_cast<Text*>(panel_paygem->getChildByName("txt_pay_gems"));
    txt_pay_gems->setString(StringUtils::format("-%d", m_gamePlayData->_payGem));

    //居中设置
    float width = txt_pay_gems->getContentSize().width + icon_gem->getContentSize().width + 6.0f;
    panel_paygem->setContentSize(Size(width, panel_paygem->getContentSize().height));
    txt_pay_gems->setPositionX(0);
    icon_gem->setPositionX(width);

    //bottom panel
    Layout* panel_bottom = dynamic_cast<Layout*>(m_ui->getChildByName(BOTTOM_PANEL_NAME));
    auto fontstage = dynamic_cast<Sprite*>(panel_bottom->getChildByName("newWordGame_icon_frontstage"));
    
    auto fontstageSize = fontstage->getContentSize();
    if (!m_skeleton_egg) {
        m_skeleton_egg = SkeletonAnimation::createWithJsonFile("spine/egg_s.json", "spine/egg_s.atlas", 1.0f);
        m_skeleton_egg->setPosition(Vec2(fontstageSize.width*0.2, fontstageSize.height*0.29));
        fontstage->addChild(m_skeleton_egg, 10);
    }
    m_skeleton_egg->setAnimation(0, "stand", true);
    updateEggProc();

    if (!m_gamePlayData->_bInitMonsterFlag) {
        for (int i = 0; i <  m_gamePlayData->_monsterList.size(); i++) {
            SkeletonAnimation* monster = setMonster(m_gamePlayData->_monsterList[i]->_name, "sleep");
            m_gamePlayData->_monsterList[i]->_monster = monster;
        }
        m_gamePlayData->_bInitMonsterFlag = true;
    }else {
        for (int i = 0; i <  m_gamePlayData->_monsterList.size(); i++) {
            if (m_gamePlayData->_monsterList[i]->_monster) {
                setOneMonster(m_gamePlayData->_monsterList[i]->_monster, eMonsterStatus::sleep);
            }
        }
    }
    if (!this->isScheduled(CC_SCHEDULE_SELECTOR(MyWordGame::updateMonsterZorder))) {
        this->schedule(CC_SCHEDULE_SELECTOR(MyWordGame::updateMonsterZorder), 1.0f);
    }
}

void MyWordGame::statusStartView()
{
    updateDoSubjectInfo();
    
    Layout* panel_center = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME));
    Layout* panel_subject = dynamic_cast<Layout*>(panel_center->getChildByName("panel_subject"));
    panel_subject->setVisible(true);
    Layout* panel_begin = dynamic_cast<Layout*>(panel_center->getChildByName("panel_begin"));
    panel_begin->setVisible(false);
    
    for (int i = 0; i <  m_gamePlayData->_monsterList.size(); i++) {
        if (m_gamePlayData->_monsterList[i]->_monster) {
            setOneMonster(m_gamePlayData->_monsterList[i]->_monster, eMonsterStatus::erand);
        }
    }
    if (!m_subjectItemLayer) {
        m_subjectItemLayer = createMyWordGameSubjectItemLayer();
    }
    
    auto btn_sound = dynamic_cast<Button*>(panel_subject->getChildByName("btn_sound"));
    UiBase::addClickEventListener(btn_sound, [=](Ref* pSender){
        for (int i = 0; i < m_testSubject.size(); i++) {
            if (m_testSubject[i]->_bRight) {
                playAudio(m_testSubject[i]->_audioPath);
                break;
            }
        }
    });

    m_copySubjectList = m_gamePlayData->_subjectList;
    setGameStatus(ePlayGameStatus::eStatusStartEffect);
}

void MyWordGame::statusStartEffect(std::vector<GameSubject*> vector)
{
    if (vector.size() <= 0) { //题目已做完
        getDataFromSever(2);
        m_isbeginTime = false;
        return;
    }
    //取一轮题库
    std::vector<GameSubject*> srcVector = m_gamePlayData->_subjectList;
    
    srand(unsigned(time(NULL)));
    random_shuffle(vector.begin(), vector.end());
    
    string audioPath = "";
    m_testSubject.clear();
    m_testSubject = getTestSubjectList(vector, srcVector);
    for (int i = 0; i < m_testSubject.size(); i++) {
        if (m_testSubject[i]->_bRight) {
            audioPath = m_testSubject[i]->_audioPath;
        }
    }

    //显示题目
    if ((WRONG_SUBJECT_NUMBER + 1) == m_testSubject.size()) {
        
        float panelWidth = m_ui->getContentSize().width;
        Layout* panel_center = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME));
        Layout* panel_subject = dynamic_cast<Layout*>(panel_center->getChildByName("panel_subject"));
        if (m_panel_subjectX == 0.0f) {
            m_panel_subjectX = panel_subject->getPositionX();
        }
        panel_subject->setPosition(Vec2(-2 * panelWidth, panel_subject->getPositionY()));
        
        auto callfun1 = CallFunc::create([=](){
            m_subjectItemLayer->setUnitData(m_testSubject);
            m_subjectItemLayer->reloadData();
            
            setGameStatus(ePlayGameStatus::eStatusGame);
        });
        auto move1 = MoveTo::create(0.25, Vec2(m_panel_subjectX, panel_subject->getPositionY()));
        auto action1 = EaseSineOut::create(move1);
        
        auto callfun2 = CallFunc::create([=](){
            playAudio(audioPath);
        });
        panel_subject->runAction(Sequence::create(callfun1, action1, DelayTime::create(0.5f), callfun2, NULL));
    }
}

void MyWordGame::statusStartGame()
{
    Layout* panel_center = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME));
    Layout* panel_subject = dynamic_cast<Layout*>(panel_center->getChildByName("panel_subject"));
    auto txt_subject_name = dynamic_cast<Text*>(panel_subject->getChildByName("txt_subject_name"));
    txt_subject_name->setString(m_testName);
    auto txt_subject_prompt = dynamic_cast<Text*>(panel_subject->getChildByName("txt_subject_prompt"));
    txt_subject_prompt->setVisible(false);

    m_gamePlayData->_curSubjectId++;
    updateDoSubjectInfo();
}

void MyWordGame::showOneEggMonster()
{
    if (m_gameResultData->_bUpgradeFlag) {
        auto btn_back = dynamic_cast<Button*>(m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("btn_back"));
        btn_back->setEnabled(false);
        
        if (!m_skeleton_senceLight) {
            m_skeleton_senceLight = SkeletonAnimation::createWithJsonFile("spine/sence_light.json", "spine/sence_light.atlas", 1.0f);
            m_skeleton_senceLight->setPosition(Vec2(m_ui->getContentSize().width * 0.5, m_ui->getContentSize().height * 0.5));
            m_skeleton_senceLight->setRotation(90);
            m_ui->addChild(m_skeleton_senceLight, 99);
        }
        m_skeleton_senceLight->setAnimation(0, "stand", true);
        SoundManager::getInstance()->playSoundEffect("music/snd_qstart.mp3");
        
        string path1 = StringUtils::format("spine/%s.json", m_gameResultData->_monsterName.c_str());
        string path2 = StringUtils::format("spine/%s.atlas", m_gameResultData->_monsterName.c_str());
        SkeletonAnimation* tmpMonster = SkeletonAnimation::createWithJsonFile(path1, path2, 0.5f);
        tmpMonster->setAnimation(0, "free", true);
        tmpMonster->setPosition(Vec2(m_ui->getContentSize().width * 0.46, m_ui->getContentSize().height * 0.46));
        m_ui->addChild(tmpMonster, 100);
        auto call1 = CallFunc::create([=](){
            if (m_skeleton_senceLight) {
                auto s1 = ScaleTo::create(0.25f, 0.01f);
                auto f1 = FadeTo::create(0.25f, 0);
                auto call = CallFunc::create([=](){
                    if (m_skeleton_senceLight) {
                        m_skeleton_senceLight->stopAllActions();
                        m_skeleton_senceLight->clearTracks();
                        m_skeleton_senceLight->removeFromParent();
                        m_skeleton_senceLight = nullptr;
                    }
                });
                m_skeleton_senceLight->runAction(Sequence::create(Spawn::create(s1, f1, NULL), call, NULL));
                if (tmpMonster) {
                    tmpMonster->setVisible(false);
                }
            }
        });
        auto call2 = CallFunc::create([=](){
            monsterUnit* monsterItem = new monsterUnit();
            monsterItem->_name = m_gameResultData->_monsterName;
            monsterItem->_monster = setMonster(monsterItem->_name, "sleep");
            m_gamePlayData->_monsterList.push_back(monsterItem);
        });
        auto call3 = CallFunc::create([=](){
            if (btn_back) {
                btn_back->setEnabled(true);
            }
            
            setGameStatus(ePlayGameStatus::eStatusResult);
            
            if (tmpMonster) {
                tmpMonster->clearTracks();
                tmpMonster->removeFromParent();
            }
        });
        tmpMonster->runAction(Sequence::create(DelayTime::create(3.0), call1, call2, DelayTime::create(1.5), call3, NULL));
    }
}

void MyWordGame::updateEggStar(float t)
{
    if (!m_skeleton_star) return;
    
    if (m_gamePlayData->_curEggProc >= m_gamePlayData->_totalEggProc) {
        this->unschedule(CC_SCHEDULE_SELECTOR(MyWordGame::updateEggStar));
        
        auto call1 = CallFunc::create([=](){
            if (m_skeleton_star) {
                m_skeleton_star->setAnimation(0, "stand_end", false);
                SoundManager::getInstance()->playSoundEffect("music/snd_score.mp3");
            }
        });
        auto call2 = CallFunc::create([=](){
            if (m_skeleton_egg) {
                m_skeleton_egg->setAnimation(0, "break", false);
            }
        });
        auto call3 = CallFunc::create([=](){
            showOneEggMonster();
        });
        m_skeleton_star->runAction(Sequence::create(call1, DelayTime::create(0.5f), call2, DelayTime::create(3.2f), call3, NULL));
    }else {
        if (m_gamePlayData->_rightSubject > 0) {
            m_gamePlayData->_rightSubject--;
            m_gamePlayData->_curEggProc++;
            updateEggProc();
            m_skeleton_star->setAnimation(0, "stand", false);
            SoundManager::getInstance()->playSoundEffect("music/snd_score.mp3");
        }else {
            this->unschedule(CC_SCHEDULE_SELECTOR(MyWordGame::updateEggStar));
            setGameStatus(ePlayGameStatus::eStatusResult);
        }
    }
}

void MyWordGame::statusEndEgg()
{
    if (m_gamePlayData->_rightSubject > 0) {
        if (!m_skeleton_star) {
            Layout* panel_bottom = dynamic_cast<Layout*>(m_ui->getChildByName(BOTTOM_PANEL_NAME));
            auto size = panel_bottom->getContentSize();
            m_skeleton_star = SkeletonAnimation::createWithJsonFile("spine/effect_star.json", "spine/effect_star.atlas", 0.45f);
            m_skeleton_star->setPosition(Vec2(size.width * 0.13, 40.0f));
            panel_bottom->addChild(m_skeleton_star);
        }
        auto txt_game_end = m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("txt_game_end");
        if ( txt_game_end ) {
            txt_game_end->setVisible(true);
            float y = txt_game_end->getPositionY();
            auto m1 = MoveTo::create(1.5f, Vec2(txt_game_end->getPositionX(), y - 2.0f));
            auto m2 = MoveTo::create(1.5f, Vec2(txt_game_end->getPositionX(), y + 2.0f));
            auto e1 = EaseSineOut::create(m1);
            auto e2 = EaseSineIn::create(m2);
            txt_game_end->runAction(RepeatForever::create(Sequence::create(e1, e2, NULL) ));
        }
        
        this->schedule(CC_SCHEDULE_SELECTOR(MyWordGame::updateEggStar), 0.8f);
    }else {
        setGameStatus(ePlayGameStatus::eStatusResult);
    }
}

void MyWordGame::statusResult()
{
    auto txt_game_end = m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("txt_game_end");
    if ( txt_game_end ) {
        txt_game_end->stopAllActions();
        txt_game_end->setVisible(false);
    }
    
    auto gameResultLayer = MyWordGameResult::create(m_ui->getParent(), m_gameResultData);
    gameResultLayer->setCallBackFunc(CC_CALLBACK_2(MyWordGame::touchResultCallBack, this));
    gameResultLayer->reloadData();
}

void MyWordGame::touchResultCallBack(int type, void *data)
{
    if (type == 1 && data) { //再玩一次
        this->unschedule(CC_SCHEDULE_SELECTOR(MyWordGame::updateMonsterZorder));
        m_gameStatus = eStatusNone;
        clearDataList();
        initData(data);
        setGameStatus(ePlayGameStatus::eStatusInit);
        SoundManager::getInstance()->playBackSound("music/sleepbg.mp3", true);
    }else if(type == 2) { //回到生词本
        MyWord::create(this->m_ui->getParent(), m_bDefShowLetter);
        this->m_ui->removeFromParent();
    }
}

SkeletonAnimation* MyWordGame::setMonster(string nameStr, string animationName)
{
    Layout* panel_bottom = dynamic_cast<Layout*>(m_ui->getChildByName(BOTTOM_PANEL_NAME));
    auto backstage = dynamic_cast<Sprite*>(panel_bottom->getChildByName("newWordGame_icon_backstage"));
    auto backstageSize = backstage->getContentSize();
    
    string jsonPath = StringUtils::format("spine/%s.json", nameStr.c_str());
    string atlasPath = StringUtils::format("spine/%s.atlas", nameStr.c_str());
    SkeletonAnimation* monster = SkeletonAnimation::createWithJsonFile(jsonPath, atlasPath, 0.5f);
    monster->setAnimation(0, animationName, true);
    monster->setPosition(randomMonsterPos());
    monster->setLocalZOrder((int)m_gamePlayData->_monsterList.size() + 1);
    backstage->addChild(monster);
    
    return monster;
}

void MyWordGame::updateMonsterZorder(float t)
{
    if( m_isbeginTime ) {
        m_gameResultData->_usedTime++;
    }
    
    //zorder排序
    std::sort(m_gamePlayData->_monsterList.begin(), m_gamePlayData->_monsterList.end(), [](monsterUnit *a, monsterUnit *b) -> bool{
        return a->_monster->getPositionY() > b->_monster->getPositionY();
    });
    int count = (int)m_gamePlayData->_monsterList.size();
    int maxZorder = count;
    for (int i = 0; i < count; i++) {
        m_gamePlayData->_monsterList[i]->_monster->setLocalZOrder(maxZorder);
        maxZorder--;
    }
}

Vec2 MyWordGame::randomMonsterPos()
{
    Layout* panel_bottom = dynamic_cast<Layout*>(m_ui->getChildByName(BOTTOM_PANEL_NAME));
    auto backstage = dynamic_cast<Sprite*>(panel_bottom->getChildByName("newWordGame_icon_backstage"));
    auto backstageSize = backstage->getContentSize();
    float radomX = random(backstageSize.width*0.2, backstageSize.width*0.8);
    float radomY = random(backstageSize.height*0.29, backstageSize.height*0.32);
    Vec2 radomPos = Vec2(radomX, radomY);
    return radomPos;
}

void MyWordGame::setOneMonster(SkeletonAnimation* monster, eMonsterStatus status)
{
    if (nullptr == monster) return;
    switch(status) {
        case eMonsterStatus::sleep:
            monster->stopAllActions();
            monster->setAnimation(0, "sleep", true);
            break;
        case eMonsterStatus::move: {
            monster->setAnimation(0, "walk", true);
            
            auto callfun = CallFunc::create([=](){
                setOneMonster(monster, eMonsterStatus::stand);
            });
            Vec2 pos = randomMonsterPos();
            auto m1 = MoveTo::create(5 + random(0, 1) * 5, pos);
            monster->runAction(Sequence::create(m1, callfun, NULL));
        }
            break;
        case eMonsterStatus::stand: {
            monster->setAnimation(0, "free", true);
            
            auto callfun = CallFunc::create([=](){
                setOneMonster(monster, eMonsterStatus::erand);
            });
            Vec2 pos = monster->getPosition();
            auto m1 = MoveTo::create(2 + random(0, 1) * 2, pos);
            monster->runAction(Sequence::create(m1, callfun, NULL));
        }
            break;
        case eMonsterStatus::erand: {
            if (random(1, 2) == 1) {
                setOneMonster(monster, eMonsterStatus::move);
            }else {
                setOneMonster(monster, eMonsterStatus::stand);
            }
        }
            break;
        case eMonsterStatus::win: {
            monster->setAnimation(0, "win", true);
            auto callfun = CallFunc::create([=](){
                setOneMonster(monster, eMonsterStatus::erand);
            });
            monster->runAction(Sequence::create(DelayTime::create(2), callfun, NULL));
        }
            break;
        case eMonsterStatus::fail: {
            monster->setAnimation(0, "fail", true);
            auto callfun = CallFunc::create([=](){
                setOneMonster(monster, eMonsterStatus::erand);
            });
            monster->runAction(Sequence::create(DelayTime::create(2), callfun, NULL));
        }
            break;
        default:
            break;
    }
}

void MyWordGame::getDataFromSever(int flag)
{
    HttpDataToString dataStr;
    dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
    if (flag == 0) {
        CDataHttp::getInstance()->HttpGet(eHttpType::getNewWordsGameRule, dataStr.getString());
    }else if (flag == 1) {
        dataStr.addData("gold_coin", StringUtils::format("%d", m_gamePlayData->_payGem));
        CDataHttp::getInstance()->HttpPost(eHttpType::getNewWordsGameStart, dataStr.getString());
    }else {
        std::string r_words = "";
        std::string w_words = "";
        
        int rcount = (int)m_gamePlayData->_rightWordVect.size();
        if (rcount > 0) {
            for (int i = 0; i < rcount; i++) {
                r_words = r_words + StringUtils::format("%d", m_gamePlayData->_rightWordVect[i]);
                if (i != (rcount - 1)) {
                    r_words = r_words + ",";
                }
            }
        }
        dataStr.addData("r_words", r_words);
        
        int wcount = (int)m_gamePlayData->_wrongWordVect.size();
        if (wcount > 0) {
            for (int i = 0; i < wcount; i++) {
                w_words = w_words + StringUtils::format("%d", m_gamePlayData->_wrongWordVect[i]);
                if (i != (wcount - 1)) {
                    w_words = w_words + ",";
                }
            }
        }
        dataStr.addData("w_words", w_words);
        
        dataStr.addData("time", StringUtils::format("%ld", m_gameResultData->_usedTime));
        CDataHttp::getInstance()->HttpPost(eHttpType::getNewWordsGameEnd, dataStr.getString());
    }
}

void MyWordGame::endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json)
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
            
            this->unschedule(CC_SCHEDULE_SELECTOR(MyWordGame::updateMonsterZorder));
            m_gameStatus = eStatusNone;
            clearDataList();
            initData(data);
            setGameStatus(ePlayGameStatus::eStatusInit);
        }
    }else if (nOp == eHttpType::getNewWordsGameStart) {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e) {
            
            stuJson returnData = json.getJson("return_data");
            stuJson returnWords = returnData.getJson("words");
            
            stuJson returnURLdata = returnData.getJson("url");
            string imgUrl = returnURLdata.getValue("word_img_url");
            string audioUrl = returnURLdata.getValue("word_audio_url");
            
            int wordsCount = returnData.GetArraySize("words");
            for (int i = 0; i < wordsCount; i++) {
                stuJson one = returnData.GetArrayJson("words", i);
                GameSubject* item = new GameSubject();
                item->_id = atoi(one.getValue("id").c_str());
                item->_name = one.getValue("word");
                item->_desc = one.getValue("discription_short");
                item->_audioPath = audioUrl + one.getValue("audio");
                m_gamePlayData->_subjectList.push_back(item);
                DownImg::getInstance()->DownFile(item->_audioPath.c_str(), nullptr);
            }
            m_gamePlayData->_curSubjectCount = wordsCount;
            
        }else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            int return_code = atoi(json.getValue("return_code").c_str());
            
            if (return_code == 6051) { //宝石数量不够
                std::string return_msg = json.getValue("return_msg");
                TipManager::getInstance()->showMsgTips(return_msg.c_str());
                return;
            }
        }
        if (m_gamePlayData->_subjectList.size() <= 0) {
//            TipManager::getInstance()->showMsgTips("需要10个生词以上才可以开始游戏哦~");
            auto dialog = AlertDialog::create("需要10个生词以上才可以开始游戏哦，去读书获得更多的生词吧!");
            dialog->setOkCallBack(CC_CALLBACK_0(MyWordGame::enterMainPage, this));
            m_ui->getParent()->addChild(dialog);
            return;
        }
        //animation
        Layout* panel_center = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME));
        Layout* panel_begin = dynamic_cast<Layout*>(panel_center->getChildByName("panel_begin"));
        Layout* panel_paygem = dynamic_cast<Layout*>(panel_begin->getChildByName("panel_paygem"));
        float panel_paygemY = panel_paygem->getPositionY();
        auto m1 = MoveTo::create(0.25f, Vec2(panel_paygem->getPositionX(), panel_paygem->getPositionY() - 80.0f));
        auto f1 = FadeTo::create(0.25f, 0);
        auto call = CallFunc::create([=](){
            panel_paygem->setPositionY(panel_paygemY);
        });
        panel_paygem->runAction(Sequence::create(Spawn::create(m1, f1, NULL), call, NULL));
        
        int coin = UserManager::getInstance()->getUserEnt()->_Coin;
        UserManager::getInstance()->getUserEnt()->_Coin = coin - m_gamePlayData->_payGem;
        updateUserGemInfo();
        
        //start timer
        m_isbeginTime = true;
        m_gameResultData->_usedTime = 0;

        SoundManager::getInstance()->playSoundEffect("music/snd_nuts.mp3");
        setGameStatus(ePlayGameStatus::eStatusStart);
        SoundManager::getInstance()->playBackSound("music/biggamebg.mp3", true);
    }else if (nOp == eHttpType::getNewWordsGameEnd) {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            long usedTime = m_gameResultData->_usedTime;
            
            m_gameResultData = new GameResultData();
            
            stuJson returnData = json.getJson("return_data");
            stuJson returnMonsterPoint = returnData.getJson("user_monster_point");
            int point = atoi(returnMonsterPoint.getValue("point").c_str());
            int total_point = atoi(returnMonsterPoint.getValue("total_point").c_str());
            string upgrade_flag = returnMonsterPoint.getValue("upgrade_flag");
            int m_id = atoi(returnMonsterPoint.getValue("m_id").c_str());
            
            m_gameResultData->_monsterId = m_id;
            m_gameResultData->_monsterName = StringUtils::format("monster%d", m_id);
            m_gameResultData->_monsterPoint = point;
            m_gameResultData->_monsterTotalPoint = total_point;
            if (upgrade_flag == "true") {
                m_gameResultData->_bUpgradeFlag = true;
            }
            
            int updateWordsCount = returnData.GetArraySize("upgrade_words");
            for (int i = 0; i < updateWordsCount; i++) {
                stuJson one = returnData.GetArrayJson("upgrade_words", i);
                int updateId = atoi(one.getValue("id").c_str());
                string word = one.getValue("word");
                int skill_level = atoi(one.getValue("skill_level").c_str());

                UpgradeWordsStruct* item = new UpgradeWordsStruct();
                item->_id = updateId;
                item->_word = word;
                item->_skillLevel = skill_level;
                m_gameResultData->_upgradWordsList.push_back(item);
            }
            
            m_gameResultData->_usedTime = usedTime;
            m_gameResultData->_correctRate = (m_gamePlayData->_rightSubject * 100)/m_gamePlayData->_curSubjectCount;
            
            setGameStatus(ePlayGameStatus::eStatusEndEgg);
        }else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            std::string return_msg = json.getValue("return_msg");
//            TipManager::getInstance()->showMsgTips(return_msg.c_str());
            //错误，直接读取下一轮游戏开始数据
            getDataFromSever(0);
        }
    }else if (nOp == eHttpType::commonApplePayCheck) { //苹果支付后验证 or 订单状态查询
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            stuJson returnData = json.getJson("return_data");
            string payResult = returnData.getValue("flag");
            int gemCount = atoi(returnData.getValue("coins").c_str());
            if (0 == payResult.compare("true")) {
                UserManager::getInstance()->getUserEnt()->_Coin = gemCount;
                updateUserGemInfo();
            }
#endif
        }
    }else if (nOp == eHttpType::queryPayStatus) { //
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            stuJson returnData = json.getJson("return_data");

            int iPayResult = atoi(returnData.getValue("status").c_str());
            int gemCount = atoi(returnData.getValue("coins").c_str());
            
            if ( iPayResult == 1 ) {
                UserManager::getInstance()->getUserEnt()->_Coin = gemCount;
                updateUserGemInfo();
            }
        }
    }
}

void MyWordGame::enterMainPage()
{
    SoundManager::getInstance()->pauseBackSound();
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    m_ui->removeFromParent();
}
