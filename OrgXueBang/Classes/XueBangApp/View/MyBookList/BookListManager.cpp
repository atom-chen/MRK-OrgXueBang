//
//  BookListManager.cpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/4/12.
//

#include "BaseLoad.h"
#include "HttpManager.h"
#include "UserManager.hpp"
#include "StringUtil.hpp"
#include "BookListViews.hpp"
#include "TipManager.hpp"

#include "BookListManager.hpp"
#include "SceneManager.hpp"

const int PAGE_NUM = 20; //一页请求多少个

BookListManager* BookListManager::m_instance = nullptr;

BookListManager* BookListManager::instance() { return m_instance; }

BookListManager* BookListManager::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new BookListManager;
    }
    return m_instance;
}

BookListManager::BookListManager()
:m_pListViews(nullptr)
,m_status(BookListStatus::none)
,m_iReadingPage(0)
,m_iFinishPage(0)
,m_iCollectPage(0)

,m_iTotalReadingPage(0)
,m_iTotalFinishPage(0)
,m_iTotalCollectPage(0)
{
    
}
BookListManager::~BookListManager()
{
    
}

void BookListManager::setStatus(BookListStatus status)
{
    m_status = status;
}

void BookListManager::reloadListView()
{
    m_vecReadingData.clear();
    m_vecFinishData.clear();
    m_vecCollectData.clear();
    
    m_iReadingPage = 0;
    m_iFinishPage = 0;
    m_iCollectPage = 0;
    
    m_iTotalReadingPage = 0;
    m_iTotalFinishPage = 0;
    m_iTotalCollectPage = 0;
    
    BookListManager::instance()->requestFinishData();
    BookListManager::instance()->requestReadingData();
    BookListManager::instance()->requestCollectData();
}


void BookListManager::requestReadingData()
{
    if (m_iReadingPage > m_iTotalReadingPage)
    {
        return;
    }
    if (CDataHttp::getInstance()->GetHttpOpState(eHttpType::booklist_reading) == eHttpStateBase::eHttpStateBase_send)
    {
        return;
    }
    
    HttpDataToString dataStr;
    dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
    dataStr.addData("status", "0");
    m_iReadingPage++;
    auto page = StringUtils::format("%d", m_iReadingPage);
    auto limit = StringUtils::format("%d", PAGE_NUM);
    dataStr.addData("page", page);
    dataStr.addData("limit", limit);
    CDataHttp::getInstance()->HttpGet(eHttpType::booklist_reading, dataStr.getString());
}

void BookListManager::requestFinishData()
{
    if (m_iFinishPage > m_iTotalFinishPage)
    {
        return;
    }
    if (CDataHttp::getInstance()->GetHttpOpState(eHttpType::booklist_finish) == eHttpStateBase::eHttpStateBase_send)
    {
        return;
    }
    
    HttpDataToString dataStr;
    dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
    dataStr.addData("status", "1");
    m_iFinishPage++;
    auto page = StringUtils::format("%d", m_iFinishPage);
    auto limit = StringUtils::format("%d", PAGE_NUM);
    dataStr.addData("page", page);
    dataStr.addData("limit", limit);
    CDataHttp::getInstance()->HttpGet(eHttpType::booklist_finish, dataStr.getString());
}

void BookListManager::requestCollectData()
{
    if (m_iCollectPage > m_iTotalCollectPage)
    {
        return;
    }
    if (CDataHttp::getInstance()->GetHttpOpState(eHttpType::favourite) == eHttpStateBase::eHttpStateBase_send)
    {
        return;
    }
    
    HttpDataToString dataStr;
    dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
    m_iCollectPage++;
    auto page = StringUtils::format("%d", m_iCollectPage);
    auto limit = StringUtils::format("%d", PAGE_NUM);
    dataStr.addData("page", page);
    dataStr.addData("limit", limit);
    
    CDataHttp::getInstance()->HttpGet(eHttpType::favourite, dataStr.getString());
}


