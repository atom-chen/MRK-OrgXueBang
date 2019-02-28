//
//  MachinePageStdafx.h
//  ReadingMate
//
//  Created by hyp on 18/11/15.
//
//

#ifndef MessageManager_hpp
#define MessageManager_hpp

#include <stdio.h>
#include "BaseHeader.h"
//消息参数1
class MessageParam : public Ref
{
public:
    vector<string> paramStr;
    vector<float> paramFloat;
    vector<int> paramInt;
};
//消息参数2
template<class T>
class MessageParamT : public Ref
{
public:
    MessageParamT(T* value){
        paramValue = value;
    }
    T* paramValue;
};
//回调方法
//typedef std::function<void(string)> SEL_Message;
typedef void (Ref::*SEL_Message)(Ref*);
#define CC_CALLMESSAGE_SELECTOR(_SELECTOR) static_cast<SEL_Message>(&_SELECTOR)
//消息信息
struct MessageInfo
{
    string m_name;
    Ref* m_ref;
    SEL_Message m_methods;
    
    MessageInfo(string name,Ref* ref,SEL_Message methods){
        m_name = name;
        m_ref = ref;
        m_methods = methods;
    }
    MessageInfo(){
        
    }
    ~MessageInfo(){
        
    }
};
//消息管理机制
class MessageManager {
public:
    /*单例*/
    static MessageManager* getInstance();
    static void destroyInstance();
    /*注册*/
    static void addObserver(Ref* ref,SEL_Message methods,string name);
    
    static void addObserver(Ref* ref,SEL_Message methods,int keyName);
    /*删除注册*/
    static void removeObserver(Ref* ref);
    /*执行*/
    static void postNotification(string name,Ref* parameter = nullptr);
    static void postNotification(int keyName,Ref* parameter = nullptr);
private:
    /*初始化函数*/
    bool init();
    void free();
    /*注册*/
    void addRegistered(Ref* ref,SEL_Message methods,string name);
    /*删除注册*/
    void removeRegistered(Ref* ref);
    /*执行*/
    void runRegisteredMessage(string name,Ref* parameter);
    /*判断是否存在*/
    list<MessageInfo>::iterator getMessageInfo(MessageInfo message);
private:
    list<MessageInfo> m_messageArr;

};
#endif /* MessageManager_hpp */

