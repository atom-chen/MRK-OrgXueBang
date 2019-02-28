#import <UIKit/UIKit.h>
#import "IDZAQAudioPlayer.h"

typedef void (^OggFinishCallBack)(int iId, NSString* fileName);


@interface IDZOggController : NSObject<IDZAudioPlayerDelegate>


- (void)play:(NSString *)filePath start:(NSInteger)startTag total:(NSInteger)totalTag;

- (void)pause;

- (void)stop;

- (BOOL)isStop;

- (void)resume;

- (void)setPitch: (float)pitch;

@property (nonatomic, copy) OggFinishCallBack finishCallBack;

@property (nonatomic, copy) NSString *oggName;

@property (nonatomic, assign) int oggId;

@property (nonatomic, assign) float totalSec;
@property (nonatomic, assign) bool bPartTime;   //分段播放，需要计时

@end
