//
//  BookDataManager.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/20.
//
//

#ifndef BookDataManager_hpp
#define BookDataManager_hpp

#include "PreIncludes.hpp"
#include "BookStruct.hpp"
#include "stdafx.h"

class BookDataManager : public Ref
{
public:
    BookDataManager();
    virtual ~BookDataManager();
    
    static BookDataManager* getInstance();
    
    void resetBookData();
    
    void clearData();
    
    void release();
    
private:
    WholeBook m_wholeBook;//整本书的数据
public:
    WholeBook getWholeBook()
    {
        return m_wholeBook;
    }
    
    void setWholeBook(WholeBook& wholeBook)
    {
        m_wholeBook = wholeBook;
    }
    
    time_t m_openTime;
    time_t getOpenTime()
    {
        return m_openTime;
    }
    void setOpenTime(time_t mTime)
    {
        m_openTime = mTime;
    }
    
private:
    int m_lastPage;
    
    Size size_body;
    
    eReadMode m_readMode;
public:
    int getLastPage()
    {
        return m_lastPage;
    }
    void setLastPage(int iLast)
    {
        m_lastPage = iLast;
    }
    
    void setBodySize(Size size)
    {
        size_body = size;
    }
    Size getBodySize()
    {
        return size_body;
    }
    
    void setReadMode(eReadMode mode)
    {
        m_readMode = mode;
        if(mode == eReadAuto){
            Device::setKeepScreenOn(true);
        }
    }
    
    eReadMode getReadMode()
    {
        return m_readMode;
    }
    void saveBookSoundSpeed();
    MYDEF_GETSETVALUE(float,BookSoundSpeed);
    MYDEF_GETSETVALUE(bool,SpeedIsChange);
};

#endif /* BookDataManager_hpp */
