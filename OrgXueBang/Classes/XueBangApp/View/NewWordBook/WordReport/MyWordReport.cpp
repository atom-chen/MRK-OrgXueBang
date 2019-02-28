//
//  MyWordReport.cpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/14.
//

#include "MyWordReport.hpp"
#include "GameMacro.hpp"
#include "UserManager.hpp"
#include "UiBase.h"
//#include "MyWordReportShare.hpp"

MyWordReport *MyWordReport::create(Node* parent)
{
    talkingInterface::pageBegin("生词报告");
    
    auto node = parent->getChildByName("MyWordReport");
    if (node) {
        return (MyWordReport *)node->getChildByName("MyWordReport");
    }
    
    MyWordReport *ret = new (std::nothrow) MyWordReport();
    if (ret && ret->init()) {
        parent->addChild(ret->m_ui);
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

MyWordReport::~MyWordReport()
{
    talkingInterface::pageEnd("生词报告");
}

bool MyWordReport::OnBaseKeyBack()
{
    this->m_ui->removeFromParent();
    return true;
}

bool MyWordReport::init()
{
    m_ui = CSLoader::createNodeWithVisibleSize("View/NewWordBook/layer_myWordReport.csb");
    UiBase::resizeBackImg(m_ui);
    this->setName("MyWordReport");
    m_ui->addChild(this, -1);
    
    m_ui->setName("MyWordReport");
    
    createReportInfo();

    auto title_name = dynamic_cast<Text*>(m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("node_userTitle")->getChildByName("panel_top")->getChildByName("title_name"));
    title_name->setString("单词报告");
    
    auto btn_back = dynamic_cast<Widget*>(m_ui->getChildByName(TOP_PANEL_NAME)->getChildByName("node_userTitle")->getChildByName("panel_top")->getChildByName("btn_back"));
    UiBase::addClickEventListener(btn_back, [=](Ref* pSender){
        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
        m_ui->removeFromParent();
    });
    
//    auto btn_share = dynamic_cast<Button*>(m_ui->getChildByName(BOTTOM_PANEL_NAME)->getChildByName("btn_share"));
//    UiBase::addClickEventListener(btn_share, [=](Ref* pSender){
//        SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
//        MyWordReportShare::create(m_ui->getParent());
//    });
    
    getDataFromSever();
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_ui);
    
    return true;
}

void MyWordReport::onEnter()
{
    Node::onEnter();
}

void MyWordReport::createReportInfo()
{
    float panelHight = m_ui->getContentSize().height;
    
    Layout* panel_center = dynamic_cast<Layout*>(m_ui->getChildByName(CENTER_PANEL_NAME));
    Layout* panel_bottom = dynamic_cast<Layout*>(m_ui->getChildByName(BOTTOM_PANEL_NAME));
    
    m_panel_centerY = panel_center->getPositionY();
    m_panel_bottomY = panel_bottom->getPositionY();
    
    panel_center->setPositionY(panelHight);
    panel_bottom->setPositionY(-panelHight);
    
    auto move1 = MoveTo::create(0.25, Vec2(panel_center->getPositionX(), m_panel_centerY));
    panel_center->runAction(EaseSineIn::create(move1));

    auto move2 = MoveTo::create(0.3, Vec2(panel_bottom->getPositionX(), m_panel_bottomY));
    panel_bottom->runAction(EaseSineIn::create(move2));
}

void MyWordReport::showReportInfo()
{
    auto panel_center = (Layout*)m_ui->getChildByName(CENTER_PANEL_NAME);
    Layout* panel_info1 = dynamic_cast<Layout*>(panel_center->getChildByName("panel_info1"));
    Layout* panel_info2 = dynamic_cast<Layout*>(panel_center->getChildByName("panel_info2"));

    auto txt_learn = dynamic_cast<Text*>(panel_info1->getChildByName("txt_learn"));
    txt_learn->setString(StringUtils::format("%d", m_todayLearn));

    auto txt_upgrade = dynamic_cast<Text*>(panel_info1->getChildByName("txt_upgrade"));
    txt_upgrade->setString(StringUtils::format("%d", m_todayUpgrade));

    auto txt_rate = dynamic_cast<Text*>(panel_info1->getChildByName("txt_rate"));
    txt_rate->setString(m_gameRate);

    auto txt_totalwords = dynamic_cast<Text*>(panel_info1->getChildByName("txt_totalwords"));
    txt_totalwords->setString(StringUtils::format("%d", m_totalWords));
    
    auto txt_level1 = dynamic_cast<Text*>(panel_info2->getChildByName("txt_level1"));
    txt_level1->setString(StringUtils::format("%d/%d", m_level1Words, m_totalWords));

    auto txt_level2 = dynamic_cast<Text*>(panel_info2->getChildByName("txt_level2"));
    txt_level2->setString(StringUtils::format("%d/%d", m_level2Words, m_totalWords));

    auto txt_level3 = dynamic_cast<Text*>(panel_info2->getChildByName("txt_level3"));
    txt_level3->setString(StringUtils::format("%d/%d", m_level3Words, m_totalWords));
    
    LoadingBar* level1Bar = (LoadingBar*)panel_info2->getChildByName("loadbar_level1_bg")->getChildByName("loadbar_level1");
    if (m_totalWords > 0) {
        level1Bar->setPercent((m_level1Words * 100)/m_totalWords);
    }else {
        level1Bar->setPercent(0.0f);
    }

    LoadingBar* level2Bar = (LoadingBar*)panel_info2->getChildByName("loadbar_level2_bg")->getChildByName("loadbar_level2");
    if (m_totalWords > 0) {
        level2Bar->setPercent((m_level2Words * 100)/m_totalWords);
    }else {
        level2Bar->setPercent(0.0f);
    }

    LoadingBar* level3Bar = (LoadingBar*)panel_info2->getChildByName("loadbar_level3_bg")->getChildByName("loadbar_level3");
    if (m_totalWords > 0) {
        level3Bar->setPercent((m_level3Words * 100)/m_totalWords);
    }else {
        level3Bar->setPercent(0.0f);
    }
}

void MyWordReport::getDataFromSever()
{
    HttpDataToString dataStr;
    dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
    CDataHttp::getInstance()->HttpGet(eHttpType::getNewWordsReportV2, dataStr.getString());
}

void MyWordReport::endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json)
{
    if (nOp == eHttpType::getNewWordsReportV2)
    {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e) {

            stuJson returnData = json.getJson("return_data");
            stuJson returnPoster = returnData.getJson("poster");

            m_todayLearn = atoi(returnPoster.getValue("today_learn").c_str());
            m_todayUpgrade = atoi(returnPoster.getValue("today_upgrade").c_str());
            m_gameRate = returnPoster.getValue("game_rate");
            m_totalWords = atoi(returnPoster.getValue("total_words").c_str());

            int n = returnPoster.GetArraySize("level_words");
            for (int i = 0; i < n; i++) {
                int key = atoi(returnPoster.GetArrayValue("level_words", i).c_str());
                if (i == 0) {
                    m_level0Words = key;
                }else if(i == 1) {
                    m_level1Words = key;
                }else if(i == 2) {
                    m_level2Words = key;
                }else if(i == 3) {
                    m_level3Words = key;
                }
            }
            showReportInfo();
        }
    }
}
