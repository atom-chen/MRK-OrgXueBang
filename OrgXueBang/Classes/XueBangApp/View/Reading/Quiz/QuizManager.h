//
//  QuizManager.h
//  ReadingMate
//
//  Created by 丁晶 on 2017/1/5.
//
//

#ifndef QuizManager_h
#define QuizManager_h

//#include "cocos2d.h"
#include "BookStruct.hpp"
#include "PageQuizBase.hpp"

class QuizManager
{
public:
    static QuizManager* getInstance();
    eQuizType getQuizType(OnePage* page);
    
//    PageQuizBase* createQuiz(eQuizType type, Widget* pageUI);
};

#endif /* QuizManager_h */
