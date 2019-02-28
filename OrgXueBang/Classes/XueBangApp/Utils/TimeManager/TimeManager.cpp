//
//  TimeManager.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/2/18.
//
//

#include "TimeManager.hpp"
//#include "ComDlg.hpp"
//#include "Game.hpp"
//#include "DiscoveryLayer.hpp"
//#include "VodInterface.hpp"
//#include "PopupPageStdafx.h"
TimeManager::TimeManager()
:m_over30Mins(false)
,m_onlineTime(0.0f)
{
    m_duration = 0.0f;
    m_onlineTime = 0.0f;
    m_hasSyncTimeSuccess = false;
}

TimeManager::~TimeManager()
{
}

TimeManager* TimeManager::getInstance()
{
    static TimeManager s_random;
    return &s_random;
}

void TimeManager::update(float dt)
{
    m_duration += dt;
    m_onlineTime += dt;
    if(!m_over30Mins && m_onlineTime > 1200.0f)//超过二十分钟弹窗提示
    {
//        m_over30Mins = true;
        m_onlineTime = 0;
        //add by hyp for 发现详情不能弹出20分钟提醒
//        PopupInfo popupInfo = PopupInfo("popup04",PopupLevel::Level_1,PopupType::Type_4);
//        popupInfo.setPopupText("你已经在线使用20分钟了哦，让眼睛休息一下吧！");
//        popupInfo.setPopupName("20min");
//        popupInfo.setBtnType(1);
//        popupInfo.setJudge([=](){
//            return isRestTipsEnable();
//        });
//        PopupManager::getInstance()->addRegistered(popupInfo);
    
        
//        //add by zzl for 发现详情不能弹出20分钟提醒
//        if (isRestTipsEnable()) {
//            auto scene = UiBase::getInstance()->getRunningScene();
//            ComDlg* dlg = ComDlg::create(scene);
//            dlg->setStringAndBtnType("你已经在线使用20分钟了哦，让眼睛休息一下吧！", 1);
//            dlg->getUi()->setName("20min");
//        }
    }
    
//    if (m_onlineTime >= 86400) {             //在线一天清除首页书架缓存
//        Reador::getInstance()->m_bReloadHomeBooks = true;
//        m_onlineTime = 0.0f;
//    }
}

//存在以下场景时不弹出20分钟休息提醒
bool TimeManager::isRestTipsEnable()
{
    bool ret = true;
    auto scene = Director::getInstance()->getRunningScene();
    
    if (nullptr != scene->getChildByName("20min")) {
        ret = false;
        return ret;
    }
    if (nullptr != scene->getChildByName("DiscoveryLayer")) {
        ret = false;
        return ret;
    }
    if (nullptr != scene->getChildByName("ActivityClassH5")) {
        ret = false;
        return ret;
    }
    if (nullptr != scene->getChildByName("ActivityClassHomework")) {
        ret = false;
        return ret;
    }
    
    return ret;
}

void TimeManager::startUpdate()
{
    if (Director::getInstance()->getScheduler()->isScheduled(schedule_selector(TimeManager::update),this))
    {
        Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    }
    m_duration = 0.0f;
    m_onlineTime = 0.0f;
    time(&m_currentTime);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(TimeManager::update), this, 0, false);
}

void TimeManager::stopUpdate()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}

//重置在线时长标记
void TimeManager::resetOnlineTime()
{
    m_over30Mins = false;
//    Reador::getInstance()->m_bReloadHomeBooks = true;
    m_onlineTime = 0.0f;
}
