//
//  PageQuizBase.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#ifndef PageQuizBase_hpp
#define PageQuizBase_hpp

#include "PageBase.hpp"
#include "BookStruct.hpp"
#include "stdafx.h"


class PageQuizBase : public PageBase
{
    enum ButtonType
    {
        eBtnBack = 0,   //顶部-返回
        eTxtQuiz,
        
        eBntCount,
    };
    
public:
    PageQuizBase();
    virtual ~PageQuizBase();
    
    static PageQuizBase* create(Widget* pageUI);
    
    virtual bool init(Widget* pageUI);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void addEvents() override;
    virtual void loadPage(WholeBook &wholeBook, int index) override;
    virtual void freePage() override;
    
    virtual void update(float dt) override;
    
    virtual int  playAudio(std::function<void (int, const std::string &)> finishCall = nullptr) override;
    virtual void setAutoPlay() override;
    virtual void stopAudio() override;
    void setQuizType(eQuizType type)
    {
        m_quizType = type;
    }
    eQuizType getQuizType()
    {
        return m_quizType;
    }
private:
    
    eQuizType m_quizType;
};


#endif /* PageQuizBase_hpp */
