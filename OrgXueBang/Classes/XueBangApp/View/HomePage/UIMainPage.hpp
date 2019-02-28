//
//  UIMainPage.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/15.
//

#ifndef UIMainPage_hpp
#define UIMainPage_hpp

#include "stdafx.h"

#include "dataBook.h"

class UIMainPage : public Layer
,public TableViewDataSource
,public TableViewDelegate
,public BaseKeyListener
{
public:
    CREATE_FUNC(UIMainPage);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    virtual bool OnBaseKeyBack() override;
    
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) override;
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(TableView *table) override;
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override;
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) override;
    
    virtual void tableCellTouched(TableView* table, TableViewCell* cell) override;
//    virtual void tableCellHighlight(TableView* table, TableViewCell* cell) override;
//    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell) override;
//    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell) override;
private:
    UIMainPage();
    ~UIMainPage();
    void initUI();
    void initTableView();
    void btnClickHandle(Ref* pSender);
    void onRevMsgDoHandle(Ref* ref);
    void goToCell(int index);
    void reflashData();
private:
    std::unordered_map<string, Widget*> mapWidget;
    TableView* mTableView;
    Size mItemSize;
    bool isGoToCurCell;
    bool isReqLoading;
    
    struct timeval _lastUpdate;
};

#endif /* UIMainPage_hpp */
