//
//  BookGuide.cpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/10/31.
//

#include "BookGuide.hpp"
#include "BaseLayer.hpp"
//#include "MachineStart.hpp"
#include "MachineManager.h"
#include "BookBody.hpp"

BookGuide::BookGuide(bool isDuBaFlag)
:isDuBa(isDuBaFlag)
{
}
BookGuide::~BookGuide()
{
}
BookGuide* BookGuide::create(bool isDuBaFlag)
{
//    isDuBaFlag = true;
    BookGuide *ret = new (std::nothrow) BookGuide(isDuBaFlag);
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}
void BookGuide::onEnter()
{
    BaseLayer::onEnter();
}
void BookGuide::onExit()
{
    BaseLayer::onExit();
}

void BookGuide::setBookBody(BookBody* body)
{
    mBookBody = body;
}
bool BookGuide::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    initUI();
    return true;
}
void BookGuide::initUI()
{
    auto mUI = CSLoader::createNode("View/Reading/BookGuide.csb");
    this->addChild(mUI);
    
    //关键词txt_keywords
    //词数txt_words_num
    //简介txt_introduce
    //播放按钮btn_produce
    //导读页独有-继续阅读--v2朗读btn_oral
    //导读页独有-重新开始--v2答题btn_read
    //导读页独有-开始--v2阅读btn_quiz
    
    vector<string> vecBtnName = {"btn_read","btn_oral","btn_quiz","btn_record_show","btn_main_right","btn_read_0","btn_machine"};
    vector<string> vecNodeName = {
        "slider_keywords","slider_introduce","list_keywords","list_introduce",
        "panel_guide","img_gde_bg","img_gde_top",
        "panel_anchor","txt_title","txt_words_tip","txt_words_num",
        "txt_keywords","txt_introduce","normalRead","machineRead",
    };
    
    for (int i=0; i < vecBtnName.size(); i++) {
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), vecBtnName[i]);
        btn->addTouchEventListener(CC_CALLBACK_2(BookGuide::touchEvent, this));
        if(i < 5){
            btn->setColor(Color3B::GRAY);
        }
        
        mapWidget[vecBtnName[i]] = btn;
    }
    for (int i=0; i < vecNodeName.size(); i++) {
        Widget* btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), vecNodeName[i]);
        mapWidget[vecNodeName[i]] = btn;
        
        if(vecNodeName[i] == "slider_keywords" or vecNodeName[i] == "slider_introduce"){
            auto temp = static_cast<Slider*>(btn);
            temp->setTouchEnabled(true);
            temp->setPercent(0);
            temp->addEventListener(CC_CALLBACK_2(BookGuide::sliderEvent, this));
        }else if(vecNodeName[i] == "list_keywords" or vecNodeName[i] == "list_introduce"){
            auto temp = static_cast<ListView*>(btn);
            temp->setScrollBarEnabled(false);
            temp->addEventListener((cocos2d::ui::ListView::ccListViewCallback)CC_CALLBACK_2(BookGuide::listItemEventSelect, this));
            temp->addEventListener((cocos2d::ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(BookGuide::listItemEventScroll,this));
        }
    }
    mapWidget["img_gde_top"]->setPositionY(mapWidget["panel_guide"]->getContentSize().height + mapWidget["img_gde_top"]->getContentSize().height);
    
    mapWidget["history_score"] = (Widget*)mapWidget["panel_anchor"]->getChildByName("history_score");
    mapWidget["history_nut"] = (Widget*)mapWidget["panel_anchor"]->getChildByName("history_nut");
    
    
    bool formMachine = false;
    mapWidget["normalRead"]->setVisible(not formMachine);
    mapWidget["machineRead"]->setVisible(formMachine);
    
    mapWidget["btn_read_0"]->setEnabled(true);
    mapWidget["btn_machine"]->setEnabled(true);
    
    mapWidget["btn_main_right"]->setEnabled(true);
    mapWidget["btn_main_right"]->setColor(Color3B::WHITE);
    
    if(isDuBa){
        mapWidget["normalRead"]->setVisible(true);
        mapWidget["machineRead"]->setVisible(false);
        mapWidget["history_score"]->setVisible(false);
        mapWidget["history_nut"]->setVisible(false);
        mapWidget["btn_oral"]->setVisible(false);
        mapWidget["btn_quiz"]->setVisible(false);
        mapWidget["btn_record_show"]->setVisible(false);
        mapWidget["btn_main_right"]->setVisible(false);
        
        mapWidget["btn_read"]->setPositionX(0);
    }
    setBtnShow();
}

