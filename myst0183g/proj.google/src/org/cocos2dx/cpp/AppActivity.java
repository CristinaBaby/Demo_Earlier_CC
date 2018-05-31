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
package org.cocos2dx.cpp;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import com.moremysteries.android_hog_lostgarden.R;

import com.common.android.IapResult;
import com.common.android.OnSetupListener;
import com.common.android.PlatformCode;
import com.common.android.jni.MoreGamesActivityForJNI;
import com.common.android.jni.STMopubAds;
import com.common.android.jni.STMopubAds.STAmazonAdSize;
import com.common.android.newsblast.ErrorCode;
import com.common.android.newsblast.NewsBean;
import com.common.android.newsblast.NewsBlast;
import com.common.android.newsblast.NewsBlastListener;
import com.common.android.utils.Utils;
import com.diwublog.AnalyticX.AnalyticXBridge;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.widget.FrameLayout.LayoutParams;



public class AppActivity extends Cocos2dxActivity implements NewsBlastListener {
	private static final float DESIGN_WIDTH = 1024.f;
	private static final float DESIGN_HEIGHT = 768.f;
	private static final float ADVIEW_BG_HEIGHT = 65.f;

    private static int newBlastIndex=0;
    
	private NewsBlast mNewsBlast;
	private boolean isNewsBlastShowing = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if (getPlatformCode() == PlatformCode.GOOGLEPLAY) {
			setPublicKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAodVIE/E55iZvlfgDv1eY7dq0YBJGSVUJNDeA47cAGFnELhe+rU+FsLGQzfi+MbdwYos7HwBtaLmJnmfbvVY6YoR3KW5VuWIZpYN2wFFspkIgjxtGtDUSm4HZhqL/hyVYj2vgvvWwrTP7rhpIIVMqI4A069zrRVqm8QcnrcgMkJ6f62LgxuZv7sjoPkGvMIYQktGBRmiaBsDXsFNI6ZQCjQDIuTxS+YbHFa3LnL18L73T+waD1eGkwp+hRn5GbGrVufunuvs9+l3Lg44jV02M07wg3ZBKRamgLLKLL7Z6F5590U7I7zeZSU3nGi+jJA0uMuW8nKV5Cbpp+Dh2inhviwIDAQAB");
		}

		setSetupBillingListener(new OnSetupListener() {

			@Override
			public void onIabSetupFinished(IapResult result) {

				if (result.isSuccess()) {
					if (getDebugMode())
						Log.i("", "=======Billing support======");
				} else {
					if (getDebugMode())
						Log.i("", "=======Billing unsupported======");
				}
			}
		});

		onStartSetupBilling();


        //init JNI enviroment
		setupNativeEnvironment();

		//init JNI model
		STMopubAds.setup(this, true);
		//Set Amazon Ad Size
		STMopubAds.getInstance().setAmazonSizeType(STAmazonAdSize.SIZE_728x90, STAmazonAdSize.SIZE_320x50);

		float adHeight = ADVIEW_BG_HEIGHT * getUIScale();
		if (Utils.isTablet(this)) {
			if (adHeight > 90)
				adHeight = 90;
		} else {
			if (adHeight > 50)
				adHeight = 50;
		}
		//set banner size and position
		STMopubAds.getInstance().setBannnerAdViewLayoutParams(LayoutParams.MATCH_PARENT, adHeight,
				Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL);

        // flurry jni
		AnalyticXBridge.sessionContext = this.getApplicationContext();

		// init nesblast
		mNewsBlast = new NewsBlast(this, getPlatformCode());
		mNewsBlast.registerListener(this);
		// NewsBlast.debug = getDebugMode();

		//set more game layout id
		MoreGamesActivityForJNI.ACTIVITY_LAYOUT_ID = R.layout.activity_more_games;
		MoreGamesActivityForJNI.WEBVIEW_ID = R.id.more_game_view;
		MoreGamesActivityForJNI.TITLE_PROGRESS_ID = R.id.title_progress;
	}

	@Override
	protected void onResume() {
		super.onResume();
        ++newBlastIndex;
        //make sure locked screen not use method
        if (!isNewsBlastShowing && newBlastIndex % 10 == 0) {
            //  resume rerequest new blast
            mNewsBlast.continueNews();
            mNewsBlast.doNewsBlast_always();
        }
	}

	@Override
	protected void onPause() {
		mNewsBlast.breakOff();
		super.onPause();
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
		return true;
	}

	@Override
	protected void init_RateUs() {
		// we do this in cpp files
	}

	private float getUIScale() {
		DisplayMetrics dm = this.getResources().getDisplayMetrics();
		float vsizeW = dm.widthPixels;
		float vsizeH = dm.heightPixels;

		float scaleX = vsizeW / DESIGN_WIDTH;
		float scaleY = vsizeH / DESIGN_HEIGHT;

		return Math.min(scaleX, scaleY);
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
}
