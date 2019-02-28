//
//  TipLayer.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/7.
//
//

#ifndef TipLayer_hpp
#define TipLayer_hpp

#include "stdafx.h"
#include "TipType.hpp"

class TipLayer : public BaseLayer
{
public:
    enum eActionType{
        eNone,
        eCenterScale,
        eTopDown,
        eFadeInout,
    };
    
public:
    TipLayer();
    virtual ~TipLayer();
    
    CREATE_FUNC(TipLayer);
    virtual void addEvents(){};
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void openTip();
    virtual void closeTip();
    
    virtual void inAction(eActionType action);
    virtual void outAction(eActionType action);
    virtual void removeSelfCallFunc();
    
    void setTipType(eTipType type){m_tipType = type;};
    eTipType getTipType(){return m_tipType;};
    
    void setActionType(eActionType action)
    {
        m_actionType = action;
    }
    
    void inActionCenterScale();
    void outActionCenterScale();
    void inActionTopDown();
    void outActionTopDown();
    
    void inActionCallFunc();
    virtual void onKeyBackClick();
    
private:
    eActionType m_actionType;
protected:
    eTipType    m_tipType;
};

#endif /* TipLayer_hpp */
