//
//  UILoading.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/16.
//

#ifndef UILoading_hpp
#define UILoading_hpp

#include "stdafx.h"

class UILoading : public Layer
{
public:
    CREATE_FUNC(UILoading);
    virtual bool init() override;
    
    static void showLoader(float minShowTime = 0.5);
    static void HideLoader();
    static void showFrog();
    static bool isShow();
private:
    void initUI();
    void createFrog();
    void createFrogLater(float delay);
private:
    std::unordered_map<string, Widget*> mapWidget;
    SkeletonAnimation* m_frog;
    bool m_bDel;
};

#endif /* UILoading_hpp */
