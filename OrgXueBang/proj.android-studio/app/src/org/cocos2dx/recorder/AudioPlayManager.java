package org.cocos2dx.recorder;

import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.os.Build;
import android.util.Log;

import com.readingmate.book.AppActivity;

import org.cocos2dx.util.DeviceWrapper;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.HashMap;

public class AudioPlayManager {
    public static final String TAG = "AudioPlayManager";
    private static AudioPlayManager mInstance;
    public HashMap<String,MediaPlayer> _MapCallBack = new HashMap(); ;
    public int _soundID = 1;
    /**
     * 获取单例引用
     *
     * @return
     */
    public static AudioPlayManager getInstance() {
        if (mInstance == null) {
            synchronized (AudioRecordManager.class) {
                if (mInstance == null) {
                    mInstance = new AudioPlayManager();
                }
            }
        }
        return mInstance;
    }
    //处理音频
    public String DealWithAudio(String inputStr){
        try {
            JSONObject jsonObj=new JSONObject(inputStr);
            String strType = jsonObj.getString("type");
            if (strType.compareTo("playSound")==0){
                int value = playSound(jsonObj);
                return String.valueOf(value);
            }else if (strType.compareTo("stopSound")==0){
                String audioID = jsonObj.getString("audioID");
                boolean callback = jsonObj.getBoolean("callback");
                stopSound(audioID,callback);
            }else if (strType.compareTo("clearSound")==0){
                boolean callback = jsonObj.getBoolean("callback");
                clearSound(callback);
            }else if (strType.compareTo("getSoundDuration")==0){
                String audioID = jsonObj.getString("audioID");
                float value = getSoundDuration(audioID);
                return String.valueOf(value);
            }else if (strType.compareTo("getSoundCurrentTime")==0){
                String audioID = jsonObj.getString("audioID");
                float value = getSoundCurrentTime(audioID);
                return String.valueOf(value);
            }else if (strType.compareTo("pauseSound")==0){
                String audioID = jsonObj.getString("audioID");
                pauseSound(audioID);
            }else if (strType.compareTo("resumeSound")==0){
                String audioID = jsonObj.getString("audioID");
                resumeSound(audioID);
            }else if (strType.compareTo("volumeSound")==0){
                String audioID = jsonObj.getString("audioID");
                String volume = jsonObj.getString("volume");
                volumeSound(audioID,volume);
            }else if (strType.compareTo("speedSound")==0){
                String audioID = jsonObj.getString("audioID");
                String speed = jsonObj.getString("speed");
//                speedSound(audioID,speed);
            }else if (strType.compareTo("supportSpeedChange")==0){
                int value = supportSpeedChange();
                return String.valueOf(value);
            }

        } catch (JSONException e) {
            e.printStackTrace();
        }
        return "-1";
    }
    //播放音频
    public  int playSound(JSONObject jsonObj) {
        try {
            String file = jsonObj.getString("file");
            boolean loop = jsonObj.getBoolean("loop");
            double rate = jsonObj.getDouble("rate");
            final int iStartTag = jsonObj.getInt("iStartTag");
            int iTotalTag = jsonObj.getInt("iTotalTag");


            MediaPlayer mPlayer = new MediaPlayer();
            if (file.substring(0, 7).compareTo("assets/")==0)
            {
                String fileName = file.substring(7, file.length());
                AssetFileDescriptor fd = DeviceWrapper.getActivity().getAssets().openFd(fileName);
                mPlayer.setDataSource(fd.getFileDescriptor(), fd.getStartOffset(), fd.getLength());
            }else
            {
                mPlayer.setDataSource(file);
            }
            mPlayer.prepareAsync();
            mPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mediaPlayer) {
                    if (iStartTag!=0) {
                        mediaPlayer.seekTo((int)(iStartTag/100.0f*1000));
                    } // 装载完毕回调
                    mediaPlayer.start();
                }
            });

            //设置参数
            mPlayer.setLooping(loop);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                mPlayer.setPlaybackParams(mPlayer.getPlaybackParams().setSpeed((float) rate));
            }

            //加入字典
            _soundID++;
            final int  curSoundID = _soundID;
            _MapCallBack.put(String.valueOf(curSoundID),mPlayer);
            //播放完回调
            mPlayer.setOnCompletionListener(
                    new MediaPlayer.OnCompletionListener() {
                        @Override
                        public void onCompletion(MediaPlayer mediaPlayer) {
                            Log.e(TAG, "mediaPlayer");
                            mediaPlayer.release();
                            _MapCallBack.remove(String.valueOf(curSoundID));
                            mediaPlayer = null;
                            AppActivity.onJavaCallback(AppActivity.myjni_type_Audio, ""+curSoundID);
                        }
                    }
            );
            return curSoundID;
        } catch (IOException e) {
            Log.e(TAG, "prepare() failed");
        }   catch (JSONException e) {
            e.printStackTrace();
        }
        return -1;
    }

    //清除声音缓存
    void clearSound(boolean callback)
    {
        for (String key : _MapCallBack.keySet()) {
            MediaPlayer mPlayer = _MapCallBack.get(key);
            mPlayer.stop();
            mPlayer.release();
            if (callback){
                AppActivity.onJavaCallback(AppActivity.myjni_type_Audio, key);
            }
        }
        _MapCallBack.clear();
        _soundID = 1;
    }
    /*获取声音长度  -1 代表失败*/
    float getSoundDuration(String audioID)
    {
        if(_MapCallBack.containsKey(audioID)){
            MediaPlayer mPlayer = _MapCallBack.get(audioID);
            return mPlayer.getDuration() / 1000.0f;
        }
        return -1;
    }
    /*获取当前时间  -1 代表失败*/
    float getSoundCurrentTime(String audioID)
    {
        if(_MapCallBack.containsKey(audioID)){
            MediaPlayer mPlayer = _MapCallBack.get(audioID);
            return mPlayer.getCurrentPosition() / 1000.0f;
        }
        return -1;
    }
    /*停止声音*/
    void stopSound(String audioID,boolean callback)
    {
        if(_MapCallBack.containsKey(audioID)){
            MediaPlayer mPlayer = _MapCallBack.get(audioID);
            mPlayer.stop();
            mPlayer.release();
            _MapCallBack.remove(audioID);
            mPlayer = null;
            if (callback){
                AppActivity.onJavaCallback(AppActivity.myjni_type_Audio, audioID);
            }
        }
    }
    //暂停音效
    void pauseSound(String audioID)
    {
        if(_MapCallBack.containsKey(audioID)){
            MediaPlayer mPlayer = _MapCallBack.get(audioID);
            mPlayer.pause();
        }
    }
    //恢复音效
    void resumeSound(String audioID)
    {
        if(_MapCallBack.containsKey(audioID)){
            MediaPlayer mPlayer = _MapCallBack.get(audioID);
            mPlayer.start();
        }
    }
    //调整音量
    void volumeSound(String audioID,String volume)
    {
        if(_MapCallBack.containsKey(audioID)){
            MediaPlayer mPlayer = _MapCallBack.get(audioID);
            float value = Float.parseFloat(volume);
            mPlayer.setVolume(value,value);
        }
    }
    //调整速度
    void speedSound(String audioID,String speed)
    {
        if(_MapCallBack.containsKey(audioID)){
            MediaPlayer mPlayer = _MapCallBack.get(audioID);
            float value = Float.parseFloat(speed);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                mPlayer.setPlaybackParams(mPlayer.getPlaybackParams().setSpeed(value));
            }
        }
    }
    //调整速度
    int supportSpeedChange()
    {
        return  (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M?1:0);
    }
}
