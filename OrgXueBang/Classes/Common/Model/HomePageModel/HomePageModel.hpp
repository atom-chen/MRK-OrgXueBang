//
//  HomePageModel.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/23.
//

#ifndef HomePageModel_hpp
#define HomePageModel_hpp

#include "BaseLoad.h"
#include "EntityStdafx.h"

#include "dataBook.h"

class HomePageModel
{
public:
    HomePageModel();
    ~HomePageModel();
public:
    void parseHomePageBookList(const stuJson& json);
    
public:
    EntHomePageBookList* getHomePageBookListEnt();
    int getAllWeekBookListCount();
    
    EntOneWeekBookList* getOneWeekBookListByIndex(int index);
    void clearData();
    int getIndexByWeekID();
    void setBookReaded(string bookID);
    bool isHadLocalData(int pageIndex);
    bool isCurWeekID(string weekID);
private:
    int getIndexByWeekID(string weekID);
    void sortWeekBook();
private:
    EntHomePageBookList* mEntHomePageBookList;
    map<int,bool> mapLocalData;
};

#endif /* HomePageModel_hpp */
