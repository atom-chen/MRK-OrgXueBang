//
//  SPRecorder.h
//  ReadingFriends
//
//  Created by liuwei on 16/9/27.
//
//

#ifndef SPRecorder_h
#define SPRecorder_h

#import <UIKit/UIKit.h>

typedef void (^SPRecorderBlcok)();
typedef void (^SPRecorderProgressBlock)(CGFloat currentTime);
typedef void (^SPRecorderFinishBlcok)(NSURL *fileUrl);
typedef void (^SPRecorderPlayOver)();
typedef void (^SPRecorderCancelBlock)();


/* spx录音 */
@interface SPRecorder : NSObject

@property (nonatomic, assign) BOOL isRecording;
@property (nonatomic, assign) BOOL isPlaying;
@property (nonatomic, copy) NSString *filename;


/* 开始录音 */
- (BOOL)startRecord:(NSString *)fileName;
/* 结束录音 */
- (BOOL)endRecord;
/* 取消录音 */
- (BOOL)cancelRecord;


/* 播放录音 */
- (BOOL)startPlaying;
/* 播放录音 */
- (BOOL)startPlaying:(NSString *)filename;
/* 停止播放录音 */
- (BOOL)stopPlaying;
/* 取得录音资源总时长by yy*/
- (NSInteger)getRecordDuration:(NSURL *)filename;

/* 录音回调函数 */
@property (nonatomic, copy) SPRecorderBlcok startBlock;
@property (nonatomic, copy) SPRecorderProgressBlock progressBlock;
@property (nonatomic, copy) SPRecorderFinishBlcok finishBlock;
@property (nonatomic, copy) SPRecorderPlayOver playOver;
@property (nonatomic, copy) SPRecorderCancelBlock cancelBlock;
@property (nonatomic, assign) CGFloat timeCallInterval;


/** 设置开始录音回调 **/
- (void)setStartRecordCallBlcok:(SPRecorderBlcok)block;
/** 设置录音中进度回调 **/
- (void)setRecordTimeInterval:(CGFloat)interval andCallBlcok:(SPRecorderProgressBlock)block;
/** 设置录音完成回调 **/
- (void)setEndRecordCallBlcok:(SPRecorderFinishBlcok)block;
/** 设置播放完成回调 **/
- (void)setEndPlayCallBlcok:(SPRecorderPlayOver)block;
/** 设置取消录音回调 **/
- (void)setCancelRecordBlcok:(SPRecorderCancelBlock)block;

@end


#endif /* SPRecorder_h */
