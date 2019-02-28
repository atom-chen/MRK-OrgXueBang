//
//  NewUtilManager.hpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/3/7.
//

#ifndef NewUtilManager_hpp
#define NewUtilManager_hpp
#include "cocos2d.h"

class NewUtilManager : public cocos2d::Ref
{
    NewUtilManager();
    ~NewUtilManager();
public:
    static NewUtilManager* getInstance();

    
public:
    void setJumpQuiz(bool jump);
    bool getJumpQuiz();
private:
    bool m_bJumpQuiz;
};

#endif /* NewUtilManager_hpp */
