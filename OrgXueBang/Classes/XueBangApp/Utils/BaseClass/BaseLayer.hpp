//
//  BaseLayer.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/11.
//
//

#ifndef BaseLayer_hpp
#define BaseLayer_hpp

#include "PreIncludes.hpp"




#define CLICK_EFFECT(sender, type) \
do{\
Button* btn = (Button*)sender; \
if(type == cocos2d::ui::Widget::TouchEventType::BEGAN) \
{\
btn->setScale(0.9f);\
}\
else if(type == cocos2d::ui::Widget::TouchEventType::ENDED || type == cocos2d::ui::Widget::TouchEventType::CANCELED) \
{\
btn->setScale(1.0f); \
}\
}while(0)

#define CLICK_CHECK_TIME(sender, type) \
do{ \
if(isForbiddenAction(sender,type)) \
{ \
return; \
} \
}while(0)

//#define CLICK_CHECK_END(sender, type) \
//do{ \
//if (type != ui::Widget::TouchEventType::ENDED) \
//{ \
//return; \
//} \
//}while(0)

#define CLICK_CHECK(sender, type) \
do{\
if (type != cocos2d::ui::Widget::TouchEventType::ENDED) \
{ \
return; \
} \
if(isForbiddenAction(sender,type)) \
{ \
return; \
}\
}while(0)

#define CLICK_FILTER(sender, type) \
do{\
Button* btn = (Button*)sender; \
if(type == cocos2d::ui::Widget::TouchEventType::BEGAN) \
{\
btn->setScale(0.9f); \
return; \
}\
else if(type == cocos2d::ui::Widget::TouchEventType::MOVED) \
{\
return; \
}\
else if(type == cocos2d::ui::Widget::TouchEventType::CANCELED) \
{\
btn->setScale(1.0f); \
return; \
}\
else if(type == cocos2d::ui::Widget::TouchEventType::ENDED) \
{\
btn->setScale(1.0f); \
}\
}while(0)


class BaseLayer : public Layer
{
public:
    BaseLayer();
    virtual ~BaseLayer();
    
    CREATE_FUNC(BaseLayer);
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    static void resizeUI(Layout* json_root);
    virtual bool load(const char* gameUIFile);
    
    // 基类继承于此， 通过这个挂接事件
    virtual void addEvents(){};
    virtual void update(float dt){};
    virtual void resetUIData(){};
    virtual void refreshUIView(){};// 刷新子类ui
    
    void refreshCallBack(Ref *target = nullptr);
    
    Widget* getRootWidget(){
        return m_rootWidget;
    }
    
    bool isForbiddenAction(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void cleanup();
    
protected:
    Widget*	m_rootWidget;
    timeval m_actionTime;
};

#endif /* BaseLayer_hpp */
