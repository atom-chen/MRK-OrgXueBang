//
//  QuizTextSort.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/22.
//
//

#ifndef QuizTextSort_h
#define QuizTextSort_h

#include "QuizBase.h"


class QuizTextSort : public QuizBase
{
public:
    static QuizTextSort* create(cocos2d::Node* parent);
    virtual bool init() override;
    
    void setQuestion();
    void onSelect(int n);
    
    virtual void setParam(std::string imgFile, std::string voiceFile, std::string xmlFile) override;
protected:
    cocos2d::Node* m_parent;
    cocos2d::Node* m_ui;
    
    std::string m_question;
    std::vector<stuQuizAnswerText> m_answer;
};


#endif /* QuizTextSort_h */
