//
//  UiBase.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2016/11/30.
//
//

#include <regex>

#include "editor-support/cocostudio/CocoStudio.h"

#include "UiBase.h"
#include "BaseLoad.h"
#include "baseIdfa.h"
//#include "Game.hpp"
//#include "Reador.hpp"

#include "SceneManager.hpp"

#include <regex.h>

#include "json/writer.h"
#include "json/stringbuffer.h"



USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio::timeline;
using namespace cocos2d::experimental::ui;

#include "GameMacro.hpp"


//#include <spine/spine-cocos2dx.h>
#include <spine/SkeletonAnimation.h>
using namespace spine;

UiBase* UiBase::m_uiBase = nullptr;

UiBase* UiBase::getInstance()
{
    if (m_uiBase == nullptr) {
        m_uiBase = new UiBase;
        
//        auto visibleSize = Director::getInstance()->getVisibleSize();
////#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
////        if (visibleSize.height == 1624) { //针对 iphone x 上下要留出空间
////            visibleSize.height -= (88 + 68);
////        }
////#endif
////        m_uiBase->m_visibleHeight = visibleSize.height;
//        m_uiBase->m_visibleHeight = ceil(visibleSize.height / visibleSize.width * DESIGN_RESOLUTION_WIDTH);
        
        m_uiBase->setVisibleHeight(DESIGN_RESOLUTION_HEIGHT);
    }
    return m_uiBase;
}

Node* UiBase::loadCSB(const std::string &filename)
{
    auto ui = CSLoader::createNode(filename);
    if (ui != nullptr) {
        auto uiSize = ui->getContentSize();
        ui->setContentSize(Size(uiSize.width, m_visibleHeight));
        Helper::doLayout(ui);
        
        Layout* panel_top = (Layout*)ui->getChildByName(TOP_PANEL_NAME);
        if (panel_top != NULL) {
            Size panelSize = panel_top->getContentSize();
            m_topPanelHeight = panelSize.height;
        } else {
            m_topPanelHeight = 0;
        }
        
        Layout* panel_bottom = (Layout*)ui->getChildByName(BOTTOM_PANEL_NAME);
        if (panel_bottom != NULL) {
            Size panelSize = panel_bottom->getContentSize();
            m_bottomPanelHeight = panelSize.height;
        } else {
            m_bottomPanelHeight = 0;
        }
        
        Layout* panel_center = (Layout*)ui->getChildByName(CENTER_PANEL_NAME);
        if (panel_center != NULL) {
            m_centerPanelHeight = m_visibleHeight - m_topPanelHeight - m_bottomPanelHeight;
            Size panelSize = Size(uiSize.width, m_centerPanelHeight);
            panel_center->setContentSize(panelSize);
            panel_center->setPosition(Vec2(0, m_bottomPanelHeight));
            
            Helper::doLayout(panel_center);
        }
        
        resizeBackImg(ui);
    }
    return ui;
}

int UiBase::setText(cocos2d::Node *text, std::string &str, int minHeight)
{
    Text* t = dynamic_cast<Text*>(text);
    if (t == nullptr) {
        return 0;
    }
    
    t->ignoreContentAdaptWithSize(false);
    Size txtSize = t->getContentSize();
    txtSize.height = 0;
    t->setTextAreaSize(txtSize);
    
    t->setString(str);
    txtSize = t->getVirtualRendererSize();
    if (txtSize.height < minHeight) {
        txtSize.height = minHeight;
    }
    t->setContentSize(txtSize);
    return txtSize.height;
}

void UiBase::setChatText(cocos2d::Node* text, const std::string& str, int minHeight, int minWidth, int maxWidth)
{
    Text* t = dynamic_cast<Text*>(text);
    if (t == nullptr) {
        return;
    }
    
    t->ignoreContentAdaptWithSize(false);
    t->setTextAreaSize(Size(0, 0));
    t->setString(str);
    Size txtSize = t->getVirtualRendererSize();
    if (txtSize.width <= maxWidth) {
        if (txtSize.height < minHeight) {
            txtSize.height = minHeight;
        }
        t->setContentSize(txtSize);
        return;
    }
    
//    t->ignoreContentAdaptWithSize(false);
    txtSize.width = maxWidth;
    txtSize.height = 0;
    t->setTextAreaSize(txtSize);
    
    t->setString(str);
    txtSize = t->getVirtualRendererSize();
    if (txtSize.height < minHeight) {
        txtSize.height = minHeight;
    }
    t->setContentSize(txtSize);
    
}


