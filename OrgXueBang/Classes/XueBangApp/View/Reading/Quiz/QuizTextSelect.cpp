//
//  QuizTextSelect.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/16.
//
//
#include "GameMacro.hpp"

#include "QuizTextSelect.h"

#include "SoundManager.hpp"

#include "UiBase.h"

#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

#include "TipManager.hpp"

QuizTextSelect *QuizTextSelect::create(Node* parent, eQuizType quizType)
{
    QuizTextSelect *ret = new (std::nothrow) QuizTextSelect();
    ret->m_quizType = quizType;
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


bool QuizTextSelect::init()
{

    
    m_ui = CSLoader::createNode("ui/quiz/layer_textSelect.csb");
    m_ui->addChild(this, -1);
    
//    Size parentSize = m_parent->getContentSize();
//    m_ui->setContentSize(parentSize);
//    Helper::doLayout(m_ui);
    

//    setParam("", "", FileUtils::getInstance()->getWritablePath() + "test_textselect.xml");
    cocos2d::ui::ScrollView* scroll_question = dynamic_cast<cocos2d::ui::ScrollView*>(m_ui->getChildByName("scroll_question"));
//    scroll_question->setBackGroundImageSize(Size(750, 1334));
    scroll_question->setLockDirection(true);

//    auto bg = CSLoader::createNode("ui/quiz/node_bg.csb");
//    float offy = (DESIGN_RESOLUTION_HEIGHT - parentSize.height) / 2;
//    bg->setPosition(Vec2(parentSize.width / 2, parentSize.height / 2 + offy));
//    scroll_question->Layout::addChild(bg, -1);
    
    auto panel_img = scroll_question->getChildByName("panel_img");
    ImageView* img_sound = dynamic_cast<ImageView*>(m_ui->getChildByName("img_sound"));
    UiBase::addAudioEventListener(img_sound, [=](Ref* pSender){
        CCLOG("play sentence audio !");
        
        int soundId = this->playSentenceAudio();
        if (soundId == AudioEngine::INVALID_AUDIO_ID) {
            TipManager::getInstance()->showTipWithString("文件下载中，请稍后再试。");
        }
    });
    
    switch (m_quizType) {
        case eQuizFillIn:
            img_sound->setVisible(false);
            break;
        case eQuizListen:
            break;
        case eQuizPicText:
            img_sound->setVisible(false);
            break;
        case eQuizListenFill:
            panel_img->setVisible(false);
            break;
        case eQuizListenSelect:
            panel_img->setVisible(false);
            break;
        case eQuizTextSort:
            panel_img->setVisible(false);
            img_sound->setVisible(false);
            break;
        case eQuizTextSelect:
            panel_img->setVisible(false);
            img_sound->setVisible(false);
            break;
        case eQuizWordSelect:
            img_sound->setVisible(false);
            break;
        default:
            CCLOG("this quiztype %d not support for QuizTextSelect", m_quizType);
            panel_img->setVisible(false);
            img_sound->setVisible(false);
            break;
    }
    return true;
}


void QuizTextSelect::setQuestion()
{
    
    int y = 300, h;
    cocos2d::ui::ScrollView* scroll_question = dynamic_cast<cocos2d::ui::ScrollView*>(m_ui->getChildByName("scroll_question"));
//    scroll_question->setSwallowTouches(false);
    scroll_question->setScrollBarEnabled(false);
    Size scrollSize = scroll_question->getContentSize();
    
    Size size3;
    
    static Vec2 p;

    m_errorCount = 0;
    m_optionCount = (int)m_answer.size();
    
    for (int i = (int)m_answer.size() - 1; i >= 0; i--) {
        
        Node* answerNode = CSLoader::createNode("ui/quiz/node_textAnswer.csb");
        scroll_question->addChild(answerNode);
        m_answer[i].node = answerNode;
//        m_answer[i].node->getChildByName("panel_node")->getChildByName("img_txt")->getChildByName("img_select")->setLocalZOrder(-1);
        Layout* node = dynamic_cast<Layout*>(answerNode->getChildByName("panel_node"));
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
        
        node->getChildByName("img_point")->getChildByName("img_select")->setVisible(false);
        node->getChildByName("img_select")->setVisible(false);
        
        Text* text = dynamic_cast<Text*>(node->getChildByName("text_answer"));
        h = UiBase::setText(text, m_answer[i].text, 101 - 40);

        size3 = node->getContentSize();
        size3.height = h + 40;
        node->setContentSize(size3);
        Helper::doLayout(node);
        node->getParent()->setPosition(Vec2(30, y));
        y += size3.height + 20;
        
//        auto img_txt = node->getChildByName("img_txt");
//        Text* text_select = dynamic_cast<Text*>(img_txt->getChildByName("text_select"));
//        Vec2 p = img_txt->getPosition();
//        p.y = size3.height - 60;
//        img_txt->setPosition(p);
//
//        string answer;
//        answer.push_back('A' + i);
//        text_select->setString(answer);
    }
    
    Layout* panel_question = dynamic_cast<Layout*>(scroll_question->getChildByName("panel_question"));
    Text* text_question = dynamic_cast<Text*>(panel_question->getChildByName("text_question"));
//    m_question += "Hello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\nHello wolrd !\n";
    
//    h += 50;
    h = UiBase::setText(text_question, m_question, 100);
//    if (h >=200) {
//        h +=120;
//    }
    h += 50;
    
    size3 = panel_question->getContentSize();
    size3.height = h;
    panel_question->setContentSize(size3);
    y += 100;
    panel_question->setPosition(Vec2(0, y));
    y += size3.height;
    
    auto panel_img = scroll_question->getChildByName("panel_img");
    if (panel_img->isVisible()) {
        size3 = panel_img->getContentSize();
        panel_img->setPosition(Vec2(375, y));
        y += size3.height + 30;
    }
    
//    ImageView* img_sound = dynamic_cast<ImageView*>(scroll_question->getChildByName("img_sound"));
//    if (img_sound->isVisible()) {
//        size3 = img_sound->getContentSize();
//        img_sound->setPosition(Vec2(375, y));
//        y += size3.height + 30;
//    }

    y += 100;   //让开顶部栏
    scroll_question->setInnerContainerSize(Size(scrollSize.width, y));
    
    if (y > scrollSize.height) {
        scroll_question->jumpToBottom();
        scroll_question->jumpToTop();
    }
    
    setScore(getScore());
}

void QuizTextSelect::onSelect(int n)
{
    CCLOG("select %d, score = %d", n, m_answer[n].score);
    if (m_answer[n].score == 100) {
        CCLOG("success !!!!");
        setScore(100);
        __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_NUT, (Ref*)(intptr_t)1);
        playAnswerAudio(true);
        
    } else {
        setScore(0);
        __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_NUT, (Ref*)(intptr_t)0);
        playAnswerAudio(false);
        m_errorCount++;
    }
    
    for (int i = 0; i < m_answer.size(); i++) {
        if (i == n) {
            m_answer[i].node->getChildByName("panel_node")->getChildByName("img_point")->getChildByName("img_select")->setVisible(true);
            m_answer[i].node->getChildByName("panel_node")->getChildByName("img_select")->setVisible(true);
        } else {
            m_answer[i].node->getChildByName("panel_node")->getChildByName("img_point")->getChildByName("img_select")->setVisible(false);
            m_answer[i].node->getChildByName("panel_node")->getChildByName("img_select")->setVisible(false);
        }
    }
}

