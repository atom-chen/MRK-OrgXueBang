//
//  UIMainBookCell.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/23.
//

#include "stdafx.h"
#include "dataBook.h"

#ifndef UIMainBookCell_hpp
#define UIMainBookCell_hpp

class UIMainBookCell : public cocos2d::extension::TableViewCell
{
public:
    CREATE_FUNC(UIMainBookCell);
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;

    void initUI();
    void updateCell(EntOneWeekBookList* entBookList,bool isCurWeek);
private:
    UIMainBookCell();
    ~UIMainBookCell();
    void touchBookEvent(Ref *pSender, Widget::TouchEventType eventType);
    void doClickBookHandle(void* data);
    void onRevMsgDoHandle(Ref* ref = nullptr);
private:
    std::unordered_map<string, Widget*> mapWidget;
    std::unordered_map<string, Node*> mapBookWidget;
    float maxDistance;
    Vec2 touchBeginPos;
    EntOneWeekBookList* curEntOneWeekBookList;
};

#endif /* UIMainBookCell_hpp */
