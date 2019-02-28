//
//  BookListManager.hpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/4/12.
//

#ifndef BookListManager_hpp
#define BookListManager_hpp


#include "CDataHttp.hpp"
#include "BookListMain.hpp"

struct BOOK_INFO
{
    std::string strId;
    std::string strName;
    std::string strWordCount;
    std::vector<std::string> vecTopic;
    std::string strLead;
    std::string strCover;
    std::string strLevel;
};

class BookListViews;
class BookListManager: public cocos2d::Ref
, public CDataHttpDelegate
{
    BookListManager();
    ~BookListManager();
    static BookListManager* m_instance;
public:
    static BookListManager* instance();
    static BookListManager* getInstance();
    void endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json) override;
public:
    void clearData();
    void setListViews(BookListViews* view);
    void reloadListView();
    
    void requestReadingData();
    void requestCollectData();
    void requestFinishData();
    const std::vector<BOOK_INFO>& getReadingData();
    const std::vector<BOOK_INFO>& getFinishData();
    const std::vector<BOOK_INFO>& getCollectData();
    
    void setStatus(BookListStatus status);
    
    static void delInstance();
private:
    void _parseJson(const stuJson& json, BookListStatus status);
    void _showError();

    
private:
    BookListStatus m_status;
    
    std::vector<BOOK_INFO> m_vecReadingData;
    std::vector<BOOK_INFO> m_vecFinishData;
    std::vector<BOOK_INFO> m_vecCollectData;
    BookListViews* m_pListViews;
    
    int m_iReadingPage;
    int m_iFinishPage;
    int m_iCollectPage;
    
    int m_iTotalReadingPage;
    int m_iTotalFinishPage;
    int m_iTotalCollectPage;
};

#endif /* BookListManager_hpp */
