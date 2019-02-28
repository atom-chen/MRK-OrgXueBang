//
//  MachinePageStdafx.h
//  ReadingMate
//
//  Created by hyp on 18/11/15.
//
//

#include "MessageManager.hpp"

static MessageManager* g_MessageManager = nullptr;
/*单例*/
MessageManager* MessageManager::getInstance()
{
    if (g_MessageManager == nullptr) {
        g_MessageManager = new MessageManager;
        g_MessageManager->init();
    }
    return g_MessageManager;
    
}
void MessageManager::destroyInstance()
{
    if (g_MessageManager) {
        g_MessageManager->free();
        g_MessageManager = nullptr;
    }
}

/*初始化函数*/
bool MessageManager::init()
{
    return true;
}
void MessageManager::free()
{
    m_messageArr.clear();
}

void MessageManager::addObserver(Ref* ref,SEL_Message methods,string name)
{
    MessageManager::getInstance()->addRegistered(ref,methods,name);
}

void MessageManager::addObserver(Ref* ref,SEL_Message methods,int keyName)
{
    MessageManager::getInstance()->addObserver(ref,methods,StringUtils::format("%d",keyName));
}

void MessageManager::removeObserver(Ref* ref)
{
    MessageManager::getInstance()->removeRegistered(ref);
}
void MessageManager::postNotification(string name,Ref* parameter)
{
    MessageManager::getInstance()->runRegisteredMessage(name,parameter);
}

void MessageManager::postNotification(int keyName,Ref* parameter)
{
    MessageManager::getInstance()->postNotification(StringUtils::format("%d",keyName),parameter);
}

void MessageManager::addRegistered(Ref* ref,SEL_Message methods,string name)
{
    for (auto iter = m_messageArr.begin();iter!= m_messageArr.end();iter++) {
        if ((*iter).m_name == name&&(*iter).m_ref == ref) {
            return ;
        }
    }
    m_messageArr.push_back(MessageInfo(name,ref,methods));
}
void MessageManager::removeRegistered(Ref* ref)
{
    for (auto iter = m_messageArr.begin();iter!= m_messageArr.end();) {
        if ((*iter).m_ref == ref) {
            iter = m_messageArr.erase(iter);
        } else {
            iter++;
        }
    }
}
void MessageManager::runRegisteredMessage(string name,Ref* parameter )
{
    //防止执行的时候  删除了元素导致链表指针错乱  注释 by hyp
    bool bOk;
    set<Ref*> used;
    do {
        bOk = false;
        for (auto iter = m_messageArr.begin();iter!= m_messageArr.end();iter++) {
            if ((*iter).m_name == name) {
                if ((*iter).m_ref && used.find((*iter).m_ref) == used.end())
                {
                    used.insert((*iter).m_ref);
                     ((*iter).m_ref->*(*iter).m_methods)(parameter);
                    bOk = true;
                    break;
                }
            }
        }
    }while(bOk);
}
list<MessageInfo>::iterator MessageManager::getMessageInfo(MessageInfo message)
{
    for (auto iter = m_messageArr.begin();iter!= m_messageArr.end();iter++) {
        if ((*iter).m_name == message.m_name&&(*iter).m_ref == message.m_ref) {
            return iter;
        }
    }
    return m_messageArr.end();
}

