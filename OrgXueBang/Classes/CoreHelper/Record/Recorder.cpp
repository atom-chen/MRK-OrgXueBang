#include "stdafx.h"
#include "Recorder.h"
#include "TipManager.hpp"
#include "BoyWithMsg.hpp"

Recorder * Recorder::m_pRecordInstance = nullptr;
Recorder::Recorder()
{
    m_startRecordCallBack = nullptr;
    m_recordingCallBack   = nullptr;
    m_endRecordCallBack   = nullptr;
    m_endPlayCallBack = nullptr;
    m_dRecordStartTime    = 0.0f;
}

Recorder::~Recorder()
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->unschedule(schedule_selector(Recorder::_updateRecord), this);
}


void Recorder::addBeginRecoderCall(startRecord callBack)
{
    m_startRecordCallBack = callBack;
}


void Recorder::addRecordingCall(recording callBack)
{
    m_recordingCallBack = callBack;
}



void Recorder::addEndRecordCall(endRecord callBack)
{
    m_endRecordCallBack = callBack;
}

void Recorder::addEndPlayCall(playOver callBack)
{
    m_endPlayCallBack = callBack;
}

void Recorder::addCancelRecordCall(cancelRecord callBack)
{
    m_cancelRecordCallBack = callBack;
}

void Recorder::clearRecorder()
{
    Recorder::getInstance()->cancelRecoder();
    Recorder::getInstance()->stopPlayRecord();
    
    m_startRecordCallBack = nullptr;
    m_recordingCallBack = nullptr;
    m_endRecordCallBack = nullptr;
    m_endPlayCallBack = nullptr;
}


//-------------录音ui相关-------------

const static float MIN_RECORD_SECOND    = 2.0f;
const static float MAX_RECORD_SECOND    = 15.0f;
const static int   TAG_RECORD           = 6688;
const static std::string SCHEDULER_NAME = "STOPRECORDER";

void Recorder::_startRecordUI()
{
    m_dRecordStartTime = cocos2d::utils::gettime();
    log("_startRecordUI %f", m_dRecordStartTime);
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    if (!scheduler->isScheduled(schedule_selector(Recorder::_updateRecord), this))
    {
        scheduler->schedule(schedule_selector(Recorder::_updateRecord), this, 0.0f, false);
    }
}


void Recorder::_endRecordUI()
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    auto diff = cocos2d::utils::gettime() - m_dRecordStartTime;
    log("_endRecordUI %f  diff = %f", m_dRecordStartTime, diff);
    
    if (diff < MIN_RECORD_SECOND)//小于最小时间
    {
        auto callback = [this](float dt){
            this->stopRecoder();//再走一次stop false, 时间判断加取消ui
        };
        if (!scheduler->isScheduled(SCHEDULER_NAME, this))
        {
            scheduler->schedule(callback, this, 0, 0, MIN_RECORD_SECOND-diff, false, SCHEDULER_NAME);
            _enableRecordTouch(false);//禁止点击
        }
    }
    else
    {
        this->stopRecoder(true);//真正结束
        
        this->_cancelRecordUI();
    }
}


void Recorder::_cancelRecordUI()
{
    m_dRecordStartTime = 0.0f;
    
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->unschedule(schedule_selector(Recorder::_updateRecord), this);
    scheduler->unschedule(SCHEDULER_NAME, this);
    
    //延时一会才可以点击, 让回调先回去
    scheduler->schedule([this](float dt){
        _enableRecordTouch(true);
    }, this, 0, 0, 1.0f, false, cocos2d::StringUtils::format("%ld",time(nullptr)));
}




void Recorder::_enableRecordTouch(bool bEnable)
{
    Node* nodeBase = UiBase::getInstance()->getRunningScene();
    auto dispatch = cocos2d::Director::getInstance()->getEventDispatcher();
    if (bEnable)
    {
        if (nodeBase->getChildByTag(TAG_RECORD))
        {
            nodeBase->removeChildByTag(TAG_RECORD);
        }
    }
    else
    {
        if (!nodeBase->getChildByTag(TAG_RECORD))
        {
            auto layer = cocos2d::Layer::create();
            nodeBase->addChild(layer, std::numeric_limits<int>::max(), TAG_RECORD);
            
            auto listener = cocos2d::EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [layer](cocos2d::Touch* touch, cocos2d::Event* event){
                //tips
                //layer->addChild(CLayerMsg::createWithXmlKey("tip_string02"));
//屏蔽录音操作提示
//                TipLayer* boyMsg = nullptr;
//                if (!TipManager::getInstance()->isOpenTip(eTipBoyWithMsg, boyMsg)) {
//                    boyMsg = TipManager::getInstance()->showTip(eTipBoyWithMsg, TipLayer::eActionType::eCenterScale);
//                    BoyWithMsg* tip = (BoyWithMsg*)boyMsg;
//                    tip->setMsgInfo(eTipMsgRecorder, "慢一点，我都反应不过来啦");
//                }
                return true;
            };
            dispatch->addEventListenerWithSceneGraphPriority(listener, layer);
        }
    }
}




void Recorder::_updateRecord(float dt)
{
    auto diff = cocos2d::utils::gettime() - m_dRecordStartTime;
    if (m_recordingCallBack)
    {
//        m_recordingCallBack(diff/MAX_RECORD_SECOND);
        m_recordingCallBack(diff);
    }
    cocos2d::log("record %f", diff);
    if (diff >= MAX_RECORD_SECOND)
    {
        //this->stopRecoder();
    }
}
