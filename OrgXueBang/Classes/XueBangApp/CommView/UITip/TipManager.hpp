//
//  TipManager.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/7.
//
//

#ifndef TipManager_hpp
#define TipManager_hpp

#include "stdafx.h"
#include "TipType.hpp"
#include "TipLayer.hpp"

typedef list<TipLayer*> TipList;

class TipManager : public Ref
{
public:
    ~TipManager();
    
    static TipManager* getInstance();
    
    TipLayer* openTip(eTipType type, eZorder order = eZorderTipLayer);
    TipLayer* showTip(eTipType type, TipLayer::eActionType action = TipLayer::eActionType::eNone, float interval = 1.0f);
    
    bool removeTip(TipLayer* tip);
    
    void closeCurTip();
    
    void onExitScene();
    
    bool isOpenTip(eTipType type, TipLayer* &tip);
    
    TipLayer* getCurTip();
    
    eTipType getCurTipType();
    
    TipLayer* getTipWithType(eTipType type);
    
    bool isEmpty();
    
    void removeTips();
    
    void showTipWithString(const char* str, float dt = 2.0f, bool visible = true);
    
    int tipListNum();
    
    bool isNoAnyTip();
    
    void showMsgTips(std::string msg, int fontSize = 38, Vec2 showPos = Vec2::ZERO);
    
    void showLoading(Node* parent, Vec2 pos = Vec2::ZERO);
    void removeLoading(Node* parent);
private:
    TipManager();
    
    TipLayer* createTipWithType(eTipType type);
    
    void onRemoveResultPop();
    
private:
    TipList* m_tipList;
    
    std::string m_tipString;//提示信息
    Widget*     m_tipView;//
    Node *      m_tipParent;
};


#endif /* TipManager_hpp */
