//
//  SoundPlayIOS.m
//  ReadingMate
//
//  Created by hyp on 2018/10/17.
//
//

#import "SoundPlayIOS.h"
#include "SoundInterface.h"
static SoundPlayIOS* _SoundPlayIOSObj = nullptr;

@implementation SoundPlayIOS

+ (SoundPlayIOS*) getInstance
{
    if (_SoundPlayIOSObj == nullptr) {
        _SoundPlayIOSObj = [[[SoundPlayIOS alloc] init] autorelease];
        [_SoundPlayIOSObj retain];
    }
    return _SoundPlayIOSObj;
}
- (id) init
{
    if (self = [super init]) {
        _soundDic = [[[[NSMutableDictionary alloc]init]autorelease]retain];
        _soundID = 1;
        [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
    }
    return self;
}
- (void) dealloc
{
    [super dealloc];
    [_soundDic release];
}


/*播放声音,返回声音ID*/
- (int) playSound:(const std::string&) file  loop:(bool) loop rate:(float) rate iStartTag:(int)start  iTotalTag:(int) total;
{
    NSError *err = nil;
    NSString* fileStr = [NSString stringWithCString:file.c_str() encoding:NSUTF8StringEncoding];
    NSData *fileData = [[NSFileManager defaultManager] contentsAtPath:fileStr];
    AVAudioPlayer* musicPlayer = [[[AVAudioPlayer alloc] initWithData:fileData fileTypeHint:AVFileTypeMPEGLayer3 error:&err]autorelease] ;
    if (musicPlayer&&!err){
        [musicPlayer setDelegate:self];
        [musicPlayer setEnableRate:true];
        [musicPlayer setVolume:1.0];
        [musicPlayer setRate:rate];
        [musicPlayer setNumberOfLoops:(loop?-1:0)];
        [musicPlayer setCurrentTime:start == 0?0:(start/100.0f)];
        [musicPlayer prepareToPlay];
        [musicPlayer play];
        //计数
        _soundID=(_soundID>10000000?1:++_soundID);
        [_soundDic setObject:musicPlayer forKey:[NSString stringWithFormat:@"%i",_soundID]];
        return _soundID;
    } else if (err) {
        NSLog(@"声音播放错误err::%@",err);
    }
    return -1;
}

/*获取当前时间  -1 代表失败*/
- (float)getSoundCurrentTime:(int) audioID
{
    NSString* audioStr = [NSString stringWithFormat:@"%i",audioID];
    if([[_soundDic allKeys] containsObject:audioStr])
    {
        AVAudioPlayer* musicPlayer = [_soundDic objectForKey:audioStr];
        float currentTime = [musicPlayer currentTime];
        return currentTime;
    }
    return -1;
}
/*获取声音长度  -1 代表失败*/
- (float)getSoundDuration:(int) audioID
{
    NSString* audioStr = [NSString stringWithFormat:@"%i",audioID];
    if([[_soundDic allKeys] containsObject:audioStr])
    {
        AVAudioPlayer* musicPlayer = [_soundDic objectForKey:audioStr];
        float duration = [musicPlayer duration];
        return duration;
    }
    return -1;
}
//暂停音效
- (void)pauseSound:(int) audioID;
{
    NSString* audioStr = [NSString stringWithFormat:@"%i",audioID];
    if([[_soundDic allKeys] containsObject:audioStr])
    {
        AVAudioPlayer* musicPlayer = [_soundDic objectForKey:audioStr];
        [musicPlayer pause];
    }
    return ;
}
//恢复音效
- (void)resumeSound:(int) audioID  
{
    NSString* audioStr = [NSString stringWithFormat:@"%i",audioID];
    if([[_soundDic allKeys] containsObject:audioStr])
    {
        AVAudioPlayer* musicPlayer = [_soundDic objectForKey:audioStr];
        [musicPlayer play];
    }
    return ;
}
/*停止声音*/
- (void)stopSound:(int) audioID callback:(bool)callback
{
    NSString* audioStr = [NSString stringWithFormat:@"%i",audioID];
    if([[_soundDic allKeys] containsObject:audioStr])
    {
        AVAudioPlayer* musicPlayer = [_soundDic objectForKey:audioStr];
        [musicPlayer stop];
        [_soundDic removeObjectForKey:audioStr];//删除
//        [musicPlayer release];
        if (callback) {
            SoundInterface::runFinishCallBack([audioStr intValue]);
        }
    }
    return ;
}
//调整音量
- (void)volumeSound:(int)audioID volume:(float)volume
{
    NSString* audioStr = [NSString stringWithFormat:@"%i",audioID];
    if([[_soundDic allKeys] containsObject:audioStr])
    {
        AVAudioPlayer* musicPlayer = [_soundDic objectForKey:audioStr];
        [musicPlayer setVolume:volume];
    }
    return ;
}
//调整速度
- (void)speedSound:(int)audioID speed:(float)speed
{
    NSString* audioStr = [NSString stringWithFormat:@"%i",audioID];
    if([[_soundDic allKeys] containsObject:audioStr])
    {
        AVAudioPlayer* musicPlayer = [_soundDic objectForKey:audioStr];
        [musicPlayer setRate:speed];
    }
    return ;
}
/*清除缓存*/
- (void)clearSound:(bool)callback
{
    //停止所有音频
    for (NSString *key in _soundDic) {
        AVAudioPlayer* musicPlayer = [_soundDic objectForKey:key];
        [musicPlayer stop];
        if (callback) {
            SoundInterface::runFinishCallBack([key intValue]);
        }
    }
    [_soundDic removeAllObjects];
    _soundID = 1;
}

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag{
    for (NSString *key in _soundDic) {
        if ([_soundDic objectForKey:key]==player) {
            int sound = [key intValue];
            [_soundDic removeObjectForKey:key];//删除
//            [player release];
            SoundInterface::runFinishCallBack(sound);
            break;
        }
    }
    
}
-(void)audioPlayerBeginInterruption:(AVAudioPlayer *)player{
    
}
- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError * __nullable)error{
    NSLog(@"error:%@",error);
    //删除
    for (NSString *key in _soundDic) {
        if ([_soundDic objectForKey:key]==player) {
            [_soundDic removeObjectForKey:key];
//            [player release];
            SoundInterface::clearSound([key intValue]);
            break;
        }
    }
}
@end

