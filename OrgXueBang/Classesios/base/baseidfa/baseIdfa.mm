//
//  Recorder.cpp
//  ReadingFriends
//
//  Created by Summer on 16/7/20.
//
//

#include "baseIdfa.h"
#import <AdSupport/AdSupport.h>
#import <CoreTelephony/CTCarrier.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import "sys/utsname.h"
#import <regex>

#import "baseIdfv.h"
#import "KeychainItemWrapper.h"

BOOL isBreakOutPrison()
{
#if TARGET_IPHONE_SIMULATOR
    return NO;
#endif
    // 常见越狱文件
    const char *examineBreak_Tool_pathes[] = {
        "/Applications/Cydia.app",
        "/Library/MobileSubstrate/MobileSubstrate.dylib",
        "/bin/bash",
        "/usr/sbin/sshd",
        "/etc/apt"
    };
    // 方式1.判断是否存在越狱文件
    for (int i = 0; i < 5; i++) {
        if ([[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:examineBreak_Tool_pathes[i]]]){
            return YES;
        }
    }
    // 方式2.判断是否存在cydia应用
//    if([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"cydia://"]]){
//        return YES;
//    }
    
    // 方式3.读取系统所有的应用名称
    if ([[NSFileManager defaultManager] fileExistsAtPath:@"/User/Applications/"]){
        return YES;
    }
    // 方式4.读取环境变量
    char *env = getenv("DYLD_INSERT_LIBRARIES");
    if(env){
        return YES;
    }
    return NO;
}

