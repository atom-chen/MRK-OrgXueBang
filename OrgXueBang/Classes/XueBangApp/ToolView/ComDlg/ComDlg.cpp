//
//  ComDlg.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/2/19.
//

#include "ComDlg.hpp"

bool ComDlg::init()
{
    if(!Layer::init()){
        return false;
    }
    initUI();
    return true;
}

void ComDlg::initUI()
{
    string csb = "View/ToolView/ComDlg.csb";
    auto mUI = CSLoader::createNodeWithVisibleSize(csb);
    
    this->addChild(mUI);
    
    vector<string> vecBtnName = {"imgKnown","btnOK","btnCancle"};
    vector<string> vecNodeName = {
        "txtInfo",
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        BtnUtil::addClickEventListener(btn,CC_CALLBACK_1(ComDlg::btnClickHandle, this));
        mapWidget[name] = btn;
    }
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
}

void ComDlg::btnClickHandle(Ref* pSender)
{
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    string backFlag = "-1";
    if(name == "imgKnown")
    {
        backFlag = "known";
    }
    else if(name == "btnOK")
    {
        backFlag = "ok";
    }
    else if(name == "btnCancle")
    {
        backFlag = "cancel";
    }
    
    if(backFlag != "-1"){
        if (m_callback) {
            m_callback(backFlag);
        }
        this->removeFromParent();
    }
}
void ComDlg::setStringAndBtnType(const string& txt, int btn)
{
    (dynamic_cast<Text*>(mapWidget["txtInfo"]))->setString(txt);
    
    if (btn == 2) {
        mapWidget["imgKnown"]->setVisible(false);
        mapWidget["btnOK"]->setVisible(true);
        mapWidget["btnCancle"]->setVisible(true);
    } else {
        mapWidget["imgKnown"]->setVisible(true);
        mapWidget["btnOK"]->setVisible(false);
        mapWidget["btnCancle"]->setVisible(false);
    }
}
void ComDlg::setCallback(const dlgCallback &callback)
{
    m_callback = callback;
}
