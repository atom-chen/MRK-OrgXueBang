//
//  PageQuizTemp.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#ifndef PageQuizTemp_hpp
#define PageQuizTemp_hpp

#include "PageQuizBase.hpp"
#include "BookStruct.hpp"
#include "stdafx.h"
#include "QuizBase.h"

class PageQuizTemp : public PageQuizBase, public CDownloadDelegate
{
public:
    PageQuizTemp();
    virtual ~PageQuizTemp();
    
    static PageQuizTemp* create(eQuizType quizType);
    static PageQuizTemp* create(Widget* pageUI, eQuizType quizType);
    
    virtual bool init(Widget* pageUI) override;
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void addEvents() override;
    virtual void loadPage(WholeBook &wholeBook, int index) override;
    virtual void freePage() override;
    
    virtual void update(float dt) override;
    
    virtual void endDownload(string& url, string& file, int iTag = -1, eBookResType resType = eBookResNaN) override;
    
    virtual int  playAudio(std::function<void (int, const std::string &)> finishCall = nullptr) override;
    void stopAudio() override;
    
    void onTurnIn() override;
    void onTurnOut() override;
    
    bool showQuizOkEffect();
private:
    
    QuizBase* m_quizNode;
    Node* m_pageNode;
};


#endif /* PageQuizTemp_hpp */
