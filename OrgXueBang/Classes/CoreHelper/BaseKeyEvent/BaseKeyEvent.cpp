//

#include "stdafx.h"

BaseKeyListener::BaseKeyListener(void)
{
    BaseKeyEvent::getInstance()->AddListener(this);
}

BaseKeyListener::~BaseKeyListener(void)
{
    BaseKeyEvent::getInstance()->RemoveListener(this);
}

bool BaseKeyListenerDoNone::OnBaseKeyBack()
{
    return true;
}

bool BaseKeyListenerDoRemove::OnBaseKeyBack()
{
    auto p = this;
    Node * p1 = dynamic_cast<Node * > (p);
    if(p1)
    {
        p1->removeFromParent();
    }
    return true;
}


static BaseKeyEvent* gBKEventInstance = nullptr;

BaseKeyEvent* BaseKeyEvent::getInstance()
{
    if (gBKEventInstance == nullptr) {
        gBKEventInstance = new BaseKeyEvent;
    }
    return gBKEventInstance;
}

void BaseKeyEvent::destroyInstance()
{
    if (gBKEventInstance) {
		delete gBKEventInstance;
        gBKEventInstance = nullptr;
    }
}

BaseKeyEvent::BaseKeyEvent(void)
{
    m_vListener.clear();
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event*){
        if (m_vListener.size() > 0 && key == EventKeyboard::KeyCode::KEY_BACK) {
            m_vListener[m_vListener.size() - 1]->OnBaseKeyBack();
        }
    };
    EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithFixedPriority(listener, 1);
}

BaseKeyEvent::~BaseKeyEvent(void)
{
	m_vListener.clear();
}

void BaseKeyEvent::AddListener(BaseKeyListener* p)
{
    RemoveListener(p);
    m_vListener.push_back(p);
}

void BaseKeyEvent::RemoveListener(BaseKeyListener* p)
{
    for (auto it = m_vListener.begin(); it != m_vListener.end(); it++) {
        if (*it == p) {
            m_vListener.erase(it);
            break;
        }
    }
}
