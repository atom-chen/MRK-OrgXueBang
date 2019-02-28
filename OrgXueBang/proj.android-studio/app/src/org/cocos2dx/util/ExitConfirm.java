package org.cocos2dx.util;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnClickListener;
import android.content.DialogInterface.OnKeyListener;
import android.net.Uri;
import android.view.KeyEvent;
import android.util.Log;

public class ExitConfirm {
	public static boolean bDialogShow = false;
	public static String TAG = "ExitConfirm";

	public static void showExitDialog() {
		Log.e(TAG, "==YY==showExitDialog0");
		if (bDialogShow) {
			return;
		} else {
			ExitConfirm.bDialogShow = true;
		}
		Log.e(TAG, "==YY==showExitDialog1 !");
		DeviceWrapper.getActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				AlertDialog.Builder builder = new Builder(DeviceWrapper.getActivity());
				
				builder.setMessage("Message");
				builder.setTitle("Title");

				builder.setCancelable(false);
				builder.setOnKeyListener(new OnKeyListener() {
					@Override
					public boolean onKey(DialogInterface arg0, int arg1,KeyEvent arg2) {
						return true;
					}
				});

				builder.setPositiveButton("确认",new OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog,int which) {
								
								ExitConfirm.bDialogShow = false;
								dialog.dismiss();

								Cocos2dxGLSurfaceView glView = DeviceWrapper.getGLSurfaceView();
								if (null == glView)
									return;

								glView.queueEvent(new Runnable() {
									@Override
									public void run() {
										Log.e(TAG, "==YY==Btn Sure Clicked !");
									}
								});
							}
						});

				builder.setNegativeButton("取消",new OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog,int which) {
								ExitConfirm.bDialogShow = false;
								dialog.dismiss();
								
								Cocos2dxGLSurfaceView glView = DeviceWrapper.getGLSurfaceView();
								if (null == glView)
									return;

								glView.queueEvent(new Runnable() {
									@Override
									public void run() {
										Log.e(TAG, "==YY==Btn Cancel Clicked !");
									}
								});
							}
						});

				builder.create().show();
			}
		});
	}

	public static void showBookQuit() {
		Log.e(TAG, "==YY==showBookQuit0");
		if (bDialogShow) {
			return;
		} else {
			ExitConfirm.bDialogShow = true;
		}
		Log.e(TAG, "==YY==showBookQuit1 !");
		DeviceWrapper.getActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				AlertDialog.Builder builder = new Builder(DeviceWrapper.getActivity());

				builder.setMessage("您确认要停止读书返回书架？");
				builder.setTitle("退出读书");

				builder.setCancelable(false);
				builder.setOnKeyListener(new OnKeyListener() {
					@Override
					public boolean onKey(DialogInterface arg0, int arg1,KeyEvent arg2) {
						return true;
					}
				});

				builder.setPositiveButton("确认",new OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog,int which) {

						ExitConfirm.bDialogShow = false;
						dialog.dismiss();

						Cocos2dxGLSurfaceView glView = DeviceWrapper.getGLSurfaceView();
						if (null == glView)
							return;

						glView.queueEvent(new Runnable() {
							@Override
							public void run() {
								Log.e(TAG, "==YY==Btn Sure Clicked !");
							}
						});
					}
				});

				builder.setNegativeButton("取消",new OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog,int which) {
						ExitConfirm.bDialogShow = false;
						dialog.dismiss();

						Cocos2dxGLSurfaceView glView = DeviceWrapper.getGLSurfaceView();
						if (null == glView)
							return;

						glView.queueEvent(new Runnable() {
							@Override
							public void run() {
								Log.e(TAG, "==YY==Btn Cancel Clicked !");
							}
						});
					}
				});

				builder.create().show();
			}
		});
	}
}
