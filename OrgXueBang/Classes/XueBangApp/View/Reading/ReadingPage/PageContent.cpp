//
//  PageContent.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#include "PageContent.hpp"
#include "TipManager.hpp"
//#include "BookScene.hpp"
#include "BookBody.hpp"
#include "BookDataManager.hpp"
#include "Recorder.h"
#include "TipWaiting.hpp"
#include "FileLog.h"
#include "PageInteract.hpp"

PageContent::PageContent()
{
    DownImg::getInstance()->registCallback(this);
    m_pageSize = UiBase::getInstance()->getRunningScene()->getContentSize();
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PageContent::soundSpeedEvent), MSG_SET_SOUND_SPEED_NOTICE, nullptr);
}

PageContent::~PageContent()
{
    __NotificationCenter::getInstance()->removeObserver(this, MSG_SET_SOUND_SPEED_NOTICE);
    DownImg::getInstance()->unregistCallback(this);
}

void PageContent::onEnter()
{
    PageBase::onEnter();
    scheduleUpdate();
}

void PageContent::onExit()
{
    unscheduleUpdate();
    PageBase::onExit();
}

PageContent* PageContent::create()
{
    PageContent *ret = new (std::nothrow) PageContent();

    auto mUI = CSLoader::createNode("View/Reading/BookPageContent.csb");
    Widget* pageUI = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_pdf");
    pageUI->removeFromParent();
    pageUI->retain();

    if (ret && ret->init(pageUI))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

PageContent* PageContent::create(Widget* pageUI)
{
    PageContent *ret = new (std::nothrow) PageContent();
    if (ret && ret->init(pageUI))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool PageContent::init(Widget* pageUI)
{
    if (!PageBase::init())
    {
        return false;
    }

    this->load(pageUI);
    return true;
}

void PageContent::addEvents()
{
    Widget* pageWidget = getPageWidget();

    img_content = (ImageView*)pageWidget->getChildByName("img_content");
    img_content->setVisible(false);
}

void PageContent::touchEvent(Ref *sender, Widget::TouchEventType type)
{
    CLICK_FILTER(sender, type);
    ButtonType btnType = (ButtonType)(dynamic_cast<Widget*>(sender))->getActionTag();
    CFileLog::LogSaveFormat("===PageContent::touchEvent==%d", (int)btnType);
    switch (btnType)
    {
        case eBtnBack:
        {

        }
            break;

        case eBtnPlay:
        {
            this->playAudio();
        }
            break;
        case eBtnRecord:
        {

        }
            break;
        case eBtnReplay:
        {

        }
            break;

        default:
            break;
    }
}

void PageContent::update(float dt)
{
    PageBase::update(dt);
}

void PageContent::loadPage(WholeBook &wholeBook, int index)
{
    PageBase::loadPage(wholeBook, index);
    wholeBook.downAll(index);
}

void PageContent::freePage()
{
    string strPath = m_wholeBook->getPicPath(m_pageIndex);

    if (strPath.length() > 0)
    {
        //释放贴图
        Director::getInstance()->getTextureCache()->removeTextureForKey(strPath);
    }
    PageBase::freePage();
}

void PageContent::endDownload(string& url, string& file, int iTag, eBookResType resType)
{
#if DOWNLOAD_LOG
    SILog("下载结束", iTag)
    SSLog("下载文件", file.c_str())
    SILog("文件类型", resType)
#endif
    if (m_pageIndex == iTag) {
        if (resType == eBookResPng) {
            //有png资源的，需要绑定并加载图片
            //取得资源图片，获取原始大小和比例，用于计算目标大小
            Sprite* spTmp = Sprite::create(file);
            if (spTmp != nullptr) {
                Size size_sp = spTmp->getContentSize();

                Size size_body = BookDataManager::getInstance()->getBodySize();

                float scaleX = size_sp.width / m_pageSize.width;
                float scaleY = size_sp.height / m_pageSize.height;
                //计算结束，spTmp可以丢弃了
                Size size_final = size_sp / MAX(scaleX, scaleY);
                setPageSize(size_final);
                if(size_body.width == 0 && size_body.height == 0)
                {
                    BookDataManager::getInstance()->setBodySize(size_final);
                    //size_body = size_final;
                }

                //这块待优化
                m_pageWidget->setContentSize(size_final);
                m_pageWidget->setPosition(Vec2::ZERO);
                img_content->setContentSize(size_final);
                Helper::doLayout(m_pageWidget);
                img_content->loadTexture(file);//实质资源加载
                img_content->setVisible(true);
            }else{
                FileUtils::getInstance()->removeFile(file);
                m_wholeBook->m_bookPages->at(m_pageIndex)->m_downPicFailed = true;
            }
        }
        else if (resType == eBookResSnd)
        {
            Node* nodeBase = UiBase::getInstance()->getRunningScene();
            BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);
            if (bookBody && bookBody->getCurPage() == m_pageIndex) {//防止snd下载太慢，回调错位问题
                string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
                string strPathXml = m_wholeBook->getXmlPath(m_pageIndex);
                if(strPathSnd != "" && FileUtils::getInstance()->isFileExist(strPathSnd)
                   && strPathXml != "" && FileUtils::getInstance()->isFileExist(strPathXml))
                {
                    __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_INTERACT, (Ref*)(intptr_t)m_pageIndex);
                }
            }
        }
        else if (resType == eBookResOth)
        {
            Node* nodeBase = UiBase::getInstance()->getRunningScene();
            BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);
            if (bookBody && bookBody->getCurPage() == m_pageIndex) {//防止snd下载太慢，回调错位问题
                string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);
                string strPathXml = m_wholeBook->getXmlPath(m_pageIndex);
                if(strPathSnd != "" && FileUtils::getInstance()->isFileExist(strPathSnd)
                   && strPathXml != "" && FileUtils::getInstance()->isFileExist(strPathXml))
                {
                    __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_INTERACT, (Ref*)(intptr_t)m_pageIndex);
                }
            }
        }

        //判断三个文件的下载状态
        TipLayer *tip;
        if (//(!bookBody||bookBody->getCurPage() == m_pageIndex)&&
            m_wholeBook->gotFullRes(m_pageIndex)
            && TipManager::getInstance()->isOpenTip(eTipWaiting, tip)) {
            tip->closeTip();
        }
    }
}

