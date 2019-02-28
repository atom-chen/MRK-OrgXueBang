//
//  UserModel.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#include "UserModel.hpp"

UserModel::UserModel()
{
    mEntUser = new EntUser();
}
UserModel::~UserModel()
{
    
}

void UserModel::parseLoginInfo(const stuJson& json)
{
    stuJson returnData = json.getJson("return_data");
    
    mEntUser->_UserID = returnData.getValue("user_id");
    mEntUser->_UserName = returnData.getValue("user_name");
    mEntUser->_SchoolName = returnData.getValue("school_name");
    mEntUser->_GradeName = returnData.getValue("drade_name");
    mEntUser->_ClassName = returnData.getValue("class_name");
    mEntUser->_TeacherName = returnData.getValue("teacher_name");
    mEntUser->_ExpireTimeStr = returnData.getValue("expire_time");
    
    if(returnData.getValue("pic_name") != "-1"){
        mEntUser->_HeadIcon = returnData.getValue("pic_name");
    }
    mEntUser->_UserToken = returnData.getValue("token");
    mEntUser->_ExpireTime = atol(returnData.getValue("expire_time").c_str());
}
void UserModel::setUserToken(std::string userToken)
{
    mEntUser->_UserToken = userToken;
}
std::string UserModel::getUserToken()
{
    return mEntUser->_UserToken;
}

bool UserModel::isLogined()
{
    return ("" != mEntUser->_UserToken);
}
EntUser* UserModel::getUserEnt()
{
    return mEntUser;
}

void UserModel::setHeadIcon(string icon)
{
    mEntUser->_HeadIcon = icon;
}
