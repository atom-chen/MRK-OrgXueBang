//
//  MyWordGameResult.hpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/20.
//

#ifndef MyWordGameResult_hpp
#define MyWordGameResult_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "stdafx.h"
#include "CDataHttp.hpp"

typedef std::function<void(const int type, void* data)> FuncCallBack;

class GameResultData;
class MyWordGameResult : public cocos2d::Node, public BaseKeyListener, public TableViewDataSource, public CDataHttpDelegate
{
public:
    MyWordGameResult();
    ~MyWordGameResult();
    static MyWordGameResult* create(cocos2d::Node* parent, void *data);

    virtual bool init(void *data);
    virtual bool OnBaseKeyBack() override;
    
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) override;
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(TableView *table) override;
    
    void getDataFromSever();
    virtual void endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json) override;
    
    void reloadData();
    void setCallBackFunc(const FuncCallBack& callback) {m_callback = callback;};
    string changeToTimeStr(long time);
    void starAnimation(int idx, int level, Node* star1, Node* star2, Node* star3);
protected:
    void initData(void *data);
private:
    cocos2d::Node* m_ui;
    TableView* m_tableView;
    Widget* m_item;
    
    GameResultData* m_data;
    
    FuncCallBack m_callback;
};

#endif /* MyWordGameResult_hpp */
