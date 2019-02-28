//
//  BookBody.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/2/27.
//
//

#ifndef BookBody_hpp
#define BookBody_hpp

#include "BaseLayer.hpp"
#include "BookStruct.hpp"
#include "stdafx.h"
#include "PageInteract.hpp"
#include "BookGuide.hpp"
#include "CDataHttp.hpp"

enum eAccessRight
{
    eAccessNone,    //没交钱禁止读书
    eAccessFree,    //免费书
    eAccessGuest,   //游客限制
    eAccessTrial,   //试用过期
};

class BookBody : public BaseLayer, public CDataHttpDelegate,BaseKeyListener
{
    enum ButtonType
    {
        eBtnBack,
        eBtnProduce,
        eBtnLeft,
        eBtnRight,
        eBtnAutoPlay,       //自动播放
        eBtnCollectAdd, //顶部-收藏-添加
        eBtnCollectSub, //顶部-收藏-移除
    };
public:
    BookBody(int type = 0);
    virtual ~BookBody();
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onEnterTransitionDidFinish() override;
//    CREATE_FUNC(BookBody);
    static BookBody* create(int type = 0);
    virtual void addEvents() override;
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void touchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void update(float dt) override;
    virtual void endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json) override;
    
    virtual void refreshUIView() override;
    void initInterPanel(Ref* index);
    void resetWholeBook();
    void autoTurnPage(Ref* iPage);
    //外放给导读页的接口
    void playPageTurnAction(int iTarget);
    NodeGrid* createPage(int iIndex);
    void turnPageOver(int iIndex);
    int getCurPage()
    {
        return iCurPage;
    }
    static void turnNextPage();
    static void turnPreviousPage();
    
    void setInterVisible(Ref* isVisible);
    void setAutoBtnState(Ref* isEnable);
    void onRecordTimeOut(Ref* iPage);
    virtual bool OnBaseKeyBack()override;
    
    //btnIndex:1阅读，2朗读，3答题，4录音秀，5next
    bool doBtnHandler(int btnIndex);
private:
    WholeBook m_wholeBook;  //整本书的数据
    
    int iAllPage;
    int iCurPage;
    int iLstPage;
    int iOralPage;  //口语题起始页
    int iQuizPage;  //答题起始页
    int iRecognizePageIdx;  //请求打分结果的页数
    
    Vec2 m_fMove;
    bool m_bCanTurnPage;
    
    cocos2d::EventListenerTouchOneByOne* _touchListener;
    
    Vec2 minContainerOffset();
    Vec2 maxContainerOffset();
    void setZoomScale(float s);
private:
    PageInteract* pageInter;
    Button* btn_record;
    
    Widget* panel_pdf;
    Widget* panel_quiz_oral;
    Widget* panel_quiz_start;
    Widget* panel_quiz_temp;
    Widget* panel_interact;     //交互面板
    Widget* panel_content_bg;
    
    Widget* panel_center;
    Widget* panel_body;
    BookGuide* BookGuideLayer;
private:
    Vec2 _touchPoint;
    float _touchLength;
    float _scaleStart;
    std::vector<Touch*> _touches;
    Size _viewSize;
    Rect _viewRect;
    float _minScale, _maxScale, _minSlide;
    
    //严格模式，true-出屏拖动；false-不满可动
    bool _bSeriour;
public:
    bool getCanTurn()
    {
        return m_bCanTurnPage;
    }
    void initPage(int iPage);
    void recyclePage();
    int iLastPage;  //动态回收的辅助标记
    //外放给正文页，用于检测相关状态
    PageInteract* getInteractPage()
    {
        return pageInter;
    }
    void reloadOnePage(Ref* iPage);
private:
    //为了防止切页造成按钮状态异常，绕个大圈，设置标记，专用于丢弃残缺的录音或回放状态
    //在录音过程中翻页导致录音中断，保存错位，为此设置标记，丢弃一个不完整标记的录音记录
    bool bRecordComplete;
    //在回放过程中翻页导致回放结束，保存错位，为此设置标记，丢弃一个不完整标记的回放回调
    bool bReplayComplete;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //iOS那边接好了cancelCallBack,但是Android没有cancel机制，不好扩展
    //在录音过程中关闭交互面板导致录音中断，再按返回键导致回调时UI空指针，为此设置标记，屏蔽一个取消的录音记录
    bool bRecordCanceled;
#endif
public:
    bool getRecordComplete()
    {
        return bRecordComplete;
    }
    void setRecordComplete(bool bComplete)
    {
        bRecordComplete = bComplete;
    }
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    void setRecordCanceled(bool bCancel)
    {
        bRecordCanceled = bCancel;
    }
#endif
    
    bool getReplayComplete()
    {
        return bReplayComplete;
    }
    void setReplayComplete(bool bReplay)
    {
        bReplayComplete = bReplay;
    }
private:
    Button* btn_left;
    Button* btn_right;
    Button* btn_auto;
    Button* btn_back;
    
    Button* btn_collect;
    bool bInterReady;
    bool bWaitRecognize;
private:
    bool bEnroll;   //用户读书权限：0-无；1-有。
    bool bGuideOver;//导读页结束标记
    bool bForceExit;//菜单退出标记
private:
    eAccessRight canRead();
    bool checkReadTip();
    void doActionOpen();
    void doActionClose();
    void doSimplePlay();
    
private:
    std::unordered_map<int, int>     m_mapNative;   //语音Quiz3/3的最高分记录
    std::unordered_map<int, int>     m_mapAcc;      //语音Quiz3/3的最高分记录
    std::unordered_map<int, int>     m_mapFluency;  //语音Quiz3/3的最高分记录
    std::unordered_map<int, int>     m_mapScore;    //语音Quiz3/3的最高分记录
    
private:
    void dealScoreOutLog(); //统计成绩，并发出关书日志
    bool bDoubleClicked;
    void switchBtnCollect(bool bPositive);
    bool boolCanTouch();//add by hyp
public:
    bool getWaitRecognize()
    {
        return bWaitRecognize;
    }
    
    Node* m_recordLoadingNode;
    bool m_bRecordReady;
    void showRecordLoading(bool bShow);
    
    bool showQuizOkEffect();
    void dealRecordScore(string& return_msg);
    void handleScoreData(const stuJson& jsonData,int iCurPage);
    void handleData(const stuJson& jsonData);
    
    void setVisibleTopBar(bool isShow = true);
    void doBackHandle(bool isAllBack = false);
private:
    bool isDuBa;
    
    void onRevMsgDoHandle(Ref* ref = nullptr);
};


#endif /* BookBody_hpp */
