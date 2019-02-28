//
//  NetworkService.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/4/14.
//
//

#ifndef NetworkService_hpp
#define NetworkService_hpp

#include <map>
#include <string>
using namespace std;
#include "HttpTag.h"

class NetworkService
{
private:
    NetworkService();
    void init();
public:
    virtual ~NetworkService();
    
    static NetworkService* getInstance();
    void setServerAddress(string address);
    string getServiceUrl(eHttpType type, string otherArgs = "");
    string getDataServerAddress(){return m_ServerAddress;};
    
private:
    map<eHttpType, string> m_serviceMap;
    string m_ServerAddress;
};

#endif /* NetworkService_hpp */
