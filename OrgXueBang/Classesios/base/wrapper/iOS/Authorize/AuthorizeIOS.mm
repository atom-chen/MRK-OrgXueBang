//
//  AuthorizeIOS.cpp
//  ReadingFriends
//
//  Created by metrics001 on 16/10/20.
//
//
#include "AuthorizeIOS.h"
#include "AuthorizeManager.h"

bool AuthorizeIOS::checkPhoto()
{
    return [[AuthorizeManager sharedManager] checkPhoto];
}
bool AuthorizeIOS::checkMicrophone()
{
    return [[AuthorizeManager sharedManager] checkMicrophone];
}
bool AuthorizeIOS::checkPush()
{
    return [[AuthorizeManager sharedManager] checkPush];
}
bool AuthorizeIOS::checkNetwork()
{
    return [[AuthorizeManager sharedManager] checkNetwork];
}
float AuthorizeIOS::getBright()
{
    return  [[UIScreen mainScreen] brightness];
}
void AuthorizeIOS::setBright(float value)
{
    [[UIScreen mainScreen] setBrightness:value];;
}
void AuthorizeIOS::openUrl(string urlStr)
{
    NSString *NSStr = [NSString stringWithUTF8String:urlStr.c_str()];
    NSURL *url = [[ NSURL alloc ] initWithString:NSStr];
    [[UIApplication sharedApplication] openURL:url];
}

void AuthorizeIOS::hideStatusbar(bool bHide)
{
    [[UIApplication sharedApplication] setStatusBarHidden:bHide];
}