static NSString* deviceModelName(NSString* deviceModel)
{
    if ([deviceModel isEqualToString:@"iPhone3,1"])    return @"iPhone 4";
    if ([deviceModel isEqualToString:@"iPhone3,2"])    return @"iPhone 4";
    if ([deviceModel isEqualToString:@"iPhone3,3"])    return @"iPhone 4";
    if ([deviceModel isEqualToString:@"iPhone4,1"])    return @"iPhone 4S";
    if ([deviceModel isEqualToString:@"iPhone5,1"])    return @"iPhone 5";
    if ([deviceModel isEqualToString:@"iPhone5,2"])    return @"iPhone 5 (GSM+CDMA)";
    if ([deviceModel isEqualToString:@"iPhone5,3"])    return @"iPhone 5c (GSM)";
    if ([deviceModel isEqualToString:@"iPhone5,4"])    return @"iPhone 5c (GSM+CDMA)";
    if ([deviceModel isEqualToString:@"iPhone6,1"])    return @"iPhone 5s (GSM)";
    if ([deviceModel isEqualToString:@"iPhone6,2"])    return @"iPhone 5s (GSM+CDMA)";
    if ([deviceModel isEqualToString:@"iPhone7,1"])    return @"iPhone 6 Plus";
    if ([deviceModel isEqualToString:@"iPhone7,2"])    return @"iPhone 6";
    if ([deviceModel isEqualToString:@"iPhone8,1"])    return @"iPhone 6s";
    if ([deviceModel isEqualToString:@"iPhone8,2"])    return @"iPhone 6s Plus";
    if ([deviceModel isEqualToString:@"iPhone8,4"])    return @"iPhone SE";
    // 日行两款手机型号均为日本独占，可能使用索尼FeliCa支付方案而不是苹果支付
    if ([deviceModel isEqualToString:@"iPhone9,1"])    return @"国行、日版、港行iPhone 7";
    if ([deviceModel isEqualToString:@"iPhone9,2"])    return @"港行、国行iPhone 7 Plus";
    if ([deviceModel isEqualToString:@"iPhone9,3"])    return @"美版、台版iPhone 7";
    if ([deviceModel isEqualToString:@"iPhone9,4"])    return @"美版、台版iPhone 7 Plus";
    if ([deviceModel isEqualToString:@"iPhone10,1"])   return @"iPhone_8";
    if ([deviceModel isEqualToString:@"iPhone10,4"])   return @"iPhone_8";
    if ([deviceModel isEqualToString:@"iPhone10,2"])   return @"iPhone_8_Plus";
    if ([deviceModel isEqualToString:@"iPhone10,5"])   return @"iPhone_8_Plus";
    if ([deviceModel isEqualToString:@"iPhone10,3"])   return @"iPhone_X";
    if ([deviceModel isEqualToString:@"iPhone10,6"])   return @"iPhone_X";
    if ([deviceModel isEqualToString:@"iPhone11,2"])   return @"iPhone XS";
    if ([deviceModel isEqualToString:@"iPhone11,4"])   return @"iPhone XS Max";
    if ([deviceModel isEqualToString:@"iPhone11,6"])   return @"iPhone XS Max";
    if ([deviceModel isEqualToString:@"iPhone11,8"])   return @"iPhone XR";
    
    if ([deviceModel isEqualToString:@"iPod1,1"])      return @"iPod Touch 1G";
    if ([deviceModel isEqualToString:@"iPod2,1"])      return @"iPod Touch 2G";
    if ([deviceModel isEqualToString:@"iPod3,1"])      return @"iPod Touch 3G";
    if ([deviceModel isEqualToString:@"iPod4,1"])      return @"iPod Touch 4G";
    if ([deviceModel isEqualToString:@"iPod5,1"])      return @"iPod Touch (5 Gen)";
    if ([deviceModel isEqualToString:@"iPad1,1"])      return @"iPad";
    if ([deviceModel isEqualToString:@"iPad1,2"])      return @"iPad 3G";
    if ([deviceModel isEqualToString:@"iPad2,1"])      return @"iPad 2 (WiFi)";
    if ([deviceModel isEqualToString:@"iPad2,2"])      return @"iPad 2";
    if ([deviceModel isEqualToString:@"iPad2,3"])      return @"iPad 2 (CDMA)";
    if ([deviceModel isEqualToString:@"iPad2,4"])      return @"iPad 2";
    if ([deviceModel isEqualToString:@"iPad2,5"])      return @"iPad Mini (WiFi)";
    if ([deviceModel isEqualToString:@"iPad2,6"])      return @"iPad Mini";
    if ([deviceModel isEqualToString:@"iPad2,7"])      return @"iPad Mini (GSM+CDMA)";
    if ([deviceModel isEqualToString:@"iPad3,1"])      return @"iPad 3 (WiFi)";
    if ([deviceModel isEqualToString:@"iPad3,2"])      return @"iPad 3 (GSM+CDMA)";
    if ([deviceModel isEqualToString:@"iPad3,3"])      return @"iPad 3";
    if ([deviceModel isEqualToString:@"iPad3,4"])      return @"iPad 4 (WiFi)";
    if ([deviceModel isEqualToString:@"iPad3,5"])      return @"iPad 4";
    if ([deviceModel isEqualToString:@"iPad3,6"])      return @"iPad 4 (GSM+CDMA)";
    if ([deviceModel isEqualToString:@"iPad4,1"])      return @"iPad Air (WiFi)";
    if ([deviceModel isEqualToString:@"iPad4,2"])      return @"iPad Air (Cellular)";
    if ([deviceModel isEqualToString:@"iPad4,4"])      return @"iPad Mini 2 (WiFi)";
    if ([deviceModel isEqualToString:@"iPad4,5"])      return @"iPad Mini 2 (Cellular)";
    if ([deviceModel isEqualToString:@"iPad4,6"])      return @"iPad Mini 2";
    if ([deviceModel isEqualToString:@"iPad4,7"])      return @"iPad Mini 3";
    if ([deviceModel isEqualToString:@"iPad4,8"])      return @"iPad Mini 3";
    if ([deviceModel isEqualToString:@"iPad4,9"])      return @"iPad Mini 3";
    if ([deviceModel isEqualToString:@"iPad5,1"])      return @"iPad Mini 4 (WiFi)";
    if ([deviceModel isEqualToString:@"iPad5,2"])      return @"iPad Mini 4 (LTE)";
    if ([deviceModel isEqualToString:@"iPad5,3"])      return @"iPad Air 2";
    if ([deviceModel isEqualToString:@"iPad5,4"])      return @"iPad Air 2";
    if ([deviceModel isEqualToString:@"iPad6,3"])      return @"iPad Pro 9.7";
    if ([deviceModel isEqualToString:@"iPad6,4"])      return @"iPad Pro 9.7";
    if ([deviceModel isEqualToString:@"iPad6,7"])      return @"iPad Pro 12.9";
    if ([deviceModel isEqualToString:@"iPad6,8"])      return @"iPad Pro 12.9";
    
    if ([deviceModel isEqualToString:@"AppleTV2,1"])      return @"Apple TV 2";
    if ([deviceModel isEqualToString:@"AppleTV3,1"])      return @"Apple TV 3";
    if ([deviceModel isEqualToString:@"AppleTV3,2"])      return @"Apple TV 3";
    if ([deviceModel isEqualToString:@"AppleTV5,3"])      return @"Apple TV 4";
    
    if ([deviceModel isEqualToString:@"i386"])         return @"Simulator";
    if ([deviceModel isEqualToString:@"x86_64"])       return @"Simulator";
    
    return deviceModel;
}



