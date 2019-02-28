//
//  ComDlg.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/2/19.
//

#ifndef ComDlg_hpp
#define ComDlg_hpp

#include "stdafx.h"

class ComDlg : public Layer
{
public:
    typedef function<void(const string&)> dlgCallback;
    CREATE_FUNC(ComDlg);
    
    //    virtual void onEnter() override;
    //    virtual void onExit() override;
    virtual bool init() override;
    
public:
    void setCallback(const dlgCallback& callback);
    void setStringAndBtnType(const string& txt, int btn = 1);
    
private:
    void initUI();
    void btnClickHandle(Ref* pSender);
private:
    std::unordered_map<string, Widget*> mapWidget;
    dlgCallback m_callback;
};

#endif /* ComDlg_hpp */
