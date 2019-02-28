//
//  HomePageManager.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/23.
//

#ifndef HomePageManager_hpp
#define HomePageManager_hpp

#include "CDataHttp.hpp"
#include <stdio.h>
using namespace std;

#include "HomePageModel.hpp"

class HomePageManager : public cocos2d::Ref, public CDataHttpDelegate
{
public:
    static HomePageManager* getInstance();
    static void destroyInstance();
    //解析报文
    void endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json) override;
public:
    void reqHomePageBookList(int curPageIndex = 0,int pageLimitNum = 20);
    bool reqHomePageBookList(bool reqNextPage = false,bool forceReq = false);
    
    void setBookReaded(string bookID);
    void setReadingBookInfo(string curWeek,string curBookID);
public:
    HomePageModel* getHomePageModel();
    EntHomePage* getHomePageEnt();
private:
    HomePageManager();
    ~HomePageManager();
private:
    void init();
private:
    HomePageModel* mHomePageModel;
    static HomePageManager* instance;
    bool forceReq;
    string curReadingWeek,curReadingBookID;
};

#endif /* HomePageManager_hpp */
