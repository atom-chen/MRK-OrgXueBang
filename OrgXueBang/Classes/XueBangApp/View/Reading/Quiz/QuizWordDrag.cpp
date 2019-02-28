//
//  QuizWordDrag.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/13.
//
//
#include "GameMacro.hpp"

#include "QuizWordDrag.h"

#include "SoundManager.hpp"

#include "UiBase.h"

#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

#include "TipManager.hpp"

#include "NewHandTip.hpp"

#define QUIZ_BLANK "[]"
#define QUIZ_BLANK1 '['
#define QUIZ_BLANK2 ']'

#define QUIZ_LETTER_WIDTH 50
#define QUIZ_LETTER_CHECK (100 * 100)

QuizWordDrag* QuizWordDrag::create(Node* parent)
{
    QuizWordDrag *ret = new (std::nothrow) QuizWordDrag();
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


bool QuizWordDrag::init()
{
    
    m_ui = CSLoader::createNode("ui/quiz/layer_wordDarg.csb");
    m_ui->addChild(this, -1);
    
//    Size parentSize = m_parent->getContentSize();
//    m_ui->setContentSize(parentSize);
//    m_ui->setPositionY((m_parent->getContentSize().height - m_ui->getContentSize().height) / 2);
//    Helper::doLayout(m_ui);
    
    m_ui->getChildByName("panel_dragLetters")->setVisible(false);
    
//    setWord();
//    setParam("", "", FileUtils::getInstance()->getWritablePath() + "test_worddrag.xml");
    
    ImageView* btn_sound = dynamic_cast<ImageView*>(m_ui->getChildByName("img_sound"));
    UiBase::addAudioEventListener(btn_sound, [=](Ref* pSender){
        CCLOG("play sentence audio !");
        
        int soundId = this->playSentenceAudio();
        if (soundId == AudioEngine::INVALID_AUDIO_ID) {
            TipManager::getInstance()->showTipWithString("文件下载中，请稍后再试。");
        }
    });
    btn_sound->setVisible(false);
    m_ui->getChildByName("panel_img")->setVisible(false);

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
    
    return true;
}

QuizWordDrag::~QuizWordDrag()
{
    m_layer->removeFromParent();
}

void QuizWordDrag::setWord(const string& targetWord, const string& matchWord, const vector<int>& blankList)
{

    const char* w = targetWord.c_str();//"readingmate";
    m_letterNum = (int)strlen(w);
    
    Layout* panel_word = dynamic_cast<Layout*>(m_ui->getChildByName("panel_word"));
    Size wordSize = panel_word->getContentSize();

    int space = 5;
    int x;
    x = (wordSize.width - (m_letterNum - 1) * (QUIZ_LETTER_WIDTH + space)) / 2;
    for (int i = 0; i < m_letterNum; i++) {
        m_letter[i].letter = w[i];
        m_letter[i].state = 0;
        m_letter[i].node = CSLoader::createNode("ui/quiz/node_letter.csb");
        Text* text_letter = dynamic_cast<Text*>(m_letter[i].node->getChildByName("text_letter"));
        text_letter->setFontSize(80);
        CCLOG("x = %d", (x + i * (QUIZ_LETTER_WIDTH + space)));
        m_letter[i].node->setPosition(Vec2(x + i * (QUIZ_LETTER_WIDTH + space), wordSize.height / 2));
        panel_word->addChild(m_letter[i].node);
    }

    for (int i = 0; i < blankList.size(); i++) {
        m_letter[blankList[i]].state = 1;
    }
    for (int i = 0; i < m_letterNum; i++) {
        m_letter[i].setState(m_letter[i].state);
    }
    
    const char* d = matchWord.c_str();//"eagt";
    m_letterDragNum = (int)strlen(d);
    
    m_errorCount = 0;
    m_optionCount = m_letterDragNum;
    
    Layout* panel_dragLetters = dynamic_cast<Layout*>(m_ui->getChildByName("panel_dragLetters"));
//    Size dragSize = panel_dragLetters->getContentSize();
//    space = 10;
//    x = (dragSize.width - m_letterDragNum * letterWidth - (m_letterDragNum - 1) * space) / 2;
    
    panel_dragLetters->setVisible(true);
    
    for (int i = 0; i < m_letterDragNum; i++) {
        m_letterDrag[i].letter = d[i];
        m_letterDrag[i].m_canDrag = true;
        
        m_letterDrag[i].node = panel_dragLetters->getChildByName(StringUtils::format("node_letter_%d", i));
        m_letterDrag[i].m_originPos = m_letterDrag[i].node->getPosition();
        Text* text_letter = dynamic_cast<Text*>(m_letterDrag[i].node->getChildByName("text_letter"));
        std::string l = "";
        l.push_back(m_letterDrag[i].letter);
        text_letter->setString(l);
        
        auto img_ball = dynamic_cast<ImageView*>(m_letterDrag[i].node->getChildByName("img_ball"));
        img_ball->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
            Node* node = img_ball->getParent();
            node->setLocalZOrder(10);
            if (type == Widget::TouchEventType::BEGAN) {
//                CCLOG("click %d", i);
                this->m_minLetter = -1;
            } else if (type == Widget::TouchEventType::MOVED) {
                Vec2 oldPos = node->getPosition();
                oldPos.x += this->m_touchPosNew.x - this->m_touchPosOld.x;
                oldPos.y += this->m_touchPosNew.y - this->m_touchPosOld.y;
                node->setPosition(oldPos);
                this->m_touchPosOld.set(this->m_touchPosNew);
                
                Vec2 wPos = node->getParent()->convertToWorldSpace(oldPos);
                auto minLetter = this->getPointNode(wPos);
                
                if (minLetter != this->m_minLetter) {
                    if (this->m_minLetter >= 0) {
                        m_letter[this->m_minLetter].node->setScale(1.0);
                    }
                    this->m_minLetter = minLetter;
                    if (this->m_minLetter >= 0) {
                        m_letter[this->m_minLetter].node->setScale(2.0);
                    }
                }
            } else {
//                node->setPosition(m_letterDrag[i].m_originPos);
                Vec2 wPos = node->getParent()->convertToWorldSpace(node->getPosition());
                this->checkDrag(i, wPos);
                
                if (this->m_minLetter >= 0) {
                    m_letter[this->m_minLetter].node->setScale(1.0);
                    this->m_minLetter = -1;
                }
            }
        });
    }
    for (int i = m_letterDragNum; i < MAX_LETTER_QUIZWORDDRAG; i++) {
        panel_dragLetters->getChildByName(StringUtils::format("node_letter_%d", i))->setVisible(false);
    }
    
    if (m_audioFile.length() < 4) {
        m_ui->getChildByName("img_sound")->setVisible(false);
    }
    setScore(0);

    auto page = m_wholeBook->m_bookPages->at(m_pageIndex);
    auto passage_type = page->m_paramJson.getValue("passage_type");
    auto task_type = page->m_paramJson.getValue("task_type");
    if (passage_type.find("Image") != string::npos || task_type.find("Image") != string::npos) {
        m_ui->getChildByName("panel_img")->setVisible(true);
    } else {
//        auto imgPos = m_ui->getChildByName("panel_img")->getPosition();
//        m_ui->getChildByName("img_sound")->setPosition(imgPos);
        panel_word->setPositionY(panel_word->getPosition().y + 250);
    }
    if (passage_type.find("Audio") != string::npos || task_type.find("Audio") != string::npos) {
        m_ui->getChildByName("img_sound")->setVisible(true);
    }
}

