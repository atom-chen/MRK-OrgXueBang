//
//  BookDataManager.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/20.
//
//

#include "BookDataManager.hpp"
BookDataManager::BookDataManager()
{
    m_lastPage = 0;
    size_body = Size::ZERO;
    m_readMode = eReadNone;
    m_SpeedIsChange = false;
    m_BookSoundSpeed = 1.0f;
    if (SoundManager::supportSpeedChange()) {
        m_BookSoundSpeed = UserDefault::getInstance()->getFloatForKey("BookSoundSpeed",1.0f);
    }
}
BookDataManager::~BookDataManager()
{
    release();
}

BookDataManager* BookDataManager::getInstance()
{
    static BookDataManager instance;
    return &instance;
}

void BookDataManager::resetBookData()
{
    clearData();
}

void BookDataManager::clearData()
{
    
}

void BookDataManager::release()
{
    
}
void BookDataManager::saveBookSoundSpeed()
{
    UserDefault::getInstance()->setFloatForKey("BookSoundSpeed",m_BookSoundSpeed);
    UserDefault::getInstance()->flush();
}
