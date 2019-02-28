
/*
 按键消息分发
 by：高超
 */

#ifndef __BASE_KeyEvent_h
#define __BASE_KeyEvent_h

//需要响应返回按钮的，继承这个类，重写 OnBaseKey
//EventKeyboard::KeyCode::KEY_BACK

class BaseKeyListener
{
public:
    BaseKeyListener(void);
    ~BaseKeyListener(void);
    virtual bool OnBaseKeyBack() = 0;
};

//如果啥都不做，那么调这个继承，用于那些弹出了板，类似蜗牛
class BaseKeyListenerDoNone : public BaseKeyListener
{
public:
    BaseKeyListenerDoNone(void){};
    ~BaseKeyListenerDoNone(void){};
    virtual bool OnBaseKeyBack();
};

//继承这个，点返回直接remove，需要同时继承node
class BaseKeyListenerDoRemove : public BaseKeyListener
{
public:
    BaseKeyListenerDoRemove(void){};
    ~BaseKeyListenerDoRemove(void){};
    virtual bool OnBaseKeyBack();
};

class BaseKeyEvent
{
public:
    static BaseKeyEvent* getInstance();
    static void destroyInstance();

    void AddListener(BaseKeyListener* p);
    void RemoveListener(BaseKeyListener* p);
private:
    BaseKeyEvent(void);
    ~BaseKeyEvent(void);
private:
    std::vector<BaseKeyListener*> m_vListener;
};

#endif
