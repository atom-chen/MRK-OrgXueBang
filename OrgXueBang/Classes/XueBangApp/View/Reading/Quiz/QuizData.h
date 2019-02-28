//
//  QuizData.h
//  ReadingMate
//
//  Created by 丁晶 on 2017/1/6.
//
//

#include "cocos2d.h"

using namespace std;

#ifndef QuizData_h
#define QuizData_h


typedef struct QuizAnswerText {
    int score;
    string text;
    cocos2d::Node* node;
}stuQuizAnswerText;
#endif /* QuizData_h */