CBaseIdfa* CBaseIdfa::m_pBashIdfa = nullptr;
CBaseIdfa::CBaseIdfa()
{
}

CBaseIdfa::~CBaseIdfa()
{
}

CBaseIdfa* CBaseIdfa::getInstance()
{
    if (m_pBashIdfa == nullptr)
    {
        m_pBashIdfa = new CBaseIdfa;
    }
    return m_pBashIdfa;
}


const char* CBaseIdfa::getIdfaStr()
{
    /** 初始化一个保存用户帐号的KeychainItemWrapper */
    KeychainItemWrapper *wrapper = [[KeychainItemWrapper alloc] initWithIdentifier:@"Account Number" accessGroup:nil];
//    [wrapper resetKeychainItem];
    //从keychain里取出帐号密码
    NSString *account = [wrapper objectForKey:(id)kSecAttrAccount];
    NSString *password = [wrapper objectForKey:(id)kSecValueData];
    [wrapper release];
    if (account != nil && account.length > 6 && password != nil && password.length > 6)
    {
        if ([password isEqualToString:@"metrics313@(^$T*&)"]) {
            return [account  cStringUsingEncoding:NSUTF8StringEncoding];
        }
    }
    
    {
        NSString *udid = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
        
        /** 初始化一个保存用户帐号的KeychainItemWrapper */
        KeychainItemWrapper *wrapper = [[KeychainItemWrapper alloc] initWithIdentifier:@"Account Number" accessGroup:nil];
        //keychain 保存帐号
        [wrapper setObject:udid forKey:(id)kSecAttrAccount];
        //keychain 保存密码
        [wrapper setObject:@"metrics313@(^$T*&)" forKey:(id)kSecValueData];
        
        [wrapper release];
        return [udid  cStringUsingEncoding:NSUTF8StringEncoding];
    }
    
    
    
//    NSString *adId = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
//    if ([adId isEqualToString:@"00000000-0000-0000-0000-000000000000"]) {
//        return getIdfvStr();
//    }
////    adId = [baseIdfv getIDFV];
//    return [adId UTF8String];
}

const char* CBaseIdfa::getIdfvStr()
{
    NSString *idfv = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    return [idfv UTF8String];
}

const char* CBaseIdfa::getSystemVersion()
{
    NSString *adId = [[UIDevice currentDevice] systemVersion];
    return [adId UTF8String];
}

const char* CBaseIdfa::getmodel()
{
//    NSString *adId = [UIDevice currentDevice].model;
    //return [adId UTF8String];
    
    
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString *deviceString = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    
    NSString* modelName = deviceModelName(deviceString);
    
    
    return [modelName UTF8String];
}


