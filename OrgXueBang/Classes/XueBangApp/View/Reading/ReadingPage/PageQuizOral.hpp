//
//  PageQuizOral.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#ifndef PageQuizOral_hpp
#define PageQuizOral_hpp

#include "PageQuizBase.hpp"
#include "BookStruct.hpp"
#include "stdafx.h"
#include "MyText.hpp"

class PageQuizOral : public PageQuizBase, public CDownloadDelegate
{
    enum ButtonType
    {
        eBtnBack = 0,   //顶部-返回
        eTxtQuiz,
        
        eBtnQuizRecord, //交互-录音
        eBtnQuizPlay,   //交互-播放
        eBtnQuizListen, //交互-回放
        
        eBtnBroadcast,  //底部-播放
        
        eBtnLeft,
        eBtnRight,
        eSklScore,      //打分动画
        eBtnDummy,      //音效
        eBntCount,
    };
    
public:
    PageQuizOral();
    virtual ~PageQuizOral();
    
    static PageQuizOral* create();
    static PageQuizOral* create(Widget* pageUI);
    
    virtual bool init(Widget* pageUI);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void addEvents() override;
    virtual void loadPage(WholeBook &wholeBook, int index) override;
    virtual void freePage() override;
    
    void touchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void update(float dt) override;
    
    virtual void endDownload(string& url, string& file, int iTag = -1, eBookResType resType = eBookResNaN) override;
    
    int  playAudio(std::function<void (int, const std::string &)> finishCall = nullptr) override;
    void stopAudio() override;
    void playRecord();
    void startRecord();
    
    virtual void showOralResult() override;
    
    void onTurnIn() override;
    void onTurnOut() override;
    
    void startWheelAnim(int iTag);
    void stopWheelAnim(int iTag);
    void setBtnState(bool bStart, int iType);
    void setBtnState(int iType, bool bEnabled);
    
    virtual void stopRecordAndPlayback() override;//add by dj
private:
    float _fMin,_fMax,_fTime;
    
    
    Text* txt_content;
    Text* txt_tip;
    MyText* myText;
    
    Widget* panel_menu;
    
    ImageView* img_gear_fg0;
    ImageView* img_gear_fg1;
    ImageView* img_gear_fg2;
    
    Button* btn_play;
    Button* btn_record;
    Button* btn_replay;
    
    TextBMFont* bmf_score_0;
    TextBMFont* bmf_score_1;
    TextBMFont* bmf_score_2;
    TextBMFont* bmf_anim;
    ImageView* img_quiz_score;
    
    ImageView* img_score_bg_0;
    ImageView* img_score_bg_1;
    ImageView* img_score_bg_2;
    
    SkeletonAnimation* skeleton_play;
    SkeletonAnimation* skeleton_record;
    SkeletonAnimation* skeleton_replay;
    //调整音速//add by hyp
    Label* m_soundLabel;
    Slider* m_soundSlider;
    void soundSliderEvent(Ref *sender, Slider::EventType type);
    void updateSoundSlider(Ref *sender);
};


#endif /* PageQuizOral_hpp */
