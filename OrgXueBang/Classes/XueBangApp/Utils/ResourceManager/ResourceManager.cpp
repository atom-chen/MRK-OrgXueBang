//
//  ResourceManager.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/11.
//
//

#include "ResourceManager.hpp"
#include "BaseLayer.hpp"
#include "CostTime.hpp"
ResourceManager::ResourceManager()
{
    init();
}

ResourceManager::~ResourceManager()
{
    
}

void ResourceManager::release()
{
    map<string, Layout* >::iterator iter = m_uiMap.begin();
    for(; iter != m_uiMap.end(); ++iter){
        CC_SAFE_RELEASE_NULL(iter->second);
    }
    
    m_uiMap.clear();
}

void ResourceManager::init()
{
    
}

ResourceManager* ResourceManager::getInstance()
{
    static ResourceManager instance;
    return &instance;
}

Layout* ResourceManager::getUIFromJson(const char* ui, bool needScale)
{
    map<string, Layout*>::iterator iter = m_uiMap.find(ui);
    Layout* panel = NULL;
    COST_TIME_TAG(ui, "getUIFromJson !")
    if( iter == m_uiMap.end())
    {
        Node* node = CSLoader::createNode(ui);
        COST_TIME_TAG(ui, "createNode over !")
        panel = dynamic_cast<Layout*>(node->getChildByName("panel_root"));
        
        panel->removeFromParent();
        
        auto cacheIter = m_cacheUIMap.find(ui);
        if(cacheIter != m_cacheUIMap.end()){
            m_uiMap.insert(make_pair(ui, panel));
            panel->retain();
        }
        
        if(needScale)
        {
            BaseLayer::resizeUI(panel);
        }
    }
    else
    {
        panel = iter->second;		
    }
    COST_TIME_TAG(ui, "getUIFromJson over !")
    return panel;
}


