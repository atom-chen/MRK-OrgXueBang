//
//  TipHobbySucc.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/15.
//
//

#ifndef TipHobbySucc_hpp
#define TipHobbySucc_hpp

#include "TipLayer.hpp"
#include "stdafx.h"

class TipHobbySucc : public TipLayer, public BaseKeyListener
{
    enum ButtonType
    {
        eBtnClose = 0,
        eBtnSure,
        eBtnCancle,
    };
    
public:
    TipHobbySucc();
    virtual ~TipHobbySucc();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void addEvents() override;
    
    CREATE_FUNC(TipHobbySucc);
    
    virtual void refreshUIView() override;
    virtual bool OnBaseKeyBack() override;
};


#endif /* TipHobbySucc_hpp */
