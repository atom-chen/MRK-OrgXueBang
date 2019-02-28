//
//  AlertDialog.hpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/24.
//

#ifndef AlertDialog_hpp
#define AlertDialog_hpp

#include "stdafx.h"

typedef std::function <void()> AlertDialogOKFunc;
typedef std::function <void()> AlertDialogCancelFunc;

class AlertDialog : public cocos2d::Node, public BaseKeyListener
{
public:
    AlertDialog();
    ~AlertDialog();
    
    static AlertDialog* create(string content);
    virtual bool init(string content);
    virtual bool OnBaseKeyBack() override;

    void setOkCallBack(AlertDialogOKFunc callBack) { m_okCallBack = callBack; };
    void setCancelCallBack(AlertDialogCancelFunc callBack) { m_cancelCallBack = callBack; };
    void setShowOnlyOKBtn();
    void setBothBtnPosition();
    
    void setBtnOkImage(const std::string& normal,
                       const std::string& selected,
                       const std::string& disabled = "",
                       Widget::TextureResType texType = Widget::TextureResType::LOCAL);
    void setBtnCancelImage(const std::string& normal,
                           const std::string& selected,
                           const std::string& disabled = "",
                           Widget::TextureResType texType = TextureResType::LOCAL);

    
private:
    void btnClickHandle(Ref* pSender);
private:
    std::unordered_map<string, Widget*> mapWidget;
    AlertDialogOKFunc m_okCallBack;
    AlertDialogCancelFunc m_cancelCallBack;
};

#endif /* AlertDialog_hpp */
