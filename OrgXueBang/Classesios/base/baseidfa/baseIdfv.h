//
//  baseIdfv.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/29.
//
//

#ifndef baseIdfv_h
#define baseIdfv_h
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface baseIdfv : NSObject

+ (void)save:(NSString *)service data:(id)data;

+ (id)load:(NSString *)service;

+ (void)delete:(NSString *)service;

+ (NSString *)getIDFV;

@end

#endif /* baseIdfv_h */
