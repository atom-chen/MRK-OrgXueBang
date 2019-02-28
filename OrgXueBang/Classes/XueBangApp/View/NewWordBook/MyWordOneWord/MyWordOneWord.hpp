//
//  MyWordOneWord.hpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/27.
//

#ifndef MyWordOneWord_hpp
#define MyWordOneWord_hpp

#include "cocos2d.h"
#include "stdafx.h"
#include "CDataHttp.hpp"

class MyWordOneWord : public cocos2d::Node, public BaseKeyListener, public CDataHttpDelegate
{
public:
    static MyWordOneWord* create(Node* parent, string& word, string& audioPath);
    MyWordOneWord();
    ~MyWordOneWord();
    virtual bool init(string& word, string& audioPath);
    virtual void onEnter() override;
    virtual bool OnBaseKeyBack() override;
    void playWordAudio(string path);
protected:
    
    void getDataFromSever(const std::string& word);
    virtual void endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json) override;
    
    void initView();
    void createWordSkeletonBg();
    void updateWordInfo(string& word, string& enDesc, string& cnDesc);
private:
    cocos2d::Node* m_ui;
    SkeletonAnimation* m_skeleton_word;
    SkeletonAnimation* m_skeleton_loading;

    std::string m_word;
    std::string m_audioPath;
    int m_audioId;
};

#endif /* MyWordOneWord_hpp */
