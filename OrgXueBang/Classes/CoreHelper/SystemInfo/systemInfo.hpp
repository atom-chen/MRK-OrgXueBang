//
//  systemInfo.hpp
//  ReadingMate-mobile
//
//  Created by 丁晶 on 2018/11/23.
//

#ifndef systemInfo_hpp
#define systemInfo_hpp

class systemInfo
{
private:
    systemInfo();
    ~systemInfo();
    static systemInfo* m_pSystemInfo;
    
    string m_platformId;//ios:1,android:2
    string m_idfa;
    string m_idfv;
    string m_deviceToken;
    string m_imei;
    string m_model;
    string m_system;
    string m_version;
    string m_netType;//wifi,4g,3g
    string m_wifiMac;
    string m_btMac;
    string m_carrier;
    string m_appVersion;
    string m_channelId;
    
    
public:
    static systemInfo* getInstance();
    
    const string& getPlatformId();
    const string& getIdfa();
    const string& getIdfv();
    const string& getDeviceToken();
    const string& getImei();
    const string& getModel();
    const string& getSystem();
    const string& getVersion();
    const string& getnetType();
    const string& getwifiMac();
    const string& getbtMac();
    const string& getcarrier();
    const string& getappVersion();
    const string& getchannelId();
    
    
};

#endif /* systemInfo_hpp */
