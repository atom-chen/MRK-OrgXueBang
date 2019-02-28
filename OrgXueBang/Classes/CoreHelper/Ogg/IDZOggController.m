#import "IDZOggController.h"
#import "IDZOggVorbisFileDecoder.h"



@interface IDZOggController ()
@property (nonatomic, strong) id<IDZAudioPlayer> player;
@end




@implementation IDZOggController
@synthesize player = mPlayer;


#pragma mark - Actions


- (void)play:(NSString *)filePath start:(NSInteger)startTag total:(NSInteger)totalTag
{
    NSError* error = nil;
    NSURL *oggUrl = [NSURL fileURLWithPath:filePath];
    IDZOggVorbisFileDecoder* decoder = [[IDZOggVorbisFileDecoder alloc] initWithContentsOfURL:oggUrl error:&error];
    self.player = [[IDZAQAudioPlayer alloc] initWithDecoder:decoder error:nil];
    self.player.delegate = self;
    
    [self.player setCurrentTime:(startTag/100.0f)];
    [self.player prepareToPlay];
    self.totalSec = totalTag / 100.0f;
    NSLog(@"curTme = %f  ,  %f",[self.player currentTime],self.totalSec);
    if(self.totalSec > 0.0f)
    {
        self.bPartTime = true;
    }
    else
    {
        self.bPartTime = false;
    }
    [self.player play];
}



- (void)pause
{
    [self.player pause];
}



- (void)stop
{
    [self.player stop];
    self.player.currentTime = 0;
}



- (BOOL)isStop
{
    return [self.player isStop];
}

- (void)resume
{
    [self.player play];
}

#pragma mark - IDZAudioPlayerDelegate
- (void)audioPlayerDidFinishPlaying:(id<IDZAudioPlayer>)player successfully:(BOOL)flag
{
    if (self.finishCallBack) {
        self.finishCallBack(self.oggId, self.oggName);
    }
}



- (void)audioPlayerDecodeErrorDidOccur:(id<IDZAudioPlayer>)player error:(NSError *)error
{
}

- (void)setPitch:(float)pitch
{
    [self.player setPitch:pitch];
}

@end
