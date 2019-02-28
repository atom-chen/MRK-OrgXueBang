//
//  WordFind.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/8.
//
//

#ifndef WordFind_h
#define WordFind_h

#include "cocos2d.h"
#include "dataBook.h"
#include "stdafx.h"
#include "CDataHttp.hpp"

class WordFind : public cocos2d::Node, public CDataHttpDelegate, public BaseKeyListener
{
public:
    static WordFind* create(cocos2d::Node* parent, int offy, string& word);
    ~WordFind();
    virtual bool init();

    void find(const std::string& word);
    void setResult(stuWordFind* result);
    void addToBook();
    void playSound();
    
    virtual void endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json);
    
    void setVoice(const string& ogg, int start, int total);
    virtual bool OnBaseKeyBack() override;
    
    void showAddEff();
protected:
    cocos2d::Node* m_ui;
    cocos2d::Node* m_parent;
    stuWordFind m_Find;
    int m_offy;
    string m_word;
    
    string m_oggFile;
    int m_vStart;
    int m_vTotal;
    
    SkeletonAnimation* m_sp;
};

#endif /* WordFind_h */
