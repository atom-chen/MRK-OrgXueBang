/**
 * Created by liuwei on 17/2/17.
 */

package com.gauss.recorder;

import android.Manifest;
import android.os.Environment;
import android.text.format.Time;
import android.util.Log;

import com.readingmate.book.AppActivity;

import org.cocos2dx.util.DeviceWrapper;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
public class SpeexUtil {

    private static String TAG = "liuwei";

    /**
     *  处理所有record相关调用
     */
    public static String RecoderInfo(String inputStr) {

        Log.d("liuweiinfo", inputStr);
        String str = inputStr + "callback";
        if (inputStr.startsWith("playRecoder"))
        {
            String strFile = inputStr.substring(11);
            playRecoder(strFile);
        }
        else if (inputStr.startsWith("startRecorder"))
        {
            String strFile = inputStr.substring(13);
            str = startRecoder(strFile);
        }
        else
        {
            switch (inputStr)
            {
                case "stopRecorder":
                case "cancelRecorder":
                    stopRecoder();
                    break;
                case "isRecording":
                    str = isRecording() ? "1" : "0";
                    break;
                case "isPlayRecording":
                    str = isPlayRecording() ? "1" : "0";
                    break;
                case "stopPlayRecord":
                    stopPlayRecord();
                    break;
            }
        }
        return str;
    }


    /**
     * 录音相关
     */
    public  static SpeexRecorder recorderInstance = null;
    private static String fileName = "";
    private static SpeexPlayer speexPlayer = null;//播放实例
    private static String specifyName = "";


    public static boolean isRecording() {
        Log.d("liuwei", "isRecording");
        if (fileName != "")
        {
            return true;
        }
        if (recorderInstance != null)
        {
            return recorderInstance.isRecording();
        }
        return false;
    }


    public static String startRecoder(String strFile) {

        //检测麦克风权限
        if (!DeviceWrapper.checkPermission(DeviceWrapper.getActivity()
                ,Manifest.permission.RECORD_AUDIO,"麦克风权限",100)){
            return "error";
        }
        if (!DeviceWrapper.checkPermission(DeviceWrapper.getActivity()
                ,Manifest.permission.WRITE_EXTERNAL_STORAGE,"存储权限",100)){
              return "error";
         }

        //如果正在播放, 直接返回
        if (speexPlayer != null){
            return "error";
        }

        //如果正在录音, 直接返回
        if (isRecording()) {
            return "error";
        }

        _destroyInstance();
        specifyName = strFile;
        fileName = getFileName(specifyName);
        recorderInstance = new SpeexRecorder(fileName);
        Thread th = new Thread(recorderInstance);
        th.start();
        recorderInstance.setRecording(true);

        Log.d("yueting", fileName);
        Log.d("liuwei", "startRecoder");
        return  "";
    }

    public static void stopRecoder() {
        Log.d("liuwei", "stopRecoder");



        if (recorderInstance != null)
        {
            recorderInstance.setRecording(false);
        }
    }
    public static boolean isPlayRecording() {
        Log.d(TAG, "==YY==isPlayRecording1===");
        if (speexPlayer != null)
        {
            Log.d(TAG, "==YY==isPlayRecording2===");
            return speexPlayer.isRecordPlaying();
        }
        Log.d(TAG, "==YY==isPlayRecording3===");
        return false;
    }

    public static void tipPermission()
    {
        Log.d("liuwei", "tipPermission");
        DeviceWrapper.showDialog("麦克风");
        fileName = "";
        stopRecoder();
    }

    public static void stopRecoderCallback()
    {
        Log.d("stopRecoderCallback", "liuwei " + fileName);
        AppActivity.onJavaCallback(AppActivity.myjni_type_RecordInfo, fileName);
        _destroyInstance();
    }
    private static void _destroyInstance()
    {
        fileName = "";
        recorderInstance = null;
    }



    public static void playRecoder(String file) {
        Log.d("liuwei", "playRecoder " + file);
        //如果没有播放并且没有在录音
        if (speexPlayer == null && !isRecording())
        {
            speexPlayer = new SpeexPlayer(file);
            speexPlayer.startPlay();
        }
    }



    public static void stopPlayRecord() {
        Log.d("liuwei", "stopPlayRecord");
        if (speexPlayer != null)
        {
            speexPlayer.stopPlay();
        }
        speexPlayer = null;
    }

    public static void stopPlayRecordCallback()
    {
        Log.d(TAG, "==YY==stopPlayRecordCallback===");
        AppActivity.onJavaCallback(AppActivity.myjni_type_PlayOver, fileName);
    }

    public static boolean isHaveSDcard(){
        return Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED);
    }


    public static String getSDPath(){
        String sdDir=null;
        if(isHaveSDcard()){
            sdDir = Environment.getExternalStorageDirectory().getAbsolutePath();
        }
        return sdDir;
    }


    public static String now(                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  )
    {
        Time localTime = new Time();
        localTime.setToNow();
        return localTime.format("%Y%m%d%H%M%S");
    }


    public static String getRandomString(int len) {
        String returnStr = "";
        char[] ch = new char[len];
        Random rd = new Random();
        for (int i = 0; i < len; i++) {
            ch[i] = (char) (rd.nextInt(9)+97);
        }
        returnStr = new String(ch);
        return returnStr;
    }


    public static List<String> getFileFormSDcard(File dir,String type){
        List<String> listFilesName = new ArrayList<String>();
        if(isHaveSDcard()){
            File[] files = dir.listFiles();
            if(files !=null){
                for(int i=0; i<files.length; i++){
                    if(files[i].getName().indexOf(".")>=0){
                        // 只取Type类型的文件
                        String filesResult = files[i].getName()
                                .substring(files[i].getName().indexOf("."));
                        if(filesResult.toLowerCase().equals(type.toLowerCase())){
                            listFilesName.add(files[i].getName());
                        }
                    }
                }
            }
        }
        return listFilesName;
    }


    public static String getFileName(String specifyName){
        String strSDPath = "/mnt/sdcard/";
        if (isHaveSDcard()) {
            strSDPath = getSDPath();
        }
        String strAppPath = DeviceWrapper.getActivity().getApplicationContext().getFilesDir().getAbsolutePath();
        String fullName;
        if(specifyName.length() > 0 && specifyName.contains(strAppPath))
        {
            fullName = specifyName;//.replace(strAppPath, strSDPath);
            String spxDir = fullName.substring(0, fullName.lastIndexOf("/"));
            try{
                File dir = new File(spxDir);
                if (!dir.exists()) {
                    dir.mkdirs();
                }
            }catch(Exception e){
                e.printStackTrace();
            }
        }
        else
        {
            fullName = strSDPath + "/" + "net_book";
            File dir = new File(fullName);
            if (!dir.exists()) {
                dir.mkdirs();
            }
            fullName += ("/empty.spx");
        }
        return fullName;
    }
}
