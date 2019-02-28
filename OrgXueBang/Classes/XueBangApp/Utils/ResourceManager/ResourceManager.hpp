//
//  ResourceManager.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/11.
//
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include "PreIncludes.hpp"
//#include "Constants.hpp"

class ResourceManager : public Ref
{
public:
    ResourceManager();
    ~ResourceManager();
    
    void release();
    static ResourceManager* getInstance();
    
    // ui
    Layout* getUIFromJson(const char* ui, bool needScale=true );
    
    //music
    void preloadAllMusic();
    
    static void resizeUI(Layout* json_root);
    
private:
    void init();
    
    map<string, Layout* >			m_uiMap;
    map<string, int>				m_cacheUIMap;
};

#endif /* ResourceManager_hpp */
