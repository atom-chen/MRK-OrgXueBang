//
//  CHRecorder.m
//  CHRecordDemo
//
//  Created by Summer on 16/5/23.
//  Copyright © 2016年 Summer. All rights reserved.
//

#import "CHRecorder.h"
#import <AVFoundation/AVFoundation.h>

#define CHOpenTime       if (self.progressBlock && self.timeCallInterval > 0 && !self.timer) [self _startTimer];
#define CHEndTime        if (self.timer) [self _endTimer];
#define CHEmptyPathInfo    _currentFileName = nil; _currentFilePath = nil;

typedef NS_ENUM(NSInteger, CHRecordType) {
    CHRecordTypeImmediately = 1,          // 立即录音
    CHRecordTypeAutoClose   = 2,          // 自动结束
    CHRecordTypeForecast    = 3,          // 在未来开始
    CHRecordTypeForecastAndAutoClose = 4, // 在未来开始 并且 自动结束
};

@implementation CHRecorderConfiguration


+ (instancetype)defaultConfiguration
{
    CHRecorderConfiguration *configuration = [[CHRecorderConfiguration alloc] init];
   
    configuration.audioFormat = CHRecordAudioFormatWAV;
    configuration.sampleRate = 16000;
    configuration.numberOfChannels = 2;
    configuration.linearPCMBitDepth = 16;
    configuration.linearPCMIsFloat = YES;
    
//    NSArray *docPaths =  NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
//    NSString *documentsDirectory = [docPaths objectAtIndex:0];
//    NSString *voiceDirectory = [documentsDirectory stringByAppendingPathComponent:@"voice"];
//    if ( ! [[NSFileManager defaultManager] fileExistsAtPath:voiceDirectory]) {
//        [[NSFileManager defaultManager] createDirectoryAtPath:voiceDirectory withIntermediateDirectories:YES attributes:nil error:NULL];
//    }
//    configuration.tempFilePath = voiceDirectory;
    //add by hyp  废弃
    
    return configuration;
}

@end

@interface CHRecorder()< AVAudioRecorderDelegate >
{
    BOOL _isInitialize;
    BOOL _taskToken;
    BOOL _isCancel;
}

@property (nonatomic, strong) AVAudioRecorder *audioRecorder;

@property (nonatomic, copy) CHRecorderBlcok startBlock;
@property (nonatomic, copy) CHRecorderProgressBlock progressBlock;
@property (nonatomic, copy) CHRecorderPauseBlcok pauseBlock;
@property (nonatomic, copy) CHRecorderFinishBlcok finishBlock;
@property (nonatomic, copy) CHRecorderPlayOver playOver;
@property (nonatomic, copy) CHRecorderBlcok cancelBlock;

/** 时间回调间隔 **/
@property (nonatomic, assign) CGFloat timeCallInterval;
@property (nonatomic, strong) NSTimer *timer;

/** 当前任务文件存放目录 */
@property (nonatomic, copy) NSString *currentFileName;
@property (nonatomic, copy) NSString *currentFilePath;

@end

@implementation CHRecorder

@synthesize audioRecorder = _audioRecorder;

+ (instancetype)shareRecorder
{
    static CHRecorder *recorder;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        recorder = [[CHRecorder alloc] init];
    });
    return recorder;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self _initialize];
    }
    return self;
}

- (void)_initialize
{
    if (_isInitialize) return;
        _isInitialize = YES;
    
    [self setConfiguration:[CHRecorderConfiguration defaultConfiguration]];
    //[self _setAudioSession];
}

- (void)prepareToRecord
{
    if (!self.audioRecorder) {
        [self createAudioRecorder];
    }
    [self.audioRecorder prepareToRecord];
}

- (bool)startRecord:(NSString *)fileName
{
    if(fileName && ![fileName isEqualToString:@""])
    {
        [self startRecordWithFileName:fileName];
    }
    else{
        [self startRecordWithDuration: -1 Time: -1 withType:CHRecordTypeImmediately];
    }
    return true;
}

/** 开始录音 - 并且设置文件名称 */
- (void)startRecordWithFileName:(NSString *)fileName
{
    [self startRecordWithFileName:fileName andFliePath:nil];
}
/** 开始录音 - 并且设置文件存放路径 */
- (void)startRecordWithFliePath:(NSString *)filePath
{
    [self startRecordWithFileName:nil andFliePath:filePath];
}
/** 开始录音 - 并且设置文件名称和存放路径 */
- (void)startRecordWithFileName:(NSString *)fileName andFliePath:(NSString *)filePath
{
    if (fileName && ![fileName isEqualToString:@""])
        [self setCurrentFileName:[NSString stringWithFormat:@"%@%@", fileName, [self _getAudioFormat]]];
    
    if (filePath && ![filePath isEqualToString:@""])
        [self setCurrentFilePath:filePath];
    
    [self startRecordWithDuration: -1 Time: -1 withType:CHRecordTypeImmediately];
}

