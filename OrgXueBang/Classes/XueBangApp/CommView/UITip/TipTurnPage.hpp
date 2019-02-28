//
//  TipTurnPage.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/15.
//
//

#ifndef TipTurnPage_hpp
#define TipTurnPage_hpp

#include "TipLayer.hpp"

class TipTurnPage : public TipLayer
{
    enum ButtonType
    {
        eBtnClose = 0,
        eBtnSure,
        eBtnCancle,
    };
    
public:
    TipTurnPage();
    virtual ~TipTurnPage();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void addEvents() override;
    
    CREATE_FUNC(TipTurnPage);
    
    virtual void refreshUIView() override;
};


#endif /* TipTurnPage_hpp */
