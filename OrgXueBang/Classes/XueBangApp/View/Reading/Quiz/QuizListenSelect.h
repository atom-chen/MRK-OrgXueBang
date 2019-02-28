//
//  QuizListenSelect.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/26.
//
//

#ifndef QuizListenSelect_h
#define QuizListenSelect_h

#include "QuizBase.h"


class QuizListenSelect : public QuizBase
{
public:
    static QuizListenSelect* create(cocos2d::Node* parent);
    virtual bool init() override;
    
    void setQuestion();
    void onSelect(int n);
    
    virtual void setParam(std::string imgFile, std::string voiceFile, std::string xmlFile) override;
protected:
    cocos2d::Node* m_parent;
    cocos2d::Node* m_ui;
};


#endif /* QuizListenSelect_h */