void BookGuide::touchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    CLICK_FILTER(sender, type);
    SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
    
    Node* pSender = (Node*)sender;
    const std::string& name = pSender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    if(name == "btn_read" or name == "btn_read_0"){
        if(mBookBody->doBtnHandler(1)){
            bool formMachine = MachineManager::getInstance()->getMachineData()->getFromMachine();
            talkingInterface::traceEvent(formMachine?"课程":"原版导读按钮",formMachine?"阅读":"看读");
        }
    }else if(name == "btn_oral"){
        if(mBookBody->doBtnHandler(2)){
            talkingInterface::traceEvent("原版导读按钮", "朗读");
        }
    }else if(name == "btn_quiz"){
        if(mBookBody->doBtnHandler(3)){
            talkingInterface::traceEvent("原版导读按钮", "答题");
        }
    }else if(name == "btn_record_show"){
        if(mBookBody->doBtnHandler(4)){
            talkingInterface::traceEvent("原版导读按钮", "录音秀");
        }
    }else if(name == "btn_main_right"){
        if(mBookBody->doBtnHandler(5)){
            talkingInterface::traceEvent("原版导读按钮", "右箭头");
        }
    }else if(name == "btn_machine"){
//        Game::sharedGame()->setGameState(eStateMain);
//        UiBase::getRunningScene()->addChild(MachineStart::create());
//        talkingInterface::traceEvent("课程","竞技");
    }
}
void BookGuide::sliderEvent(Ref *sender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_SLIDEBALL_DOWN)
    {
    }
    else if (type == Slider::EventType::ON_SLIDEBALL_UP)
    {
    }
    else if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = (Slider*)sender;
        int percent = slider->getPercent();
        const std::string& name = slider->getName();
        ListView* listTarget = nullptr;
        if(name == "slider_introduce")
        {
            listTarget = (ListView*)mapWidget["list_introduce"];
        }
        else if(name == "slider_keywords")
        {
            listTarget = (ListView*)mapWidget["list_keywords"];
        }
        ILog(percent)
        if(listTarget)
            listTarget->scrollToPercentVertical((float)percent, 0.2f, true);
    }
}

