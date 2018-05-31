/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
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
package net.applabsinc.android_freecoloringpagesforadults;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.common.android.IapResult;
import com.common.android.OnSetupListener;
import com.common.android.PlatformCode;
import com.common.android.analyticscenter.AnalyticsCenterJni;
import com.common.android.analyticscenter.AnalyticsCenter;

import com.common.android.ads.jni.BannerAdsJni;
import com.common.android.ads.jni.CrosspromoAdsJni;
import com.common.android.newsblast.NewsBean;
import com.common.android.newsblast.NewsBlast;
import com.common.android.newsblast.NewsBlastListener;
import com.common.android.jni.STInstagramFunction;
import com.common.android.newsblast.ErrorCode;

import android.app.AlertDialog;

import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.net.Uri;
import android.content.Intent;

public class AppActivity extends Cocos2dxActivity implements NewsBlastListener {

	private static int newBlastIndex=0;

	private NewsBlast mNewsBlast;
	private boolean isNewsBlastShowing = false;
	private Cocos2dxActivity _stContext=null;
	private static boolean isShowNesblast = true;
	private static AppActivity mActivity = null;
@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	mActivity = this;
/*====================Google IAP setting begin====================*/
//Google版没有用到IAP就删除以下代码
//		if (getPlatformCode() == PlatformCode.GOOGLEPLAY) {
//			setPublicKey("Your APP public key");
//		}
//
//		setSetupBillingListener(new OnSetupListener() {
//
//			@Override
//			public void onIabSetupFinished(IapResult result) {
//
//				if (result.isSuccess()) {
//					if (getDebugMode())
//						Log.i("", "=======Billing support======");
//				} else {
//					if (getDebugMode())
//						Log.i("", "=======Billing unsupported======");
//				}
//			}
//		});

//		onStartSetupBilling();
/*====================Google IAP setting end====================*/



/*==============最新广告加初始化方式，根据自己用到的广告选择加载。==============*/
		BannerAdsJni.getInstance(this);
//		InterstitialAdsJni.getInstance(this);
		CrosspromoAdsJni.getInstance(this);
//		RewardedAdsJni.getInstance(this);
//		RectAdsJni.getInstance(this);

		mNewsBlast = new NewsBlast(this, getPlatformCode());
		mNewsBlast.registerListener(this);
//
		STInstagramFunction.setup(this);

        AnalyticsCenterJni.getInstace().setDebugMode(getDebugMode());
        //财务统计相关,参数:用到哪些统计就给哪些
        AnalyticsCenterJni.getInstace().startSession(this, AnalyticsCenter.ANALYTICS | AnalyticsCenter.FLURRY | AnalyticsCenter.GOOGLE | AnalyticsCenter.QUANTCAST);


		//init JNI enviroment
		setupNativeEnvironment();
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	}

	@Override
	protected void onResume() {
		super.onResume();
		++newBlastIndex;
		//make sure locked screen not use method
		if (!isNewsBlastShowing && isShowNesblast) {
			//  resume rerequest new blast
			mNewsBlast.continueNews();
			mNewsBlast.doNewsBlast_always();
			isNewsBlastShowing = true;
			isShowNesblast = false;
		}

		// CMShareFunction.getInstance().onResume();
	}

	@Override
	public int getPlatformCode() {
		return PlatformCode.GOOGLEPLAY;
	}
	

	@Override
	public boolean getDebugMode() {
		return false;
	}

	@Override
	public boolean enableEvent() {
		return !getDebugMode();
	}

	@Override
	protected void init_RateUs() {
		// we do this in cpp files
	}

	// ----------- implements NewsBlastListener -----------
	@Override
	public void onMessage(NewsBean message) {
		isNewsBlastShowing = true;
	}

	@Override
	public void onClose() {
		isNewsBlastShowing = false;
	}

	@Override
	public void onRedirectAndClose() {
		isNewsBlastShowing = false;
	}

	@Override
	public void onError(ErrorCode code) {
		if (getDebugMode())
			Log.w("", "NEWSBLAST onError:" + code);

		isNewsBlastShowing = false;
	}

	public void showDialog(final CharSequence message)
	{
		_stContext = this;
		this.runOnUiThread(new Runnable() {
			//弹出对话框必须开启线程
			@Override
			public void run() {
				new AlertDialog.Builder(_stContext).setMessage(message).setPositiveButton("OK",null).show();
			}
		});
	}

	public static void openURL(String url){
		Uri uri = Uri.parse(url);
		Intent it = new Intent(Intent.ACTION_VIEW, uri);
		mActivity.startActivity(it);
	}

}
