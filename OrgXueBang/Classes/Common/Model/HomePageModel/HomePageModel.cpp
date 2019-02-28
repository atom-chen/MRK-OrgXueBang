//
//  HomePageModel.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/23.
//

#include "HomePageModel.hpp"


HomePageModel::HomePageModel()
{
    mEntHomePageBookList = new EntHomePageBookList();
    clearData();
}
HomePageModel::~HomePageModel()
{
    
}

EntHomePageBookList* HomePageModel::getHomePageBookListEnt()
{
    return mEntHomePageBookList;
}

int HomePageModel::getAllWeekBookListCount()
{
    return (int)mEntHomePageBookList->vecOneWeekBookList.size();
}

void HomePageModel::clearData()
{
    mEntHomePageBookList->init();
    for(int i = mEntHomePageBookList->vecOneWeekBookList.size()-1;i >= 0 ;i-- ){
        EntOneWeekBookList* bl = mEntHomePageBookList->vecOneWeekBookList.at(i);
        for (int j = bl->vecBook.size()-1;j >= 0 ; j--) {
            delete bl->vecBook.at(j);
        }
        delete bl;
    }
    mEntHomePageBookList->vecOneWeekBookList.clear();
    mapLocalData.clear();
}

EntOneWeekBookList* HomePageModel::getOneWeekBookListByIndex(int index)
{
    if(index < 0 || index >= mEntHomePageBookList->vecOneWeekBookList.size()){
        return nullptr;
    }
    return mEntHomePageBookList->vecOneWeekBookList.at(index);
}

bool HomePageModel::isCurWeekID(string weekID)
{
    return (weekID == mEntHomePageBookList->_CurWeekID);
}

void HomePageModel::parseHomePageBookList(const stuJson& json)
{
    stuJson returnData = json.getJson("return_data");
    mEntHomePageBookList->_CurWeekID = returnData.getValue("cur_week_id");
    
    int count = atoi(returnData.getValue("pagecount").c_str());
    int index = atoi(returnData.getValue("cur_page").c_str());
    mEntHomePageBookList->_PageCount = count;
    mEntHomePageBookList->_CurPageIndex = index;
    
    if(count > 0){
        if(mapLocalData.size() == 0){
            mEntHomePageBookList->_FrontPageIndex = index;
            mEntHomePageBookList->_AfterPageIndex = index;
        }
        if(mapLocalData[index] != true){
            string baseBookCoverUrl = returnData.getValue("book_cover_url");
            int n = returnData.GetArraySize("homepage_booklist");
            for (int i = 0; i < n; i++) {
                stuJson hwList = returnData.GetArrayJson("homepage_booklist", i);
                
                EntOneWeekBookList* entWeekList = new EntOneWeekBookList();
                entWeekList->_WeekID = hwList.getValue("week_id");
                entWeekList->_Title = hwList.getValue("title");
                entWeekList->_DateTime = hwList.getValue("datetime");
                
                int num = hwList.GetArraySize("booklist");
                for(int j = 0; j < num; j++){
                    stuJson booklist = hwList.GetArrayJson("booklist", j);
                    EntBook* entbook = new EntBook();
                    entbook->_BookID = booklist.getValue("book_id");
                    entbook->_BookName = booklist.getValue("book_name");
                    entbook->_CoverUrl = baseBookCoverUrl + booklist.getValue("cover_url");
                    entbook->_IsReadFlag = (booklist.getValue("read_flag") == "1");//0未读，1已读
                    string level = booklist.getValue("level");
                    std::transform(level.begin(), level.end(), level.begin(), ::tolower);
                    entbook->_BookLevel = level;//使用时判断长度 level.length >= 1
                    
                    entWeekList->vecBook.push_back(entbook);
                }
                //        mEntHomePageBookList->mapOneWeekBookList[entWeekList->_WeekID] = entWeekList;
                mEntHomePageBookList->vecOneWeekBookList.push_back(entWeekList);
            }
            sortWeekBook();
        }
        mapLocalData[index] = true;
    }
}

int HomePageModel::getIndexByWeekID()
{
    return getIndexByWeekID(mEntHomePageBookList->_CurWeekID);
}
int HomePageModel::getIndexByWeekID(string weekID)
{
    int index = -1;
    for (int i=0; i < mEntHomePageBookList->vecOneWeekBookList.size(); i++) {
        if(mEntHomePageBookList->vecOneWeekBookList.at(i)->_WeekID == weekID){
            index = i;
            break;
        }
    }
    return index;
}

void HomePageModel::setBookReaded(string bookID)
{
    for (auto weekUnit : mEntHomePageBookList->vecOneWeekBookList) {
        for(auto book : weekUnit->vecBook){
            if(book->_BookID == bookID){
                book->_IsReadFlag = true;
            }
        }
    }
}

void HomePageModel::sortWeekBook()
{
    std::sort(mEntHomePageBookList->vecOneWeekBookList.begin(),
              mEntHomePageBookList->vecOneWeekBookList.end(),
              [](EntOneWeekBookList* a,EntOneWeekBookList* b){
                  return (atol(a->_WeekID.c_str()) < atol(b->_WeekID.c_str()));
              });
}

bool HomePageModel::isHadLocalData(int pageIndex)
{
    return (mapLocalData[pageIndex] == true);
}
