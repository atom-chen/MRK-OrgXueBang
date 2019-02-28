//
//  QuizListenSelectPic.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/26.
//
//
#include "GameMacro.hpp"

#include "QuizListenSelectPic.h"

#include "SoundManager.hpp"

#include "UiBase.h"

#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

#include "TipManager.hpp"

QuizListenSelectPic *QuizListenSelectPic::create(Node* parent)
{
    QuizListenSelectPic *ret = new (std::nothrow) QuizListenSelectPic();
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

bool QuizListenSelectPic::init()
{
    m_initOk = false;
    
    m_ui = CSLoader::createNode("ui/quiz/layer_listenSelectPicture.csb");
    m_ui->addChild(this, -1);
    
//    Size parentSize = m_parent->getContentSize();
//    m_ui->setContentSize(parentSize);
//    Helper::doLayout(m_ui);
    
    ImageView* img_sound = dynamic_cast<ImageView*>(m_ui->getChildByName("img_sound"));
    UiBase::addAudioEventListener(img_sound, [=](Ref* pSender){
        this->playSentenceAudio();
    });
    
    return true;
}

void QuizListenSelectPic::setQuestion()
{
    int quesCount = (int)m_answer.size();
    int begin, end;
    if (quesCount == 3) {
        begin = 0;
        end = 2;
    } else {
        begin = 1;
        end = 4;
    }
    m_errorCount = 0;
    m_optionCount = quesCount;
    
    Layout* panel_answer = dynamic_cast<Layout*>(m_ui->getChildByName("panel_answer"));
    
    char nodeName[36];
    
    for (int i = 0; i < 5; i++) {
        int n = i - begin;
        Node* node;
        sprintf(nodeName, "node_%d", i);
        node = panel_answer->getChildByName(nodeName)->getChildByName("img_item");
        node->getChildByName("img_select")->setVisible(false);
        panel_answer->getChildByName(nodeName)->getChildByName("img_blank")->setVisible(false);
        m_items[i] = node;
        if (i >= begin && i <= end) {
//            auto panel_item = dynamic_cast<ImageView*>(node->getChildByName("img_item"));
            node->getChildByName("text_word")->setVisible(false);
            ((ImageView*)node)->addClickEventListener([=](Ref* pSender){
                this->onSelect(n);
                for (int j = 0; j < 5; j++) {
                    m_items[j]->getChildByName("img_select")->setVisible(false);
                }
                node->getChildByName("img_select")->setVisible(true);
            });
            
            string picFile = m_wholeBook->getQuizPicPath(m_pageIndex, m_answer[n].img);
            ImageView* img_word = dynamic_cast<ImageView*>(node->getChildByName("img_word"));
            m_answer[n].node = img_word;
            if (FileUtils::getInstance()->isFileExist(picFile)) {
//                img_word->loadTexture(picFile);
                auto imgSize = img_word->getContentSize();
                UiBase::LoadTexture(img_word, picFile, imgSize);
            }
            
        } else {
            node->setVisible(false);
        }
    }
    
    m_initOk = true;
    
    setScore(0);
}

void QuizListenSelectPic::onSelect(int n)
{
    CCLOG("select %d score = %d", n, m_answer[n].score);
    if (m_answer[n].score == 100) {
//        m_ui->removeFromParent();
        setScore(100);
        
        __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_NUT, (Ref*)(intptr_t)1);
        playAnswerAudio(true);
    } else {
        setScore(0);
        
        __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_NUT, (Ref*)(intptr_t)0);
        playAnswerAudio(false);
        m_errorCount++;
    }
}

void QuizListenSelectPic::setParam(string imgFile, string voiceFile, string xmlFile)
{
    if (imgFile.length() > 0 && m_initOk) {
        for (auto answer : m_answer) {
            string picFile = m_wholeBook->getQuizPicPath(m_pageIndex, answer.img);
            if (imgFile == picFile && answer.node) {
//                dynamic_cast<ImageView*>(answer.node)->loadTexture(imgFile);
                ImageView* img_word = dynamic_cast<ImageView*>(answer.node);
                auto imgSize = img_word->getContentSize();
                UiBase::LoadTexture(img_word, picFile, imgSize);
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
        
        XMLElement* answer = question->FirstChildElement("answer");
        while (answer) {
            
            const char* textAns = answer->Attribute("image");
            if (textAns) {
                CCLOG("answer = %s", textAns);
                stuQuizAnswerImg ans;
                ans.score = atoi(answer->Attribute("score"));
                ans.img = textAns;
                ans.node = nullptr;
                UiBase::replace_all_distinct(ans.img, "\\", "/");
                m_answer.push_back(ans);
                
                m_wholeBook->downQuizPic(m_pageIndex, ans.img);
            }
            answer = answer->NextSiblingElement("answer");
        }
    } while (0);
    
    if (m_answer.size() < 1) {
        return;
    }
    
    setQuestion();
    
    QuizBase::setParam(imgFile, voiceFile, xmlFile);
}

int QuizListenSelectPic::inPage()
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

int QuizListenSelectPic::playSentenceAudio()
{
    int soundId = AudioEngine::INVALID_AUDIO_ID;
    //音效路径
    string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
    
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        soundId = SoundManager::getInstance()->playSound(strPathSnd);
    }
    return soundId;
}