void QuizWordDrag::QuizLetter::setState(int state)
{
    this->state = state;
    Text* text_letter = dynamic_cast<Text*>(node->getChildByName("text_letter"));
    auto img_ball = dynamic_cast<ImageView*>(node->getChildByName("img_ball"));
    auto img_blank = dynamic_cast<ImageView*>(node->getChildByName("img_blank"));

    std::string l = "";
    
    switch (state) {
        case 0:
//            text_letter->setFontSize(80);
//            text_letter->setPositionY(10);
            text_letter->setTextColor(Color4B(0x52, 0x16, 0x22, 0xff));
            l.push_back(letter);
            text_letter->setString(l);
            img_ball->setVisible(false);
            img_blank->setVisible(false);
            break;
        case 1:
            text_letter->setString("");
            img_ball->setVisible(false);
            img_blank->setVisible(true);
            break;
        case 2:
//            text_letter->setFontSize(80);
//            text_letter->setPositionY(10);
            text_letter->setTextColor(Color4B(0, 0x80, 0xc3, 0xff));
            l.push_back(letter);
            text_letter->setString(l);
            img_ball->setVisible(false);
            img_blank->setVisible(false);
            break;
            
        default:
            break;
    }
}

void QuizWordDrag::checkDrag(int n, cocos2d::Vec2 pos)
{
//    Layout* panel_word = dynamic_cast<Layout*>(m_ui->getChildByName("panel_word"));
//    Vec2 nPos = panel_word->convertToNodeSpace(pos);
//
//    for (int i = 0; i < m_letterNum; i++) {
//        if (m_letter[i].state == 1) {
//            auto pos = m_letter[i].node->getPosition();
//            Rect rect(pos.x - 25, pos.y - 25, 50, 50);
//            if (rect.containsPoint(nPos)) {
//                CCLOG("%c find %c", m_letterDrag[n].letter, m_letter[i].letter);
//                if (m_letterDrag[n].letter == m_letter[i].letter) {
//                    m_letterDrag[n].node->setVisible(false);
//                    m_letter[i].setState(2);
//                    checkOk();
//                    return;
//                }
//                else {
//                    playAnswerAudio(false);
//                    break;
//                }
//            }
//        }
//    }
    if (m_minLetter >= 0) {
        CCLOG("%c find %c", m_letterDrag[n].letter, m_letter[m_minLetter].letter);
        if (m_letterDrag[n].letter == m_letter[m_minLetter].letter) {
            m_letterDrag[n].node->setVisible(false);
            m_letter[m_minLetter].setState(2);
            checkOk();
            return;
        }
        else {
            playAnswerAudio(false);
        }
    }
    m_letterDrag[n].node->setLocalZOrder(1);
    m_letterDrag[n].node->setPosition(m_letterDrag[n].m_originPos);
    m_errorCount++;
}