const char* CBaseIdfa::getversion()//"]//|否|版本|
{
    NSString *adId = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    return [adId UTF8String];
}

const char* CBaseIdfa::getcarrier()//""]//|否|运营商|
{
    CTTelephonyNetworkInfo *info = [[CTTelephonyNetworkInfo alloc] init];
    
    //获取运行商的名称
    CTCarrier *carrier = [info subscriberCellularProvider];
    NSString *mCarrier = [NSString stringWithFormat:@"%@",[carrier carrierName]];
    return [mCarrier UTF8String];
}

const char* CBaseIdfa::getplatform()//""]//|否|ios或者android|
{
    return "IOS";
}

const char* CBaseIdfa::getplatform_id()//""]//|否|分发平台id|
{
    return "101002";
//    return GetXmlString("appChannelID");
}

const char* CBaseIdfa::getbundle_id()//""]//|否|包id|
{
    NSString *identifier = [[NSBundle mainBundle] bundleIdentifier];
    return [identifier UTF8String];
}

const char* CBaseIdfa::getDeviceToken()
{
    if (m_strDeviceToken.length() == 0) {
        m_strDeviceToken = "";
    }
    return m_strDeviceToken.c_str();
}


void CBaseIdfa::setDeviceToken(std::string token)
{
    std::regex e ("<|>| ");
    m_strDeviceToken = std::regex_replace (token, e, "");
}

const char* CBaseIdfa::getDeviceInfo()
{
    if (m_strDeviceInfo.length() == 0) {
        makeDeviceInfo();
    }
    CCLOG("device info = %s", m_strDeviceInfo.c_str());
    return m_strDeviceInfo.c_str();
}

void CBaseIdfa::makeDeviceInfo()
{
    std::string jb = "0";
    if (isBreakOutPrison()) {
        jb = "1";
    }
    
    char buf[1024];
    sprintf(buf, "{\"device\" : \"%s\",\"system\" : \"%s\",\"model\" : \"%s\",\"version\" : \"%s\",\"carrier\" : \"%s\",\"platform\" : \"%s\",\"channel\" : \"%s\",\"jailbreak\" : \"%s\"}", getIdfaStr(), getSystemVersion(), getmodel(), getversion(), getcarrier(), getplatform(), getplatform_id(), jb.c_str());
    m_strDeviceInfo = buf;
}

#include <sys/sysctl.h>
#include <mach/mach.h>
float CBaseIdfa::availableMemory()
{
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn
    = host_statistics(mach_host_self(),
                      HOST_VM_INFO,
                      (host_info_t)&vmStats,
                      &infoCount);
    if(kernReturn != KERN_SUCCESS) {
        return 0;
    }
    float wire = ((vm_page_size * vmStats.wire_count) / 1024.0) / 1024.0;
    float act = ((vm_page_size * vmStats.active_count) / 1024.0) / 1024.0;
    float inact = ((vm_page_size * vmStats.inactive_count) / 1024.0) / 1024.0;
    float free = ((vm_page_size * vmStats.free_count) / 1024.0) / 1024.0;
    CCLOG("Memory INfo wire = %f, act = %f, inact = %f, free = %f", wire, act, inact, free);
    return (free + inact);
}

float CBaseIdfa::usedMemory()
{
    struct task_basic_info info;
    mach_msg_type_number_t size = sizeof(info);
    kern_return_t kerr = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size);
    if (kerr != KERN_SUCCESS) {
        return 0;
    }
    float um = info.resident_size / 1024.0 / 1024.0;
    CCLOG("Memory INfo used = %f", um);
    return um;
}

#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"
void CBaseIdfa::setMultipleTouchEnabled(bool bEnabled)
{
    CCEAGLView* eaglview = (CCEAGLView*)cocos2d::Director::getInstance()->getOpenGLView()->getEAGLView();
    [eaglview setMultipleTouchEnabled:bEnabled];
}

const char* CBaseIdfa::getTrueIdfaStr()
{
    NSString *adId = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    return [adId UTF8String];
}

