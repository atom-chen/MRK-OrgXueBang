//
//  QuizSentence.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2017/1/12.
//
//
#include "GameMacro.hpp"

#include "QuizSentence.h"

#include "SoundManager.hpp"

#include "UiBase.h"

#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

#include "TipManager.hpp"

#include "NewHandTip.hpp"

QuizSentence* QuizSentence::create(Node* parent)
{
    QuizSentence *ret = new (std::nothrow) QuizSentence();
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


bool QuizSentence::init()
{
    m_ui = CSLoader::createNode("ui/quiz/layer_sentence.csb");
    m_ui->addChild(this, -1);
    
//    Size parentSize = m_parent->getContentSize();
//    m_ui->setContentSize(parentSize);
//    m_ui->setPositionY((m_parent->getContentSize().height - m_ui->getContentSize().height) / 2);
//    Helper::doLayout(m_ui);
    
    //    setWord();
//    setParam("", "", FileUtils::getInstance()->getWritablePath() + "test_worddrag.xml");
    Button* btn_sound = dynamic_cast<Button*>(m_ui->getChildByName("btn_sound"));
    UiBase::addAudioEventListener(btn_sound, [=](Ref* pSender){
        CCLOG("play sentence audio !");
        
        int soundId = this->playSentenceAudio();
        if (soundId == AudioEngine::INVALID_AUDIO_ID) {
            TipManager::getInstance()->showTipWithString("文件下载中，请稍后再试。");
        }
    });
    
    m_layer = Layer::create();
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    m_bDraging = false;
    listener->onTouchBegan = [this](Touch* touch, Event* event)
    {
        if (m_bDraging) {
            return false;
        }
        Vec2 touchPos = touch->getLocation();
        touchPos = m_ui->convertToNodeSpace(touchPos);
        this->m_touchPosOld.set(touchPos);
        m_bDraging = true;
//                CCLOG("touch began at %f,%f", touchPos.x, touchPos.y);
        return true;
    };
    listener->onTouchMoved = [this](Touch* touch, Event* event)
    {
        Vec2 touchPos = touch->getLocation();
        touchPos = m_ui->convertToNodeSpace(touchPos);
        this->m_touchPosNew.set(touchPos);
//                CCLOG("touch move at %f,%f", touchPos.x, touchPos.y);
    };
    listener->onTouchEnded = [this](Touch* touch, Event* event)
    {
        Vec2 touchPos = touch->getLocation();
        touchPos = m_ui->convertToNodeSpace(touchPos);
        this->m_touchPosNew.set(touchPos);
        m_bDraging = false;
//                CCLOG("touch end at %f,%f", touchPos.x, touchPos.y);
    };
    listener->onTouchCancelled = [this](Touch* touch, Event* event)
    {
        Vec2 touchPos = touch->getLocation();
        touchPos = m_ui->convertToNodeSpace(touchPos);
        this->m_touchPosNew.set(touchPos);
        m_bDraging = false;
//                CCLOG("touch cancel at %f,%f", touchPos.x, touchPos.y);
    };
    m_ui->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_layer);
    
    m_ui->addChild(m_layer, 10000);
    
    
    Layout* panel_target = dynamic_cast<Layout*>(m_ui->getChildByName("panel_target"));
    char name[16];
    for (int i = 0; i < MAX_WORD_QUIZSENTENCE; i++) {
        sprintf(name, "FileNode_%d", i + 1);
        Node* node = panel_target->getChildByName(name);
        node->setVisible(false);
    }
    Layout* panel_dragLetters = dynamic_cast<Layout*>(m_ui->getChildByName("panel_match"));
    panel_dragLetters->setVisible(false);
    return true;
}

QuizSentence::~QuizSentence()
{
    m_layer->removeFromParent();
}

