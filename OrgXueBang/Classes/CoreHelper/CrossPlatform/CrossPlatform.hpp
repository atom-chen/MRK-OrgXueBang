//
//  CrossPlatform.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#ifndef CrossPlatform_hpp
#define CrossPlatform_hpp

#include <stdio.h>
#include <string>
using namespace std;

class CrossPlatform
{
public:
    static CrossPlatform* getInstance();
private:
    CrossPlatform();
    ~CrossPlatform();
public:
    string getDeviceInfo();
    string getVersion();
private:
    static CrossPlatform* instance;
};

#endif /* CrossPlatform_hpp */
