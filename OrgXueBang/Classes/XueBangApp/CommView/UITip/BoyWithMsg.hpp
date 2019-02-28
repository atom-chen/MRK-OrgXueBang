//
//  BoyWithMsg.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/15.
//
//

#ifndef BoyWithMsg_hpp
#define BoyWithMsg_hpp

#include "TipLayer.hpp"
#include "BaseKeyEvent.h"

enum eTipMsgType
{
    eTipMsgCommon,
    eTipMsgRecognize,   //打分结果
    eTipMsgRecorder,    //录音机
};

class BoyWithMsg : public TipLayer, public BaseKeyListener
{
    enum ButtonType
    {
        eBtnClose = 0,
        eBtnSure,
        eBtnCancle,
    };
    
public:
    BoyWithMsg();
    virtual ~BoyWithMsg();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void addEvents() override;
    
    CREATE_FUNC(BoyWithMsg);
    
    virtual void refreshUIView() override;
    
    void touchEvent(Ref* sender, Widget::TouchEventType type);
    
    void setMsgInfo(eTipMsgType msgType, string msgStr = "");
    virtual bool OnBaseKeyBack() override;
private:
    ImageView* img_boy;
    Text* txt_tip;
};


#endif /* BoyWithMsg_hpp */
