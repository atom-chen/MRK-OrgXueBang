/****************************************************************************
Copyright (c) 2015 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.readingmate.book;

import android.app.Activity;
import android.app.ActivityManager;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Debug;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.util.Log;

import com.gauss.recorder.SpeexUtil;

import org.cocos2dx.recorder.AudioPlayManager;
import org.cocos2dx.recorder.AudioRecordManager;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.util.DeviceWrapper;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.List;

public class AppActivity extends Cocos2dxActivity {
    private static final int  myjni_type_DeviceInfo  = 10000;
    private static final int  myjni_type_NetInfo     = 11000;
    public  static final int  myjni_type_RecordInfo  = 12000;
    public  static final int  myjni_type_PlayOver    = 12001;

    public  static final int  myjni_type_Audio       = 19001;
    public  static final int  myjni_type_Available_mem = 20000;
    public  static final int  myjni_type_Used_mem      = 21000;
    public  static final int  myjni_type_talkingData_pageBegin  = 22000;
    public  static final int  myjni_type_talkingData_pageEnd    = 23000;
    public  static final int  myjni_type_talkingData_event      = 24000;


    public  static final int  myjni_type_muitiTouch_enable    = 26000;

    public  static  AppActivity ac;
    private static String strApkPath =  "null";
    private static int recordMode = 1;
    private static final String TAGHandler = "TAGHandler";

    public static native String JavaCallCpp(int nType, String inputStr);
    public static void onJavaCallback(final int nType,final String inputStr)
    {
        // 安全线程
        DeviceWrapper.getGLSurfaceView().queueEvent(new Runnable() {
            @Override
            public void run() {
                Log.d("Cocos2dxActivity", "onJavaCallback, inputStr = " + inputStr);
                JavaCallCpp(nType, inputStr);
            }
        });
    }

    public static String CppCallJava(int nType, String inputStr)
    {
        //注意UI线程
        Log.i("CppCallJava:",inputStr + ",t:" + nType);
        String r = "type un define";
        switch (nType)
        {
            case myjni_type_Audio:
                r = AudioPlayManager.getInstance().DealWithAudio(inputStr);
                break;
            case myjni_type_RecordInfo:
                if (recordMode==1){
                    r = SpeexUtil.RecoderInfo(inputStr);
                }else {
                    r = AudioRecordManager.getInstance().RecoderInfo(inputStr);
                }
                break;
            case myjni_type_DeviceInfo:
                r = IdfaInfo();
                Log.i("myjni_type_DeviceInfo",r);
                break;
            case myjni_type_NetInfo:
                r = NetInfo();
                break;
            case myjni_type_Available_mem:
                r = getAvailableMemory();
                break;
            case myjni_type_Used_mem:
                r = getUsedMemory();
                break;

            case myjni_type_talkingData_pageBegin:
                TalkingData_pageBegin(inputStr);
                r = "OK";
                break;
            case myjni_type_talkingData_pageEnd:
                TalkingData_pageEnd(inputStr);
                r = "OK";
                break;
            case myjni_type_talkingData_event:
                TalkingData_event(inputStr);
                r = "OK";
                break;
            case myjni_type_muitiTouch_enable:
//                if (inputStr.equals("true")) {
//                    DeviceWrapper.getGLSurfaceView().setMultipleTouchEnabled(true);
//                } else {
//                    DeviceWrapper.getGLSurfaceView().setMultipleTouchEnabled(false);
//                }
//                r = "OK";
                break;
            default:
                //return "OK";
                break;
        }

        return r;
    }

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ac = this;

        // 初始化设备工具
        DeviceWrapper.setGLSurfaceView(Cocos2dxGLSurfaceView.getInstance());
        DeviceWrapper.initialized((Activity)this);

        //下载路径
        initApkPath();
    }

    @Override
    protected void onDestroy() {
        DeviceWrapper.onDestroy();
        super.onDestroy();
    }
    @Override
    protected void onResume() {
        super.onResume();
        DeviceWrapper.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        DeviceWrapper.onPause();
    }




    void initApkPath()
    {
        String dirType = "MetricsDownload";
        final File file = ac.getExternalFilesDir(dirType);
        if (file == null) {
            Log.d(TAGHandler, "Failed to get external storage files directory");
            return;
        }
        else if (file.exists())
        {
            if (!file.isDirectory()) {
                Log.d(TAGHandler, " already exists and is not a directory");
            }
        }
        else
        {
            if (!file.mkdirs()) {
                Log.d(TAGHandler, "Unable to create directory: ");
            }
        }


        strApkPath = file.toString() + "/m.apk";
        Log.d(TAGHandler, "apk path:" + strApkPath);
    }

    //设备信息
    public static String IdfaInfo() {
        Context context = ac.getBaseContext();
        Log.i("---mac", "IdfaInfo");
        String newStr;
        JSONObject obj = new JSONObject();
        try
        {
            //ID
            obj.put("Idfa", MyIdfaString());
            //系统版本
            obj.put("SystemVersion", android.os.Build.VERSION.RELEASE);
            //手机型号
            obj.put("model", android.os.Build.MODEL);
            try {
                //APP版本
                obj.put("version", context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_CONFIGURATIONS).versionName);
            } catch (Exception e) {
                e.printStackTrace();
            }

            obj.put("carrier",DeviceWrapper.getTelecomOperators());
            //包
            obj.put("bundleid", context.getPackageName());
            //token,推送
            obj.put("DeviceToken",MyIdfaString());
            //imei
            obj.put("imei",MyImei());
            //wifimac
            obj.put("wifiMac",Mywifimac());
            //androidid
            obj.put("androidId",MyAndroidId());

        } catch (JSONException e) {
            e.printStackTrace();
        }
        newStr = obj.toString().replace("\\","");
        return newStr;
    }

    public static String MyImei() {

        Context context = ac.getBaseContext();
        String szImei = "null";
        try {
            TelephonyManager TelephonyMgr = (TelephonyManager) context.getSystemService(TELEPHONY_SERVICE);
            szImei = TelephonyMgr.getDeviceId();
        }
        catch(Exception e)
        {
            Log.d("AppActivity", "MyImei: Error getting deviceID. e: ");
        }
        return  szImei;
    }

    public static String Mywifimac() {

        //先尝试wifi
        NetworkInterface networkInterface = null;
        StringBuffer buf = new StringBuffer();
        String macAddress = "null";
        try {
            networkInterface = NetworkInterface.getByName("wlan0");
            if (networkInterface == null) {
                networkInterface = NetworkInterface.getByName("eth1");
            }
            if (networkInterface == null) {
                Context context = ac.getBaseContext();
                WifiManager wm = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
                if(wm != null)
                    macAddress = wm.getConnectionInfo().getMacAddress();
            }
            else
            {
                byte[] addr = networkInterface.getHardwareAddress();

                for (byte b : addr) {
                    buf.append(String.format("%02X:", b));
                }
                if (buf.length() > 0) {
                    buf.deleteCharAt(buf.length() - 1);
                }
                macAddress = buf.toString();
            }

        } catch (SocketException e) {
            e.printStackTrace();
            return "null";
        }
        return  macAddress;
    }


    public static String MyAndroidId() {
        Context context = ac.getBaseContext();
        return Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
    }
    /*
     * 获取唯一串号
     *
     * @param
     * @return
     */
    public static String MyIdfaString() {

        Context context = ac.getBaseContext();
        String szImei = "null";
        try {
            TelephonyManager TelephonyMgr = (TelephonyManager) context.getSystemService(TELEPHONY_SERVICE);
            szImei = TelephonyMgr.getDeviceId();
        }
        catch(Exception e)
        {
            Log.d("AppActivity", "MyIdfaString: Error getting deviceID. e: ");
        }

        String szDevIDShort = "35" + //we make this look like a valid IMEI

                Build.BOARD.length()%10 +
                Build.BRAND.length()%10 +
                //  Build.CPU_ABI.length()%10 +
                Build.DEVICE.length()%10 +
                Build.DISPLAY.length()%10 +
                Build.HOST.length()%10 +
                Build.ID.length()%10 +
                Build.MANUFACTURER.length()%10 +
                Build.MODEL.length()%10 +
                Build.PRODUCT.length()%10 +
                Build.TAGS.length()%10 +
                Build.TYPE.length()%10 +
                Build.USER.length()%10 ; //13 digits

        String szAndroidID = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);

        WifiManager wm = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
        String szWLANMAC = wm.getConnectionInfo().getMacAddress();

        BluetoothAdapter m_BluetoothAdapter = null; // Local Bluetooth adapter
        m_BluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        String szBTMAC = m_BluetoothAdapter.getAddress();

        String szLongID = szImei + szDevIDShort
                + szAndroidID+ szWLANMAC + szBTMAC;

        MessageDigest m = null;
        try {
            m = MessageDigest.getInstance("MD5");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        m.update(szLongID.getBytes(),0,szLongID.length());

        byte p_md5Data[] = m.digest();

        String szUniqueID = new String();
        for (int i=0;i<p_md5Data.length;i++) {
            int b =  (0xFF & p_md5Data[i]);

            if (b <= 0xF)
                szUniqueID+="0";

            szUniqueID+=Integer.toHexString(b);
        }
        szUniqueID = szUniqueID.toUpperCase();
//        ac.mHandler.sendMessage(ac.mHandler.obtainMessage( MSG_PUSH_ALIAS, szUniqueID));
        return szUniqueID;
    }


    /**
     * 判断是否是WIFI网络
     *
     * @param context
     * @return
     */
    public static String NetInfo() {
        Context context = ac.getBaseContext();
        ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkINfo = cm.getActiveNetworkInfo();
        String newStr;
        JSONObject obj = new JSONObject();
        try {
            if (networkINfo != null)
            {

                if (networkINfo.getType() == ConnectivityManager.TYPE_WIFI)
                    obj.put("net", "2");
                else if (networkINfo.getType() == ConnectivityManager.TYPE_MOBILE)
                    obj.put("net", "1");
                else
                    obj.put("net", "0");
            }
            else
                obj.put("net", "0");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        newStr = obj.toString().replace("\\","");
        return newStr;
    }

    public static String getAvailableMemory()
    {
        ActivityManager am = (ActivityManager) ac.getSystemService(Context.ACTIVITY_SERVICE);
        ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
        am.getMemoryInfo(mi);
        double av = mi.availMem / 1024.0 / 1024.0;
        Log.d(TAGHandler, "available memory = " + av);
        return String.valueOf(av);
    }

    public static String getUsedMemory()
    {
        ActivityManager am = (ActivityManager) ac.getSystemService(Context.ACTIVITY_SERVICE);
        List<ActivityManager.RunningAppProcessInfo> aList = am.getRunningAppProcesses();
        String pName = ac.getBaseContext().getPackageName();
        for (ActivityManager.RunningAppProcessInfo info : aList)
        {
            if (info.processName.equals(pName))
            {
                int pid = info.pid;
                int[] myMempid = new int[] { pid };
                Debug.MemoryInfo[] memoryInfo = am.getProcessMemoryInfo(myMempid);
                double av = memoryInfo[0].dalvikPrivateDirty / 1024.0;
                Log.d(TAGHandler, "used memory = " + av);
                return String.valueOf(av);
            }
        }
        return "0";
    }

    public static void TalkingData_pageBegin(String pageName)
    {
//        TCAgent.onPageStart(ac.getBaseContext(), pageName);
    }

    public static void TalkingData_pageEnd(String pageName)
    {
//        TCAgent.onPageEnd(ac.getBaseContext(), pageName);
    }

    public static void TalkingData_event(String eventParam)
    {
        try {
            JSONObject jsonObj=new JSONObject(eventParam);
            String eId = jsonObj.getString("eventId");
            String eLabel = jsonObj.getString("eventLabel");
//            TCAgent.onEvent(ac.getBaseContext(), eId, eLabel);
        } catch (JSONException e) {
            e.printStackTrace();
        }

    }
}
