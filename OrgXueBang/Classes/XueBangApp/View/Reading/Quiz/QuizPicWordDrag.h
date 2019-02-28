//
//  QuizPicWordDrag.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/23.
//
//

#ifndef QuizPicWordDrag_h
#define QuizPicWordDrag_h

#include "QuizBase.h"


typedef struct QuizPicDrag{
    std::string word;
    bool m_canDrag;
    Vec2 m_originPos;
    cocos2d::Node *node;
}stuQuizPicDrag;


class QuizPicWordDrag : public QuizBase
{
    typedef struct QuizTargetOne {
        int nId;
        string text;
        string audio;
    }stuQuizTargetOne;
    
    typedef struct QuizMatchOne {
        int nId;
//        int order;
        string img;
        string audio;
//        string text;
    }stuQuizMatchOne;

public:
    static QuizPicWordDrag* create(cocos2d::Node* parent);
    ~QuizPicWordDrag();
    virtual bool init() override;
    
    void setWord();
    void checkDrag(int n, Vec2 pos);
    void checkOk();
    virtual void setParam(std::string imgFile, std::string voiceFile, std::string xmlFile) override;
//    void setResPath(const string& path){m_resPath = path;};
    void playAudio(int n);
protected:
    cocos2d::Node* m_parent;
    cocos2d::Node* m_ui;
    stuQuizPicDrag m_pic[4];
    stuQuizPicDrag m_word[4];
    cocos2d::Layer* m_layer;
    
    Vec2 m_touchPosOld, m_touchPosNew;
    bool m_bDraging;
    
    int m_quesCount;
    
    vector<stuQuizTargetOne> m_targets;
    vector<stuQuizMatchOne> m_matches;
    
//    string m_resPath;
    bool m_initOk;
};


#endif /* QuizPicWordDrag_h */
