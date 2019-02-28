//
//  QuizListenSelect.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/26.
//
//
#include "GameMacro.hpp"

#include "QuizListenSelect.h"


QuizListenSelect *QuizListenSelect::create(Node* parent)
{
    QuizListenSelect *ret = new (std::nothrow) QuizListenSelect();
    ret->m_parent = parent;
    if (ret && ret->init())
    {
        parent->addChild(ret->m_ui);
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool QuizListenSelect::init()
{
    m_ui = CSLoader::createNode("ui/quiz/layer_listenSelect.csb");
    m_ui->addChild(this, -1);
    
    Size parentSize = m_parent->getContentSize();
    m_ui->setContentSize(parentSize);
    Helper::doLayout(m_ui);
    
    char name[24];
    for (int i = 1; i < 5; i++) {
        sprintf(name, "node_answer%d", i);
        Layout* node = dynamic_cast<Layout*>(m_ui->getChildByName(name)->getChildByName("panel_answer"));
        node->setSwallowTouches(false);
        node->addClickEventListener([=](Ref* pSender){
            this->onSelect(i);
        });
    }

    setQuestion();
    
    return true;
}

void QuizListenSelect::setQuestion()
{
    std::string answer[4] = {"A", "B", "C", "D"};
    std::string answer1[4] = {"this is answer A", "this is answer B", "this is answer C", "this is answer D"};
    
    char name[24];
    for (int i = 1; i < 5; i++) {
        sprintf(name, "node_answer%d", i);
        Layout* node = dynamic_cast<Layout*>(m_ui->getChildByName(name)->getChildByName("panel_answer"));
        Text* text = dynamic_cast<Text*>(node->getChildByName("text_item"));
        text->setString(answer[i - 1]);
        Text* text1 = dynamic_cast<Text*>(node->getChildByName("text_content"));
        text1->setString(answer1[i - 1]);
    }
}

void QuizListenSelect::onSelect(int n)
{
    CCLOG("select %d", n);
    if (n == 1) {
        m_ui->removeFromParent();
    }
}

void QuizListenSelect::setParam(string imgFile, string voiceFile, string xmlFile)
{
    
}




