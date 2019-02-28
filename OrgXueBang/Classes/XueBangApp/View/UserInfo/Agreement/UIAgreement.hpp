//
//  UIAgreement.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#ifndef UIAgreement_hpp
#define UIAgreement_hpp

#include "stdafx.h"

class UIAgreement : public Layer , public BaseKeyListenerDoRemove
{
public:
    CREATE_FUNC(UIAgreement);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
private:
    void initUI();
    void btnClickHandle(Ref* pSender);
private:
    std::unordered_map<string, Widget*> mapWidget;
    cocos2d::experimental::ui::WebView* m_webView;
};

#endif /* UIAgreement_hpp */
