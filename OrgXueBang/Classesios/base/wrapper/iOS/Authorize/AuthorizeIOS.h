//
//  AuthorizeIOS.hpp
//  ReadingFriends
//
//  Created by metrics001 on 16/10/20.
//
//

#ifndef AuthorizeIOS_hpp
#define AuthorizeIOS_hpp

#include <stdio.h>
using namespace std;


class AuthorizeIOS {
public:
    static bool checkPhoto();
    static bool checkMicrophone();
    static bool checkPush();
    static bool checkNetwork();
    
    
    static float getBright();
    static void  setBright(float value);
    
    static void  openUrl(string urlStr);
    
    static void hideStatusbar(bool);
};
#endif /* AuthorizeIOS_hpp */