void QuizSentence::setWord()
{

    Layout* panel_target = dynamic_cast<Layout*>(m_ui->getChildByName("panel_target"));
    char name[16];
    
    m_errorCount = 0;
    m_optionCount = m_letterDragNum;
    
    float maxW = 0;
    Layout* panel_dragLetters = dynamic_cast<Layout*>(m_ui->getChildByName("panel_match"));
    panel_dragLetters->setVisible(true);
    for (int i = 0; i < MAX_WORD_QUIZSENTENCE; i++) {
        sprintf(name, "FileNode_%d", i + 1);
        Node* node = panel_dragLetters->getChildByName(name);
        if (i < m_letterDragNum) {
            node->setVisible(true);
            m_letterDrag[i].letter = m_matches[m_matches[i].order - 1].text;
            m_letterDrag[i].node = node;
            m_letterDrag[i].m_canDrag = true;
            m_letterDrag[i].m_originPos = node->getPosition();
            m_letterDrag[i].audio = m_matches[m_matches[i].order - 1].audio;
            
            Text* text_letter = dynamic_cast<Text*>(m_letterDrag[i].node->getChildByName("text_word"));
            text_letter->setString(m_letterDrag[i].letter);
            
            auto tSize = text_letter->getContentSize();
            if (tSize.width > maxW) {
                maxW = tSize.width;
            }
            
            Layout* panel_back = dynamic_cast<Layout*>(m_letterDrag[i].node->getChildByName("panel_word"));
            panel_back->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
                Node* node = panel_back->getParent();
                node->setLocalZOrder(10);
                if (type == Widget::TouchEventType::BEGAN) {
                } else if (type == Widget::TouchEventType::MOVED) {
                    Vec2 oldPos = node->getPosition();
                    oldPos.x += this->m_touchPosNew.x - this->m_touchPosOld.x;
                    oldPos.y += this->m_touchPosNew.y - this->m_touchPosOld.y;
                    node->setPosition(oldPos);
                    this->m_touchPosOld.set(this->m_touchPosNew);
                } else {
                    Vec2 oldPos = node->getPosition();
                    Vec2 orgPos = m_letterDrag[i].m_originPos;
                    Rect rect(orgPos.x - 25, orgPos.y - 25, 50, 50);
                    if (rect.containsPoint(oldPos)) {
                        this->playAudio(i);
                    } else {
                        Vec2 wPos = node->getParent()->convertToWorldSpace(oldPos);
                        this->checkDrag(i, wPos);
                    }
                }
            });
        } else {
            node->setVisible(false);
        }
    }
    
    if (maxW < 100) {
        m_maxFont = 72;
    } else if (maxW < 120)  {
        m_maxFont = 60;
    } else if (maxW < 120)  {
        m_maxFont = 60;
    } else if (maxW < 150)  {
        m_maxFont = 48;
    } else if (maxW < 180)  {
        m_maxFont = 40;
    } else {
        m_maxFont = 36;
    }
    
    for (int i = 0; i < MAX_WORD_QUIZSENTENCE; i++) {
        sprintf(name, "FileNode_%d", i + 1);
        Node* node = panel_target->getChildByName(name);
        if (i < m_letterNum) {
            node->setVisible(true);
            m_letter[i].letter = m_matches[i].text;
            m_letter[i].state = 1;
            m_letter[i].node = node;
            Text* text_letter = dynamic_cast<Text*>(node->getChildByName("text_word"));
            text_letter->setFontSize(m_maxFont);
            m_letter[i].setState(m_letter[i].state);
        } else {
            node->setVisible(false);
        }
        
    }
    setScore(0);

}

void QuizSentence::QuizLetter::setState(int state)
{
    this->state = state;
    Text* text_letter = dynamic_cast<Text*>(node->getChildByName("text_word"));
    Layout* panel_back = dynamic_cast<Layout*>(node->getChildByName("panel_word"));
    auto img_back = dynamic_cast<ImageView*>(node->getChildByName("img_back"));
    auto img_blank = dynamic_cast<ImageView*>(node->getChildByName("img_blank"));
    panel_back->setVisible(false);
    
    switch (state) {
        case 0:
            text_letter->setTextColor(Color4B(0x52, 0x16, 0x22, 0xff));
            text_letter->setString(letter);
            text_letter->setVisible(true);
            img_blank->setVisible(false);
            img_back->setVisible(false);
            break;
        case 1:
            text_letter->setString("");
            img_blank->setVisible(true);
            img_back->setVisible(false);
            break;
        case 2:
            text_letter->setTextColor(Color4B(0, 0x80, 0xc3, 0xff));
            text_letter->setString(letter);
            text_letter->setVisible(true);
            img_blank->setVisible(false);
            img_back->setVisible(false);
            break;
            
        default:
            break;
    }
}

void QuizSentence::checkDrag(int n, cocos2d::Vec2 pos)
{
    Layout* panel_word = dynamic_cast<Layout*>(m_ui->getChildByName("panel_target"));
    Vec2 nPos = panel_word->convertToNodeSpace(pos);
    
    for (int i = 0; i < m_letterNum; i++) {
        if (m_letter[i].state == 1) {
            auto pos = m_letter[i].node->getPosition();
//            CCLOG("%f - 25 = %f", pos.y, pos.y - 25);
            Rect rect(pos.x - 120, pos.y - 50, 240, 100);
            if (rect.containsPoint(nPos)) {
                CCLOG("%s find %s", m_letterDrag[n].letter.c_str(), m_letter[i].letter.c_str());
                if (m_letterDrag[n].letter == m_letter[i].letter) {
                    m_letterDrag[n].node->setVisible(false);
                    m_letter[i].setState(2);
                    checkOk();
                    return;
                }
                else {
                    playAnswerAudio(false);
                    break;
                }
            }
        }
    }
    m_letterDrag[n].node->setLocalZOrder(1);
    m_letterDrag[n].node->setPosition(m_letterDrag[n].m_originPos);
    m_errorCount++;
}

