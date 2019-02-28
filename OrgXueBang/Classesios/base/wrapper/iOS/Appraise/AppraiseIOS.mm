//
//  AppraiseIOS.cpp
//  ReadingFriends
//
//  Created by metrics001 on 16/10/20.
//
//
#include "AppraiseIOS.h"
#include "AppraiseManager.h"

void AppraiseIOS::showAppraise()
{
    [[AppraiseManager sharedManager] checkAppraise];
}
