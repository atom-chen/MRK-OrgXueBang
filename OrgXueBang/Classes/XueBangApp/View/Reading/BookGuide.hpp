//
//  BookGuide.hpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/10/31.
//

#ifndef BookGuide_hpp
#define BookGuide_hpp

#include "stdafx.h"
#include "BaseLayer.hpp"
#include "BookStruct.hpp"

class BookBody;

class BookGuide : public BaseLayer
{
public:
    BookGuide(bool isDuBaFlag = false);
    virtual ~BookGuide();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
//    CREATE_FUNC(BookGuide);
    static BookGuide* create(bool isDuBaFlag = false);
    void setBookBody(BookBody* body);
public:
    
    void setWholeBookUpdateUI(WholeBook& wholeBook);
    void doActionOpen();
    void doActionClose();
    Widget* getWidget(string name);
private:
    void initUI();
    void touchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
    void listItemEventScroll(Ref* pSender, cocos2d::ui::ScrollView::EventType type);
    void listItemEventSelect(Ref *pSender, ListView::EventType type);
    void updateUI(Ref* data);
    void setBtnShow();
private:
    BookBody* mBookBody;
    std::unordered_map<string, Widget*> mapWidget;
    bool isDuBa;
};

#endif /* BookGuide_hpp */
