//
//  UIUserInfo.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#ifndef UIUserInfo_hpp
#define UIUserInfo_hpp

#include "stdafx.h"

class UIUserInfo : public Layer , public BaseKeyListenerDoRemove
{
public:
    CREATE_FUNC(UIUserInfo);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
private:
    void initUI();
    void btnClickHandle(Ref* pSender);
    void onUpdateHeadIcon(Ref* ref);
    void doAllListCell();
    void initUIData();
private:
    std::unordered_map<string, Widget*> mapWidget;
};

#endif /* Login_hpp */
