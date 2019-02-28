//
//  NetworkReachabilityManager.cpp
//  ReadingFriends
//
//  Created by Summer on 16/7/25.
//
//
#import "AFNetworkReachabilityManager.h"
#include "NetworkReachabilityManager.h"
//#include "NotificationDefine.h"
//#include "NotificationManager.h"

/**
 *  管理器静态实例
 *
 *  @return 管理器实例
 */
NetworkRecahabilityManager *NetworkRecahabilityManager::mangerInstance = nullptr;

/**
 *  OC 管理器
 */
static AFNetworkReachabilityManager *manager;

/**
 *  单利管理器获取
 *
 *  @return 管理器实例对象
 */
NetworkRecahabilityManager *NetworkRecahabilityManager::shareManager()
{
    if (NetworkRecahabilityManager::mangerInstance == nullptr) {
        NetworkRecahabilityManager::mangerInstance = new NetworkRecahabilityManager();
    }
    return NetworkRecahabilityManager::mangerInstance;
}

/**
 *  构造方法
 */
NetworkRecahabilityManager::NetworkRecahabilityManager()
{
    manager = [AFNetworkReachabilityManager sharedManager];
    
    [[NSNotificationCenter defaultCenter] addObserverForName:AFNetworkingReachabilityDidChangeNotification
                                                      object:nil queue:[NSOperationQueue mainQueue]
                                                  usingBlock:^(NSNotification * _Nonnull note) {
     
//        NotificationManager::getInstance()->notify(NOTIFY_TYPE::RECHABILITY_DID_CHANGE);
    }];
}


/**
 *  析构方法
 */
NetworkRecahabilityManager::~NetworkRecahabilityManager()
{
}

/**
 *  是否有网络连接
 *
 *  @return 是或否
 */
bool NetworkRecahabilityManager::isReachable()
{
    return manager.isReachable;
}

/**
 *  是否是移动网络
 *
 *  @return 是或否
 */
bool NetworkRecahabilityManager::isReachableViaWWAN()
{
    return manager.isReachableViaWWAN;
}

/**
 *  是否是WiFi网络
 *
 *  @return 是或否
 */
bool NetworkRecahabilityManager::isReachableViaWiFi()
{
    return manager.isReachableViaWiFi;
}

/**
 *  获取当前网路状态
 *
 *  @return 网路状态
 */
NetworkReachabilityStatus NetworkRecahabilityManager::getCurrentStatus()
{
    return (NetworkReachabilityStatus)manager.networkReachabilityStatus;
}

/**
 *  开始监听网络变换
 */
void NetworkRecahabilityManager::startMonitoring()
{
    [manager startMonitoring];
}

/**
 *  停止监听网络变化
 */
void NetworkRecahabilityManager::stopMonitoring()
{
    [manager stopMonitoring];
}




