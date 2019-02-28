//
//  TipLoader.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/14.
//
//

#ifndef TipLoader_hpp
#define TipLoader_hpp

#include "TipLayer.hpp"

class TipLoader : public BaseLayer
{
//    enum ButtonType
//    {
//        eBtnClose = 0,
//    };
public:
    TipLoader();
    virtual ~TipLoader();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void addEvents() override;
    
//    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)override{return true;} ;
//    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)override{} ;
//    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)override{} ;
    
    CREATE_FUNC(TipLoader);
    
    virtual void refreshUIView() override;
    
    static void showLoader(float minShowTime = 0.5);
    static void HideLoader();
    static void showFrog();
    static bool isShow();
    void createFrog();
    void createFrogLater(float delay);
private:
    SkeletonAnimation* m_frog;
    cocos2d::EventListenerTouchOneByOne* _touchListener;
    bool m_bDel;
};


#endif /* TipLoader_hpp */
