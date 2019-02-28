//
//  MyWordTableByLetterChild.hpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/12.
//

#ifndef MyWordTableByLetterChild_hpp
#define MyWordTableByLetterChild_hpp

//#include "cocos2d.h"
//#include "extensions/cocos-ext.h"
//
//USING_NS_CC;
//USING_NS_CC_EXT;

#include "stdafx.h"

//#include "dataHttp.h"
class wordStruct;

class MyWordTableByLetterChild : public TableViewDataSource
{
public:

    ~MyWordTableByLetterChild();
    
    void createTable(Node* parent);
    void reloadData();
    TableView* getTableView(){return m_tableView;};
    void setUnitData(std::vector<wordStruct*>& wordList);

    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);

    void touchEvent(Ref* sender, Widget::TouchEventType type);
    void nodeSelectAnimation(Node* node, bool bSelect);
protected:
    TableView* m_tableView;
    Widget* m_item;
    
    std::vector<wordStruct*> m_wordList;
    Vec2 m_clickPos;
};

#endif /* MyWordTableByLetterChild_hpp */
