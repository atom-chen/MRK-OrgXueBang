//
//  GlobalManager.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#ifndef GlobalManager_hpp
#define GlobalManager_hpp

#include "CDataHttp.hpp"
#include <stdio.h>
using namespace std;


class GlobalManager : public cocos2d::Ref, public CDataHttpDelegate
{
public:
    static GlobalManager* getInstance();
    static void destroyInstance();
    //解析报文
    void endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json) override;
public:
    
public:
    static void addRequest(HttpRequest* request);
private:
    GlobalManager();
    ~GlobalManager();
private:
};
#endif /* GlobalManager_hpp */
