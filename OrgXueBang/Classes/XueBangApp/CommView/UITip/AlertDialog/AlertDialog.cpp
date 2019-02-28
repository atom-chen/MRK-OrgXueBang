//
//  AlertDialog.cpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/24.
//

#include "AlertDialog.hpp"
#include "BtnUtil.hpp"

AlertDialog::AlertDialog()
:m_okCallBack(nullptr)
,m_cancelCallBack(nullptr)
{
}

AlertDialog::~AlertDialog()
{
    m_okCallBack = nullptr;
    m_cancelCallBack = nullptr;
}

bool AlertDialog::OnBaseKeyBack()
{
    this->removeFromParent();
    return true;
}

AlertDialog *AlertDialog::create(string content)
{
    AlertDialog *ret = new (std::nothrow) AlertDialog();
    if (ret && ret->init(content))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}
bool AlertDialog::init(string content)
{
    auto mUI = CSLoader::createNodeWithVisibleSize("View/ToolView/UIAlertDialog.csb");
    this->addChild(mUI);
    this->setName("AlertDialog");
    
    vector<string> vecBtnName = {"btn_ok","btn_cancel"};
    vector<string> vecNodeName = {
        "panel_center","txt_content","txtErrorTip",
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(AlertDialog::btnClickHandle, this));
        mapWidget[name] = btn;
    }
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
    (dynamic_cast<Text*>(mapWidget["txt_content"]))->setString(content);
    
    return true;
}

void AlertDialog::setBtnOkImage(const std::string& normal,
                                const std::string& selected,
                                const std::string& disabled,
                                Widget::TextureResType texType)
{
    ((Button*)mapWidget["btn_ok"])->loadTextures(normal, selected, disabled, texType);
}

void AlertDialog::setBtnCancelImage(const std::string& normal,
                                    const std::string& selected,
                                    const std::string& disabled,
                                    Widget::TextureResType texType)
{
    
    ((Button*)mapWidget["btn_cancel"])->loadTextures(normal, selected, disabled, texType);
}

void AlertDialog::btnClickHandle(Ref* pSender)
{
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    
    SoundManager::getInstance()->playSoundEffect("music/snd_btn.mp3");
    if(name == "btn_ok")
    {
        if ( m_okCallBack ) {
            m_okCallBack();
        }
    }
    else if(name == "btn_cancel")
    {
        if ( m_cancelCallBack ) {
            m_cancelCallBack();
        }
    }
    this->removeFromParent();
}
void AlertDialog::setShowOnlyOKBtn()
{
    mapWidget["btn_ok"]->setPositionX(mapWidget["panel_center"]->getContentSize().width*0.5f);
    mapWidget["btn_cancel"]->setVisible(false);
}
void AlertDialog::setBothBtnPosition()
{
    Vec2 okPos = mapWidget["btn_ok"]->getPosition();
    Vec2 cancelPos = mapWidget["btn_cancel"]->getPosition();
    mapWidget["btn_ok"]->setPosition(cancelPos);
    mapWidget["btn_cancel"]->setPosition(okPos);
}
