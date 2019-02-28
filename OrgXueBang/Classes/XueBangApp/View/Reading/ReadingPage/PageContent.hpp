//
//  PageContent.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#ifndef PageContent_hpp
#define PageContent_hpp

#include "PageBase.hpp"
#include "BookStruct.hpp"
#include "stdafx.h"


class PageContent : public PageBase, public CDownloadDelegate
{
    enum ButtonType
    {
        eBtnBack = 0,   //顶部-返回
        
        eBtnPlay,  //底部-播放
        eBtnRecord,
        eBtnReplay,
        
        eBntCount,
    };
    
public:
    PageContent();
    virtual ~PageContent();
    
    static PageContent* create();
    static PageContent* create(Widget* pageUI);
    
    virtual bool init(Widget* pageUI);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void addEvents() override;
    virtual void loadPage(WholeBook &wholeBook, int index) override;
    virtual void freePage() override;
    
    void touchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void update(float dt) override;
    
    virtual void endDownload(string& url, string& file, int iTag = -1, eBookResType resType = eBookResNaN) override;
    virtual void endDownloadFail(string& url, string& file, int iTag = -1, eBookResType resType = eBookResNaN) override;
    int  playAudio(std::function<void (int, const std::string &)> finishCall = nullptr) override;
    void stopAudio() override;
    
    void onTurnIn() override;
    void onTurnOut() override;
    
    virtual void showOralResult() override;
    
    void soundSpeedEvent(Ref* ref);
private:
    ImageView* img_content;
    
    Text* txt_content;
};


#endif /* PageContent_hpp */
