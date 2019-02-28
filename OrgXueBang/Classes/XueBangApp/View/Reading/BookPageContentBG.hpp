//
//  BookPageContentBG.hpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/11/2.
//

#ifndef BookPageContentBG_hpp
#define BookPageContentBG_hpp

#include "stdafx.h"
class BookPageContentBG : public Layer
{
public:
    BookPageContentBG();
    virtual ~BookPageContentBG();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    CREATE_FUNC(BookPageContentBG);

private:
    void initUI();
};

#endif /* BookPageContentBG_hpp */
