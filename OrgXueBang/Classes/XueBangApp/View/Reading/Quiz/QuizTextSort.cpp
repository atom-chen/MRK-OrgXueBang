//
//  QuizTextSort.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/22.
//
//
#include "GameMacro.hpp"

#include "QuizTextSort.h"

#include "UiBase.h"

#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

QuizTextSort *QuizTextSort::create(Node* parent)
{
    QuizTextSort *ret = new (std::nothrow) QuizTextSort();
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

bool QuizTextSort::init()
{
    m_ui = CSLoader::createNode("ui/quiz/layer_textSort.csb");
    m_ui->addChild(this, -1);
    
    Size parentSize = m_parent->getContentSize();
    m_ui->setContentSize(parentSize);
    Helper::doLayout(m_ui);
    
    
//    setParam("", "", FileUtils::getInstance()->getWritablePath() + "test_textsort.xml");
    return true;
}

void QuizTextSort::setQuestion()
{
    
    int y = 20, h;
    cocos2d::ui::ScrollView* scroll_question = dynamic_cast<cocos2d::ui::ScrollView*>(m_ui->getChildByName("scroll_question"));
    Size scrollSize = scroll_question->getContentSize();
    
    Size size3;

    static Vec2 p;

    for (int i = (int)m_answer.size() - 1; i >= 0; i--) {
        Node* answerNode = CSLoader::createNode("ui/quiz/node_textAnswer1.csb");
        scroll_question->addChild(answerNode);
        Layout* node = dynamic_cast<Layout*>(answerNode->getChildByName("panel_answer"));
        
        node->setSwallowTouches(false);
        node->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType eventType){
            if (eventType == Widget::TouchEventType::BEGAN) {
                p = ((Node*)pSender)->convertToWorldSpace(Vec2(0, 0));
            } else if (eventType == Widget::TouchEventType::ENDED) {
                Vec2 newPos = ((Node*)pSender)->convertToWorldSpace(Vec2(0, 0));
                if (std::abs(newPos.x - p.x) < 50 && std::abs(newPos.y - p.y) < 50) {
                    this->onSelect(i);
                }
            }
        });
        
        Text* text = dynamic_cast<Text*>(node->getChildByName("text_item"));
        string answer;
        answer.push_back('A' + i);
        text->setString(answer);
        
        Text* textAnswer = dynamic_cast<Text*>(node->getChildByName("text_content"));
        textAnswer->setString(m_answer[i].text);
        
        size3 = node->getContentSize();
        node->getParent()->setPosition(Vec2(0, y));
        y += size3.height + 20;
        
        
    }
    
    
    Layout* panel_question = dynamic_cast<Layout*>(scroll_question->getChildByName("panel_question"));
    Text* text_question = dynamic_cast<Text*>(panel_question->getChildByName("text_question"));
    
    h = UiBase::setText(text_question, m_question);
    h += 20;
    size3 = panel_question->getContentSize();
    size3.height = h;
    panel_question->setContentSize(size3);
    panel_question->setPosition(Vec2(0, y));
    y += size3.height;
    
    
    scroll_question->setInnerContainerSize(Size(scrollSize.width, y));
    
    
}

void QuizTextSort::onSelect(int n)
{
    CCLOG("select %d, score = %d", n, m_answer[n].score);
    if (m_answer[n].score == 100) {
//        m_ui->removeFromParent();
    }
}

void QuizTextSort::setParam(std::string imgFile, std::string voiceFile, std::string xmlFile)
{
    XMLDocument pDoc;
    XMLError errorId = pDoc.LoadFile(xmlFile.c_str());
    
    if (errorId != 0) {
        CCLOG("xml %s load failed ", xmlFile.c_str());
        return;
    }
    m_question = "";
    m_answer.clear();
    
    XMLElement* root = pDoc.RootElement();
    
    
    do {
        if (!root) {
            break;
        }
        
        XMLElement* question = root->FirstChildElement("question");
        if (!question) {
            break;
        }
        const char* textAttr = question->Attribute("text");
        if (!textAttr) {
            break;
        }
        m_question = textAttr;
        UiBase::replace_all_distinct(m_question, "%3Cbr%20/%3E", "\n\n");
        m_question +=  "\n\n";
        
        XMLElement* answer = question->FirstChildElement("answer");
        while (answer) {
            
            const char* textAns = answer->Attribute("text");
            if (textAns) {
                CCLOG("answer = %s", textAns);
                stuQuizAnswerText ans;
                ans.score = atoi(answer->Attribute("score"));
                ans.text = textAns;
                m_answer.push_back(ans);
            }
            answer = answer->NextSiblingElement("answer");
        }
    } while (0);
    
    if (m_question.length() < 1 || m_answer.size() < 1) {
        return;
    }
    
    setQuestion();
    
}


