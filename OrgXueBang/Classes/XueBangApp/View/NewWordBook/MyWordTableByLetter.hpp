//
//  MyWordTableByLetter.hpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/12.
//

#ifndef MyWordTableByLetter_hpp
#define MyWordTableByLetter_hpp

//#include "cocos2d.h"
//#include "extensions/cocos-ext.h"
//
//USING_NS_CC;
//USING_NS_CC_EXT;


//#include "CDataHttp.h"
#include "stdafx.h"

class wordStruct;
class MyWordTableByLetterChild;
class MyWordTableByLetter : public TableViewDataSource
{
public:    
    typedef struct WordUnit{
        string unit;
        std::vector<wordStruct*> wordList;
        bool bHasData;
    }stuWordUnit;

public:
    MyWordTableByLetter();
    ~MyWordTableByLetter();
    
    void createTable(Node* parent);
    void setUnits(stuWordUnit* sheets[], int n);
    void onClickUnit(int n);
    void reloadData();
    TableView* getTableView(){return m_tableView;};
    
    void setUnitData(std::vector<wordStruct*> wordList, int unitId);
    void createUnitWords(Layout* node, int n);
    
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);

    void showDefaultUnit();
    void setCellSelectVisible(TableView *table, int cellIdx, bool isVisible);
    void setTableTouched(bool b);
protected:
    MyWordTableByLetterChild* m_wordTableChild;
    TableView* m_tableView;
    Widget* m_item;
    
    std::vector<stuWordUnit*> m_unitList;
    int m_showUnit;
    bool m_bTouched;
};

#endif /* MyWordTableByLetter_hpp */
