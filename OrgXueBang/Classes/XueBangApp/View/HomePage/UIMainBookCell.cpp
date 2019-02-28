//
//  UIMainBookCell.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/23.
//

#include "UIMainBookCell.hpp"
#include "Reador.hpp"
#include "HomePageManager.hpp"

void UIMainBookCell::onEnter()
{
    TableViewCell::onEnter();
}
void UIMainBookCell::onExit()
{
    TableViewCell::onExit();
    
}
UIMainBookCell::UIMainBookCell()
{
    MessageManager::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(UIMainBookCell::onRevMsgDoHandle),NSEventNameDef::ReadingEnd);
}
UIMainBookCell::~UIMainBookCell()
{
    MessageManager::getInstance()->removeObserver(this);
}
bool UIMainBookCell::init()
{
    if(!TableViewCell::init()){
        return false;
    }
    this->setName("UIMainBookCell");
//    initUI();
    CCLOG("UIMainBookCell::init");
    return true;
}

void UIMainBookCell::initUI()
{
    this->removeAllChildren();
    
    auto mUI = CSLoader::createNode("View/HomePage/NodeItemBookUnit.csb");
    this->addChild(mUI);
    
    auto nodeRoot = mUI->getChildByName("nodeRoot");
    
    vector<string> vecNodeName = {
        "PanelBG","imgBG","txtTitle","lvBook","panel_book","txtDateTime",
//        "img_book","img_BookBack","img_read","img_level",
    };
    
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(nodeRoot), name);
        mapWidget[name] = btn;
    }
    mapWidget["panel_book"]->retain();
    mapWidget["panel_book"]->removeFromParent();
    mapWidget["lvBook"]->removeAllChildren();
    
    ((ListView*)mapWidget["lvBook"])->setSwallowTouches(false);
    ((Text*)mapWidget["txtTitle"])->setString("");
}

void UIMainBookCell::updateCell(EntOneWeekBookList* entBookList,bool isCurWeek)
{
    if(!entBookList){
        CCLOG("xuebang--error,void UIMainBookCell::updateCell(EntOneWeekBookList* entBookList) entBookList==null");
        return;
    }
    curEntOneWeekBookList = entBookList;
    mapBookWidget.clear();
    
    mapWidget["lvBook"]->removeAllChildren();
    
    if(isCurWeek){
        mapWidget["imgBG"]->setColor(Color3B(255,192,203));
    }else{
        mapWidget["imgBG"]->setColor(Color3B::WHITE);
    }
    ((Text*)mapWidget["txtTitle"])->setString(entBookList->_Title);
    ((Text*)mapWidget["txtDateTime"])->setString(entBookList->_DateTime);
    for(auto book : entBookList->vecBook){
        auto item = (Layout*)mapWidget["panel_book"]->clone();
        ImageView* imgBook = (ImageView*)item->getChildByName("img_book");
        imgBook->setTouchEnabled(true);
        imgBook->setSwallowTouches(false);
        imgBook->addTouchEventListener(CC_CALLBACK_2(UIMainBookCell::touchBookEvent, this));
        imgBook->getChildByName("img_read")->setVisible(book->_IsReadFlag);
        mapBookWidget[book->_BookID] = imgBook->getChildByName("img_read");
        
        ImageView* img_level = (ImageView*)imgBook->getChildByName("img_level");
        if (book->_BookLevel.length() >= 1) {
            img_level->setVisible(true);
            img_level->loadTexture(StringUtils::format("Image/SubImg/Commom/BookItem/main_letter_%s.png", book->_BookLevel.c_str()), Widget::TextureResType::PLIST);
        } else {
            img_level->setVisible(false);
        }
//        book->_CoverUrl = "http://db-app-source.dubaner.com/resource/pdf_img/test/NOAJ-004/NOAJ-004111.png";
        DownImg::getInstance()->DownFile(book->_CoverUrl.c_str(), imgBook);//, "bookDefault.png"
        imgBook->setUserData(&book->_BookID);
        ((ListView*)mapWidget["lvBook"])->pushBackCustomItem(item);
    }
    if(entBookList->vecBook.size() > 3){
        ((ListView*)mapWidget["lvBook"])->setScrollBarEnabled(true);
    }else{
        ((ListView*)mapWidget["lvBook"])->setScrollBarEnabled(false);
    }
}

void UIMainBookCell::touchBookEvent(Ref *pSender, Widget::TouchEventType eventType)
{
    //ListView滑动小于10像素时表示点击事件，滑动时不会触发点击事件
    ImageView* cb = (ImageView*)pSender;
    if (eventType == Widget::TouchEventType::BEGAN) {
        touchBeginPos = cb->convertToWorldSpace(Vec2(0, 0));
        maxDistance = 0;
        CCLOG("BEGAN");
    } else if (eventType == Widget::TouchEventType::MOVED) {
        Vec2 newPos = cb->convertToWorldSpace(Vec2(0, 0));
        float dis = newPos.distance(touchBeginPos);
        if(dis > maxDistance){
            maxDistance = dis;
        }
        log("MOVED--%f",dis);
    } else if (eventType == Widget::TouchEventType::ENDED) {
        if (maxDistance < 10) {
            doClickBookHandle(cb->getUserData());
        }
        CCLOG("ENDED");
    } else if (eventType == Widget::TouchEventType::CANCELED) {
        CCLOG("CANCELED");
    }
}

void UIMainBookCell::doClickBookHandle(void* data)
{
    string* id = (string*)data;
    string bookID = *id;
    CCLOG("UIMainBookCell::doClickBookHandle----bookID=%s",bookID.c_str());
    
    HomePageManager::getInstance()->setReadingBookInfo(curEntOneWeekBookList->_WeekID,bookID);
    Reador::getInstance()->setCurrentBook(bookID);
    SceneManager::getInstance()->setGameState(eStateBook);
}

void UIMainBookCell::onRevMsgDoHandle(Ref* ref)
{
    MsgParam* param = (MsgParam*)ref;
    string weekID = param->getVecTemp()[0];
    string bookID = param->getVecTemp()[1];
    if(mapBookWidget[bookID]){
        mapBookWidget[bookID]->setVisible(true);
    }
}
