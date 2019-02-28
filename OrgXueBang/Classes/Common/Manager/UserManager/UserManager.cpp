//
//  UserManager.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#include "UserManager.hpp"

#include "MessageManager.hpp"
#include "CrossPlatform.hpp"
#include "CoreHelperStdafx.h"
#include "FileSave.h"
#include "CCBuffer.h"
#include "EventNameDefine.hpp"

static UserManager* instance = nullptr;
UserManager* UserManager::getInstance()
{
    if(instance == nullptr){
        instance = new UserManager();
    }
    return instance;
}

void UserManager::destroyInstance()
{
    delete instance;
    instance = nullptr;
}

UserManager::UserManager()
{
    init();
}
UserManager::~UserManager()
{
    
}
void UserManager::init()
{
    mUserModel = new UserModel();
    
    CFileSave::createSaveDir();
    loadUserToken();
}

UserModel* UserManager::getUserModel()
{
    return mUserModel;
}
EntUser* UserManager::getUserEnt()
{
    return mUserModel->getUserEnt();
}

void UserManager::loadUserToken()
{
    Data data;
    if (eFileSaveResult::ok == CFileSave::EncryptLoad("", data, "")) {
        if (data.getSize() > 0) {
            auto pBuff = CCBuffer::create((const char*)data.getBytes(), (unsigned int)data.getSize());
            string token = pBuff->readLengthAndString();
            mUserModel->setUserToken(token);
            reqUserInfo();
            return;
        }
    }
}
void UserManager::saveUserToken()
{
    string token = mUserModel->getUserToken();
    auto pBuff = CCBuffer::create();
    pBuff->writeLengthAndString(token.c_str());
    CFileSave::EncryptSave("", (unsigned char *)pBuff->data(), pBuff->length(), "");
}

void UserManager::doLogout()
{
    mUserModel->getUserEnt()->init();
    
    CFileSave::deleteSaveFile();
    destroyInstance();
}
void UserManager::reqUserInfo()
{
    if (mUserModel->getUserToken().length() < 1) {
        return;
    }
//    return;
    HttpDataToString dataStr;
    dataStr.addData("token", mUserModel->getUserToken());
    dataStr.addData("mode", "1");
    dataStr.addData("devices", CrossPlatform::getInstance()->getDeviceInfo(), true);
    CDataHttp::getInstance()->HttpGet(eHttpType::userInfo, dataStr.getString());
}

void UserManager::reqLogin(string account,string pwd)
{
    HttpDataToString dataStr;
    
    bool debug = true;
    if(debug){
        dataStr.addData("login", account);
        dataStr.addData("password", pwd);
        dataStr.addData("devices", CrossPlatform::getInstance()->getDeviceInfo());
    }else{
        dataStr.addData("accout", account);
        dataStr.addData("password", pwd);
    }
    
    CDataHttp::getInstance()->HttpPost(eHttpType::login, dataStr.getString());
}

void UserManager::reqHeadIcon(string headIcon)
{
    HttpDataToString dataStr;
    dataStr.addData("token", mUserModel->getUserToken());
    dataStr.addData("pic_name", headIcon);
    CDataHttp::getInstance()->HttpPost(eHttpType::updatepic, dataStr.getString());
}
void UserManager::endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json)
{
    eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
    if (nOp == eHttpType::login) {
        if (eHttpStateBase::eHttpStateBase_Rok == e) {
            std::string return_code = json.getValue("return_code");
            if (return_code != "6000") {
                std::string return_msg = json.getValue("return_msg");
                return;
            }
            mUserModel->parseLoginInfo(json);
            saveUserToken();
            
            MsgParam* param = new MsgParam();
            MessageManager::getInstance()->postNotification((int)nOp,(Ref*)param);
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr){
            string tipError = CDataHttp::getInstance()->GetHttpOpMsg(nOp);
            MsgParam* param = new MsgParam(tipError);
            MessageManager::getInstance()->postNotification((int)nOp,(Ref*)param);
        }
    }
    else if (nOp == eHttpType::userInfo){
        if (eHttpStateBase::eHttpStateBase_Rok == e) {
            stuJson returnData = json.getJson("return_data");
            
            mUserModel->parseLoginInfo(json);
            MessageManager::getInstance()->postNotification(NSEventNameDef::UpdateHeadIcon);
        }
    }
    else if (nOp == eHttpType::updatepic){
        if (eHttpStateBase::eHttpStateBase_Rok == e) {
            stuJson returnData = json.getJson("return_data");
            string icon = returnData.getValue("pic_name");
            mUserModel->setHeadIcon(icon);
        }
    }
}
