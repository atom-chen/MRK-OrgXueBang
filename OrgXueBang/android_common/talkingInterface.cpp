//
//  talkingInterface.mm
//  ReadingMate
//
//  Created by dj on 17/10/27.
//
//

#include "talkingInterface.h"
#include "AndroidMethod.h"
#include "json.h"

void talkingInterface::pageBegin(const string &pageName)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CppCallJava(MYJNICALL_TYPE_TALKINGDATA_PAGEBEGIN, pageName);
#endif
}

void talkingInterface::pageEnd(const string &pageName)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CppCallJava(MYJNICALL_TYPE_TALKINGDATA_PAGEEND, pageName);
#endif
}

void talkingInterface::traceEvent(const string &eventId, const string &eventLabel)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Json::Value root;
    root["eventId"] = eventId;
    root["eventLabel"] = eventLabel;
    
    CppCallJava(MYJNICALL_TYPE_TALKINGDATA_EVENT, root.toStyledString());
#endif
}


