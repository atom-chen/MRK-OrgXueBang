//
//  MyWordGameSubjectItem.hpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/17.
//

#ifndef MyWordGameSubjectItem_hpp
#define MyWordGameSubjectItem_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "stdafx.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameSubject;
typedef std::function<void(const int wordId, const string wordDesc, const bool bRight)> GameSubjectCllback;
class MyWordGameSubjectItem : public TableViewDataSource
{
public:
    MyWordGameSubjectItem();
    ~MyWordGameSubjectItem();
    
    void createTable(Node* parent);
    void reloadData();
    TableView* getTableView(){return m_tableView;};
    void setUnitData(std::vector<GameSubject*>& wordList);
    
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    void setCallBackFunc(const GameSubjectCllback& callback) {m_callback = callback;};
protected:
    TableView* m_tableView;
    Widget* m_item;
    
    GameSubjectCllback m_callback;
    
    std::vector<GameSubject*> m_subjectList;
    
    int m_rightId;
    string m_rightDescStr;
    bool m_clicked;
};

#endif /* MyWordGameSubjectItem_hpp */
