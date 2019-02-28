//
//  PageQuizCover.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#ifndef PageQuizCover_hpp
#define PageQuizCover_hpp

#include "PageBase.hpp"
#include "BookStruct.hpp"
#include "stdafx.h"


class PageQuizCover : public PageBase, public CDownloadDelegate
{
    enum ButtonType
    {
        eBtnBack = 0,   //顶部-返回
        eBtnLeft,
        eBtnRight,
        eImgFrog,
        eSklMsg,
        
        eBntCount,
    };
    
public:
    PageQuizCover();
    virtual ~PageQuizCover();
    
    static PageQuizCover* create();
    static PageQuizCover* create(Widget* pageUI);
    
    virtual bool init(Widget* pageUI);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void addEvents() override;
    virtual void loadPage(WholeBook &wholeBook, int index) override;
    virtual void freePage() override;
    
    void touchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void update(float dt) override;
    
    virtual void endDownload(string& url, string& file, int iTag = -1, eBookResType resType = eBookResNaN) override;
    
    void onTurnIn() override;
    void onTurnOut() override;
    virtual void btnBack() override;
private:
    
    Text* txt_content;
    bool m_bCanNotify;
};


#endif /* PageQuizCover_hpp */
