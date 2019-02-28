//
//  UserModel.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#ifndef UserModel_hpp
#define UserModel_hpp

#include "BaseLoad.h"
#include "EntityStdafx.h"

class UserModel
{
public:
    UserModel();
    ~UserModel();
public:
    //解析报文数据
    void parseLoginInfo(const stuJson& json);
    void setHeadIcon(string icon);
public:
    EntUser* getUserEnt();//数据结构
    
    void setUserToken(std::string userToken);
    std::string getUserToken();
    bool isLogined();
private:
    EntUser* mEntUser;
};

#endif /* UserModel_hpp */
