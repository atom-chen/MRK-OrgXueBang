package org.cocos2dx.util;

import java.io.File;
import java.io.FileNotFoundException;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.database.Cursor;
import android.media.ThumbnailUtils;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.FileOutputStream;
import java.io.IOException;

public class ImagePicker {
    
    public static final int PHOTONONE       = 10000;
    public static final int PHOTOHRAPH      = 10001;
    public static final int PHOTOZOOM       = 10002;
    public static final int PHOTORESOULT    = 10003;
    public static final int VIDEOHARPH      = 10004;

    public static final String IMAGE_UNSPECIFIED = "image/*";
    
    private static ImagePicker instance = null;
    private static Activity activity = null;
    private static String TAG = "ImagePicker";
    
    public static native void onImageSaved(String path);
    
    //拍摄照片保存路径
    private static String savePath = Environment.getExternalStorageDirectory() +"/ReadingMate";
    private static String photoName = "";
    private static Uri imgUri = null;
    private static String videoName = "";
    private static Uri videoUri = null;
    
    public static ImagePicker getInstance()
    {
        if(null == instance)
        {
            instance = new ImagePicker();
        }
        return instance;
    }
    
    //初始化
    public void init(Activity activity)
    {
        ImagePicker.activity = activity;
    }
    
    //打开相册
    static public void openPhoto()
    {
        Intent intent = new Intent(Intent.ACTION_PICK, null);
        intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, IMAGE_UNSPECIFIED);
        activity.startActivityForResult(intent, PHOTOZOOM);
    }
    
    //打开相机
    static public void openCamera()
    {
    	File destDir = new File(savePath);
    	if (!destDir.exists()) 
        {
    		destDir.mkdirs();
        }
    	photoName = "temp.jpg";
    	File file = new File(savePath + "/" + photoName);
    	imgUri = Uri.fromFile(file);
        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, imgUri);
        activity.startActivityForResult(intent, PHOTOHRAPH);
    }

    //打开录像
    static public void openVideo()
    {
        File destDir = new File(savePath);
        if (!destDir.exists())
        {
            destDir.mkdirs();
        }
        videoName = "temp.mp4";
        File file = new File(savePath + "/" + videoName);
        videoUri = Uri.fromFile(file);
        Intent intent = new Intent(MediaStore.ACTION_VIDEO_CAPTURE);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, videoUri);