void QuizWordDrag::checkOk()
{
    for (int i = 0; i < m_letterNum; i++) {
        if (m_letter[i].state == 1) {
            return;
        }
    }
    CCLOG("success !!!!");
//    m_ui->removeFromParent();
    setScore(100);
    __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_NUT, (Ref*)(intptr_t)1);
    playAnswerAudio(true);
}

void QuizWordDrag::setParam(std::string imgFile, std::string voiceFile, std::string xmlFile)
{
    if (imgFile.length() > 0) {
//        ImageView* img_picture = dynamic_cast<ImageView*>(m_ui->getChildByName("img_picture"));
//        img_picture->loadTexture(imgFile);
        auto panel_img = dynamic_cast<Layout*>(m_ui->getChildByName("panel_img"));
        UiBase::LoadTexture(dynamic_cast<ImageView*>(panel_img->getChildByName("img_picture")), imgFile, panel_img->getContentSize());
        return;
    }
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
        
        int task_number = atoi(m_wholeBook->m_bookPages->at(m_pageIndex)->m_paramJson.getValue("task_number").c_str());
        while (task_number > 1) {
            XMLElement* oldEle = curEle;
            curEle = curEle->NextSiblingElement();
            if (!curEle) {
                curEle = oldEle;
                break;
            }
            task_number--;
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
            const char* tarText = target->Attribute("text");
            if (tarText) {
                one.text = tarText;
                one.nId = atoi(target->Attribute("id"));
                m_targets.push_back(one);
            }
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
                m_matches.push_back(one);
            }
            match = match->NextSiblingElement();
        }
    } while (0);
    
    if (m_targets.size() < 1 || m_matches.size() < 1) {
        return;
    }
    
    string targetWord = "";
    vector<int> blankList;
    for (int i = 0; i < m_targets.size(); i++) {
        for (int j = 0; j < m_targets[i].text.length(); j++) {
            char c = m_targets[i].text.at(j);
            if (c == QUIZ_BLANK1) {
                blankList.push_back((int)targetWord.length());
                c = 0;
                for (int k = 0; k < m_matches.size(); k++) {
                    if (m_matches[k].nId == m_targets[i].nId) {
                        c = m_matches[k].text.at(0);
                        targetWord.push_back(c);
                        break;
                    }
                }
                if (c == 0) {
                    CCLOG("can not find %c in matches", m_targets[i].text.at(j));
                    return;
                }
            } else if (c == QUIZ_BLANK2) {
                
            } else {
                targetWord.push_back(c);
            }
        }
    }
    
    int n = (int)m_matches.size();
    for (int i = 0; i < n; i++) {
        int m = rand() % n;
        int t = m_matches[0].order;
        m_matches[0].order = m_matches[m].order;
        m_matches[m].order = t;
    }
    
    string matchWord = "";
    for (int i = 0; i < m_matches.size(); i++) {
        matchWord.push_back(m_matches[i].text.at(0));
    }
    for (int i = 0; i < m_matches.size(); i++) {
        matchWord[m_matches[i].order - 1] = m_matches[i].text.at(0);
    }
