//
//  BookListMain.hpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/4/9.
//

#ifndef BookListMain_hpp
#define BookListMain_hpp

#include "stdafx.h"

enum class BookListStatus
{
    none,
    reading,
    collect,
    finish
};

class BookListMain : public cocos2d::Layer, public BaseKeyListenerDoRemove
{
public:
    BookListMain();
    ~BookListMain();
    CREATE_FUNC(BookListMain);
    virtual bool init() override;
    virtual void onEnter() override;
private:
    void _initUI();
    void _changeStatus(BookListStatus status);
    
private:
    cocos2d::Size m_winSize;
    BookListStatus m_status;
    std::map<std::string, cocos2d::Node*> m_mapUI;
};

#endif /* BookListMain_hpp */