//        intent.putExtra(MediaStore.EXTRA_VIDEO_QUALITY, 0.5);设置视频品质， 0～1之间，默认0.5

        activity.startActivityForResult(intent, VIDEOHARPH);
    }
    
    //回调
    public void onActivityResult(int requestCode,int resultCode,Intent data)
    {
        if (resultCode == PHOTONONE)
            return;
        
        // 拍照 不能使用data，因为没有返回是空的
        if (requestCode == PHOTOHRAPH)
        {
        	if (imgUri == null)
        	{
        		Log.e(TAG, "PHOTOHRAPH imgUri is null");
        		return;
			}
            startPhotoZoom(imgUri);
        }

        // 相册
        if (requestCode == PHOTOZOOM)
        {
        	 // 读取相册缩放图片
            if (data==null )
            {
            	Log.e(TAG, "data is null");
                return;
            }
            if (data.getData()==null) 
            {
            	Log.e(TAG, "data.getData() is null");
                return;
			}
            startPhotoZoom(data.getData());
        }
        
        // 处理结果
        if (requestCode == PHOTORESOULT)
        {
        	Bitmap bitmap = decodeUriAsBitmap(imgUri);
        	if (bitmap == null)
        	{
        		Log.e(TAG, "PHOTORESOULT bitmap is null");
                return;
			}
        	Log.e(TAG, "PHOTORESOULT 图片已经保存，通知c++层，");

            JSONObject obj = new JSONObject();
            try {
                obj.put("image", savePath + "/" + photoName);
                String strResult = obj.toString().replace("\\","");
                onImageSaved(strResult);
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }

        //录像
        if (requestCode == VIDEOHARPH)
        {
            try {
                //得到图片资源的地址
                Uri uri = data.getData();
                Log.e(TAG, "VIDEOHARPH 视频保存地址 uri = " + uri.toString());

                String videoPath = savePath + "/" + videoName;
                Log.e(TAG, "VIDEOHARPH videoPath = " + videoPath);

                Bitmap bitmap = getVideoThumbnail(videoPath, 600, 600, MediaStore.Video.Thumbnails.MINI_KIND);

                if (bitmap == null)
                {
                    Log.e(TAG, "VIDEOHARPH bitmap is null");
                    return;
                }
                String absolutePath = bitmap2File(bitmap, savePath);
                Log.e(TAG, "VIDEOHARPH 图片已经保存，通知c++层，absolutePath = " + absolutePath);
                if (null != absolutePath) {
                    JSONObject obj = new JSONObject();
                    try {
                        obj.put("video", videoPath);
                        obj.put("image", absolutePath);

                        String strResult = obj.toString().replace("\\","");
                        onImageSaved(strResult);
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
                Log.e(TAG, "VIDEOHARPH err msg = " + e.getMessage());
            }
        }
    }
    
    public void startPhotoZoom(Uri uri)
    {
    	photoName = System.currentTimeMillis() + ".jpg";
        File destDir = new File(savePath);
        if (!destDir.exists())
        {
            destDir.mkdirs();
        }
        File file = new File(savePath + "/" + photoName);
        imgUri = Uri.fromFile(file);

        Intent intent = new Intent("com.android.camera.action.CROP");
        intent.setDataAndType(uri, IMAGE_UNSPECIFIED);
        intent.putExtra("crop", "true");
        intent.putExtra("aspectX", 1);
        intent.putExtra("aspectY", 1);
        intent.putExtra("outputX", 600);
        intent.putExtra("outputY", 600);
        intent.putExtra("return-data", false);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, imgUri);

        activity.startActivityForResult(intent, PHOTORESOULT);
    }
    
    private Bitmap decodeUriAsBitmap(Uri uri)
	{
		Bitmap bitmap = null;
		try {
			bitmap = BitmapFactory.decodeStream(activity.getContentResolver().openInputStream(uri));
		} catch (FileNotFoundException e) {
			// TODO: handle exception
			e.printStackTrace();
//            Log.e(TAG,"e.getMessage()------"+e.getMessage());
			return null;
		}
		
		return bitmap;
	}

    /**
     * 获取视频的缩略图
     * 先通过ThumbnailUtils来创建一个视频的缩略图，然后再利用ThumbnailUtils来生成指定大小的缩略图。
     * 如果想要的缩略图的宽和高都小于MICRO_KIND，则类型要使用MICRO_KIND作为kind的值，这样会节省内存。
     * @param videoPath 视频的路径
     * @param width 指定输出视频缩略图的宽度
     * @param height 指定输出视频缩略图的高度度
     * @param kind 参照MediaStore.Images(Video).Thumbnails类中的常量MINI_KIND和MICRO_KIND。
     *            其中，MINI_KIND: 512 x 384，MICRO_KIND: 96 x 96
     * @return 指定大小的视频缩略图
     */
    private static Bitmap getVideoThumbnail(String videoPath, int width, int height,int kind) {
        Bitmap bitmap = null;
        // 获取视频的缩略图
        bitmap = ThumbnailUtils.createVideoThumbnail(videoPath, kind);
        if(bitmap!= null){
            bitmap = ThumbnailUtils.extractThumbnail(bitmap, width, height, ThumbnailUtils.OPTIONS_RECYCLE_INPUT);
        }
        return bitmap;
    }

    /**
     * Bitmap保存成File
     *
     * @param bitmap input bitmap
     * @param name output file's name
     * @return String output file's path
     */
    private static String bitmap2File(Bitmap bitmap, String path) {
        photoName = System.currentTimeMillis() + ".jpg";
        File destDir = new File(path);
        if (!destDir.exists())
        {
            destDir.mkdirs();
        }
        File f = new File(path + "/" + photoName);
        if  (f.exists()) f.delete();

        FileOutputStream fOut = null;

        try {
            fOut = new FileOutputStream(f);
            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fOut);
            fOut.flush();
            fOut.close();
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }

        return f.getAbsolutePath();
    }
}