void BookGuide::listItemEventScroll(Ref* pSender, cocos2d::ui::ScrollView::EventType type)
{
    switch (type) {
        case cocos2d::ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            
            break;
        case cocos2d::ui::ScrollView::EventType::SCROLL_TO_TOP:
            
            break;
        case cocos2d::ui::ScrollView::EventType::CONTAINER_MOVED:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            const std::string& name = listView->getName();
            if (name == "list_introduce") {
                float minY = listView->getContentSize().height - listView->getInnerContainerSize().height;
                float h = - minY;
                //FLog(minY + percent * h / 100.0f)
                auto temp = static_cast<Slider*>(mapWidget["slider_introduce"]);
                temp->setPercent((listView->getInnerContainerPosition().y - minY) / h * 100.f);
            }else if (name == "list_keywords") {
                float minY = listView->getContentSize().height - listView->getInnerContainerSize().height;
                float h = - minY;
                //FLog(minY + percent * h / 100.0f)
                auto temp = static_cast<Slider*>(mapWidget["slider_keywords"]);
                temp->setPercent((listView->getInnerContainerPosition().y - minY) / h * 100.f);
            }
        }
            break;
        default:
            break;
    }
}
void BookGuide::listItemEventSelect(Ref *pSender, ListView::EventType type)
{
    ListView* listView = static_cast<ListView*>(pSender);
    switch (type)
    {
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
        {
            
            break;
        }
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
        {
            
            break;
        }
        default:
            break;
    }
}
void BookGuide::updateUI(Ref* data)
{
    
}
void BookGuide::setWholeBookUpdateUI(WholeBook& m_wholeBook)
{
    ((Text*)mapWidget["txt_words_num"])->setString(m_wholeBook.m_bookData->word_count);
    Text* txt_title = (Text*)mapWidget["txt_title"];
    if (m_wholeBook.m_bookData->name != "-1")
    {
        Label* lbl_title = Label::createWithTTF(m_wholeBook.m_bookData->name, txt_title->getFontName(), txt_title->getFontSize());
        lbl_title->setDimensions(txt_title->getContentSize().width, txt_title->getContentSize().height);
        lbl_title->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        lbl_title->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        lbl_title->setTextColor(txt_title->getTextColor());
        lbl_title->setPosition(txt_title->getContentSize()/2);
        lbl_title->setOverflow(Label::Overflow::SHRINK);
        lbl_title->enableShadow(txt_title->getShadowColor(), txt_title->getShadowOffset(), txt_title->getShadowBlurRadius());
        lbl_title->enableOutline(txt_title->getShadowColor(), txt_title->getOutlineSize());
        txt_title->addChild(lbl_title);
    }
    
    if (m_wholeBook.m_bookData->topics != "-1")
    {
        Text* txt_keywords = (Text*)mapWidget["txt_keywords"];
        ListView* list_keywords = (ListView*)mapWidget["list_keywords"];
        auto slider_keywords = mapWidget["slider_keywords"];
        
        UiBase::setText(txt_keywords, m_wholeBook.m_bookData->topics,0);
        txt_keywords->retain();
        txt_keywords->removeFromParent();
        list_keywords->pushBackCustomItem(txt_keywords);
        
        if(txt_keywords->getContentSize().height > list_keywords->getContentSize().height)
        {
            slider_keywords->setVisible(true);
        }
        else
        {
            slider_keywords->setVisible(false);
        }
    }
    if (m_wholeBook.m_bookData->intro != "-1")
    {
        Text* txt_introduce = (Text*)mapWidget["txt_introduce"];
        ListView* list_introduce = (ListView*)mapWidget["list_introduce"];
        auto slider_introduce = mapWidget["slider_introduce"];
        
        UiBase::setText(txt_introduce, m_wholeBook.m_bookData->intro,0);
        txt_introduce->retain();
        txt_introduce->removeFromParent();
        list_introduce->pushBackCustomItem(txt_introduce);
        
        if(txt_introduce->getContentSize().height > list_introduce->getContentSize().height)
        {
            slider_introduce->setVisible(true);
        }
        else
        {
            slider_introduce->setVisible(false);
        }
    }
    mapWidget["btn_read"]->setColor(Color3B::WHITE);
    mapWidget["btn_read"]->setEnabled(true);
    if(m_wholeBook.m_bookData->sum_status || m_wholeBook.m_bookData->read_flag)
    {
        mapWidget["btn_oral"]->setColor(Color3B::WHITE);
        mapWidget["btn_quiz"]->setColor(Color3B::WHITE);
        mapWidget["btn_record_show"]->setColor(Color3B::WHITE);
        mapWidget["btn_oral"]->setEnabled(true);
        mapWidget["btn_quiz"]->setEnabled(true);
        mapWidget["btn_record_show"]->setEnabled(true);
    }
    
    int read = m_wholeBook.m_bookData->sum_read_nuts > 5 ? 5: m_wholeBook.m_bookData->sum_read_nuts;//liuwei 历史坚果
    int quiz = m_wholeBook.m_bookData->sum_quiz_nuts> 5 ? 5: m_wholeBook.m_bookData->sum_quiz_nuts;
    int sumnuts = read + quiz;
    
    auto history_nut = mapWidget["panel_anchor"]->getChildByName("history_nut");
    auto children = history_nut->getChildren();
    for(int i = 0; i < children.size(); ++i)
    {
        children.at(i)->setVisible(false);
    }
    for(int i = 0; i < sumnuts; ++i)
    {
        children.at(i)->setVisible(true);
    }
    
    
    if(m_wholeBook.m_bookData->sum_status || m_wholeBook.m_bookData->read_flag
       //答题到一半，未走报告也，回导读点亮后两个按钮，容易跟大退图书混淆，防止用户误以为BUG，因此干掉@dj
       //||m_wholeBook.m_bookData->sum_read_1 + m_wholeBook.m_bookData->sum_read_2
       //+ m_wholeBook.m_bookData->sum_read_3 + m_wholeBook.m_bookData->sum_quiz_1
       //+ m_wholeBook.m_bookData->sum_quiz_2 + m_wholeBook.m_bookData->sum_quiz_3
       //+ m_wholeBook.m_bookData->sum_quiz_4 + m_wholeBook.m_bookData->sum_quiz_5
       //+ m_wholeBook.m_bookData->cur_read_1 + m_wholeBook.m_bookData->cur_read_2
       //+ m_wholeBook.m_bookData->cur_read_3 + m_wholeBook.m_bookData->cur_quiz_1
       //+ m_wholeBook.m_bookData->cur_quiz_2 + m_wholeBook.m_bookData->cur_quiz_3
       //+ m_wholeBook.m_bookData->cur_quiz_4 + m_wholeBook.m_bookData->cur_quiz_5 > 0
       )
    {
        mapWidget["btn_oral"]->setColor(Color3B::WHITE);
        mapWidget["btn_quiz"]->setColor(Color3B::WHITE);
        mapWidget["btn_record_show"]->setColor(Color3B::WHITE);
        mapWidget["btn_oral"]->setEnabled(true);
        mapWidget["btn_quiz"]->setEnabled(true);
        mapWidget["btn_record_show"]->setEnabled(true);
    }
}

void BookGuide::doActionOpen()
{
    this->setVisible(true);
    mapWidget["img_gde_bg"]->setOpacity(255);
    
    auto move3 = MoveTo::create(1, Vec2(mapWidget["panel_guide"]->getContentSize().width/2, 716));
    auto ease3 = EaseSineOut::create(move3);
    mapWidget["img_gde_top"]->runAction(ease3);
}

void BookGuide::doActionClose()
{
    auto move3 = MoveTo::create(1, Vec2(mapWidget["panel_guide"]->getContentSize().width/2, mapWidget["panel_guide"]->getContentSize().height + mapWidget["img_gde_top"]->getContentSize().height));
    auto ease3 = EaseSineIn::create(move3);
    mapWidget["img_gde_top"]->runAction(ease3);
}

Widget* BookGuide::getWidget(string name)
{
    return mapWidget[name];
}

void BookGuide::setBtnShow()
{
    mapWidget["normalRead"]->setVisible(true);
    mapWidget["machineRead"]->setVisible(false);
    mapWidget["history_score"]->setVisible(false);
    mapWidget["history_nut"]->setVisible(false);
    mapWidget["btn_oral"]->setVisible(false);
    mapWidget["btn_quiz"]->setVisible(false);
    mapWidget["btn_record_show"]->setVisible(false);
    mapWidget["btn_main_right"]->setVisible(false);
    
    mapWidget["btn_read"]->setPositionX(0);
}
