//
//  AuthorizeManager.m
//  SDKSample
//
//  Created by Jeason on 16/07/2015.
//
//

#import "AuthorizeManager.h"
#import "UtilsDefine.h"
#import <AssetsLibrary/AssetsLibrary.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreTelephony/CTCellularData.h>
//#import <UserNotifications/UserNotifications.h>
#import <Photos/Photos.h>

@implementation AuthorizeManager
@synthesize url = _url;
#pragma mark - LifeCycle
+(instancetype)sharedManager {
    static dispatch_once_t onceToken;
    static AuthorizeManager *instance;
    dispatch_once(&onceToken, ^{
        instance = [[[AuthorizeManager alloc] init]autorelease];
        [instance retain];
    });
    return instance;
}
- (void)dealloc {
    [_url release];
    [super dealloc];
}

- (bool)checkPhoto
{
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
    PHAuthorizationStatus authStatus = [PHPhotoLibrary authorizationStatus];
    if (authStatus == PHAuthorizationStatusNotDetermined)
    {
        [self requestPhoto];
        return NO;
    }
    if (authStatus == PHAuthorizationStatusRestricted || authStatus == PHAuthorizationStatusDenied) {
        
        [self showAlert:@"Photo"];
        return NO;
    }
#else
    ALAuthorizationStatus author = [ALAssetsLibrary authorizationStatus];
    if (authStatus == ALAuthorizationStatusNotDetermined)
    {
        [self requestPhoto];
        return NO;
    }
    if (author == ALAuthorizationStatusRestricted || author == ALAuthorizationStatusDenied) {
        [self showAlert:@"Photo"];
        return NO;
    }
#endif
    return YES;
}
- (bool)requestPhoto
{
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
    [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status){
        dispatch_async(dispatch_get_main_queue(), ^{
            switch (status) {
                case PHAuthorizationStatusAuthorized:
                {
                    NSLog(@"PHAuthorizationStatusAuthorized  !\n");
                    break;
                }
                default:
                {
                    NSLog(@"PHAuthorizationStatusAuthorized fail !\n");
                    break;
                }
            }
        });
    }];
#else
    [self showAlert:@"Photo"];
#endif
    return NO;
}

- (bool)checkMicrophone
{
//麦克风
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
    AVAudioSessionRecordPermission permissionStatus = [[AVAudioSession sharedInstance] recordPermission];
    if (permissionStatus == AVAudioSessionRecordPermissionUndetermined) {
        [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL granted) {
             NSLog(@">>>requestRecordPermission!\n");
        }];
        return NO;
    } else if (permissionStatus == AVAudioSessionRecordPermissionDenied) {
        [self showAlert:@"Microphone"];
        return NO;
    } else {
        return YES;
    }
#endif
    return YES;
}

- (bool)checkPush
{
    BOOL isOpen = NO;
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_8_0
    UIUserNotificationSettings *setting = [[UIApplication sharedApplication] currentUserNotificationSettings];
    if (setting.types != UIUserNotificationTypeNone) {
        isOpen = YES;
    }
#else
    UIRemoteNotificationType type = [[UIApplication sharedApplication] enabledRemoteNotificationTypes];
    if (type != UIRemoteNotificationTypeNone) {
        isOpen = YES;
    }
#endif
    if (isOpen == NO) {
        [self showAlert:@"Push"];
    }
    return isOpen;
}
- (bool)checkNetwork
{
    BOOL isOpen = NO;
    CTCellularData *cellularData = [[CTCellularData alloc]init];
    CTCellularDataRestrictedState state = cellularData.restrictedState;
    switch (state) {
        case kCTCellularDataRestricted:
            NSLog(@"Restricrted");
            [self showAlert:@"Network"];
            break;
        case kCTCellularDataNotRestricted:
            NSLog(@"Not Restricted");
            isOpen = YES;
            break;
        case kCTCellularDataRestrictedStateUnknown:
            NSLog(@"Unknown");
        {
            cellularData.cellularDataRestrictionDidUpdateNotifier =  ^(CTCellularDataRestrictedState state){
                //获取联网状态
                
            };
        }
            break;
        default:
            break;
    }
    return isOpen;
}

- (void)showAlert:(NSString*) key
{
    //从文件中读出配置数据
    stuJson  jsonData = readJsonData("config/ConfigData.json");
    auto appraiseData  = jsonData.getJson([key UTF8String]);
    NSString *title = [NSString stringWithUTF8String:appraiseData.getValue("Title").c_str()];
    NSString *message = [NSString stringWithUTF8String:appraiseData.getValue("Message").c_str()];
    NSString *cancel = [NSString stringWithUTF8String:appraiseData.getValue("Cancel").c_str()];
    NSString *sure = [NSString stringWithUTF8String:appraiseData.getValue("Sure").c_str()];
    NSString *ok = [NSString stringWithUTF8String:appraiseData.getValue("OK").c_str()];
    [self setUrl: [NSString stringWithUTF8String:appraiseData.getValue("Url").c_str()]];
//    NSURL *url = [[ NSURL alloc ] initWithString: [self url]];
    NSURL *url =[NSURL URLWithString:UIApplicationOpenSettingsURLString];
    bool canOpen = [[UIApplication sharedApplication] canOpenURL:url];
    //展示评价面板
    m_alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:canOpen?cancel:ok otherButtonTitles:canOpen?sure:nil,nil];
    UIActivityIndicatorView *activeView = [[[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge] autorelease];
    [activeView startAnimating];
    [m_alert addSubview:activeView];
    [m_alert show];
    activeView.center = CGPointMake(m_alert.bounds.size.width * 0.5f, m_alert.bounds.size.height - 40.0f);
}

//关闭评价面板
-(void) hideAlert{
    if (m_alert != nil) {
        [m_alert dismissWithClickedButtonIndex:0 animated:NO];
        [m_alert release];
        m_alert = nil;
    }
}
#pragma marks -- UIAlertViewDelegate --
//根据被点击按钮的索引处理点击事件
-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    switch (buttonIndex) {
        case 0:
        {
            
        }
            break;
        case 1:
        {
            if( [self url]!=nullptr){
                [[UIApplication sharedApplication] openURL: [NSURL URLWithString:UIApplicationOpenSettingsURLString]];
            }
           
//            NSURL *url = [[ NSURL alloc ] initWithString: [self url]];
//            if ([[UIApplication sharedApplication] canOpenURL:url])
//            {
//                [[UIApplication sharedApplication] openURL:url];
//            }
            
            
        }
            break;
        case 2:
        {
        
        }
            break;
        default:
            break;
    }
    //关闭提示面板
    [self hideAlert];
}

//AlertView已经消失时执行的事件
-(void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    NSLog(@"didDismissWithButtonIndex");
}

//ALertView即将消失时的事件
-(void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    NSLog(@"willDismissWithButtonIndex");
}

//AlertView的取消按钮的事件
-(void)alertViewCancel:(UIAlertView *)alertView
{
    NSLog(@"alertViewCancel");
}

//AlertView已经显示时的事件
-(void)didPresentAlertView:(UIAlertView *)alertView
{
    NSLog(@"didPresentAlertView");
}

//AlertView即将显示时
-(void)willPresentAlertView:(UIAlertView *)alertView
{
    NSLog(@"willPresentAlertView");
}


@end