void QuizTextSelect::setParam(std::string imgFile, std::string voiceFile, std::string xmlFile)
{
    if (imgFile.length() > 0) {
        ImageView* img_pic = dynamic_cast<ImageView*>(m_ui->getChildByName("scroll_question")->getChildByName("panel_img")->getChildByName("img_picture"));
        if (img_pic->isVisible()) {
//            img_pic->loadTexture(imgFile);
            UiBase::LoadTexture(img_pic, imgFile, img_pic->getContentSize());
        }
        return;
    }
    if (xmlFile.length() < 1) {
        return;
    }
    
    if (m_quizType == eQuizWordSelect) {
        anotherXml(xmlFile);
        return;
    }
    
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
        
//        int question_number = atoi(m_wholeBook->m_bookPages->at(m_pageIndex)->m_paramJson.getValue("question_number").c_str());
//        while (question_number > 1) {
//            question = question->NextSiblingElement();
//            if (!question) {
//                break;
//            }
//            question_number--;
//        }
        
        string question_number = m_wholeBook->m_bookPages->at(m_pageIndex)->m_paramJson.getValue("question_number");
        if (question_number != "1" && question_number != "-1") {
            while (question->Attribute("id") != question_number) {
                question = question->NextSiblingElement();
                if (!question) {
                    return;
                }
            }
        }
        
        
        const char* textAttr = question->Attribute("text");
        if (!textAttr) {
            break;
        }
        m_question = textAttr;
//        UiBase::replace_all_distinct(m_question, "%3Cbr%20/%3E", "\n\n");
        
        UiBase::removeUrlChar(m_question);
        
        m_question +=  "\n\n";
        
        const char* taskAudio = question->Attribute("audio");
        if (taskAudio) {
            m_audioFile = taskAudio;
            
            m_wholeBook->downQuizSound1(m_pageIndex, taskAudio);
        } else {
            m_audioFile = "";
        }
        
        XMLElement* answer = question->FirstChildElement("answer");
        while (answer) {

            const char* textAns = answer->Attribute("text");
            if (textAns) {
                CCLOG("answer = %s", textAns);
                stuQuizAnswerText ans;
                ans.score = atoi(answer->Attribute("score"));
                ans.text = textAns;
                UiBase::removeUrlChar(ans.text);
                m_answer.push_back(ans);
            }
            answer = answer->NextSiblingElement("answer");
        }
    } while (0);
    
    if (m_question.length() < 1 || m_answer.size() < 1) {
        return;
    }
    
    setQuestion();
    
    QuizBase::setParam(imgFile, voiceFile, xmlFile);
}

