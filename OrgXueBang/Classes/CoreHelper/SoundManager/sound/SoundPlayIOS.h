//
//  SoundInterface.h
//  ReadingMate
//
//  Created by 丁晶 on 2017/2/27.
//
//


#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

//typedef void(^SoundPlayCallBack)(int,NSString*);



@interface SoundPlayIOS : NSObject <AVAudioPlayerDelegate>
{
    
}
@property (nonatomic, copy) NSMutableDictionary* soundDic;
@property int soundID;


+ (SoundPlayIOS*) getInstance;


/*播放声音,返回声音ID*/
- (int) playSound:(const std::string&) file  loop:(bool) loop rate:(float) rate iStartTag:(int)start  iTotalTag:(int) total;

- (float)getSoundDuration:(int) audioID; /*获取声音长度  -1 代表失败*/
- (float)getSoundCurrentTime:(int) audioID;/*获取当前时间  -1 代表失败*/
- (void)pauseSound:(int) audioID;       //暂停音效
- (void)resumeSound:(int) audioID;      //恢复音效
- (void)volumeSound:(int) audioID  volume:(float)volume;      //调整音量
- (void)speedSound:(int)audioID speed:(float)speed;  //调整速度
- (void)stopSound:(int) audioID callback:(bool)callback;/*停止声音*/
- (void)clearSound:(bool)callback;/*清除缓存*/
@end


