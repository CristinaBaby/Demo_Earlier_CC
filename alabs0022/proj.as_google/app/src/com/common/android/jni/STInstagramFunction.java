package com.common.android.jni;

import java.io.File;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Environment;

import com.common.android.LaunchActivity;

public class STInstagramFunction {

	private final LaunchActivity stContext;
	private static STInstagramFunction mInstance;
	

	
	
	private STInstagramFunction(LaunchActivity context) {
		stContext = context;
		nativeInit();
	}
	
	
	public static void setup(LaunchActivity context) {
		if (mInstance == null)
			mInstance = new STInstagramFunction(context);
		
	}

	public static STInstagramFunction getInstance() {
		return mInstance;
	}

	public void destroy() {
		nativeDestroy();
		mInstance = null;
	}
	
	public void sendImageToInstagram(String path)
	{
		if(isInstallInstagram())
		{
			String type = "image/*";
			String filename = "/share.png";
			String mediaPath = Environment.getExternalStorageDirectory() + filename;
			createInstagramIntent(type, mediaPath);
		}
		else
		{
			stContext.runOnUiThread(new Runnable() {

				
				@Override
				public void run() {
					new AlertDialog.Builder(stContext).setMessage("No Instagram detected. Go to download now?").setPositiveButton("Yes, Get it", new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							Uri uri = Uri.parse("market://details?id=com.instagram.android");
			                Intent it = new Intent(Intent.ACTION_VIEW, uri); 
			                stContext.startActivity(it); 
						}}).setNegativeButton("No, thanks",null).show();
				}
				
			});
			
		}
	}
	
	public boolean isInstallInstagram()
	{
		return isAppInstalled(stContext,"com.instagram.android");
	}
	
	public void createInstagramIntent(String type, String mediaPath){

	    // Create the new Intent using the 'Send' action.
	    Intent share = new Intent(Intent.ACTION_SEND);

	    // Set the MIME type
	    share.setType(type);

	    // Create the URI from the media
	    File media = new File(mediaPath);
	    Uri uri = Uri.fromFile(media);

	    // Add the URI to the Intent.
	    share.putExtra(Intent.EXTRA_STREAM, uri);
	    share.setPackage("com.instagram.android");
	    // Broadcast the Intent.
	    stContext.startActivity(Intent.createChooser(share, "Share to"));
	  
	}
	
	private boolean isAppInstalled(Context context,String packagename)
	{
		PackageInfo packageInfo;        
		try {
	            packageInfo = context.getPackageManager().getPackageInfo(packagename, 0);
	         }catch (NameNotFoundException e) {
	            packageInfo = null;
	            e.printStackTrace();
	         }
	         if(packageInfo ==null){

	            return false;
	         }else{
	            return true;
	        }
	}
	
	

	private native void nativeInit();
	private native void nativeDestroy();
	
}
