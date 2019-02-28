//
//  NetworkReachabilityManager.h
//  ReadingFriends
//
//  Created by Summer on 16/7/25.
//
//  网络管理器桥接类

#ifndef NetworkReachabilityManager_h
#define NetworkReachabilityManager_h

#include <stdio.h>


/// 网路状态枚举
enum class NetworkReachabilityStatus
{
    Unknown          = -1,  // 未知
    NotReachable     = 0,   // 无网络
    ReachableViaWWAN = 1,   // 移动网络
    ReachableViaWiFi = 2    // WiFi网路
};


class NetworkRecahabilityManager
{
public:
    /**
     *  单利管理器获取
     *
     *  @return 管理器实例对象
     */
    static NetworkRecahabilityManager *shareManager();
    
    /**
     *  是否有网络连接
     *
     *  @return 是或否
     */
    bool isReachable();
    
    /**
     *  是否是移动网络
     *
     *  @return 是或否
     */
    bool isReachableViaWWAN();
    
    /**
     *  是否是WiFi网络
     *
     *  @return 是或否
     */
    bool isReachableViaWiFi();
    
    /**
     *  获取当前网路状态
     *
     *  @return 网路状态
     */
    NetworkReachabilityStatus getCurrentStatus();
    
    /**
     *  开始监听网络变换
     */
    void startMonitoring();

    /**
     *  停止监听网络变化
     */
    void stopMonitoring();
    
private:
    NetworkRecahabilityManager();
   ~NetworkRecahabilityManager();
    static NetworkRecahabilityManager *mangerInstance; // 管理器实例
    
};

#endif /* NetworkReachabilityManager_hpp */
