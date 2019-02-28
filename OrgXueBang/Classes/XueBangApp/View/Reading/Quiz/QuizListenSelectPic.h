//
//  QuizListenSelectPic.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/26.
//
//

#ifndef QuizListenSelectPic_h
#define QuizListenSelectPic_h

#include "QuizBase.h"


class QuizListenSelectPic : public QuizBase
{
    typedef struct QuizAnswerImg {
        int score;
        string img;
        cocos2d::Node* node;
    }stuQuizAnswerImg;
public:
    static QuizListenSelectPic* create(cocos2d::Node* parent);
    virtual bool init() override;
    
    void setQuestion();
    void onSelect(int n);
    
    virtual void setParam(std::string imgFile, std::string voiceFile, std::string xmlFile) override;
    int playSentenceAudio();
    virtual int inPage() override;
protected:
    cocos2d::Node* m_parent;
    cocos2d::Node* m_ui;
    
    std::vector<stuQuizAnswerImg> m_answer;
    
    bool m_initOk;
    
    Node* m_items[5];
};


#endif /* QuizListenSelectPic_h */
