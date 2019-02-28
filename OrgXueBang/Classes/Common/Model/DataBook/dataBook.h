//
//  dataBook.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/5.
//
//

#ifndef dataBook_h
#define dataBook_h

#include "BookStruct.hpp"

#include <string>
#include <vector>
using namespace std;


typedef struct BookData stuBook;

//typedef struct book
//{
//    int no;
//    std::string name;
//}stuBook;

typedef struct bookLevel
{
    string name;
    vector<stuBook> books;
//    string level2Cohort();
    string new_flag;
    ~bookLevel();
    int latest;
}stuBookLevel;

typedef struct bookTopic
{
    string themeId;     //主题ID
    string name;        //主题名称
    string intro;       //主题介绍
    string cover;       //主题图片（注意需要加前缀）
    string index;       //主题索引
    string add_time;    //添加时间
    string upd_time;    //更新时间
    string remarks;     //备注
}stuBookTopic;

typedef struct bookText
{
    string name;
    string grade;
    vector<stuBook *> books;
}stuBookText;

typedef struct wordFind
{
    string word;
    string imgUrl;
    string sound;
    string enExplain;
    string cnExplain;
    bool bNewWord;
}stuWordFind;

#endif /* dataBook_h */
