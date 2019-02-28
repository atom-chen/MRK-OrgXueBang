//
//  UISetting.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#ifndef UISetting_hpp
#define UISetting_hpp

#include "stdafx.h"

class UISetting : public Layer , public BaseKeyListenerDoRemove
{
public:
    CREATE_FUNC(UISetting);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
private:
    void initUI();
    void btnClickHandle(Ref* pSender);
private:
    std::unordered_map<string, Widget*> mapWidget;
};

#endif /* UISetting_hpp */
