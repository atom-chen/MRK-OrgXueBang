//
//  SPRecorder.m
//  ReadingFriends
//
//  Created by liuwei on 16/9/27.
//
//

#import "SPRecorder.h"
#import "RecorderManager.h"
#import "PlayerManager.h"


@interface SPRecorder () <RecordingDelegate, PlayingDelegate>
@end


@implementation SPRecorder

- (BOOL)startRecord: (NSString *) fileName {
    if (self.isPlaying) {
        return false;
    }
    
    if (!self.isRecording) {
        self.isRecording = YES;
        [RecorderManager sharedManager].delegate = self;
        [[RecorderManager sharedManager] startRecording: fileName];
        if (self.startBlock) {
            self.startBlock();
        }
        return true;
    }
    return false;
}


- (BOOL)endRecord {
    if (self.isPlaying) {
        return false;
    }
    
    if (self.isRecording)
    {
        self.isRecording = NO;
        [[RecorderManager sharedManager] stopRecording];
        return true;
    }
    return false;
}


- (BOOL)cancelRecord {
    if (self.isPlaying) {
        return false;
    }
    
    if (self.isRecording)
    {
        self.isRecording = NO;
        [[RecorderManager sharedManager] cancelRecording];
        if (self.cancelBlock) {
            self.cancelBlock();
        }
        return true;
    }
    return false;
}



- (BOOL)startPlaying {
    if (self.isRecording) {
        return false;
    }
    
    if (!self.isPlaying) {
        [PlayerManager sharedManager].delegate = nil;
        self.isPlaying = YES;
        [[PlayerManager sharedManager] playAudioWithFileName:self.filename delegate:self];
        return true;
    }
    
    return false;
}

/* 播放录音 */
- (BOOL)startPlaying:(NSString *)filename{
    if (self.isRecording) {
        return false;
    }
    
    if (!self.isPlaying) {
        [PlayerManager sharedManager].delegate = nil;
        self.isPlaying = YES;
        [[PlayerManager sharedManager] playAudioWithFileName:filename delegate:self];
        return true;
    }
    
    return false;
}


- (BOOL)stopPlaying {
    if (self.isRecording) {
        return false;
    }
    
    if (self.isPlaying)
    {
        self.isPlaying = NO;
        [[PlayerManager sharedManager] stopPlaying];
        return true;
    }
    return false;
}

//by yy
- (NSInteger)getRecordDuration:(NSURL *)filename
{
    return [[PlayerManager sharedManager] getRecordDuration:filename];
}

#pragma mark - Recording & Playing Delegate

- (void)recordingFinishedWithFileName:(NSString *)filePath time:(NSTimeInterval)interval {
    self.isRecording = NO;
    self.filename = filePath;
    
    if (self.finishBlock) {
        NSURL* url = [NSURL URLWithString:filePath];
        self.finishBlock(url);
    }
    
    //liuwei
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
    [[AVAudioSession sharedInstance] setActive:YES error:nil];
}

- (void)recordingTimeout {
    self.isRecording = NO;
}

- (void)recordingStopped {
    self.isRecording = NO;
}

- (void)recordingFailed:(NSString *)failureInfoString {
    self.isRecording = NO;
}

- (void)levelMeterChanged:(float)levelMeter {
}

- (void)playingStoped {
    self.isPlaying = NO;
    if (self.playOver) {
        self.playOver();
    }
}




#pragma mark - call Blcoks
- (void)setStartRecordCallBlcok:(SPRecorderBlcok)block
{
    self.startBlock = block;
}

- (void)setRecordTimeInterval:(CGFloat)interval andCallBlcok:(SPRecorderProgressBlock)block
{
    self.timeCallInterval = interval;
    self.progressBlock = block;//TODO: 没有处理
}

- (void)setEndRecordCallBlcok:(SPRecorderFinishBlcok)block
{
    self.finishBlock = block;
}

- (void)setEndPlayCallBlcok:(SPRecorderPlayOver)block
{
    self.playOver = block;
}

- (void)setCancelRecordBlcok:(SPRecorderCancelBlock)block
{
    self.cancelBlock = block;
}

@end
