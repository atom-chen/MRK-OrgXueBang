//
//  SceneManager.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#ifndef SceneManager_hpp
#define SceneManager_hpp

#include "PreIncludes.hpp"
#include "ConstantStdafx.hpp"

class SceneManager : public cocos2d::Ref
{
public:
    static SceneManager* getInstance();
    
    void pauseGame();
    void resumeGame();
    void exitGame();
    void replaceScene(Scene *scene);
    
    void startGame();
    void setGameState(eGameState state);
    eGameState getGameState(){ return m_gameState; }
    
    Scene* getScene(eGameState state);
    Scene* getCurScene(){ return m_curScene;}
    
    void showUILogin();
public:
    Size m_curSceneSize;        //设计的有效区域大小，原来是750 * X（X >= 1000, 充满屏幕）。新版是750 * 1334，上下左右都可能有空间；
    float m_topY;               //新版顶部最高位置，相对于有效区域
    float m_bottomY;            //新版底部最低位置，相对于有效区域
    Size m_fullSceneSize;       //全屏大小
private:
    SceneManager();
    ~SceneManager();
    
private:
    eGameState m_gameState;
    
    Scene* m_curScene;
    Scene* m_mainScene;
};

#endif /* SceneManager_hpp */