void BookListManager::_parseJson(const stuJson& returnData, BookListStatus status)
{
    std::vector<BOOK_INFO> vecInfo;
    vecInfo.clear();
    int count = 0;
    
    if (returnData.GetArraySize("books") > 0) {
        string book_cover_folder = returnData.getJson("url").getValue("book_cover_folder");
        for (int i = 0; i < returnData.GetArraySize("books"); i++)
        {
            stuJson book = returnData.GetArrayJson("books", i);
            BOOK_INFO info;
            info.strId = book.getValue("course_id");
            info.strName = book.getValue("name");
            info.strWordCount = book.getValue("word_count");
            info.strLead = book.getValue("intro");
            info.strCover = book_cover_folder + book.getValue("cover");
            info.strLevel = book.getValue("level");
            StringUtil::splitString(book.getValue("topics").c_str(), ',', info.vecTopic);
            vecInfo.push_back(info);
        }
        count = atoi(returnData.getJson("paging").getValue("totalnum").c_str());
    }
    int totalPage = count/PAGE_NUM;
    if (count % PAGE_NUM != 0)
    {
        totalPage += 1;
    }
    
    if (status == BookListStatus::reading)
    {
        m_vecReadingData.insert(m_vecReadingData.end(), vecInfo.begin(), vecInfo.end());
        m_iTotalReadingPage = totalPage;
        if ((int)m_vecReadingData.size() <= 0 && m_status == status) {
            if (SceneManager::getInstance()->getGameState() == eStateMain) {
                TipManager::getInstance()->showMsgTips("无在读书本信息！");
            }
        }
    }
    else if (status == BookListStatus::finish)
    {
        m_vecFinishData.insert(m_vecFinishData.end(), vecInfo.begin(), vecInfo.end());
        m_iTotalFinishPage = totalPage;
        if ((int)m_vecFinishData.size() <= 0 && m_status == status) {
            TipManager::getInstance()->showMsgTips("无收藏书本信息！");
        }
    }
    else if (status == BookListStatus::collect)
    {
        m_vecCollectData.insert(m_vecCollectData.end(), vecInfo.begin(), vecInfo.end());
        m_iTotalCollectPage = totalPage;
        if ((int)m_vecCollectData.size() <= 0 && m_status == status) {
            TipManager::getInstance()->showMsgTips("无已读书本信息！");
        }
    }
    
    if (m_pListViews)
    {
        m_pListViews->refreshView(status);
    }
}


void BookListManager::_showError()
{
    //            std::string return_msg = json.getValue("return_msg");
    
    //            node_none->setVisible(true);
    //            auto anim_sleep = CSLoader::createTimeline("ui/ani/songshu_sleep.csb");
    //            anim_sleep->gotoFrameAndPlay(0, true);
    //            node_none->runAction(anim_sleep);
}


void BookListManager::endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json)
{
    eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
    
    if (nOp == eHttpType::booklist_reading || nOp == eHttpType::booklist_finish)
    {
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            stuJson returnData = json.getJson("return_data");
            if (returnData.getValue("status") == "0")
            {
                _parseJson(returnData, BookListStatus::reading);
            }
            else
            {
                _parseJson(returnData, BookListStatus::finish);
            }
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            if ((json.getValue("return_code") == "6012")) {
                if (nOp == eHttpType::booklist_reading)
                {
                    stuJson returnData = json.getJson("return_data");
                    _parseJson(returnData, BookListStatus::reading);
                }else if (nOp == eHttpType::booklist_finish) {
                    stuJson returnData = json.getJson("return_data");
                    _parseJson(returnData, BookListStatus::finish);
                }
            }
            
            _showError();
        }
    }
    else if (nOp == eHttpType::favourite)
    {
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            stuJson returnData = json.getJson("return_data");
            _parseJson(returnData, BookListStatus::collect);
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            if ((json.getValue("return_code") == "6012")) {
                stuJson returnData = json.getJson("return_data");
                _parseJson(returnData, BookListStatus::collect);
            }
            _showError();
        }
    }
}



const std::vector<BOOK_INFO>& BookListManager::getReadingData()
{
    return m_vecReadingData;
}
const std::vector<BOOK_INFO>& BookListManager::getFinishData()
{
    return m_vecFinishData;
}
const std::vector<BOOK_INFO>& BookListManager::getCollectData()
{
    return m_vecCollectData;
}

void BookListManager::clearData()
{
    m_vecReadingData.clear();
    m_vecFinishData.clear();
    m_vecCollectData.clear();
    m_iReadingPage = 0;
    m_iFinishPage = 0;
    m_iCollectPage = 0;
    
    m_iTotalReadingPage = 0;
    m_iTotalFinishPage = 0;
    m_iTotalCollectPage = 0;
    
    m_pListViews = nullptr;
}


void BookListManager::setListViews(BookListViews* view)
{
    m_pListViews = view;
}

void BookListManager::delInstance()
{
    m_instance->release();
    m_instance = nullptr;
}