string& UiBase::replace_all_distinct(std::string &str, const std::string &old_value, const std::string &new_value)
{
    for(string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
        if((pos=str.find(old_value,pos)) != string::npos)
            str.replace(pos,old_value.length(), new_value);
        else
            break;
    }
    return str;
}


void UiBase::saveJsonFile(rapidjson::Document &doc, string &fileName)
{
    string dataStr = "";
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    dataStr = buffer.GetString();
    string fullPath = FileUtils::getInstance()->getWritablePath() + fileName;
    FileUtils::getInstance()->writeStringToFile(dataStr, fullPath);
}

void UiBase::loadJsonFile(rapidjson::Document &doc, string &fileName)
{
    string dataStr = "";
    string fullPath = FileUtils::getInstance()->getWritablePath() + fileName;
    dataStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    doc.Parse<0>(dataStr.c_str());
}
static Vec2 s_lastPos;

void UiBase::addClickEventListener(Widget* node, const Widget::ccWidgetClickCallback &callback)
{
    int actionTag = 9999;
    
    float scale = node->getScale();
    node->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType eventType){
        Node* unit = (Node*)pSender;
        if (eventType == Widget::TouchEventType::BEGAN) {
            s_lastPos = unit->getParent()->convertToWorldSpace(unit->getPosition());
            //            CCLOG("begin pos = %f,%f", s_lastPos.x, s_lastPos.y);
//            unit->setScale(0.9 * scale);
            unit->stopActionByTag(actionTag);
            auto a = EaseElasticOut::create(ScaleTo::create(0.9f, 0.9), 0.22f);
            a->setTag(actionTag);
            unit->runAction(a);
        } else if (eventType == Widget::TouchEventType::MOVED) {
            
           Vec2 newPos = unit->getParent()->convertToWorldSpace(unit->getPosition());
            if (fabs(newPos.y - s_lastPos.y) >=  50.0f || fabs(newPos.x - s_lastPos.x) >=  50.0f) {
                unit->stopActionByTag(actionTag);
                auto a = EaseElasticOut::create(ScaleTo::create(0.9f, 1.0), 0.22f);
                a->setTag(actionTag);
                unit->runAction(a);
            }
        } else if (eventType == Widget::TouchEventType::ENDED) {
//            unit->setScale(1.0 * scale);
            unit->stopActionByTag(actionTag);
            auto a = EaseElasticOut::create(ScaleTo::create(0.9f, 1.0), 0.22f);
            a->setTag(actionTag);
            unit->runAction(a);

            Vec2 newPos = unit->getParent()->convertToWorldSpace(unit->getPosition());
            //            CCLOG("end begin pos = %f,%f", s_lastPos.x, s_lastPos.y);
            //            CCLOG("end pos = %f,%f", newPos.x, newPos.y);
            if (abs(s_lastPos.x - newPos.x) < 50 && abs(s_lastPos.y - newPos.y) < 50) {
                callback(pSender);
            }
        } else {
            unit->stopActionByTag(actionTag);
            auto a = EaseElasticOut::create(ScaleTo::create(0.9f, 1.0), 0.22f);
            a->setTag(actionTag);
            unit->runAction(a);
        }
    });
}

void UiBase::StringReplace(string &strBase, const string &strSrc, const string &strDes)
{
    string::size_type pos = 0;
    string::size_type srcLen = strSrc.size();
    string::size_type desLen = strDes.size();
    pos=strBase.find(strSrc, pos);
    while ((pos != string::npos))
    {
        strBase.replace(pos, srcLen, strDes);
        pos=strBase.find(strSrc, (pos+desLen));
    }
}

