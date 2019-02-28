//
//  MyWordReport.hpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/14.
//

#ifndef MyWordReport_hpp
#define MyWordReport_hpp

#include "cocos2d.h"
#include "stdafx.h"
//#include "dataHttp.h"
#include "CDataHttp.hpp"

class MyWordReport : public cocos2d::Node, public BaseKeyListener, public CDataHttpDelegate
{
public:
    static MyWordReport* create(cocos2d::Node* parent);
    ~MyWordReport();
    virtual bool init() override;
    virtual void onEnter() override;
    virtual bool OnBaseKeyBack() override;
    
    void createReportInfo();
    void createWordProficiencyTable();
protected:
    cocos2d::Node* m_ui;

    void showReportInfo();

    void getDataFromSever();
    virtual void endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json) override;
private:
    float m_panel_centerY;
    float m_panel_bottomY;

    //data
    int m_todayLearn;
    int m_todayUpgrade;
    string m_gameRate;
    int m_totalWords;
    
    int m_level0Words;
    int m_level1Words;
    int m_level2Words;
    int m_level3Words;
};

#endif /* MyWordReport_hpp */
