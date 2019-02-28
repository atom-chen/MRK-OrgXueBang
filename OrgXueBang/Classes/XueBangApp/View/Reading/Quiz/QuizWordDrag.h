//
//  QuizWordDrag.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/13.
//
//

#ifndef QuizWordDrag_h
#define QuizWordDrag_h

#include "QuizBase.h"

#define MAX_LETTER_QUIZWORDDRAG 12




class QuizWordDrag : public QuizBase
{
    
    typedef struct QuizLetter{
        char letter;
        int state;//0: 显示 1:空白 2:放入
        cocos2d::Node *node;
        void setState(int state);
    }stuQuizLetter;

    typedef struct QuizLetterDrag{
        char letter;
        bool m_canDrag;
        Vec2 m_originPos;
        cocos2d::Node *node;
    }stuQuizLetterDrag;
        
    typedef struct QuizTargetOne {
        int nId;
        string text;
    }stuQuizTargetOne;
    
    typedef struct QuizMatchOne {
        int nId;
        int order;
        string text;
    }stuQuizMatchOne;
    
public:
    static QuizWordDrag* create(cocos2d::Node* parent);
    ~QuizWordDrag();
    virtual bool init() override;

   
    void setWord(const string& targetWord, const string& matchWord, const vector<int>& blankList);
    void checkDrag(int n, Vec2 pos);
    void checkOk();
    virtual void setParam(std::string imgFile, std::string voiceFile, std::string xmlFile) override;
    
    int playSentenceAudio();
    virtual int inPage() override;
    
    int getPointNode(Vec2 pt);
protected:
    cocos2d::Node* m_parent;
    cocos2d::Node* m_ui;
    int m_letterNum;
    stuQuizLetter m_letter[MAX_LETTER_QUIZWORDDRAG];
    int m_letterDragNum;
    stuQuizLetterDrag m_letterDrag[MAX_LETTER_QUIZWORDDRAG];
    
    cocos2d::Layer* m_layer;
    
    Vec2 m_touchPosOld, m_touchPosNew;
    bool m_bDraging;
    
    vector<stuQuizTargetOne> m_targets;
    vector<stuQuizMatchOne> m_matches;
    
    int m_blankNum;
    
    string m_audioFile;
    
    int m_minLetter;
};


#endif /* QuizWordDrag_h */