void UiBase::LoadTexture(cocos2d::ui::ImageView *pImg, const string &textureName, cocos2d::Size panelSize)
{
    if (pImg == nullptr) {
        return;
    }
    pImg->loadTexture(textureName);
    if (panelSize.width > 0 && panelSize.height > 0) {
        auto imgSize = dynamic_cast<Scale9Sprite*>(pImg->getVirtualRenderer())->getSprite()->getTextureRect().size;
        if (imgSize.width / imgSize.height > panelSize.width / panelSize.height) {
            imgSize.height = imgSize.height * panelSize.width / imgSize.width;
            imgSize.width = panelSize.width;
        } else {
            imgSize.width = imgSize.width * panelSize.height / imgSize.height;
            imgSize.height = panelSize.height;
        }
        pImg->setContentSize(imgSize);
    }
}

void UiBase::removeHtmlChar(string &str)
{
    StringReplace(str, "<br>", "\n");
    StringReplace(str, "</p>", "\n");
    StringReplace(str, "&nbsp;", " ");
    std::regex e ("<.*?>");
    str = std::regex_replace (str, e, "");
    
    StringReplace(str, "&copy;", "©");
}

void UiBase::removeUrlChar(string &str)
{
    std::regex e ("%3Cbr.*?%3E");
    str = std::regex_replace (str, e, "\n");
    e = std::regex ("%3C.*?%3E");
    str = std::regex_replace (str, e, "");
//    e = std::regex ("&#\\d*;");
//    str = std::regex_replace (str, e, "");
    
    StringReplace(str, "<i>", " ");
    StringReplace(str, "</i>", " ");
}

void UiBase::addAudioEventListener(Widget* node, const Widget::ccWidgetClickCallback &callback)
{
    float scale = node->getScale();
    node->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType eventType){
        Node* unit = (Node*)pSender;
        if (eventType == Widget::TouchEventType::BEGAN) {
            unit->setScale(0.9 * scale);
        } else if (eventType == Widget::TouchEventType::MOVED) {
            
        } else {
            unit->setScale(1.0 * scale);
            
            callback(pSender);
            
            SkeletonAnimation* horn = (SkeletonAnimation*)node->getChildByTag(666);
            if (horn) {
                horn->clearTracks();
                horn->setToSetupPose();
                horn->setTimeScale(1);
                horn->setAnimation(0, "horn", false);
            }
        }
    });
    
    node->setCascadeOpacityEnabled(false);
    node->setOpacity(0);
    
    auto horn = SkeletonAnimation::createWithJsonFile("spine/horn.json", "spine/horn.atlas", 1.0f);
    horn->setTimeScale(100);
    horn->setAnimation(0, "horn", false);
    horn->setPosition(Vec2(node->getContentSize().width/2, node->getContentSize().height/2));
    node->addChild(horn, 10 , 666);
}

void UiBase::playVideo(const string &url)
{
    auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
    auto parent = Director::getInstance()->getRunningScene();
    
    auto videoPlayer = VideoPlayer::create();
    videoPlayer->setPosition(Vec2(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height / 2));
    videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoPlayer->setContentSize(Size(rect.size.width, rect.size.width));
    parent->addChild(videoPlayer, 100);
    
    videoPlayer->addEventListener([=](Ref* pSender, VideoPlayer::EventType eType){
        CCLOG("eType = %d", eType);
        if (eType == VideoPlayer::EventType::COMPLETED) {
            videoPlayer->setVisible(false);
            videoPlayer->runAction(Sequence::create(DelayTime::create(0.1), RemoveSelf::create(), NULL));
        }
    });
    
    videoPlayer->setURL(url);
    videoPlayer->play();
    videoPlayer->setKeepAspectRatioEnabled(true);
    videoPlayer->setFullScreenEnabled(false);
}

