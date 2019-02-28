//
//  AudioCallBackMsg.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/7/19.
//
//

#ifndef AudioCallBackMsg_hpp
#define AudioCallBackMsg_hpp

#include <stdio.h>
#include "cocos2d.h"


class AudioCallBackMsg : public cocos2d::Ref
{
public:
    static AudioCallBackMsg* getInstance();

    void callBack(int ret, float dt);
    
    void stop(float dt);
    void reset();
private:
    AudioCallBackMsg();
    ~AudioCallBackMsg();
    
    int m_ret;
};

#endif /* AudioCallBackMsg_hpp */
