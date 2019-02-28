//
//  RecordBookData.hpp
//  ReadingMate-mobile
//
//  Created by metrics001 on 2018/3/20.
//

#ifndef RecordBookData_hpp
#define RecordBookData_hpp

#include <stdio.h>
#include "BookStruct.hpp"


enum eRecordBookMode
{
    eNormal = 0,    //正常
    eRecord,    //录音模式
    ePlay,    //播放模式
};

class RecordBookData
{
public:
    /*单例*/
    static RecordBookData* getInstance();
    /*销毁*/
    static void destroyInstance();
    /*初始化函数*/
    bool init();
   
    bool getEnroll(){return bEnroll;};
    void setEnroll(bool enroll){bEnroll =  enroll;};
    
    eRecordBookMode getRecordBookMode(){return m_recordMode;};
    void setRecordBookMode(eRecordBookMode recordMode){m_recordMode =  recordMode;};
    
    WholeBook& getBookData(){return m_wholeBook;};
    void setBookData(WholeBook wholeBook){m_wholeBook =  wholeBook;};
private:
     WholeBook m_wholeBook;  //整本书的数据
     eRecordBookMode m_recordMode;
     bool bEnroll;   //用户读书权限：0-无；1-有。

};
#endif /* RecordBookData_hpp */
