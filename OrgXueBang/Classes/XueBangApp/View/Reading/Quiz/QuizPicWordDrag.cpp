//
//  QuizPicWordDrag.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/23.
//
//
#include "GameMacro.hpp"

#include "QuizPicWordDrag.h"
#include "UiBase.h"
#include "DownImg.h"

#include "SoundManager.hpp"

#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

#include "TipManager.hpp"

QuizPicWordDrag* QuizPicWordDrag::create(Node* parent)
{
    QuizPicWordDrag *ret = new (std::nothrow) QuizPicWordDrag();
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

bool QuizPicWordDrag::init()
{
    m_initOk = false;
    
    m_ui = CSLoader::createNode("ui/quiz/layer_pictureWord.csb");
    m_ui->addChild(this, -1);
    
//    Size parentSize = m_parent->getContentSize();
//    m_ui->setContentSize(parentSize);
//    m_ui->setPositionY((m_parent->getContentSize().height - m_ui->getContentSize().height) / 2);
//    Helper::doLayout(m_ui);
    
//    setWord();
//    setResPath("https://storage.metricschina.com/prod/Library/OAA-003000/");
//    setParam("", "", FileUtils::getInstance()->getWritablePath() + "test_picdrag.xml");
    
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
    
    m_ui->getChildByName("panel_question")->setVisible(false);
    m_ui->getChildByName("panel_answer")->setVisible(false);
    return true;
}

QuizPicWordDrag::~QuizPicWordDrag()
{
    m_layer->removeFromParent();
}

void QuizPicWordDrag::setWord()
{
    m_quesCount = (int)m_matches.size();
    int begin, end;
    if (m_quesCount == 3) {
        begin = 0;
        end = 2;
    } else {
        begin = 1;
        end = 4;
    }
//    const char* w[3] = {"reading", "writing", "cocos2dx"};
    m_errorCount = 0;
    m_optionCount = m_quesCount;
    
    Layout* panel_question = dynamic_cast<Layout*>(m_ui->getChildByName("panel_question"));
    panel_question->setVisible(true);
    Layout* panel_answer = dynamic_cast<Layout*>(m_ui->getChildByName("panel_answer"));
    panel_answer->setVisible(true);
    char nodeName[36];
    for (int i = 0; i < 5; i++) {
        int n = i - begin;
        Node* node;
        sprintf(nodeName, "node_%d", i);
        node = panel_question->getChildByName(nodeName);
        if (i >= begin && i <= end) {
            auto img_item = dynamic_cast<Widget*>(node->getChildByName("img_item"));
            m_pic[n].node = node;
            m_pic[n].m_canDrag = true;
            m_pic[n].word = m_targets[n].text;
            m_pic[n].m_originPos = node->getPosition();
            node->getChildByName("img_blank")->setVisible(false);
            img_item->getChildByName("img_select")->setVisible(false);
            
            img_item->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
                Node* node = img_item->getParent();
                node->setLocalZOrder(10);
                if (type == Widget::TouchEventType::BEGAN) {
                    //                CCLOG("click %d", i);
                } else if (type == Widget::TouchEventType::MOVED) {
                    Vec2 oldPos = node->getPosition();
                    oldPos.x += this->m_touchPosNew.x - this->m_touchPosOld.x;
                    oldPos.y += this->m_touchPosNew.y - this->m_touchPosOld.y;
                    node->setPosition(oldPos);
                    this->m_touchPosOld.set(this->m_touchPosNew);
                } else {
                    //                node->setPosition(m_letterDrag[i].m_originPos);
                    Vec2 wPos = node->getParent()->convertToWorldSpace(node->getPosition());
                    this->checkDrag(n, wPos);
                }
            });
            
            ImageView* img_word = dynamic_cast<ImageView*>(img_item->getChildByName("img_word"));
            string picFile = m_wholeBook->getQuizPicPath(m_pageIndex, m_matches[n].img);
            if (FileUtils::getInstance()->isFileExist(picFile)) {
//                img_word->loadTexture(picFile);
                auto imgSize = img_word->getContentSize();
                UiBase::LoadTexture(img_word, picFile, imgSize);
            }
            img_item->getChildByName("text_word")->setVisible(false);
        } else {
            node->setVisible(false);
        }
        
        node = panel_answer->getChildByName(nodeName);
        if (i >= begin && i <= end) {
            auto img_item = dynamic_cast<Widget*>(node->getChildByName("img_item"));
//            img_item->setTouchEnabled(false);
            m_word[n].node = node;
            m_word[n].m_canDrag = true;
            m_word[n].word = m_targets[n].text;
            m_word[n].m_originPos = node->getPosition();
            img_item->getChildByName("img_select")->setVisible(false);
            
            img_item->addClickEventListener([=](Ref* pSender){
                this->playAudio(n);
            });
            
            Text* text_word = dynamic_cast<Text*>(img_item->getChildByName("text_word"));
            text_word->setString(m_targets[n].text);
            ImageView* img_word = dynamic_cast<ImageView*>(img_item->getChildByName("img_word"));
            string picFile = m_wholeBook->getQuizPicPath(m_pageIndex, m_matches[n].img);
            if (FileUtils::getInstance()->isFileExist(picFile)) {
//                img_word->loadTexture(picFile);
                auto imgSize = img_word->getContentSize();
                UiBase::LoadTexture(img_word, picFile, imgSize);
            }
            img_word->setVisible(false);
        } else {
            node->setVisible(false);
        }
    }
    
    m_initOk = true;
    
    setScore(0);
}

