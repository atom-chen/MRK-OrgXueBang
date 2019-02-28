//
//  QuizTextSelect.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/16.
//
//

#ifndef QuizTextSelect_h
#define QuizTextSelect_h

#include "QuizBase.h"


class QuizTextSelect : public QuizBase
{
public:
    static QuizTextSelect* create(cocos2d::Node* parent, eQuizType quizType);
    virtual bool init() override;

    
    void setQuestion();
    void onSelect(int n);
    
    virtual void setParam(std::string imgFile, std::string voiceFile, std::string xmlFile) override;
    int playSentenceAudio();
    virtual int inPage() override;
    
    void anotherXml(std::string& xmlFile);
protected:
    cocos2d::Node* m_parent;
    cocos2d::Node* m_ui;
    
    std::string m_question;
    std::vector<stuQuizAnswerText> m_answer;
    
    eQuizType m_quizType;
    
    std::string m_audioFile;
};


#endif /* QuizTextSelect_h */