void QuizSentence::checkOk()
{
    for (int i = 0; i < m_letterNum; i++) {
        if (m_letter[i].state == 1) {
            return;
        }
    }
    CCLOG("success !!!!");
    setScore(100);
//    m_ui->removeFromParent();
    __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_NUT, (Ref*)(intptr_t)1);
    playAnswerAudio(true);
}

void QuizSentence::setParam(std::string imgFile, std::string voiceFile, std::string xmlFile)
{
    if (xmlFile.length() < 1) {
        return;
    }
    
    XMLDocument pDoc;
    XMLError errorId = pDoc.LoadFile(xmlFile.c_str());
    
    if (errorId != 0) {
        CCLOG("xml %s load failed ", xmlFile.c_str());
        return;
    }
    m_targets.clear();
    m_matches.clear();
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
        
//        int task_number = atoi(m_wholeBook->m_bookPages->at(m_pageIndex)->m_paramJson.getValue("task_number").c_str());
//        while (task_number > 1) {
//            curEle = curEle->NextSiblingElement();
//            if (!curEle) {
//                break;
//            }
//            task_number--;
//        }
        
        string task_number = m_wholeBook->m_bookPages->at(m_pageIndex)->m_paramJson.getValue("task_number");
        if (task_number != "1" && task_number != "-1") {
            while (curEle->Attribute("id") !=task_number) {
                curEle = curEle->NextSiblingElement();
                if (!curEle) {
                    return;
                }
            }
        }
        
        
        const char* taskAudio = curEle->Attribute("audio");
        if (taskAudio) {
            m_audioFile = taskAudio;
            
            m_wholeBook->downQuizSound(m_pageIndex, taskAudio);
        } else {
            m_audioFile = "";
        }
        XMLElement* targets = curEle->FirstChildElement("targets");
        if (!targets) {
            break;
        }
        XMLElement* target = targets->FirstChildElement("target");
        while (target) {
            stuQuizTargetOne one;
            
            one.nId = atoi(target->Attribute("id"));
            m_targets.push_back(one);
            
            target = target->NextSiblingElement("target");
        }
        
        XMLElement* matches = curEle->FirstChildElement("matches");
        if (!matches) {
            break;
        }
        XMLElement* match = matches->FirstChildElement();
        while (match) {
            stuQuizMatchOne one;
            const char* tarText = match->Attribute("text");
            if (tarText) {
                one.text = tarText;
                one.nId = atoi(match->Attribute("id"));
//                one.order = atoi(match->Attribute("order"));
                one.order = (int)m_matches.size() + 1;
                const char* audio = match->Attribute("audio");
                if (audio) {
                    one.audio = audio;
                    
                    m_wholeBook->downQuizSound(m_pageIndex, audio);
                } else {
                    one.audio = "";
                }
                m_matches.push_back(one);
            }
            match = match->NextSiblingElement();
        }
    } while (0);
    
    m_letterNum = (int)m_targets.size();
    m_letterDragNum = (int)m_matches.size();
    if (m_letterNum < 1 || m_letterDragNum != m_letterNum) {
        return;
    }
    int n = (int)m_matches.size();
    for (int i = 0; i < n; i++) {
        int m = rand() % n;
        int t = m_matches[0].order;
        m_matches[0].order = m_matches[m].order;
        m_matches[m].order = t;
    }
    setWord();
    
    QuizBase::setParam(imgFile, voiceFile, xmlFile);
}

void QuizSentence::playAudio(int n)
{
    if (m_letterDrag[n].audio.length() < 1) {
        return;
    }
    CCLOG("play audio %s", m_letterDrag[n].audio.c_str());
    
    //音效路径
    string strPathSnd = m_wholeBook->getQuizSoundPath(m_pageIndex, m_letterDrag[n].audio);
    
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        SoundManager::getInstance()->playSound(strPathSnd);
    }

}

int QuizSentence::inPage()
{
    QuizBase::inPage();
    
    if (!m_bXmlRead) {
        return AudioEngine::INVALID_AUDIO_ID;
    }
    
//    if (!NewHandTip::isShow("QuizSentence")) {
//        NewHandTip::setShow("QuizSentence");
//        Button* btn_sound = dynamic_cast<Button*>(m_ui->getChildByName("panel_target")->getChildByName("btn_sound"));
//        NewHandTip::create(UiBase::getInstance()->getRunningScene(), btn_sound, "听读音拖动单词拼句子");
//    }
    
    return playSentenceAudio();
}

int QuizSentence::playSentenceAudio()
{
    int soundId = AudioEngine::INVALID_AUDIO_ID;
    //音效路径
    string strPathSnd = m_wholeBook->getQuizSoundPath(m_pageIndex, m_audioFile);
    
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        soundId = SoundManager::getInstance()->playSound(strPathSnd);
    }
    return soundId;
}
