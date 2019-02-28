//
//  TipWaiting.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/14.
//
//

#ifndef TipWaiting_hpp
#define TipWaiting_hpp

#include "TipLayer.hpp"

class TipWaiting : public TipLayer
{
public:
    TipWaiting();
    virtual ~TipWaiting();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void addEvents() override;
    
    CREATE_FUNC(TipWaiting);
    
    virtual void refreshUIView() override;
};


#endif /* TipWaiting_hpp */
