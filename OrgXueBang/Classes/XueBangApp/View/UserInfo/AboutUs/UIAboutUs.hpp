//
//  UIAboutUs.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#ifndef UIAboutUs_hpp
#define UIAboutUs_hpp

#include "stdafx.h"

class UIAboutUs : public Layer , public BaseKeyListenerDoRemove
{
public:
    CREATE_FUNC(UIAboutUs);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
private:
    void initUI();
    void btnClickHandle(Ref* pSender);
private:
    std::unordered_map<string, Widget*> mapWidget;
};

#endif /* UIAboutUs_hpp */
