package org.cocos2dx.recorder;

import android.Manifest;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaPlayer;
import android.media.MediaRecorder;
import android.util.Log;

import com.gauss.recorder.SpeexUtil;
import com.readingmate.book.AppActivity;

import org.cocos2dx.util.DeviceWrapper;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class AudioRecordManager {
    public static final String TAG = "AudioRecordManager";
    private AudioRecord mRecorder;
    private DataOutputStream dos;
    private Thread recordThread;
    private boolean isStart = false;
    private static AudioRecordManager mInstance;
    private  int bufferSize;
    private RecordState state = RecordState.Normal;
    private String savePath;
    private MediaPlayer  mPlayer = null;

    enum RecordState{// 0 默认  1录音中  2播放中
        Normal ,
        Record ,
        Play ,
    }

    public AudioRecordManager() {
        bufferSize = AudioRecord.getMinBufferSize(16000, AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT);
        mRecorder = new AudioRecord(MediaRecorder.AudioSource.VOICE_COMMUNICATION, 16000, AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT, bufferSize * 2);
    }

    /**
     * 获取单例引用
     *
     * @return
     */
    public static AudioRecordManager getInstance() {
        if (mInstance == null) {
            synchronized (AudioRecordManager.class) {
                if (mInstance == null) {
                    mInstance = new AudioRecordManager();
                }
            }
        }
        return mInstance;
    }


    /**
     *  处理所有record相关调用
     */
    public  String RecoderInfo(String inputStr) {

        Log.d("RecoderInfo", inputStr);
        String str = inputStr + "callback";
        if (inputStr.startsWith("playRecoder"))
        {
            String strFile = inputStr.substring(11);
            playRecoder(strFile);
        }
        else if (inputStr.startsWith("startRecorder"))
        {
            String strFile = inputStr.substring(13);
            str = startRecord(strFile);
        }
        else
        {
            switch (inputStr)
            {
                case "stopRecorder":
                case "cancelRecorder":
                    stopRecord();
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
     * 销毁线程方法
     */
    private void destroyThread() {
        try {
            isStart = false;
            if (null != recordThread && Thread.State.RUNNABLE == recordThread.getState()) {
                try {
                    Thread.sleep(500);
                    recordThread.interrupt();
                } catch (Exception e) {
                    recordThread = null;
                }
            }
            recordThread = null;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            recordThread = null;
        }
    }

    /**
     * 启动录音线程
     */
    private void startThread() {
        destroyThread();
        isStart = true;
        if (recordThread == null) {
            recordThread = new Thread(recordRunnable);
            recordThread.start();
        }
    }

    /**
     * 录音线程
     */
    Runnable recordRunnable = new Runnable() {
        @Override
        public void run() {
            try {
                android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
                int bytesRecord;
                //int bufferSize = 320;
                byte[] tempBuffer = new byte[bufferSize];
                if (mRecorder.getState() != AudioRecord.STATE_INITIALIZED) {
                    stopRecord();
                    return;
                }
                mRecorder.startRecording();
//                writeToFileHead();
                while (isStart) {
                    if (null != mRecorder) {
                        bytesRecord = mRecorder.read(tempBuffer, 0, bufferSize);
                        if (bytesRecord == AudioRecord.ERROR_INVALID_OPERATION || bytesRecord == AudioRecord.ERROR_BAD_VALUE) {
                            continue;
                        }
                        if (bytesRecord != 0 && bytesRecord != -1) {
                            //在此可以对录制音频的数据进行二次处理 比如变声，压缩，降噪，增益等操作
                            //我们这里直接将pcm音频原数据写入文件 这里可以直接发送至服务器 对方采用AudioTrack进行播放原数据
                            dos.write(tempBuffer, 0, bytesRecord);
                        } else {
                            break;
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

    };

    /**
     * 保存文件
     *
     * @param path
     * @throws Exception
     */
    private void setPath(String path) throws Exception {
        path = path+".wav";
        savePath = SpeexUtil.getFileName(path);
        File file = new File(savePath);
        if (file.exists()) {
            file.delete();
        }
        file.createNewFile();
        dos = new DataOutputStream(new FileOutputStream(file, true));
    }

    /**
     * 启动录音
     *
     * @param path
     */
    public String startRecord(String path) {
        //检测麦克风权限
        if (!DeviceWrapper.checkPermission(DeviceWrapper.getActivity()
                , Manifest.permission.RECORD_AUDIO,"麦克风权限",100)){
            return "error";
        }
        if (!DeviceWrapper.checkPermission(DeviceWrapper.getActivity()
                ,Manifest.permission.WRITE_EXTERNAL_STORAGE,"存储权限",100)){
            return "error";
        }

        //如果正在播放, 直接返回
        if (isPlayRecording()){
            return "error";
        }

        //如果正在录音, 直接返回
        if (isRecording()) {
            return "error";
        }
        try {
            state = RecordState.Record;
            setPath(path);
            startThread();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "right";
    }

    /**
     * 停止录音
     */
    public void stopRecord() {
        try {
            destroyThread();
            if (mRecorder != null) {
                if (mRecorder.getState() == AudioRecord.STATE_INITIALIZED) {
                    mRecorder.stop();
                }
//                if (mRecorder != null) {
//                    mRecorder.release();
//                }
            }
            if (dos != null) {
                dos.flush();
                dos.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        state = RecordState.Normal;
        stopRecoderCallback();
    }

    public  void playRecoder(String file) {
        state = RecordState.Play;
        mPlayer = new MediaPlayer();
        try {
            mPlayer.setDataSource(file);
            mPlayer.prepare();
            mPlayer.start();
            mPlayer.setOnCompletionListener(
                    new MediaPlayer.OnCompletionListener() {
                        @Override
                        public void onCompletion(MediaPlayer mediaPlayer) {
                            stopPlayRecord();
                        }
                    }
            );
        } catch (IOException e) {
            Log.e("Recoder", "prepare() failed");
        }
    }

    public  void stopPlayRecord() {
        state = RecordState.Normal;
        mPlayer.release();
        mPlayer = null;
        stopPlayRecordCallback();
    }

    public  boolean isRecording() {
        return state == RecordState.Record;
    }

    public  boolean isPlayRecording() {
        return state == RecordState.Play;
    }

    public void stopPlayRecordCallback()
    {
        Log.d(TAG, "==YY==stopPlayRecordCallback===");
        AppActivity.onJavaCallback(AppActivity.myjni_type_PlayOver,savePath);
    }
    public  void stopRecoderCallback()
    {
        Log.d(TAG, "stopRecoderCallback " + savePath);
        AppActivity.onJavaCallback(AppActivity.myjni_type_RecordInfo, savePath);
    }
}
