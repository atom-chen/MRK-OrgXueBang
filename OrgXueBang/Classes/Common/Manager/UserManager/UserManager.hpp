//
//  UserManager.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#ifndef UserManager_hpp
#define UserManager_hpp

#include "CDataHttp.hpp"
#include <stdio.h>
using namespace std;

#include "UserModel.hpp"

class UserManager : public cocos2d::Ref, public CDataHttpDelegate
{
public:
    static UserManager* getInstance();
    static void destroyInstance();
    //接收网络数据，通知UI层数据已有
    void endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json) override;
public:
    void reqUserInfo();//在UI层--请求网络数据
    void reqLogin(string account,string pwd);
    void reqHeadIcon(string headIcon);
    
    void doLogout();
public:
    UserModel* getUserModel();//数据与状态，网络数据的解析
    EntUser* getUserEnt();//数据
private:
    UserManager();
    ~UserManager();
private:
    void init();
    void loadUserToken();
    void saveUserToken();
private:
    UserModel* mUserModel;
};
#endif /* UserManager_hpp */