//    for (int i = 0; i < m_matches.size(); i++) {
//        matchWord[i] = m_matches[i].text.at(0);
//    }
    m_blankNum = (int)blankList.size();
    
    CCLOG("targetWord = %s", targetWord.c_str());
    CCLOG("matchWord = %s", matchWord.c_str());
    CCLOG("blank size = %d", m_blankNum);
    
    setWord(targetWord, matchWord, blankList);
    
    QuizBase::setParam(imgFile, voiceFile, xmlFile);
}

int QuizWordDrag::inPage()
{
    QuizBase::inPage();
    
    if (!m_bXmlRead) {
        return AudioEngine::INVALID_AUDIO_ID;
    }
    
//    if (!NewHandTip::isShow("QuizWordDrag")) {
//        NewHandTip::setShow("QuizWordDrag");
//
//        ImageView* btn_sound = dynamic_cast<ImageView*>(m_ui->getChildByName("img_sound"));
//
//        vector<Node*> nodeList;
//        nodeList.push_back(btn_sound);
//        nodeList.push_back(m_letterDrag[0].node);
//
//        vector<string> msgList = {"听一下需要你作答的问题吧", "拖动字母拼成需要的单词吧"};
//        NewHandTip::create(UiBase::getInstance()->getRunningScene(), nodeList, msgList);
//    }
    
    
    return playSentenceAudio();
}

int QuizWordDrag::playSentenceAudio()
{
    int soundId = AudioEngine::INVALID_AUDIO_ID;
    if (!m_ui->getChildByName("img_sound")->isVisible()) {
        return soundId;
    }

    if (m_audioFile.length() < 4) {
        string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
        
        if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
            SoundManager::getInstance()->VolumeBackSound(1.0f);
            soundId = SoundManager::getInstance()->playSound(strPathSnd);
        }
        return soundId;
    }
    //音效路径
    string strPathSnd = m_wholeBook->getQuizSoundPath(m_pageIndex, m_audioFile);
    
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        soundId = SoundManager::getInstance()->playSound(strPathSnd);
    }
    return soundId;
}

int QuizWordDrag::getPointNode(cocos2d::Vec2 pt)
{
    Layout* panel_word = dynamic_cast<Layout*>(m_ui->getChildByName("panel_word"));
    Vec2 nPos = panel_word->convertToNodeSpace(pt);
    float minDis = -1;
    int minLetter = -1;
    
    for (int i = 0; i < m_letterNum; i++) {
        if (m_letter[i].state == 1) {
            auto pos = m_letter[i].node->getPosition();
            auto dis = (nPos.x - pos.x) * (nPos.x - pos.x) + (nPos.y - pos.y) * (nPos.y - pos.y);
            if (minDis < 0 || minDis > dis) {
                minLetter = i;
                minDis = dis;
            }
        }
    }
    if (minDis >=0 && minDis < QUIZ_LETTER_CHECK) {
//        CCLOG("dis = %f", sqrt(minDis));
        return minLetter;
    }
    return -1;
}
