//
//  UserEntity.h
//  OrgXueBang
//
//  Created by liuwei on 2019/1/8.
//

#ifndef UserEntity_h
#define UserEntity_h

#include <stdio.h>
using namespace std;

typedef struct EntUser{
    EntUser(){
        init();
    };
    void init(){
        _UserID = "";
        _UserToken = "";
        _Account = "";
        _UserName = "";
        _HeadIcon = "face1.png";
        _SchoolName = "";
        _GradeName = "";
        _ClassName = "";
        _TeacherName = "";
        _Coin = 900;
        _ExpireTime = 0l;//time(&_ExpireTime);
    };
    string _UserID;
    string _UserToken;
    string _Account;
    string _UserName;
    string _HeadIcon;
    string _SchoolName;
    string _GradeName;
    string _ClassName;
    string _TeacherName;
    string _ExpireTimeStr;
    long   _ExpireTime;
    int    _Coin;//金币
}EntUser;

#endif /* UserEntity_h */
