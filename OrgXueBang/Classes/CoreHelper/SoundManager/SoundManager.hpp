//
//  SoundManager.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/3.
//
//

#ifndef SoundManager_hpp
#define SoundManager_hpp

#include "PreIncludes.hpp"

enum class SOUND_TYPE
{
    NORMAL,
    OGG
};
enum class SOUND_STATE
{
    NORMAL,
    PLAY,
    PAUSE,
    STOP
};
typedef struct SOUND_INFO
{
    int iId;
    SOUND_STATE state;
    SOUND_TYPE type;
}SOUND_INFO;

class SoundManager : public Ref
{
public:
    SoundManager();
    ~SoundManager();
    
    void release();
    static SoundManager* getInstance();
    
    /*背景音乐播放，只支持一个文件，新的播放立即停止旧的
     p1 文件名
     p2 是否循环
     P3 播放特效类型
     P4 每次结束回调
     */
    void playBackSound(const std::string file,bool bLoop = true, const std::function<void (int, const std::string &)> finishCall = nullptr);
    /*背景音乐音量*/
    void VolumeBackSound(float volume);
    /*暂停音乐*/
    void pauseBackSound();
    /*继续*/
    void resumeBackSound();
    /*播放音效*/ //add by hyp
    void playSoundEffect(const std::string file);
    
    /*播放声音数组*/
    void playSoundArray(std::vector<std::string> file,const std::function<void ()> finishCall = nullptr);
    /*播放声音,返回声音ID*/
    int playSound(const std::string& file,bool loop = false,const std::function<void (int, const std::string &)> finishCall = nullptr, int iStartTag = 0, int iTotalTag = 0);
    /*播放声音,返回声音ID*/
    int playSoundNew(const std::string& file,bool loop = false,float speed = 1,const std::function<void (int, const std::string &)> finishCall = nullptr, int iStartTag = 0, int iTotalTag = 0);
    /*停止声音*/
    void stopSound(int audioID,bool callback = true);
    /*获取声音长度  -1 代表失败*/
    float getSoundDuration(int audioID);
    /*获取当前时间  -1 代表失败*/
    float getSoundCurrentTime(int audioID);
    /*停止所有声音，只限于playSound播放的声音*/
    void stopAllSound(bool callback = false);
    /*暂停所有声音  切换后台专用*/
    void pauseAllSound();
    /*恢复所有声音 切换后台专用*/
    void resumeAllSound();
    
    void pauseSound(int audioID);       //暂停音效
    void resumeSound(int audioID);      //恢复音效
    void speedSound(int audioID,float speed);      //调整速度
    void setAllSoundSpeed(float speed);      //调整所有音频速度
    static bool supportSpeedChange();      //是否支持速度改变
    static bool isIOS();      //平台判断
private:
    void init();
    SOUND_INFO* getSoundByID(int audioID);
    
    int m_nBackId;
    float m_fVolume;
    std::string m_soundFile;
    float m_soundSpeed;
    //声音
    std::vector<SOUND_INFO> m_nSoundIdArray;
    
};

#endif /* SoundManager_hpp */

