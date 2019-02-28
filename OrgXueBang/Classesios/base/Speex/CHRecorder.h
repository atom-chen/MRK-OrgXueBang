//
//  CHRecorder.h
//  CHRecordDemo
//
//  Created by Summer on 16/5/23.
//  Copyright © 2016年 Summer. All rights reserved.
//  录音机模块


#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

/** 回调闭包 **/
typedef void (^CHRecorderBlcok)();
typedef void (^CHRecorderPauseBlcok)(BOOL isPause);
typedef void (^CHRecorderProgressBlock)(CGFloat currentTime);
typedef void (^CHRecorderFinishBlcok)(NSURL *fileUrl);
typedef void (^CHRecorderPlayOver)();

/** 录音格式 **/
typedef NS_ENUM(NSInteger, CHRecordAudioFormat) {
    CHRecordAudioFormatMP3 = 1,
    CHRecordAudioFormatWAV = 2,
    CHRecordAudioFormatPCM = kAudioFormatLinearPCM,
    CHRecordAudioFormatAAC = kAudioFormatMPEG4AAC
};

/**
 kAudioFormatLinearPCM
 kAudioFormatMPEG4AAC
 kAudioFormatAppleLossless
 kAudioFormatAppleMA4
 kAudioFormatiLBC
 kAudioFormatULaw
 */


/** 录音配置 **/
@interface CHRecorderConfiguration : NSObject

/** 默认配置 **/
+ (instancetype)defaultConfiguration;

/** 录音格式 参考 `CHRecordAudioFormat` default: WAV **/
@property (nonatomic, assign, readwrite) CHRecordAudioFormat audioFormat;
/** 采样率 default: 8000 **/
@property (nonatomic, assign, readwrite) NSInteger sampleRate;
/** 设置通道 default: 1 **/
@property (nonatomic, assign, readwrite) NSInteger numberOfChannels;
/** 采样点位数 default: 16bit **/
@property (nonatomic, assign, readwrite) NSInteger linearPCMBitDepth;
/** 录音完毕是否自动播放 default: NO **/
@property (nonatomic, assign, getter=isAutoPlay) BOOL autoPlay;
/** 是否使用浮点数采样 default: YES **/
@property (nonatomic, assign, getter=islinearPCMIsFloat) BOOL linearPCMIsFloat;
/** 临时文件保存路径 default:  Cache文件夹 **/
@property (nonatomic, copy, readwrite) NSString *tempFilePath;

@end


/** 录音机 **/
@interface CHRecorder : NSObject

/** 单利方法 **/
+ (instancetype)shareRecorder;


/** 录音配置 **/
@property (nonatomic, strong) CHRecorderConfiguration *configuration;
/** 当前录制时间 **/
@property (nonatomic, assign) CGFloat currentDuration;
/** 是否正在录音中 **/
@property (nonatomic, assign, readonly, getter=isRecording) BOOL recording;
/** 是否是手动暂停 **/
@property (nonatomic, assign, readonly, getter=isPause) BOOL pause;
/** 当前录制文件保存路径 **/
@property (nonatomic, strong, readonly) NSURL *currentFileUrl;


/** 准备录音 - 不是必须调用 - 调用此方法会在目录下生成录音空文件 **/
- (void)prepareToRecord;

/** 开始录音 **/
- (bool)startRecord:(NSString *) fileName;
/** 开始录音 - 并且设置文件名称 */
- (void)startRecordWithFileName:(NSString *)fileName;
/** 开始录音 - 并且设置文件存放路径 */
- (void)startRecordWithFliePath:(NSString *)filePath;
/** 开始录音 - 并且设置文件名称和存放路径 */
- (void)startRecordWithFileName:(NSString *)fileName andFliePath:(NSString *)filePath;
/** 开始录音 - 在n秒之后开始录音 **/
- (void)startRecordAtTime:(NSTimeInterval)time;
/** 开始录音 - 设置录制时间 - 在n秒后停止  **/
- (void)startRecordForDuration:(NSTimeInterval) duration;
/** 开始录音 - 在n秒后开始 - 在n秒后停止 */
- (void)startRecordAtTime:(NSTimeInterval)time forDuration:(NSTimeInterval) duration;

/** 暂停录音 **/
- (void)pauseRecord;
/** 继续录音 **/
- (void)resumeRecord;
/** 结束录音 **/
- (void)endRecord;
/** 取消录音 **/
- (void)cancelRecord;


/** 设置开始录音回调 **/
- (void)setStartRecordCallBlcok:(CHRecorderBlcok)block;
/** 设置录音中进度回调 **/
- (void)setRecordTimeInterval:(CGFloat)interval andCallBlcok:(CHRecorderProgressBlock)block;
/** 设置暂停录音/继续录音回调 **/
- (void)setPauseOrResumeRecordCallBlcok:(CHRecorderPauseBlcok)block;
/** 设置录音完成回调 **/
- (void)setEndRecordCallBlcok:(CHRecorderFinishBlcok)block;
/** 设置播放完成回调 **/
- (void)setEndPlayCallBlcok:(CHRecorderPlayOver)block;
/** 设置取消录音回调 **/
- (void)setCancelRecordCallBlock:(CHRecorderBlcok)block;


@end
