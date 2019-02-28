//
//  OggManager.hpp
//  ReadingFriends
//
//  Created by liuwei on 16/9/28.
//
//

#ifndef OggManager_hpp
#define OggManager_hpp

#ifdef __OBJC__
@class IDZOggController;
#else
typedef struct objc_object IDZOggController;
#endif

typedef std::function<void (int, const std::string &)> FinishCallBack;

class COggManager : cocos2d::Ref
{
    COggManager();
    ~COggManager();
public:
    static COggManager* getInstance();
    static COggManager* m_sOggManager;
    
public:
    /* 播放声音 */
    int playSound(std::string strName, bool bloop = false, FinishCallBack finishCall = nullptr, int startTag = 0, int totalTag = 0);
    
    /* 停止声音 */
    void stopSound(int iId);
    
    /* 暂停声音 */
    void pauseSound(int iId);
    
    /* 继续声音 */
    void resumeSound(int iId);
    
    void setPitch(int iId, float pitch);
private:
    /* 检测停止的声音 */
    void _checkStopSound(float dt);
    
    /* 播放声音 */
    void _playSound(int iId, std::string strName, bool bloop, FinishCallBack finishCall,int startTag, int totalTag);
    
private:
    std::map<int, IDZOggController*> m_mapOggControllers;
    bool m_bLazyInitLoop;
    int m_iCurrentId;
};


#endif /* OggManager_h */
