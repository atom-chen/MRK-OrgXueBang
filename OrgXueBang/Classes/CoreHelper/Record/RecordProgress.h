//
//  RecordProgress.hpp
//  ReadingFriends
//
//  Created by liuwei on 16/12/15.
//
//

#ifndef RecordProgress_hpp
#define RecordProgress_hpp

#include <stdio.h>
#include "cocos2d.h"


class CRecordProgress
{
public:
    static void startRecordProgress(cocos2d::Node* button, const stuJson& json, float scale);
    
    static void inRecordProgress(cocos2d::Node* button, float t);
    
    static void endRecordProgress(cocos2d::Node* button);
};

#endif /* RecordProgress_hpp */