/** 在多少秒之后开始录音 **/
- (void)startRecordAtTime:(NSTimeInterval)time
{
    [self startRecordWithDuration: -1 Time: time withType:CHRecordTypeForecast];
}
/** 开始录音 - 设置录制时间 - 自动结束 **/
- (void)startRecordForDuration:(NSTimeInterval) duration
{
    [self startRecordWithDuration:duration Time: -1 withType:CHRecordTypeAutoClose];
}

- (void)startRecordAtTime:(NSTimeInterval)time forDuration:(NSTimeInterval) duration
{
    [self startRecordWithDuration:duration Time:time withType:CHRecordTypeForecastAndAutoClose];
}

- (void)startRecordWithDuration:(NSTimeInterval) duration Time:(NSTimeInterval)time  withType:(CHRecordType)type
{
    if (self.isRecording) return;
    
    if (!self.audioRecorder) {
        [self createAudioRecorder];
    }
    
    switch (type) {
        case CHRecordTypeForecast:
        {
            [self.audioRecorder recordAtTime: self.audioRecorder.deviceCurrentTime + time];
        }
            break;
        case CHRecordTypeAutoClose:
        {
            [self.audioRecorder recordForDuration:duration];
        }
            break;
        case CHRecordTypeImmediately:
        {
            [self.audioRecorder record];
        }
            break;
        case CHRecordTypeForecastAndAutoClose:
        {
            [self.audioRecorder recordAtTime:self.audioRecorder.deviceCurrentTime + time forDuration:duration];
        }
            break;
            
        default:
            break;
    }
    
    if (time == -1) {
        [self.audioRecorder record];
    } else if (time == -2) {
        
    } else {
        [self.audioRecorder recordForDuration:time];
    }
    
    if (!self.isPause) {
        if (self.startBlock) {
            self.startBlock();
        }
    }
    
    _pause = NO;
    _taskToken = YES;
    
    CHOpenTime
}

- (void)pauseRecord
{
    if (!_taskToken) return;
    if (!self.isRecording) return;
    
    [self.audioRecorder pause];
    
    _pause = YES;
    
    if (self.pauseBlock) {
        self.pauseBlock(_pause);
    }
    
    CHEndTime
    
}

- (void)resumeRecord
{
    if (!_taskToken) return;
    if (!_pause) return;
    
    [self startRecord];
    
    _pause = NO;
    
    if (self.pauseBlock) {
        self.pauseBlock(_pause);
    }
    
    CHOpenTime
}

- (void)endRecord
{
    if (!_taskToken) return;
    if (!self.isRecording) return;
    
    [self.audioRecorder stop];
    
    _taskToken = NO;
    
    CHEmptyPathInfo;
    CHEndTime
}

- (void)cancelRecord
{
    if (!_taskToken) return;
    _isCancel = YES;
    [self.audioRecorder stop];
    [self.audioRecorder deleteRecording];
    if (self.cancelBlock) {
        self.cancelBlock();
    }
    _taskToken = NO;
    
    CHEmptyPathInfo;
    CHEndTime
}



#pragma mark - - < AVAudioRecorderDelegate >

/** 视频播放完成 **/
- (void)audioRecorderDidFinishRecording:(AVAudioRecorder *)recorder successfully:(BOOL)flag
{
    if (self.finishBlock && !_isCancel) {
        self.finishBlock(self.currentFileUrl);
    }
    
    [self releaseAudioRecorder];
    
    if (_isCancel) _isCancel = NO;
    
    if (self.configuration.isAutoPlay) {
        // 播放录音
    }
}

- (void)audioRecorderEncodeErrorDidOccur:(AVAudioRecorder *)recorder error:(NSError * __nullable)error
{
    
}


#pragma mark - - call Blcoks
- (void)setStartRecordCallBlcok:(CHRecorderBlcok)block
{
    self.startBlock = block;
}

/** 设置录音中进度回调 **/
- (void)setRecordTimeInterval:(CGFloat)interval andCallBlcok:(CHRecorderProgressBlock)block
{
    self.timeCallInterval = interval;
    self.progressBlock = block;
}

- (void)setPauseOrResumeRecordCallBlcok:(CHRecorderPauseBlcok)block
{
    self.pauseBlock = block;
}

- (void)setEndRecordCallBlcok:(CHRecorderFinishBlcok)block
{
    self.finishBlock = block;
}

- (void)setEndPlayCallBlcok:(CHRecorderPlayOver)block
{
    self.playOver = block;
}

- (void)setCancelRecordCallBlock:(CHRecorderBlcok)block
{
    self.cancelBlock = block;
}