int QuizTextSelect::inPage()
{
    QuizBase::inPage();
    
    if (!m_bXmlRead) {
        return AudioEngine::INVALID_AUDIO_ID;
    }
    
    int soundId = AudioEngine::INVALID_AUDIO_ID;
    ImageView* img_sound = dynamic_cast<ImageView*>(m_ui->getChildByName("img_sound"));
    if (img_sound->isVisible()) {
        soundId = playSentenceAudio();
    }
    return soundId;
}

int QuizTextSelect::playSentenceAudio()
{
    if (m_soundId != AudioEngine::INVALID_AUDIO_ID) {
        SoundManager::getInstance()->stopSound(m_soundId);
    }
    int soundId = AudioEngine::INVALID_AUDIO_ID;
    //音效路径
    
    if (m_audioFile.length() < 4) {
        string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
        
        if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
            SoundManager::getInstance()->VolumeBackSound(1.0f);
            soundId = SoundManager::getInstance()->playSound(strPathSnd);
        }
        m_soundId = soundId;
        return soundId;
    }
    string strPathSnd = m_wholeBook->getQuizSoundPath1(m_pageIndex, m_audioFile);
    
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        soundId = SoundManager::getInstance()->playSound(strPathSnd);
    }
    m_soundId = soundId;
    return soundId;
}

void QuizTextSelect::anotherXml(std::string &xmlFile)
{
    XMLDocument pDoc;
    XMLError errorId = pDoc.LoadFile(xmlFile.c_str());
    
    if (errorId != 0) {
        CCLOG("xml %s load failed ", xmlFile.c_str());
        return;
    }
    
    XMLElement* root = pDoc.RootElement();
    do {
        if (!root) {
            break;
        }
        XMLElement* curEle = root->FirstChildElement("tasks");
        if (!curEle) {
            break;
        }
        curEle = curEle->FirstChildElement("task");
        if (!curEle) {
            break;
        }
        
        XMLElement* targets = curEle->FirstChildElement("targets");
        if (!targets) {
            break;
        }
        XMLElement* target = targets->FirstChildElement("target");
        int qId = 0;
        while (target) {
            const char* textAttr = target->Attribute("text");
            if (!textAttr) {
                break;
            }
            m_question = textAttr;
            qId = atoi(target->Attribute("id"));
            target = target->NextSiblingElement("target");
        }
        
        XMLElement* matches = curEle->FirstChildElement("matches");
        if (!matches) {
            break;
        }
        XMLElement* match = matches->FirstChildElement();
        while (match) {
            
            const char* textAns = match->Attribute("text");
            if (textAns) {
                CCLOG("answer = %s", textAns);
                stuQuizAnswerText ans;
                int aId = atoi(match->Attribute("id"));
                ans.score = (qId == aId) ? 100 : 0;
                ans.text = textAns;
                UiBase::removeUrlChar(ans.text);
                
                if (cocos2d::random()%2) {
                    m_answer.push_back(ans);
                } else {
                    m_answer.insert(m_answer.begin(), ans);
                }
                
            }
            match = match->NextSiblingElement();
        }
    } while (0);
    
    if (m_question.length() < 1 || m_answer.size() < 1) {
        return;
    }
    
    setQuestion();
    
    QuizBase::setParam("", "", xmlFile);
}

