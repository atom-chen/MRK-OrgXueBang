//
//  NewUtilManager.cpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/3/7.
//


#include "NewUtilManager.hpp"


NewUtilManager::NewUtilManager()
:m_bJumpQuiz(false)
{
}

NewUtilManager::~NewUtilManager()
{
}

NewUtilManager* NewUtilManager::getInstance()
{
    static NewUtilManager instance;
    return &instance;
}




void NewUtilManager::setJumpQuiz(bool jump)
{
    m_bJumpQuiz = jump;
}
bool NewUtilManager::getJumpQuiz()
{
    return m_bJumpQuiz;
}
