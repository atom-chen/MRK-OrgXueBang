//
//  talkingInterface.mm
//  ReadingMate
//
//  Created by dj on 17/10/27.
//
//


#ifndef talkingInterface_h
#define talkingInterface_h

#include "cocos2d.h"
USING_NS_CC;

using namespace std;

class talkingInterface
{
public:
    static void pageBegin(const string& pageName);
    static void pageEnd(const string& pageName);
    
    static void traceEvent(const string& eventId, const string& eventLabel);
};

#endif /* talkingInterface_h */
