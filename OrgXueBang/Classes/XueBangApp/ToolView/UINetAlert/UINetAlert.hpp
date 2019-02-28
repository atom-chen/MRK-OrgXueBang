//
//  UINetAlert.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/16.
//

#ifndef UINetAlert_hpp
#define UINetAlert_hpp

#include "stdafx.h"

class UINetAlert : public Layer
{
public:
    CREATE_FUNC(UINetAlert);
    virtual bool init() override;
    
    void addRequest(HttpRequest* request);
private:
    void initUI();
    void btnClickHandle(Ref* pSender);
private:
    std::unordered_map<string, Widget*> mapWidget;
    vector<HttpRequest*> m_requestList;
};

#endif /* UINetAlert_hpp */