void PageContent::endDownloadFail(string& url, string& file, int iTag, eBookResType resType)
{
    if (m_pageIndex == iTag) {
        if (resType == eBookResPng)
        {
            m_wholeBook->m_bookPages->at(m_pageIndex)->m_downPicFailed = true;
        }
        else if (resType == eBookResSnd)
        {
            m_wholeBook->m_bookPages->at(m_pageIndex)->m_downSndFailed = true;
        }
        else
        {
            m_wholeBook->m_bookPages->at(m_pageIndex)->m_downDatFailed = true;
        }

        TipManager::getInstance()->showTipWithString("文件下载失败，请稍后重试！");

        Node* nodeBase = UiBase::getInstance()->getRunningScene();
        BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);

        ILog(bookBody->getCurPage())
        ILog(m_pageIndex)
        TipLayer *tip;
        if (//(!bookBody||bookBody->getCurPage() == m_pageIndex)&&
            m_wholeBook->gotFullRes(m_pageIndex)
            && TipManager::getInstance()->isOpenTip(eTipWaiting, tip)) {
            tip->closeTip();
        }
    }
}

int PageContent::playAudio(std::function<void (int, const std::string &)> finishCall)
{
    Node* nodeBase = UiBase::getInstance()->getRunningScene();
    BookBody* bookBody = (BookBody*)nodeBase->getChildByTag(eLayerBookBody);

    //交互页的播放状态、录音状态--拦截
    if(bookBody)
    {
        PageInteract* pageInter = bookBody->getInteractPage();
        if((pageInter && pageInter->isAudioPlaying()))
        {
            return AudioEngine::INVALID_AUDIO_ID;
        }
    }

    if(Recorder::getInstance()->isRecording())
    {
        return AudioEngine::INVALID_AUDIO_ID;
    }

    PageBase::playAudio(finishCall);
    m_soundID = AudioEngine::INVALID_AUDIO_ID;

    //音效路径
    string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);

    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        setListenTime(true);//开始录音
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        float speed = BookDataManager::getInstance()->getBookSoundSpeed();
        if(!finishCall)
        {
            m_soundID = SoundManager::getInstance()->playSoundNew(strPathSnd, false,speed, [this](int, const std::string){
                setListenTime(false);
                m_soundID = AudioEngine::INVALID_AUDIO_ID;
                __NotificationCenter::getInstance()->postNotification(MSG_PAGE_AUTOPLAY, (Ref*)false);
            });
            __NotificationCenter::getInstance()->postNotification(MSG_PAGE_AUTOPLAY, (Ref*)true);
        }
        else
        {
            m_soundID = SoundManager::getInstance()->playSoundNew(strPathSnd, false,speed, finishCall);
        }
    }
    else
    {
        TipManager::getInstance()->showTip(eTipTurnPage, TipLayer::eActionType::eNone, 3.0f);
        //没资源，停止自动播放
        if(BookDataManager::getInstance()->getReadMode() == eReadAuto)
        {
            BookDataManager::getInstance()->setReadMode(eReadNone);
            __NotificationCenter::getInstance()->postNotification(MSG_SET_AUTOBTN, (Ref*)false);
        }
        return m_soundID;
    }
    return m_soundID;
}

void PageContent::stopAudio()
{
    //音效路径
    string strPathSnd = m_wholeBook->getSoundPath(m_pageIndex);

    if (m_soundID != AudioEngine::INVALID_AUDIO_ID && FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        SoundManager::getInstance()->stopSound(m_soundID);
    }
    PageBase::stopAudio();
}



void PageContent::onTurnIn()
{
    PageBase::onTurnIn();
    ILog(m_pageIndex)
    OnePage* onePage = m_wholeBook->m_bookPages->at(m_pageIndex);
    onePage->m_downPicFailed = false;
    onePage->m_downSndFailed = false;
    onePage->m_downDatFailed = false;
    if (!m_wholeBook->gotFullRes(m_pageIndex)
        ) {
        //打开Loading
        TipManager::getInstance()->openTip(eTipWaiting);
        m_wholeBook->downAll(m_pageIndex);
        //__NotificationCenter::getInstance()->postNotification(MSG_BOOK_RELOAD, nullptr);
    }
}

void PageContent::onTurnOut()
{
    TipLayer *tip;
    if(TipManager::getInstance()->isOpenTip(eTipWaiting, tip)) {
        tip->closeTip();
    }
    setListenTime(false);
    PageBase::onTurnOut();
}

void PageContent::showOralResult()
{
    if (m_wholeBook->m_mapRecordResult.find(m_pageIndex) == m_wholeBook->m_mapRecordResult.end())
    {
        return;
    }
}

void PageContent::soundSpeedEvent(Ref* ref)
{
    //为了实时变速  add by hyp
    if(SoundManager::isIOS()&&SoundManager::supportSpeedChange()){
        float speed = ((intptr_t)ref)/100.0f;
        SoundManager::getInstance()->speedSound(m_soundID,speed);
    }
}
