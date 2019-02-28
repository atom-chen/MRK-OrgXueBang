//
//  AppraiseManager.m
//  SDKSample
//
//  Created by Jeason on 16/07/2015.
//
//

#import "AppraiseManager.h"
#import "UtilsDefine.h"
@implementation AppraiseManager

#pragma mark - LifeCycle
+(instancetype)sharedManager {
    static dispatch_once_t onceToken;
    static AppraiseManager *instance;
    dispatch_once(&onceToken, ^{
        instance = [[[AppraiseManager alloc] init]autorelease];
        [instance retain];
        [instance initAppraise];
    });
    return instance;
}
- (void)dealloc {
    [m_url release];
    [m_file release];
    [super dealloc];
}
- (void)initAppraise
{
    //从文件中读出配置数据
    stuJson jsonData = readJsonData("config/ConfigData.json");
    auto appraiseData  = jsonData.getJson("AppraiseData");
    m_time = [[NSString stringWithUTF8String:appraiseData.getValue("Interval").c_str()] floatValue];
    m_url = [[NSString stringWithUTF8String:appraiseData.getValue("Url").c_str()] retain];
    
    //获取应用程序沙盒的Documents目录  写默认文件 //得到完整的文件名
    NSArray *paths=NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES);
    NSString *plistPath1 = [paths objectAtIndex:0];
    m_file =[[plistPath1 stringByAppendingPathComponent:@"ConfigData.plist"] retain];
    NSMutableDictionary *configData = [[NSMutableDictionary alloc] initWithContentsOfFile:m_file];
    if (configData==NULL) {
        configData= [[NSMutableDictionary alloc]init];
    }
    NSMutableDictionary * appraise = [configData objectForKey:@"AppraiseData"];
    if (appraise==NULL) {
        appraise= [[NSMutableDictionary alloc]init];
        NSTimeInterval nowTime =  [[NSDate date] timeIntervalSince1970 ];
        [appraise setObject:[NSString stringWithFormat:@"%lf",nowTime ] forKey:@"SignTime"];
        [appraise setObject:@"false" forKey:@"HaveAppraise"];
        [configData setObject:appraise forKey:@"AppraiseData"];
        [configData writeToFile:m_file atomically:YES];
    }
}
- (void)checkAppraise
{
    NSMutableDictionary *configData = [[NSMutableDictionary alloc] initWithContentsOfFile:m_file];
    NSMutableDictionary *appraise = [configData objectForKey:@"AppraiseData"];
    NSString *haveStr =  [appraise objectForKey:@"HaveAppraise"];
    //已经评价过了
    if ([haveStr isEqualToString:@"true"]|| [haveStr isEqualToString:@"refuse"]) {
//        return;
    }
    NSTimeInterval nowTime =  [[NSDate date] timeIntervalSince1970 ];
    //判断是否符合条件
    float signTime =  [[appraise objectForKey:@"SignTime"]  floatValue];
    if (nowTime-signTime<m_time*24*60*60) {
//        return;
    }
    //展示提示面板
    [self showAlert];
}
- (void)showAlert
{
    //从文件中读出配置数据
    stuJson  jsonData = readJsonData("config/ConfigData.json");
    auto appraiseData  = jsonData.getJson("AppraiseData");
    NSString *title = [NSString stringWithUTF8String:appraiseData.getValue("Title").c_str()];
    NSString *message = [NSString stringWithUTF8String:appraiseData.getValue("Message").c_str()];
    NSString *cancel = [NSString stringWithUTF8String:appraiseData.getValue("Cancel").c_str()];
    NSString *button1 = [NSString stringWithUTF8String:appraiseData.getValue("Button1").c_str()];
//    NSString *button2 = [NSString stringWithUTF8String:appraiseData.getValue("Button2").c_str()];
    
    //展示评价面板
    m_alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:cancel otherButtonTitles:button1,nil];
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
- (void) openUrl
{
    NSURL *url = [[ NSURL alloc ] initWithString:m_url];
    [[UIApplication sharedApplication] openURL:url];
}
#pragma marks -- UIAlertViewDelegate --
//根据被点击按钮的索引处理点击事件
-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    NSMutableDictionary *configData = [[NSMutableDictionary alloc] initWithContentsOfFile:m_file];
    NSMutableDictionary *appraise = [configData objectForKey:@"AppraiseData"];
    switch (buttonIndex) {
        case 0:
        {
            NSTimeInterval nowTime =  [[NSDate date] timeIntervalSince1970 ];
            [appraise setObject:[NSString stringWithFormat:@"%lf",nowTime ] forKey:@"SignTime"];
        }
            break;
        case 1:
        {
            [appraise setObject:@"true" forKey:@"HaveAppraise"];
            __NotificationCenter::getInstance()->postNotification("checkEvaluate", (Ref*)(intptr_t)YES);
            [self  openUrl];
        }
            break;
        case 2:
        {
            [appraise setObject:@"refuse" forKey:@"HaveAppraise"];
//            NSTimeInterval nowTime =  [[NSDate date] timeIntervalSince1970 ];
//            [appraise setObject:[NSString stringWithFormat:@"%lf",nowTime ] forKey:@"SignTime"];
            
        }
            break;
        default:
            break;
    }
    [configData writeToFile:m_file atomically:YES];
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
