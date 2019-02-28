//
//  PageQuizTemp.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/10.
//
//

#include "PageQuizTemp.hpp"
#include "TipManager.hpp"

#include "QuizWordDrag.h"
#include "QuizTextSelect.h"
//#include "QuizTextSort.h"
#include "QuizPicWordDrag.h"
//#include "QuizListenSelect.h"
#include "QuizListenSelectPic.h"
#include "QuizSentence.h"

//#include "BookBody.hpp"

PageQuizTemp::PageQuizTemp()
{
    m_pageSize = Director::getInstance()->getVisibleSize();
}

PageQuizTemp::~PageQuizTemp()
{
}

void PageQuizTemp::onEnter()
{
    PageQuizBase::onEnter();
}

void PageQuizTemp::onExit()
{
    PageQuizBase::onExit();
}

PageQuizTemp* PageQuizTemp::create(eQuizType quizType)
{
    PageQuizTemp *ret = new (std::nothrow) PageQuizTemp();
    ret->setQuizType(quizType);
    
    auto mUI = CSLoader::createNode("View/Reading/BookPage.csb");
    Widget* pageUI = Helper::seekWidgetByName(static_cast<Widget*>(mUI), "panel_quiz_temp");
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

PageQuizTemp* PageQuizTemp::create(Widget* pageUI, eQuizType quizType)
{
    PageQuizTemp *ret = new (std::nothrow) PageQuizTemp();
    ret->setQuizType(quizType);
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

bool PageQuizTemp::init(Widget* pageUI)
{
    if (!PageQuizBase::init(pageUI))
    {
        return false;
    }
    
    this->load(pageUI);
    
    m_pageNode = CSLoader::createNode("ui/quiz/layer_quizPage.csb");
    m_pageNode->setLocalZOrder(10);
    pageUI->addChild(m_pageNode);
    
    auto panel_previous = dynamic_cast<Layout*>(m_pageNode->getChildByName("panel_previous"));
//    panel_previous->addClickEventListener([](Ref* pSender){
//        BookBody::turnPreviousPage();
//    });
    panel_previous->setVisible(false);
    auto panel_next = dynamic_cast<Layout*>(m_pageNode->getChildByName("panel_next"));
//    panel_next->addClickEventListener([](Ref* pSender){
//        BookBody::turnNextPage();
//    });
//    panel_next->getChildByName("img_next")->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 2), DelayTime::create(0.5), ScaleTo::create(1, 1), NULL)));
    panel_next->setVisible(false);
    return true;
}

void PageQuizTemp::addEvents()
{
    Widget* pageWidget = getPageWidget();
    m_quizNode = nullptr;
    eQuizType eType = getQuizType();
    switch (eType) {
        case eQuizSentence:             //单词组句
            m_quizNode = QuizSentence::create(pageWidget);
            break;
        case eQuizFillIn:               //看图填空
            m_quizNode = QuizTextSelect::create(pageWidget, eType);
            break;
        case eQuizListen:               //看图文本听音选择
            m_quizNode = QuizTextSelect::create(pageWidget, eType);
            break;
        case eQuizPicText:             //看图文本选择
            m_quizNode = QuizTextSelect::create(pageWidget, eType);
            break;
        case eQuizListenFill:           //听音填空
            m_quizNode = QuizTextSelect::create(pageWidget, eType);
            break;
        case eQuizListenSelectPic:       //听音选图
            m_quizNode = QuizListenSelectPic::create(pageWidget);
            break;
        case eQuizListenSelect:          //听音选择
            m_quizNode = QuizTextSelect::create(pageWidget, eType);
            break;
        case eQuizPicWordDrag:           //图片单词对应
            m_quizNode = QuizPicWordDrag::create(pageWidget);
            break;
        case eQuizTextSort:            //文本排序
            m_quizNode = QuizTextSelect::create(pageWidget, eType);
            break;
        case eQuizTextSelect:            //文本选择
            m_quizNode = QuizTextSelect::create(pageWidget, eType);
            break;
        case eQuizWordDrag:             //字母拖拽
            m_quizNode = QuizWordDrag::create(pageWidget);
            break;
        case eQuizWordSelect:            //单词选择
            m_quizNode = QuizTextSelect::create(pageWidget, eType);
            break;
        default:
//            CCAssert(false, "error, no this type!");
            break;
    }
    if (m_quizNode) {
        auto bg = m_quizNode->getParent()->getChildByName("Panel_2")->getChildByName("Panel_3")->getChildByName("FileNode_1");
        float offy = (DESIGN_RESOLUTION_HEIGHT - pageWidget->getContentSize().height) / 2;
        bg->setPositionY(bg->getPositionY() + offy);
        
//        auto s1 = Sprite::create("img/baner.png");
//        s1->setAnchorPoint(Vec2(0, 0));
//        s1->setPosition(Vec2(0, Game::sharedGame()->m_bottomY));
//        pageWidget->addChild(s1);
//        auto s2 = Sprite::create("img/baner.png");
//        s2->setAnchorPoint(Vec2(1, 1));
//        s2->setPosition(Vec2(750, Game::sharedGame()->m_topY));
//        pageWidget->addChild(s2);
    }
}

void PageQuizTemp::update(float dt)
{
    
}

void PageQuizTemp::loadPage(WholeBook &wholeBook, int index)
{
    PageQuizBase::loadPage(wholeBook, index);
//    OnePage* onePage = wholeBook.m_bookPages->at(index-1);
    wholeBook.downAll(index);
    
    OnePage* onePage = wholeBook.m_bookPages->at(index);
    auto txt_page = dynamic_cast<Text*>(m_pageNode->getChildByName("txt_page"));
    txt_page->setString(onePage->m_pageTitle);
}

void PageQuizTemp::freePage()
{
    PageBase::freePage();
}

void PageQuizTemp::endDownload(string& url, string& file, int iTag, eBookResType resType)
{
#if DOWNLOAD_LOG
    SILog("下载结束", iTag)
    SSLog("下载文件", file.c_str())
    SILog("文件类型", resType)
#endif
    if (m_quizNode == nullptr) {
        return;
    }
    if (m_pageIndex == iTag) {
        CCLOG("downok page = %d, type = %d", m_pageIndex, resType);
        if (resType == eBookResPng) {
            //有png资源的，需要绑定并加载图片
            m_quizNode->setParam(file, "", "");
        }else if (m_quizNode && resType == eBookResOth) {
            m_quizNode->setWholeBook(m_wholeBook, m_pageIndex);
            m_quizNode->setParam("", "", file);
        }
    }
}

int PageQuizTemp::playAudio(std::function<void (int, const std::string &)> finishCall)
{
    PageQuizBase::playAudio(finishCall);
    if (m_quizNode == nullptr) {
        return AudioEngine::INVALID_AUDIO_ID;
    }
    m_soundID = m_quizNode->inPage();
    return m_soundID;
}

void PageQuizTemp::stopAudio()
{
    if (m_quizNode == nullptr) {
        return;
    }
    m_quizNode->outPage();
    
    if (m_soundID != AudioEngine::INVALID_AUDIO_ID) {
        SoundManager::getInstance()->stopSound(m_soundID);
    }
    PageQuizBase::stopAudio();
}

void PageQuizTemp::onTurnOut()
{
    setListenTime(false);
    OnePage* onePage = m_wholeBook->m_bookPages->at(m_pageIndex);
    
    int iCurScore = onePage->m_pageScore->m_score;
    int iTmpScore = iCurScore;
    if (onePage->m_pageTitle == "1/5") {
        iTmpScore = m_wholeBook->m_bookData->cur_quiz_1 = MAX(m_wholeBook->m_bookData->cur_quiz_1, iCurScore);
    } else if (onePage->m_pageTitle == "2/5"){
        iTmpScore = m_wholeBook->m_bookData->cur_quiz_2 = MAX(m_wholeBook->m_bookData->cur_quiz_2, iCurScore);
    } else if (onePage->m_pageTitle == "3/5"){
        iTmpScore = m_wholeBook->m_bookData->cur_quiz_3 = MAX(m_wholeBook->m_bookData->cur_quiz_3, iCurScore);
    } else if (onePage->m_pageTitle == "4/5"){
        iTmpScore = m_wholeBook->m_bookData->cur_quiz_4 = MAX(m_wholeBook->m_bookData->cur_quiz_4, iCurScore);
    } else if (onePage->m_pageTitle == "5/5"){
        iTmpScore = m_wholeBook->m_bookData->cur_quiz_5 = MAX(m_wholeBook->m_bookData->cur_quiz_5, iCurScore);
    }
    if(iTmpScore >= 100
       && m_wholeBook->m_bookData->correct_pages.find(onePage->m_pageID) == string::npos)
    {
        SILog("correct_pages.length", (int)m_wholeBook->m_bookData->correct_pages.length())
        SSLog("correct_pages.string1", m_wholeBook->m_bookData->correct_pages.c_str())
        if(m_wholeBook->m_bookData->correct_pages.length() > 0)
        {
            m_wholeBook->m_bookData->correct_pages += ",";
        }
        m_wholeBook->m_bookData->correct_pages += StringUtils::format("%d", onePage->m_pageID);
        SSLog("correct_pages.string2", m_wholeBook->m_bookData->correct_pages.c_str())
    }
    PageBase::onTurnOut();
}

void PageQuizTemp::onTurnIn()
{
    PageBase::onTurnIn();
    if (m_quizNode == nullptr) {
        return;
    }
    if (m_quizNode->getScore() == 100) {
        __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_NUT, (Ref*)(intptr_t)1);
    } else {
        __NotificationCenter::getInstance()->postNotification(MSG_UPDATE_NUT, (Ref*)(intptr_t)0);
    }
    
}

bool PageQuizTemp::showQuizOkEffect()
{
    if (m_quizNode == nullptr) {
        return false;
    }
    return m_quizNode->showOkEff();
}