Node* UiBase::getRunningSceneAndReleaseRes()
{
    auto main = Director::getInstance()->getRunningScene()->getChildByName(DESIGN_APP_WINDOW_NAME);
    main->removeAllChildren();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    auto am = CBaseIdfa::getInstance()->availableMemory();
//    if (am < 200) {
    if (rand() % 10 == 0) {
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    auto am = CBaseIdfa::getInstance()->usedMemory();
    if (am > 250) {
#else
    {
#endif
        SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }
    
    return main;
}

//废弃不用了
Node* UiBase::getRunningScene()
{
    if (SceneManager::getInstance()->getCurScene() == nullptr) {
        return nullptr;
    }
    auto main = SceneManager::getInstance()->getCurScene()->getChildByName(DESIGN_APP_WINDOW_NAME);
    if (main == nullptr) {
        main = Director::getInstance()->getRunningScene()->getChildByName(DESIGN_APP_WINDOW_NAME);
        CCLOG("running scene error ! game state = %d", SceneManager::getInstance()->getGameState());
    }
    return main;
}

void UiBase::resizeBackImg(cocos2d::Node *ui)
{
    auto screenSize = SceneManager::getInstance()->m_fullSceneSize;
    if (screenSize.height < 1334)
        return;
    
    auto img_fullscreen = Helper::seekWidgetByName((cocos2d::ui::Widget *)ui, "img_fullscreen");
    if (img_fullscreen) {
        img_fullscreen->setContentSize(screenSize);
        Helper::doLayout(img_fullscreen);
    }
}

void UiBase::resizeText(cocos2d::ui::Text* node, int maxW)
{
    auto txtSize = node->getContentSize();
    if (txtSize.width < maxW)
        return;
    
    string txt = node->getString();
    while(txtSize.width >= maxW - 30)
    {
        txt = txt.substr(0, txt.length() - 2);
        node->setString(txt);
        txtSize = node->getContentSize();
    };
    txt = txt + "...";
    node->setString(txt);
}

void UiBase::setBottomBtnPos(Node* node_btns)
{
    auto panel_btnForeign = dynamic_cast<Button*>(node_btns->getChildByName("btn_publish"));
    //    auto panel_btnTextbook = dynamic_cast<Widget*>(node_btns->getChildByName("btn_video"));
    auto panel_btnSeer = dynamic_cast<Widget*>(node_btns->getChildByName("btn_seer"));
    auto panel_btnMission = dynamic_cast<Widget*>(node_btns->getChildByName("btn_task"));
    auto panel_btnMe = dynamic_cast<Widget*>(node_btns->getChildByName("btn_user"));
    auto btn_card = dynamic_cast<Widget*>(node_btns->getChildByName("btn_card"));
    
    auto pos1 = panel_btnForeign->getPosition();
    auto pos2 = panel_btnMe->getPosition();
    panel_btnSeer->setVisible(false);
    btn_card->setPositionX(pos1.x + (pos2.x - pos1.x) / 3);
    panel_btnMission->setPositionX(pos1.x + (pos2.x - pos1.x) * 2 / 3);
    return;
}

void UiBase::resizeTextRect(cocos2d::ui::Text *node)
{
    auto txtSize = node->getContentSize();
    auto txt = node->getString();
    auto n = StringUtils::getCharacterCountInUTF8String(txt);
    while (1) {
        auto vSize = node->getVirtualRendererSize();
        if (vSize.height < txtSize.height) {
            break;
        } else {
            txt = Helper::getSubStringOfUTF8String(txt, 0, n - 4);
            node->setString(txt + "...");
        }
    };
}
        
cocos2d::Node* UiBase::createSpine(cocos2d::Node *nodeParent, float x, float y, const string &jsonFile, const string &atlasFile, const string &animationFile,
                         bool bLoop, float scale, float timeScale)
{
    if (nodeParent == nullptr) {
        return nullptr;
    }
    
    auto spineNode = SkeletonAnimation::createWithJsonFile(jsonFile, atlasFile, scale);
    if (spineNode == nullptr) {
        return nullptr;
    }
    spineNode->setTimeScale(timeScale);
    spineNode->setAnimation(0, animationFile, bLoop);
    spineNode->setPosition(Vec2(x, y));
    nodeParent->addChild(spineNode);
    return spineNode;
}
