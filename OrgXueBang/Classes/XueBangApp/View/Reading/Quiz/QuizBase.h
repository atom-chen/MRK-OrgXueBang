//
//  QuizBase.h
//  ReadingMate
//
//  Created by 丁晶 on 2017/1/11.
//
//

#ifndef QuizBase_h
#define QuizBase_h

#include "QuizData.h"
#include "BookStruct.hpp"

class QuizBase : public cocos2d::Node
{
public:
    virtual void setParam(std::string imgFile, std::string voiceFile, std::string xmlFile);
    void setWholeBook(WholeBook* wholeBook, int index);
    virtual int inPage();
    virtual void outPage();
    int getScore();
    void setScore(int score);
    void playAnswerAudio(bool bRight);
    
    bool showOkEff();
protected:
    WholeBook* m_wholeBook;
    int     m_pageIndex;
//    int m_score;
    bool m_bXmlRead;
    int m_soundId;
    
    int m_optionCount;//选项数
    int m_errorCount;//错误操作数
    bool m_bShowEff;
};

#endif /* QuizBase_h */
