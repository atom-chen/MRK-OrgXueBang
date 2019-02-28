//
//  BoyWithMsg.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/15.
//
//

#include "BoyWithMsg.hpp"
#include "FileLog.h"

BoyWithMsg::BoyWithMsg()
{
    
}

BoyWithMsg::~BoyWithMsg()
{
    
}

void BoyWithMsg::onEnter()
{
    TipLayer::onEnter();
}

void BoyWithMsg::onExit()
{
    TipLayer::onExit();
}

bool BoyWithMsg::init()
{
    if (TipLayer::init())
    {
        this->load("View/ToolView/layer_boy_msg.csb");
        return true;
    }
    
    return false;
}

void BoyWithMsg::addEvents()
{
    img_boy = static_cast<ImageView*>(Helper::seekWidgetByName(m_rootWidget, "img_boy"));
    txt_tip = static_cast<Text*>(Helper::seekWidgetByName(m_rootWidget, "txt_tip"));
}

void BoyWithMsg::touchEvent(Ref *sender, Widget::TouchEventType type)
{
    CLICK_FILTER(sender, type);
    ButtonType btnType = (ButtonType)(dynamic_cast<Widget*>(sender))->getActionTag();
    CFileLog::LogSaveFormat("===BoyWithMsg::touchEvent==%d", (int)btnType);
    switch (btnType)
    {
        case eBtnSure:
            closeTip();
            break;
            
        default:
            break;
    }
}

void BoyWithMsg::refreshUIView()
{
    
}

void BoyWithMsg::setMsgInfo(eTipMsgType msgType, string msgStr)
{
    string basePath = "Image/SubImg/ToolView/UIMsgTip/";
    if(msgType == eTipMsgRecognize)
    {
        img_boy->loadTexture(basePath + "exp_fill_gas.png", Widget::TextureResType::PLIST);
    }
    else if(msgType == eTipMsgRecorder || msgType == eTipMsgCommon)
    {
        img_boy->loadTexture(basePath + "exp_spk_louder.png", Widget::TextureResType::PLIST);
    }
    img_boy->ignoreContentAdaptWithSize(false);
    txt_tip->setString(msgStr);
}

bool BoyWithMsg::OnBaseKeyBack()
{
    this->closeTip();
    return true;
}
