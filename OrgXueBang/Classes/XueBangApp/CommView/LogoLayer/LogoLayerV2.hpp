//
//  LogoLayerV2.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#ifndef LogoLayerV2_hpp
#define LogoLayerV2_hpp

#include "stdafx.h"

class LogoLayerV2 : public Layer
{
public:
    CREATE_FUNC(LogoLayerV2);
    
    //    virtual void onEnter() override;
    //    virtual void onExit() override;
    virtual bool init() override;
    
private:
    void initUI();
    void btnClickHandle(Ref* pSender);
    void doPageView();
    
    void jumpNext();
    void checkEnd();
    Layer* m_layer;
    Vec2 m_touchPosOld, m_touchPosNew;
    bool m_bEnd;
private:
    std::unordered_map<string, Widget*> mapWidget;
};

#endif /* LogoLayerV2_hpp */
