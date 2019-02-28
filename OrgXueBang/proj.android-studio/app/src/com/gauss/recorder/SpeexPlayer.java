/**
 * 
 */
package com.gauss.recorder;

import java.io.File;
import android.util.Log;

import com.gauss.speex.encode.SpeexDecoder;

/**
 * @author Gauss
 * 
 */
public class SpeexPlayer {
	private String fileName = null;
	private SpeexDecoder speexdec = null;
	private volatile boolean isRecordPlaying;
	private final String TAG = "SpeexPlayer";
	public SpeexPlayer(String fileName) {
		this.isRecordPlaying = false;
		this.fileName = fileName;
		Log.d("liuweifilename", fileName);
		try {
			speexdec = new SpeexDecoder(new File(this.fileName));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void startPlay() {
		RecordPlayThread rpt = new RecordPlayThread();
		Thread th = new Thread(rpt);
		th.start();
		this.isRecordPlaying = true;
	}


    public void stopPlay() {
        if (speexdec != null)
        {
			Log.e(TAG, "==YY==stopPlay==");
            speexdec.setPaused(true);
        }
    }

	public boolean isRecordPlaying()
	{
		Log.e(TAG, "==YY==isRecordPlaying==>"+isRecordPlaying);
		Log.e(TAG, "==YY==fileName==>"+fileName);
		return this.isRecordPlaying  && this.fileName != "";
	}

	class RecordPlayThread extends Thread {

		public void run() {
			try {
				if (speexdec != null){
					speexdec.decode();
                    SpeexUtil.stopPlayRecord();
					isRecordPlaying = false;
					Log.e(TAG, "==YY==RecordPlayThread==");
				}
			} catch (Exception t) {
				t.printStackTrace();
			}
		}
	};
}
