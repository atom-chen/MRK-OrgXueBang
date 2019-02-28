//
//  AuthorizeManager.h
//  SDKSample
//
//  Created by Jeason on 16/07/2015.
//
//

#import <Foundation/Foundation.h>
@interface AuthorizeManager : NSObject<UIAlertViewDelegate> {
    UIAlertView *m_alert;
    NSString* _url;
}

+ (instancetype)sharedManager;
- (bool)checkPhoto;
- (bool)requestPhoto;
- (bool)checkMicrophone;
- (bool)checkPush;
- (bool)checkNetwork;
- (void)showAlert:(NSString*) key;
- (void)hideAlert;
@property(nonatomic,retain,readwrite) NSString* url;
@end
