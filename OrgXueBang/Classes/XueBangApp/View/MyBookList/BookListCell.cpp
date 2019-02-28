//
//  BookListCell.cpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/4/9.
//



#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "UiBase.h"
#include "UIDef.hpp"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;

#include "UserManager.hpp"
#include "DownImg.h"
#include "BookListManager.hpp"
#include "BookListCell.hpp"
#include "Reador.hpp"

BookListCell::BookListCell()
{
    
}
BookListCell::~BookListCell()
{
    
}


bool BookListCell::init()
{
    if (!TableViewCell::init())
    {
        return false;
    }
    m_winSize = Director::getInstance()->getWinSize();
    return true;
}



void BookListCell::updateCell(const BOOK_INFO& info)
{
    this->removeAllChildren();
    
    auto node = CSLoader::createNode("View/MyBookList/booklistCell.csb");
    node->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(node);
    
    //加个线
    auto draw = DrawNode::create();
    draw->drawLine(Point(30, 0), Point(this->getContentSize().width-30, 0), Color4F(0.47,0.25,0.1,1));
    this->addChild(draw);
    
    auto root = node->getChildByName("Root");
    
    //ui
    {
#if 0
        string nameStr = Utils::replaceString(info.strName, 19, "...");
//        auto name = dynamic_cast<Text*>(root->getChildByName("name"));
        auto node_cliper = dynamic_cast<Node*>(root->getChildByName("node_cliper"));
        auto name = dynamic_cast<Text*>(node_cliper->getChildByName("name"));
        name->setString(nameStr);
#else
        auto node_cliper = dynamic_cast<Node*>(root->getChildByName("node_cliper"));
        auto name = dynamic_cast<Text*>(node_cliper->getChildByName("name"));
//        string nameStr = Utils::replaceString(info.strName, 19, "...");
        name->setString(info.strName);
        
        rollTextNode(node_cliper, name);
#endif
        auto word_count = dynamic_cast<Text*>(root->getChildByName("word_count"));
        std::string strCount = "单词数：" + info.strWordCount;
        word_count->setString(strCount);
        
        string tmpStrLead = info.strLead;
        std::u16string utf16String;
        if (StringUtils::UTF8ToUTF16(tmpStrLead, utf16String)) {
            int len = static_cast<int>(utf16String.length());
            if (len > 28) {
                utf16String = utf16String.substr(0, 28);
                StringUtils::UTF16ToUTF8(utf16String, tmpStrLead);
                tmpStrLead = tmpStrLead + "...";
            }
        }
//        string leadStr = Utils::replaceString(info.strLead, 56, "...");
        auto lead = dynamic_cast<Text*>(root->getChildByName("lead"));
        lead->setString(tmpStrLead);
    }
    
    //topic
    {
        auto topic_bg = dynamic_cast<ImageView*>(root->getChildByName("topic_bg"));
        auto topic_text = dynamic_cast<Text*>(root->getChildByName("topic_text"));
        
        auto pos = topic_bg->getPosition();
        for (int i = 0; i < info.vecTopic.size(); ++i)
        {
            auto text = topic_text->clone();
            dynamic_cast<Text*>(text)->setString(info.vecTopic[i]);
            
            auto bg = topic_bg->clone();
            bg->setContentSize(Size(text->getContentSize().width + 30, bg->getContentSize().height));
            
            
            text->setPosition(pos + Vec2(15, -3));
            bg->setPosition(pos);
            
            root->addChild(bg);
            root->addChild(text);
            
            //判断超出
            auto worldPos = root->convertToWorldSpace(Vec2(bg->getPositionX() + bg->getContentSize().width, bg->getPositionY()));
            if (worldPos.x > 750)
            {
                bg->setVisible(false);
                dynamic_cast<Text*>(text)->setString("...");
                break;
            }
            
            
            pos = pos + Vec2(bg->getContentSize().width + 20, 0);
        }
        
        
        topic_bg->setVisible(false);
        topic_text->setVisible(false);
    }
    
    //book
    {
        auto book = root->getChildByName("book");
        ImageView* img_book = (ImageView*)book->getChildByName("img_book");
        auto sizeNode = Node::create();
        sizeNode->setContentSize(book->getChildByName("panel_book")->getContentSize());
        img_book->setUserObject(sizeNode);
        
        auto img_BookBack = img_book->getChildByName("img_BookBack");
        img_BookBack->setVisible(false);
        img_BookBack->setLocalZOrder(-1);
        
        DownImg::getInstance()->DownFile(info.strCover.c_str(), img_book, "bookDefault.png");
        

        
        ImageView* img_level = (ImageView*)img_book->getChildByName("img_level");
        
        string level = info.strLevel;
        std::transform(level.begin(), level.end(), level.begin(), ::tolower);
        if (level.length() >= 1) {
            img_level->setVisible(true);
            img_level->loadTexture(StringUtils::format("Image/SubImg/Commom/BookItem/main_letter_%s.png", level.c_str()), Widget::TextureResType::PLIST);
        } else {
            img_level->setVisible(false);
        }
        

        img_book->setSwallowTouches(false);
        img_book->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType eventType){
            if (eventType == Widget::TouchEventType::BEGAN)
            {
                this->m_posBookTouch = ((Node*)pSender)->convertToWorldSpace(Vec2(0, 0));
            }
            else if (eventType == Widget::TouchEventType::ENDED)
            {
                Vec2 newPos = ((Node*)pSender)->convertToWorldSpace(Vec2(0, 0));
                if (std::abs(newPos.x - this->m_posBookTouch.x) < 50 && std::abs(newPos.y - this->m_posBookTouch.y) < 50)
                {
                    Reador::getInstance()->setCurrentBook(info.strId);
                    SceneManager::getInstance()->setGameState(eStateBook);
                }
            }
        });
        
 
