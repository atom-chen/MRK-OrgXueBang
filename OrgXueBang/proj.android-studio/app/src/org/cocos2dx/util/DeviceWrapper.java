/*
 * 设备相关信息的warpper
 * 
 * created by scott 2014.01.06
 * */

package org.cocos2dx.util;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.BatteryManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.PowerManager;
import android.os.StatFs;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.Gravity;
import android.widget.Toast;

//import com.uuzuche.lib_zxing.activity.CaptureActivity;
//import com.uuzuche.lib_zxing.activity.CodeUtils;
//import com.uuzuche.lib_zxing.activity.ZXingLibrary;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static android.content.Context.POWER_SERVICE;
//import static com.readingmate.book.AppActivity.myjni_type_ScanQR;
//import static com.readingmate.book.AppActivity.onJavaCallback;

public class DeviceWrapper {

	private static String mDeviceId = null;
	private static String mNewDeviceId = null;
	private static final String AUC_PREFERENCE	= "tjcPrefrences";	// Prefs file.
	private static final String PREF_EMULATOR_DEVICE_ID	= "emulatorDeviceId";
	private static Activity m_pActivity = null;
	private static Cocos2dxGLSurfaceView mGLSurfaceView;
	private static String mUrl = null;
	public static final int REQUEST_CODE = 111;
	private static BroadcastReceiver batteryLevelRcvr;
	private static IntentFilter batteryLevelFilter;
	public  static int  batteryStatus = -1;
	private static PowerManager pManager = null;
	private static PowerManager.WakeLock mWakeLock = null;
	private static boolean mHome = false;


	private static void LogD(String msg) {
		Log.d("android DeviceWrapper", msg);
	}
	
	public static Cocos2dxGLSurfaceView getGLSurfaceView() {
		return mGLSurfaceView;
	}
	
	public static void setGLSurfaceView(Cocos2dxGLSurfaceView view) {
		mGLSurfaceView = view;
	}
	
	public static Activity getActivity() {
		return m_pActivity;
	}
	
	public static void initialized(Activity pInstance) {
		m_pActivity = pInstance;
        //二维码扫描
//		ZXingLibrary.initDisplayOpinion(m_pActivity);
		//获取电池状态
		pManager = ((PowerManager) m_pActivity.getSystemService(POWER_SERVICE));
//		monitorBatteryState();//获取电池状态
		unlockState();//不锁屏
	}
	public static void onDestroy() {
//		m_pActivity.unregisterReceiver(batteryLevelRcvr);
	}
	public static void onResume() {
		DeviceWrapper.mHome = false;
		DeviceWrapper.unlockState();
	}
	public static void onPause() {
		DeviceWrapper.mHome = true;
		DeviceWrapper.lockState();
	}
	/*
	 * 逻辑层调用一些初始化的事情.由于有些调用次序的问题,需要放在这个地方才行
	 **/
	public static void initWrapperByLogic()	
	{

		final Activity activity = m_pActivity;
		//LogD("fuck initStatWrapper");
		DeviceWrapper.getActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() 
			{
		
			}
		});
	}
	/*
     * 打开网址
    **/
	public static void openUrl(String url){
		mUrl = url;
		DeviceWrapper.getActivity().runOnUiThread(new Runnable() {
			@Override
			public void run()
			{
				Uri uri = Uri.parse(mUrl);
				Intent it = new Intent(Intent.ACTION_VIEW, uri);
				m_pActivity.startActivity(it);
			}
		});
	}
	/*
     * 二维码扫描
    **/
	public static void scanQR(){
//      if (!checkPermission(m_pActivity, Manifest.permission.CAMERA,"相机权限",100)){
//		  return;
//	  }
//
//		DeviceWrapper.getActivity().runOnUiThread(new Runnable() {
//			@Override
//			public void run()
//			{
//				CaptureActivity.m_pActivity = m_pActivity;
//				Intent intent = new Intent(m_pActivity.getApplication(), CaptureActivity.class);
//				m_pActivity.startActivityForResult(intent, REQUEST_CODE);
//			}
//		});
	}

	public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		/**
		 * 处理二维码扫描结果
		 */
