//
//  AppraiseManager.h
//  SDKSample
//
//  Created by Jeason on 16/07/2015.
//
//

#import <Foundation/Foundation.h>
@interface AppraiseManager : NSObject<UIAlertViewDelegate> {
    float m_time;
    UIAlertView *m_alert;
    NSString* m_url;
    NSString* m_file;
}

+ (instancetype)sharedManager;
- (void)initAppraise;
- (void)checkAppraise;
- (void)showAlert;
- (void)hideAlert;
- (void)openUrl;
@end
