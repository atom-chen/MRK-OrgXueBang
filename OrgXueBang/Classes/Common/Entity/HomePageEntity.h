//
//  HomePageEntity.h
//  OrgXueBang
//
//  Created by liuwei on 2019/1/23.
//

#ifndef HomePageEntity_h
#define HomePageEntity_h

#include "BookStruct.hpp"

#include <stdio.h>
using namespace std;

typedef struct EntBook{
    EntBook():_BookID(""),_BookName(""),_CoverUrl(""),_IsReadFlag(false),
    _NewFlag(""),_BookLevel("A"){
        
    };
    string _BookID;
    string _BookName;
    string _CoverUrl;//封面
    bool _IsReadFlag;
    string _NewFlag;
    string _BookLevel;
}EntBook;

typedef struct EntOneWeekBookList{
    EntOneWeekBookList():_WeekID(""),_Title(""),_DateTime(""){
        
    };
    string _WeekID;
    string _Title;
    string _DateTime;
    vector<EntBook*> vecBook;
}EntOneWeekBookList;

typedef struct EntHomePageBookList{
    EntHomePageBookList(){
        init();
    };
    void init(){
        _FrontPageIndex = 0;
        _AfterPageIndex = 0;
        _CurPageIndex = 0;
        _PageCount = 0;
        _CurWeekID = "0";
    };
    int _FrontPageIndex;
    int _AfterPageIndex;
    int _CurPageIndex;
    int _PageCount;
    string _CurWeekID;
    vector<EntOneWeekBookList*> vecOneWeekBookList;
    map<string,EntOneWeekBookList*> mapOneWeekBookList;
}EntHomePageBookList;

typedef struct BookData EntHomePage;



#endif /* HomePageEntity_h */
