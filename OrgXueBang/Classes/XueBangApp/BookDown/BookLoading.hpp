//
//  BookLoading.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#ifndef BookLoading_hpp
#define BookLoading_hpp

#include "stdafx.h"

class BookLoading : public Layer
{
public:
    static BookLoading* create(std::string bookID);
    
    BookLoading();
    virtual ~BookLoading();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
private:
    void initUI();
    void loadingResult(Ref* ref);
    void updateProgress(Ref* ref);
private:
    std::string m_bookID;
    std::unordered_map<string, Widget*> mapWidget;
};

#endif /* BookLoading_hpp */
