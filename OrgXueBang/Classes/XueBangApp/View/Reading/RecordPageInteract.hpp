//
//  RecordPageInteract.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#ifndef RecordPageInteract_hpp
#define RecordPageInteract_hpp

#include "PageBase.hpp"
#include "BookStruct.hpp"
#include "stdafx.h"
#include "MyText.hpp"


class RecordPageInteract : public PageBase
{
    enum ButtonType
    {
        eBtnBack = 0,   //顶部-返回
        
        eBtnInterRecord,//交互-录音
        eBtnInterPlay,  //交互-播放
        eBtnInterListen,//交互-回放
        eBtnInterOnlyListen,//交互-只回放
        
        
        eBtnHandle,
        
        eTxtQuiz,       //Quiz-MyText
        eSklScore,      //交互打分动画
        eBtnDummy,
        
        eBntCount,
    };
    
public:
    RecordPageInteract();
    virtual ~RecordPageInteract();
    
    static RecordPageInteract* create(Widget* pageUI);
    bool load(const char* gameUIFile);
    virtual bool init(Widget* pageUI);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void addEvents() override;
    virtual void loadPage(WholeBook &wholeBook, int index) override;
    virtual void freePage() override;
    
    void touchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void update(float dt) override;
    
    int getViewHight(){ return m_viewHeight; }
    virtual void showOralResult() override;
    
    int  playAudio(std::function<void (int, const std::string &)> finishCall = nullptr) override;
    void stopAudio() override;
    
//    void recordLimit(float dt);
    
    void setInterState(eInterState state)
    {
        ImageView* img_inter_yun = static_cast<ImageView*>(Helper::seekWidgetByName(this->getPageWidget(), "img_inter_yun"));
        img_inter_yun->setPositionY(0);
        m_interState = state;
    }
    eInterState getInterState()
    {
        return m_interState;
    }
    
    bool bInterDoAnim()
    {
        return img_inter_yun->getActionByTag(123) != nullptr;
    }
    void stopInterAnim();
    void onInterAnimEnded(cocos2d::Node *sender, eInterState state);
    void doInterAnimation();

    void startWheelAnim(int iTag);
    void stopWheelAnim(int iTag, bool bManue = false);
    
    void setBtnState(bool bStart, int iType);
    void setBtnState(int iType, bool bEnabled);
    
    virtual void onRecordTimeOut() override;
    
    void setPageAutoPlay(Ref* isPlaying);
private:
    float _fMin,_fMax,_fTime;
    
    Text* txt_content;
    
    Widget* m_rootWidget;
    Widget* panel_menu;         //交互菜单
    //Button* btn_record;     //交互-录音
    
    MyText* myText;
    
    int m_viewHeight;
    eInterState m_interState; //交互界面状态
    float fRecordLimit;     //自动结束录音时限
    
    ImageView* img_gear_fg0;
    ImageView* img_gear_fg1;
    ImageView* img_gear_fg2;
    ImageView* img_gear_fg3;
    
    Button* btn_play;
    Button* btn_record;
    Button* btn_replay;
    Button* btn_playRecord;
    ImageView* img_inter_yun;
    
    TextBMFont* bmf_score;
    TextBMFont* bmf_anim;
    ImageView* img_inter_scorebg;
    ImageView*  img_score_bg;
    
    SkeletonAnimation* skeleton_play;
    SkeletonAnimation* skeleton_record;
    SkeletonAnimation* skeleton_replay;
    SkeletonAnimation* skeleton_record_replay;
    
    //add by hyp
    //调整音速//add by hyp
    Label* m_soundLabel;
    Slider* m_soundSlider;
    void soundSliderEvent(Ref *sender, Slider::EventType type);
public:
    static void addSoundSlider(Widget* pageWidget,Label* soundLabel,Slider* soundSlider);
    static void addSoundSliderEvent(Ref *sender, Slider::EventType type,Label* label,int soundId);
    static void updateSoundSlider(Ref *sender,Label* soundLabel,Slider* soundSlider);
    static float conversionSlider(float value,int type);
public:
    void updateUIByMode();
    void interListen(bool onlyListen);
};


#endif /* RecordPageInteract_hpp */