void QuizPicWordDrag::checkDrag(int n, cocos2d::Vec2 pos)
{
    CCLOG("drag end %d", n);
    
    Layout* panel_answer = dynamic_cast<Layout*>(m_ui->getChildByName("panel_answer"));
    Vec2 nPos = panel_answer->convertToNodeSpace(pos);
    
    for (int i = 0; i < m_quesCount; i++) {
        if (m_word[i].m_canDrag) {
            Rect rect(m_word[i].m_originPos.x - 120, m_word[i].m_originPos.y - 120, 240, 240);
            if (rect.containsPoint(nPos)) {
                CCLOG("%s find %s", m_pic[n].word.c_str(), m_word[i].word.c_str());
                if (m_pic[n].word == m_word[i].word) {
                    m_pic[n].node->setVisible(false);
                    m_pic[n].m_canDrag = false;
                    m_word[i].m_canDrag = false;
                    m_word[i].node->getChildByName("img_item")->getChildByName("img_word")->setVisible(true);
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
    m_pic[n].node->setLocalZOrder(1);
    m_pic[n].node->setPosition(m_pic[n].m_originPos);
    m_errorCount++;
}

void QuizPicWordDrag::checkOk()
{
    for (int i = 0; i < m_quesCount; i++) {
        if (m_pic[i].node->isVisible()) {
            return;
        }
    }
    CCLOG("success !!!!");
//    m_ui->removeFromParent();
    setScore(100);
    __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_NUT, (Ref*)(intptr_t)1);
    playAnswerAudio(true);
}

void QuizPicWordDrag::setParam(std::string imgFile, std::string voiceFile, std::string xmlFile)
{
    if (imgFile.length() > 0 && m_initOk) {
        for (int i = 0; i < m_matches.size(); i++) {
            string picFile = m_wholeBook->getQuizPicPath(m_pageIndex, m_matches[i].img);
            if (imgFile == picFile) {
                if (m_pic[i].node) {
                    ImageView* img_word = dynamic_cast<ImageView*>(m_pic[i].node->getChildByName("img_item")->getChildByName("img_word"));
//                    img_word->loadTexture(imgFile);
                    auto imgSize = img_word->getContentSize();
                    UiBase::LoadTexture(img_word, picFile, imgSize);
                }
                if (m_word[i].node) {
                    ImageView* img_word = dynamic_cast<ImageView*>(m_word[i].node->getChildByName("img_item")->getChildByName("img_word"));
//                    img_word->loadTexture(imgFile);
                    auto imgSize = img_word->getContentSize();
                    UiBase::LoadTexture(img_word, picFile, imgSize);
                }
                break;
            }
        }
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
        XMLElement* targets = curEle->FirstChildElement("targets");
        if (!targets) {
            break;
        }
        XMLElement* target = targets->FirstChildElement("target");
        while (target) {
            stuQuizTargetOne one;
            
            const char* audioText = target->Attribute("audio");
            if (audioText) {
                one.audio = audioText;
                
                m_wholeBook->downQuizSound(m_pageIndex, one.audio);
            }
            
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
            const char* audioText = match->Attribute("audio");
            if (audioText) {
                one.audio = audioText;
                
                m_wholeBook->downQuizSound(m_pageIndex, one.audio);
            }
            
            const char* tarText = match->Attribute("img");
            if (tarText) {
                one.img = tarText;
                UiBase::replace_all_distinct(one.img, "\\", "/");
                one.nId = atoi(match->Attribute("id"));
//                one.order = atoi(match->Attribute("order"));
                
                if (audioText == nullptr) {
                    for (auto t : m_targets) {
                        if (one.nId == t.nId) {
                            if (t.audio.length() > 0) {
                                one.audio = t.audio;
                            }
                            break;
                        }
                    }
                }
                
                m_matches.push_back(one);
                
                m_wholeBook->downQuizPic(m_pageIndex, one.img);
            }

            match = match->NextSiblingElement();
        }
    } while (0);
    
    if (m_targets.size() < 2 || m_matches.size() > 4 || (m_targets.size() != m_matches.size())) {
        return;
    }
    
    setWord();

    QuizBase::setParam(imgFile, voiceFile, xmlFile);
}

void QuizPicWordDrag::playAudio(int n)
{
    if (m_matches[n].audio.length() < 1) {
        return;
    }
    CCLOG("play audio %s", m_matches[n].audio.c_str());
    
    //音效路径
    string strPathSnd = m_wholeBook->getQuizSoundPath(m_pageIndex, m_matches[n].audio);
    
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        SoundManager::getInstance()->playSound(strPathSnd);
    } else {
        TipManager::getInstance()->showTipWithString("文件下载中，请稍后再试。");
    }

}