//        CheckBox* chech_mark = (CheckBox*)img_book->getChildByName("chech_mark");
//        chech_mark->setVisible(true);
//        chech_mark->setSelected(true);
//        chech_mark->addEventListener([=](Ref* pSender, CheckBox::EventType eventType)
//        {
////            int n = panel_book[0]->getTag();
////            SoundManager::getInstance()->playSoundEffect("music/snd_mark.mp3");
////            if (eventType == CheckBox::EventType::SELECTED) {
////                this->onMarkBook(n * 3 + i, true);
////            } else if (eventType == CheckBox::EventType::UNSELECTED) {
////                this->onMarkBook(n * 3 + i, false);
////            }
//        });
    }
}

void BookListCell::rollTextNode(Node* parentNode, Text* text)
{
    float def_txt_width = 460.0f;
    float txtWidth = (int)text->getContentSize().width;
    if (txtWidth <= def_txt_width) {
        //书名宽度小于 def_txt_width = 460，无需滚动显示
        return;
    }
    text->setVisible(false);
    CCLOG("txtWidth = %f", txtWidth);

    TTFConfig ttfConfig;
    ttfConfig.fontFilePath = MYDEF_TTF_WDT_TIANNIU;
    ttfConfig.fontSize = text->getFontSize();
    ttfConfig.distanceFieldEnabled = false;
    ttfConfig.outlineSize = 0;

    Label* newTxt = Label::createWithTTF(ttfConfig, text->getString());
    CCAssert(NULL != newTxt, "");
    newTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    newTxt->setPosition(Vec2(0.0f, 10.0f));

    DrawNode* shap = DrawNode::create();
    Vec2 point[4] = {Vec2(0.0f, 0.0f), Vec2(def_txt_width, 0.0f), Vec2(def_txt_width, 60.0f), Vec2(0.0f, 60.0f)};
    shap->drawPolygon(point, 4, Color4F(255, 255, 255, 255), 2, Color4F(255, 255, 255, 255));

    ClippingNode* cliper = ClippingNode::create();
    cliper->setStencil(shap);
    cliper->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    cliper->setPosition(Vec2(0.0f, -6.0f));
    parentNode->addChild(cliper);
    //把要滚动的文字加入到裁剪区域
    cliper->addChild(newTxt);

#if 0
    auto call = CallFunc::create([=](){
        if ( newTxt ) {
            float tmpX = txtWidth - def_txt_width;
            Vec2 pos = newTxt->getPositionX() < -tmpX ? Vec2(0.0f, newTxt->getPositionY()) : Vec2(newTxt->getPositionX() - 1.0f, newTxt->getPositionY());
            newTxt->runAction(MoveTo::create(0.2f, pos));
        }
    });
#else
    auto call = CallFunc::create([=](){
        if ( newTxt ) {
            float tmpX = txtWidth;
            Vec2 pos = newTxt->getPositionX() < -tmpX ? Vec2(0.0f, newTxt->getPositionY()) : Vec2(newTxt->getPositionX() - 1.5f, newTxt->getPositionY());
            if (pos.x == 0.0f) {
                newTxt->setVisible(false);
                newTxt->setPositionX(def_txt_width);
                newTxt->setVisible(true);
            }else {
                newTxt->runAction(MoveTo::create(0.2f, pos));
            }
        }
    });
#endif
    auto s = Sequence::create(call, NULL);
    newTxt->runAction(RepeatForever::create(s));
}
