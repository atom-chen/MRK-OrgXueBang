//
//  MyWord.hpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/8.
//

#ifndef MyWord_hpp
#define MyWord_hpp

#include "cocos2d.h"
#include "stdafx.h"
#include "CDataHttp.hpp"

class MyWordTableByLetter;
class MyWordTableByProficiency;

typedef struct wordStruct
{
    int id;
    string word;
    string image;
    string audio;
    string desc;
    int level;
    int right_gameTimes;
    int total_gameTimes;
}stuWord;

class MyWord : public cocos2d::Node, public BaseKeyListener, public CDataHttpDelegate
{
public:
    static MyWord* create(cocos2d::Node* parent, bool bDefShowLetter = false);
    MyWord();
    ~MyWord();
    virtual bool init(bool bDefShowLetter = false);
    virtual void onEnter() override;
    virtual bool OnBaseKeyBack() override;
    virtual void onExit() override;
    
    void createWordLetterTable();
    void createWordProficiencyTable();
protected:
    void showWordLetterTable(bool firstEnter = false);
    void showWordProficiencyTable(bool firstEnter = false);
    
    void getDataFromSever(int type);
    virtual void endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json) override;
    
private:
    cocos2d::Node* m_ui;
    float wordTableLetterX;
    float wordTableProficiencyX;
    bool m_bDefaultShowLetter;
    
    SkeletonAnimation* m_skeleton_btnPlay;
    MyWordTableByLetter* m_wordTableLetter;
    MyWordTableByProficiency* m_wordTableProficiency;
};

#endif /* MyWord_hpp */
