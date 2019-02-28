//
//  QuizOkEffect.hpp
//  ReadingMate-mobile
//
//  Created by 丁晶 on 2018/3/20.
//

#ifndef QuizOkEffect_hpp
#define QuizOkEffect_hpp

#include "stdafx.h"

class QuizOkEffect : public cocos2d::Node
{
public:
    static QuizOkEffect* create(cocos2d::Node* parentNode, int wordType);
   
    virtual bool init() override;
    
protected:
    cocos2d::Node* m_ui;
    int m_type;// 1:good 2:great 3:perfect
};


#endif /* QuizOkEffect_hpp */