#pragma mark - - 
- (BOOL)isRecording
{
    return self.audioRecorder.isRecording;
}

- (void)createAudioRecorder
{
    [self _setAudioSession];
    
//    NSString *filePath = [NSString stringWithFormat:@"%@/%@", self.currentFilePath, self.currentFileName];
    
    NSURL *fileURL = [NSURL fileURLWithPath:self.currentFileName];
    
    
    NSDictionary *settingDic = [self _getAudioSetting];
    NSError *error = nil;
    [self setAudioRecorder:[[AVAudioRecorder alloc] initWithURL:fileURL settings:settingDic error:&error]];
    _audioRecorder.delegate = self;
    _audioRecorder.meteringEnabled = YES;
    
    if (error){
        NSLog(@"CH: 录音机创建失败: %@", error.localizedDescription);
    }
}

- (void)releaseAudioRecorder
{
    [self setAudioRecorder:nil];
    [self _restorationAudioSessionSetting];
}

- (NSString *)currentFilePath
{
    if (_currentFilePath == nil) {
        _currentFilePath = self.configuration.tempFilePath;
        
    }
    return _currentFilePath;
}

- (NSString *)currentFileName
{
    if (_currentFileName == nil) {
        _currentFileName = [self _getFileName];
    }
    return _currentFileName;
}

- (NSURL *)currentFileUrl
{
    return self.audioRecorder.url;
}

- (NSString *)_getFileName
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy_MM_dd_HH_mm_ss"];
    NSString *dateString = [dateFormatter stringFromDate:[NSDate date]];
    
    return [NSString stringWithFormat:@"%@%@", dateString, [self _getAudioFormat]];
}

- (NSString *)_getAudioFormat
{
    switch (self.configuration.audioFormat) {
            
        case CHRecordAudioFormatPCM:
        {
            return @".pcm";
        }
            break;
        case CHRecordAudioFormatMP3:
        {
            return @".mp3";
        }
            break;
        case CHRecordAudioFormatWAV:
        {
            return @".wav";
        }
            break;
        case CHRecordAudioFormatAAC:
        {
            return @".aac";
        }
            break;
            
        default:
            return @".pcm";
            break;
    }
}


#pragma mark - - 录音设置相关

- (NSDictionary *)_getAudioSetting
{
    NSMutableDictionary *dicM=[NSMutableDictionary dictionary];
    
    NSNumber *formatID = [NSNumber numberWithInteger:[self _getAudioFormatID]];
    
    [dicM setObject:formatID forKey:AVFormatIDKey];
    [dicM setObject:[NSNumber numberWithInteger:self.configuration.sampleRate] forKey:AVSampleRateKey];
    [dicM setObject:[NSNumber numberWithInteger:self.configuration.numberOfChannels] forKey:AVNumberOfChannelsKey];
    [dicM setObject:[NSNumber numberWithInteger:self.configuration.linearPCMBitDepth] forKey:AVLinearPCMBitDepthKey];
    [dicM setObject:[NSNumber numberWithBool:self.configuration.linearPCMIsFloat] forKey:AVLinearPCMIsFloatKey];

    return dicM;
}

- (AudioFormatID)_getAudioFormatID
{
    switch (self.configuration.audioFormat) {
            
        case CHRecordAudioFormatMP3:
        {
            return kAudioFormatLinearPCM;
        }
            break;
        case CHRecordAudioFormatWAV:
        {
            return kAudioFormatLinearPCM;
        }
            break;
        case CHRecordAudioFormatAAC:
        {
            return kAudioFormatLinearPCM;
        }
            break;
        case CHRecordAudioFormatPCM:
        {
            return kAudioFormatLinearPCM;
        }
            break;
            
        default:
            return kAudioFormatLinearPCM;
            break;
    }
}

#pragma mark - - 私有方法

- (void)_startTimer
{
    [self _endTimer];
    self.timer = [NSTimer scheduledTimerWithTimeInterval:self.timeCallInterval target:self selector:@selector(_recordTimeCall) userInfo:nil repeats:YES];
}

- (void)_endTimer
{
    [self.timer invalidate];
    [self setTimer:nil];
}

- (void)_recordTimeCall
{
    if (!self.isRecording) return;
    if (self.progressBlock) {
        self.progressBlock(_audioRecorder.currentTime);
    }
}

-(void)_setAudioSession
{
    AVAudioSession *audioSession=[AVAudioSession sharedInstance];
    [audioSession setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
    [audioSession setActive:YES error:nil];
}

- (void)_restorationAudioSessionSetting
{
    AVAudioSession *audioSession=[AVAudioSession sharedInstance];
    [audioSession setCategory:AVAudioSessionCategoryPlayback error:nil];
    [audioSession setActive:YES error:nil];
}


@end