//		if (requestCode == REQUEST_CODE) {
//			//处理扫描结果（在界面上显示）
//			if (null != data) {
//				Bundle bundle = data.getExtras();
//				if (bundle == null) {
//					return;
//				}
//				if (bundle.getInt(CodeUtils.RESULT_TYPE) == CodeUtils.RESULT_SUCCESS) {
//					String result = bundle.getString(CodeUtils.RESULT_STRING);
//					onJavaCallback(myjni_type_ScanQR,result);
////					Toast.makeText(m_pActivity, "解析结果:" + result, Toast.LENGTH_LONG).show();
//				} else if (bundle.getInt(CodeUtils.RESULT_TYPE) == CodeUtils.RESULT_FAILED) {
////					Toast.makeText(m_pActivity, "解析二维码失败", Toast.LENGTH_LONG).show();
//				}
//			}
//		}
	}

	/*
     * 检查权限
     **/
	public static boolean checkPermission(final Activity activity,String permission,final String hint, int requestCode) {
		//检查权限
		int state = ContextCompat.checkSelfPermission(activity, permission);
		if (state ==  PackageManager.PERMISSION_GRANTED) {
			return true; //已经拥有权限
		}
		if (!ActivityCompat.shouldShowRequestPermissionRationale(activity, permission))
		{
			//申请权限
			ActivityCompat.requestPermissions(activity,new String[]{permission},requestCode);
			DeviceWrapper.getActivity().runOnUiThread(new Runnable() {
				@Override
				public void run()
				{
					Toast toast = Toast.makeText(activity.getBaseContext(),"没有"+hint, Toast.LENGTH_SHORT);
					toast.setGravity(Gravity.CENTER, 0, 0);
					toast.show();
				}
			});

		}
		else {
			//显示我们自定义的一个窗口引导用户开启权限
			showDialog(hint);
		}
		return false;
	}
	public static void showDialog(final String hint) {
		DeviceWrapper.getActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				AlertDialog.Builder builder = new AlertDialog.Builder(m_pActivity);
				builder.setMessage("去设置里面开启" + hint);
				builder.setTitle("没有权限");
				builder.setPositiveButton("好的", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {

					}
				});
				builder.create().show();
			}
		});
	}


	/*
     * 获取电池状态
    **/
	public static void monitorBatteryState() {
		batteryLevelRcvr = new BroadcastReceiver() {
			public void onReceive(Context context, Intent intent) {
				DeviceWrapper.batteryStatus = intent.getIntExtra("status", -1);
				switch (DeviceWrapper.batteryStatus) {
					case BatteryManager.BATTERY_STATUS_CHARGING:
					case BatteryManager.BATTERY_STATUS_FULL:
						unlockState();
						break;
					case BatteryManager.BATTERY_STATUS_UNKNOWN:
					case BatteryManager.BATTERY_STATUS_DISCHARGING:
					case BatteryManager.BATTERY_STATUS_NOT_CHARGING:
					default:
						lockState();
						break;
				}
			}
		};
		batteryLevelFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
		m_pActivity.registerReceiver(batteryLevelRcvr, batteryLevelFilter);
	}
    /*
     * 禁止锁屏
    **/
	public static void unlockState() {
		if (null == mWakeLock&&DeviceWrapper.mHome == false){
			mWakeLock = pManager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK
					| PowerManager.ON_AFTER_RELEASE, "mWakeLock");
			mWakeLock.acquire();
		}
	}
	/*
     * 取消锁屏
     **/
	public static void lockState() {
		if(null != mWakeLock){
			mWakeLock.release();
			mWakeLock = null;
		}
	}


	public static String getChannalID(){
		String channal = null;
		
		return channal;
	}
	
	
	
	public static void startVersionCheck(){
		LogD("startVersionCheck");
	
	}

	public static boolean networkAvailable() {
		if (null == m_pActivity) {
			LogD("networkAvailable han't find tCocos2dxActivity yet!!!");
			return false;
		}
		try {
			ConnectivityManager conn = (ConnectivityManager)m_pActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo netInfo = conn.getActiveNetworkInfo();
			if (null != netInfo && netInfo.isAvailable()){
				return true;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		LogD("networkAvailable return false!");
		return false;
	}
	
	public static boolean isUsingWifi() {
	    if (null == m_pActivity) {
            LogD("isUsingWifi han't find tCocos2dxActivity yet!!!");
            return false;
        }
	    
        try {
            ConnectivityManager connManager = (ConnectivityManager) m_pActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo wifi = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI); 
            
            if(wifi != null && wifi.getState() == State.CONNECTED){
                return true;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        LogD("isUsingWifi return false!");
        return false;
	}
	
	public static boolean isUsing3G() {
        if (null == m_pActivity) {
            LogD("isUsingInternet han't find tCocos2dxActivity yet!!!");
            return false;
        }
        
        try {
            ConnectivityManager connManager = (ConnectivityManager) m_pActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo gprs = connManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE); 
            
            if(gprs != null && gprs.getState() == State.CONNECTED){
                return true;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        LogD("isUsing3G return false!");
        return false;
    }
		
	public static String getUid() {
		do
		{
			if (null != mDeviceId)
				break;

			if (null != mDeviceId && ! mDeviceId.equals(""))
				break;
			
			mDeviceId = null;
			mDeviceId = getUidCommon();
		} while(false);

		return mDeviceId;
	}
	
	/*
	 * 由于以往的udid会出现重复的或者乱码，所以，设计新的接口来处理这个问题。 通过老的udid-newudid来说实现，减少碰撞
	 * */
	public static String getNewUid() {
		do
		{
			if (null != mNewDeviceId)
				break;

			if (null != mNewDeviceId && ! mNewDeviceId.equals(""))
				break;
			
			mNewDeviceId = null;
			mNewDeviceId = getNewUidCommon();
			String oldDeviceId = getUidCommon();
			if(oldDeviceId.matches("\\w+")){
				mNewDeviceId = oldDeviceId+"-"+mNewDeviceId;
			}
			
		} while(false);
		
		LogD("getNewUid:" + mNewDeviceId);
		return mNewDeviceId;
	}
	
	public static String getNewUidCommon() {
		if (null != mNewDeviceId) return mNewDeviceId;	
		Context context = m_pActivity;
		try
		{
			SharedPreferences settings = context.getSharedPreferences(AUC_PREFERENCE, 0);
			
			StringBuffer buff = new StringBuffer();
			String deviceId = settings.getString(PREF_EMULATOR_DEVICE_ID, null);
			
			// Do we already have an emulator device id stored for this device?
			if( deviceId != null && !deviceId.equals(""))
			{
				mNewDeviceId = deviceId;
			}
			// Otherwise generate a deviceID for emulator testing.
			else
			{
				String constantChars = "1234567890abcdefghijklmnopqrstuvw";
				
				for (int i = 0; i < 32; i++)
				{
					int randomChar = (int) ( Math.random()* 100) ;
					int ch = randomChar % 30;
					buff.append(constantChars.charAt(ch));
				}
				
				mNewDeviceId = buff.toString().toLowerCase();
				
				// Save the emulator device ID in the prefs so we can reuse it.
				SharedPreferences.Editor editor = settings.edit();
				editor.putString(PREF_EMULATOR_DEVICE_ID, mDeviceId);
				editor.commit();
			}
			
		}
		catch(Exception e)
		{
			LogD("Error getting deviceID. e: " + e.toString());
			mNewDeviceId = null;
		}

		LogD("getNewUid:" + mNewDeviceId);
 		return mNewDeviceId;
	}
	
	
	public static String getUidCommon() {
		if (null != mDeviceId) return mDeviceId;	
		Context context = m_pActivity;
		try
		{
			TelephonyManager telephonyManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
			
			if (telephonyManager != null)
			{
				mDeviceId = telephonyManager.getDeviceId();
			}

			LogD("deviceID: " + mDeviceId);
			
			boolean invalidDeviceID = false;
			
			
			//----------------------------------------
			// Is the device ID null or empty?
			//----------------------------------------
			if (mDeviceId == null)
			{
				LogD("Device id is null.");
				invalidDeviceID = true;
			}
			else
			//----------------------------------------
			// Is this an emulator device ID?
			//----------------------------------------
			if (mDeviceId.length() == 0 || mDeviceId.equals("000000000000000") || mDeviceId.equals("0"))
			{
				LogD("Device id is empty or an emulator.");
				invalidDeviceID = true;
			}
			//----------------------------------------
			// Valid device ID.
			//----------------------------------------
			else
			{
				// Lower case the device ID.
				mDeviceId = mDeviceId.toLowerCase();
			}
			
			LogD("ANDROID SDK VERSION: " + android.os.Build.VERSION.SDK_INT);
			
			// Is there no IMEI or MEID?
			// Is this at least Android 2.3+?
			// Then let's get the serial.
			if (invalidDeviceID && android.os.Build.VERSION.SDK_INT >= 9)
			{
				LogD("TRYING TO GET SERIAL OF 2.3+ DEVICE...");
				
				mDeviceId = android.os.Build.SERIAL;
				
				LogD("====================");
				LogD("SERIAL: deviceID: [" + mDeviceId + "]");
				LogD("====================");
				
				//----------------------------------------
				// Is the device ID null or empty?
				//----------------------------------------
				if (mDeviceId == null)
				{
					LogD("SERIAL: Device id is null.");
					invalidDeviceID = true;
				}
				else
				//----------------------------------------
				// Is this an emulator device ID?
				//----------------------------------------
				if (mDeviceId.length() == 0 || mDeviceId.equals("000000000000000") || mDeviceId.equals("0") || mDeviceId.equals("unknown"))
				{
					LogD("SERIAL: Device id is empty or an emulator.");
					invalidDeviceID = true;
				}
				//----------------------------------------
				// Valid device ID.
				//----------------------------------------
				else
				{
					// Lower case the device ID.
					mDeviceId = mDeviceId.toLowerCase();
					invalidDeviceID = false;
				}
			}
			
			
			// Is the device ID invalid?  This is probably an emulator or pre-production device.
			if (invalidDeviceID)
			{
				SharedPreferences settings = context.getSharedPreferences(AUC_PREFERENCE, 0);
				
				StringBuffer buff = new StringBuffer();
				buff.append("EMULATOR");
				String deviceId = settings.getString(PREF_EMULATOR_DEVICE_ID, null);
				
				// Do we already have an emulator device id stored for this device?
				if( deviceId != null && !deviceId.equals(""))
				{
					mDeviceId = deviceId;
				}
				// Otherwise generate a deviceID for emulator testing.
				else
				{
					String constantChars = "1234567890abcdefghijklmnopqrstuvw";
					
					for (int i = 0; i < 32; i++)
					{
						int randomChar = (int) ( Math.random()* 100) ;
						int ch = randomChar % 30;
						buff.append(constantChars.charAt(ch));
					}
					
					mDeviceId = buff.toString().toLowerCase();
					
					// Save the emulator device ID in the prefs so we can reuse it.
					SharedPreferences.Editor editor = settings.edit();
					editor.putString(PREF_EMULATOR_DEVICE_ID, mDeviceId);
					editor.commit();
				}
			}
		}
		catch(Exception e)
		{
			LogD("Error getting deviceID. e: " + e.toString());
			mDeviceId = null;
		}

		LogD("getUid:" + mDeviceId);
 		return mDeviceId;
	}
	
	public static String getLocalMacAddress() {
		String macAdd = null;
		try {
	        WifiManager wifi = (WifiManager) m_pActivity.getSystemService(Context.WIFI_SERVICE);   
	        WifiInfo info = wifi.getConnectionInfo();
	        macAdd = info.getMacAddress();
		} catch (Exception e) {
			e.printStackTrace();
		}

		LogD("DeviceWrapper getLocalMacAddress" + macAdd);
        return macAdd;
    }
	
	public static String getPhoneNumber() {
		String phoneNumber = "";
		
		try {
			TelephonyManager phoneMgr = (TelephonyManager) m_pActivity.getSystemService(Context.TELEPHONY_SERVICE);
			//phoneNumber = phoneMgr.getLine1Number();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return phoneNumber;
	}
	
	public static String getImsiNumber() {
		return getImsiNumber(DeviceWrapper.getActivity());
	}
	
	public static String getImsiNumber(Context context) {
		String imsiNumber = "";
		try {
			TelephonyManager phoneMgr = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
			imsiNumber = phoneMgr.getSubscriberId();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return imsiNumber;
	}
	
	public static String getSimOperatorName() {
		String simOperatorName = "";
		
		try {
			TelephonyManager phoneMgr = (TelephonyManager) m_pActivity.getSystemService(Context.TELEPHONY_SERVICE);
			simOperatorName = phoneMgr.getSimOperatorName();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return simOperatorName;
	}
	
	/**
	 * 检查sd卡是否可用
	 * @return
	 */
	public static boolean checkSDCard() {
		return Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED);
	}
	
	/**
	 *sd卡剩余空间,单位字节
	 */
	public static long getFreeSpaceByKB() {
		if (checkSDCard()) {
			  // 取得sd卡路径
		     File path = Environment.getExternalStorageDirectory();   
		     StatFs sf = new StatFs(path.getPath());     
//		     long blockSize = sf.getBlockSizeLong();   
//		     long freeBlocks = sf.getFreeBlocksLong();
		     long blockSize = sf.getBlockSize(); 
		     long freeBlocks = sf.getAvailableBlocks();
 
		     return (freeBlocks * blockSize)/1024;   //单位KB  
		}
		return 0;
	}
	
	/**
	 * 拷贝分享截图到sd卡
	 * @param imageLocalPath
	 * @return 拷贝后在sd卡中的路径
	 */
	public static String copyImageSharedToSdCard(String imageLocalPath){
		if (checkSDCard()) {
			File image = new File(imageLocalPath);
			String fileName = image.getName();
			File sdcard = new File(Environment.getExternalStorageDirectory(), fileName);
			copy(image, sdcard);
			Log.d("DD", sdcard.getAbsolutePath());
			String newpath = Environment.getExternalStorageDirectory().getAbsolutePath();
			Log.d("TT", newpath);
			newpath += "/" + fileName;
			Log.d("TT22", newpath);
			return newpath;
		}
		return "";
	}
	
	/**
	 * 拷贝文件
	 * @param src
	 * @param dst
	 */
	private static void copy(File src, File dst)  {
		try{
			InputStream in = new FileInputStream(src);
			OutputStream out = new FileOutputStream(dst);
			
			// Transfer bytes from in to out
			byte[] buf = new byte[1024];
			int len;
			while ((len = in.read(buf)) > 0) {
					out.write(buf, 0, len);
				}
			
			in.close();
			out.close();
			
			}catch(Exception e){
				e.printStackTrace();
			}
	}
	
	/**
	 * 获取运营商
	 * 0  移动
	 * 1  联通
	 * 2  电信
	 * @return
	 */
	public static String getTelecomOperators() {
		String result = "获取失败";
		String imsi = getImsiNumber();
		if(imsi==null || imsi.length()<=0) {
			return result;
		}
		if(imsi.startsWith("46000") || imsi.startsWith("46002") || imsi.startsWith("46007")) {
			result = "中国移动";
		}else if(imsi.startsWith("46001")) {
			result = "中国联通";
		}else {
			result = "中国电信";
		}
		return result;
	}
	
	public static String getVersionName() {
		String strVerName = null;
		try {
			String packageName = DeviceWrapper.getActivity().getPackageName();
			strVerName = DeviceWrapper.getActivity().getPackageManager().getPackageInfo(
					packageName, 0).versionName;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return strVerName;
	}

	public static int getVersionCode() {
		int verCode = 0;
		try {
			String packageName = DeviceWrapper.getActivity().getPackageName();
			verCode = DeviceWrapper.getActivity().getPackageManager().getPackageInfo(packageName,
					0).versionCode;
		} catch (Exception e) {
		}
		return verCode;
	}
	
	
	public static void showToast(final String str) {
		DeviceWrapper.getActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast toast = new Toast(DeviceWrapper.getActivity());
				toast = Toast.makeText(DeviceWrapper.getActivity(), str, Toast.LENGTH_LONG);
				toast.setGravity(Gravity.BOTTOM, 0, 50);
				toast.show();
			}
		});
	}
	
	
	
	
	//open aipai
	public static void openAipai(final int isopen){
		DeviceWrapper.getActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() {
			
			}
		});
	}
	
	public static final String KEY_IS_NANJING_PAY = "IS_NANJING_PAY";
	
	
	
	public static String getDeviceName() {
		String devName = android.os.Build.MODEL;

		String result = "";
		String regEx = "[\\w|\\u4e00-\\u9fa5]";  //包含数字，字母，汉字    
		Pattern p = Pattern.compile(regEx);
		
		Matcher m = p.matcher(devName);
		while(m.find())
		{
			result += m.group(0);
		}
		return result;
    }

	public static void doAppraise()
	{
		try{
			Uri uri = Uri.parse("market://details?id="+getActivity().getPackageName());
			Intent intent = new Intent(Intent.ACTION_VIEW,uri);
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			getActivity().startActivity(intent);
		}catch(Exception e){
			showToast("没有可用的应用商店!");
//			Toast.makeText(getActivity(), "没有可用的应用商店!", Toast.LENGTH_SHORT).show();
		}
	}
}
