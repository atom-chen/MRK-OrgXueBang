//
//  QuizSentence.h
//  ReadingMate
//
//  Created by 丁晶 on 2017/1/12.
//
//

#ifndef QuizSentence_h
#define QuizSentence_h

#include "QuizBase.h"

#define MAX_WORD_QUIZSENTENCE 12

class QuizSentence : public QuizBase
{
    
    typedef struct QuizLetter{
        string letter;
        int state;//0: 显示 1:空白 2:放入
        cocos2d::Node *node;
        void setState(int state);
    }stuQuizLetter;
    
    typedef struct QuizLetterDrag{
        string letter;
        bool m_canDrag;
        Vec2 m_originPos;
        cocos2d::Node *node;
        string audio;
    }stuQuizLetterDrag;
    
    typedef struct QuizTargetOne {
        int nId;
    }stuQuizTargetOne;
    
    typedef struct QuizMatchOne {
        int nId;
        int order;
        string text;
        string audio;
    }stuQuizMatchOne;
    
public:
    static QuizSentence* create(cocos2d::Node* parent);
    ~QuizSentence();
    virtual bool init() override;
    
    
    void setWord();
    void checkDrag(int n, Vec2 pos);
    void checkOk();
    virtual void setParam(std::string imgFile, std::string voiceFile, std::string xmlFile) override;
    
    void playAudio(int n);
    int playSentenceAudio();
    virtual int inPage() override;
protected:
    cocos2d::Node* m_parent;
    cocos2d::Node* m_ui;
    int m_letterNum;
    stuQuizLetter m_letter[MAX_WORD_QUIZSENTENCE];
    int m_letterDragNum;
    stuQuizLetterDrag m_letterDrag[MAX_WORD_QUIZSENTENCE];
    
    cocos2d::Layer* m_layer;
    
    Vec2 m_touchPosOld, m_touchPosNew;
    bool m_bDraging;
    
    vector<stuQuizTargetOne> m_targets;
    vector<stuQuizMatchOne> m_matches;
    
    string m_audioFile;
    
    int m_blankNum;
    
    int m_maxFont;
};


#endif /* QuizSentence_h */
