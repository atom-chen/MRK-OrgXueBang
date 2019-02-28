package com.gauss.speex.encode;

import android.util.Log;

import java.util.Collections;
import java.util.LinkedList;
import java.util.List;


import com.gauss.recorder.SpeexUtil;
import com.gauss.writer.speex.SpeexWriter;

/**
 * 
 * @author Gauss
 * 
 */
public class SpeexEncoder implements Runnable {

	private final Object mutex = new Object();
	private Speex speex = new Speex();
	// private long ts;
	public static int encoder_packagesize = 1024;
	private byte[] processedData = new byte[encoder_packagesize];

	List<ReadData> list = null;
	private volatile boolean isRecording = false;
	private String fileName;
    private static int testNoSize = 0;

	public SpeexEncoder(String fileName) {
		super();
		speex.init();
		list = Collections.synchronizedList(new LinkedList<ReadData>());
		this.fileName = fileName;
	}
	

	public void run() {

		SpeexWriter fileWriter = new SpeexWriter(fileName);
		Thread consumerThread = new Thread((Runnable) fileWriter);
		fileWriter.setRecording(true);
		consumerThread.start();

		android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);

		int getSize = 0;
        int rawSize = 0;
        testNoSize = 0;
		while (this.isRecording()) {
			if (list.size() == 0) {
//				log.debug("no data need to do encode");
                if (testNoSize > 50)
                {
                    testNoSize = 0;//50次都没有被添加数据说明没有权限
                    SpeexUtil.tipPermission();
                    break;
                }
				try {
					Thread.sleep(20);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
                Log.d("liuwei", "testNoSize " + String.valueOf(testNoSize));
                testNoSize++;
				continue;
			} else {
                testNoSize = 0;
			}
			if (list.size() > 0) {
				synchronized (mutex) {
					ReadData rawdata = list.remove(0);
					getSize = speex.encode(rawdata.ready, 0, processedData, rawdata.size);
                    rawSize = rawdata.size;
//					log.info("after encode......................before=" + rawdata.size + " after=" + processedData.length + " getsize="
//							+ getSize);

				}
//                Log.d("rawSize ", String.valueOf(rawSize));

				if (getSize > 0) {
					fileWriter.putData(processedData, getSize);
//					log.info("............clear....................");
					processedData = new byte[encoder_packagesize];
				}
			}

            if (rawSize == 0)
            {
                SpeexUtil.tipPermission();
                break;
            }
		}
//		log.debug("encode thread exit");
		Log.d("liuwei", "encoder false");
		fileWriter.setRecording(false);
	}

	/**
	 * 
	 * @param data
	 * @param size
	 */
	public void putData(short[] data, int size) {
		ReadData rd = new ReadData();
		synchronized (mutex) {
			rd.size = size;
			System.arraycopy(data, 0, rd.ready, 0, size);
			list.add(rd);
		}
	}

	public void setRecording(boolean isRecording) {
		synchronized (mutex) {
			this.isRecording = isRecording;
		}
	}

	public boolean isRecording() {
		synchronized (mutex) {
			return isRecording;
		}
	}

	class ReadData {
		private int size;
		private short[] ready = new short[encoder_packagesize];
	}
}
