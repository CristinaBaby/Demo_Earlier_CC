/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

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
package com.moremysteries.android_hog_murdermansion;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

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

import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.widget.FrameLayout.LayoutParams;

public class MYST0186G extends Cocos2dxActivity implements NewsBlastListener {
	private static final float DESIGN_WIDTH = 1024.f;
	private static final float DESIGN_HEIGHT = 768.f;
	private static final float ADVIEW_BG_HEIGHT = 65.f;
    private static int _index = 0;

	private NewsBlast mNewsBlast;
	private boolean isNewsBlastShowing = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if (getPlatformCode() == PlatformCode.GOOGLEPLAY) {
			setPublicKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAhFvng5eb53R7HnWajh30mWSAZqR/btxKNvTleUT4Hv4mrIwhD6qZhIVcZWj8TswwR3/qcv7XClwTzRDikOitpVh5DNarKUm5GnHZRhzgmICprK2iHw97+LSE/Fzq+WodCUB04e3GlCDyK2GoeHpIA65q8HyaiGjH/MAkFiOPX8pNRRhjuB2oNqACvqMnu7RWCR9X5rJmpNsRij4mSWrSCXBC3yIzwQAMCCsQ8rMHxUvltkxUyVQrgMWuzwGD/huExNLOUt0CX0wTe9rNqcMOHRuyWI7p2FHivL/lMixceG1W8/KrpeXKg/ImOGmTDUo8a35pzeIDnVBxlx1sRg5oNwIDAQAB");
		}

		setSetupBillingListener(new OnSetupListener() {

			@Override
			public void onIabSetupFinished(IapResult result) {
				// 当Billing环境配置完成后，会调用此接口，所有购买操作应当在Billing环境配置成功后才能调用

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

		// 初始化JNI环境
		setupNativeEnvironment();

        // 初始化JNI广告模块
        STMopubAds.setup(this, true);
        // 如果有Amazon广告，设置Amazon广告大小
        STMopubAds.getInstance().setAmazonSizeType(STAmazonAdSize.SIZE_300x250, STAmazonAdSize.SIZE_300x250);
        // 设置广告Banner大小和位置
        if (Utils.isTablet(this))
        {
            STMopubAds.getInstance().setBannnerAdViewLayoutParams(LayoutParams.MATCH_PARENT, 90,
                                                                  Gravity.BOTTOM | Gravity.CENTER);
        }else
        {
            STMopubAds.getInstance().setBannnerAdViewLayoutParams(LayoutParams.MATCH_PARENT, 50,
                                                                  Gravity.BOTTOM | Gravity.CENTER);
        }

		// flurry jni
		AnalyticXBridge.sessionContext = this.getApplicationContext();

		// 初始 newsblast
		mNewsBlast = new NewsBlast(this, getPlatformCode());
		mNewsBlast.registerListener(this);
		// NewsBlast.debug = getDebugMode();

		// 设置more game里的layout id
		MoreGamesActivityForJNI.ACTIVITY_LAYOUT_ID = R.layout.activity_more_games;
		MoreGamesActivityForJNI.WEBVIEW_ID = R.id.more_game_view;
		MoreGamesActivityForJNI.TITLE_PROGRESS_ID = R.id.title_progress;
	}
	@Override
	protected void onResume() {
		super.onResume();

        ++_index;

            // 确保锁屏状态下不会重复调用
            if (!islock && !isNewsBlastShowing && _index%10 == 0) {
                // 每次resume时重新请求 newsblast
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

	@Override
	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		// MYST0186G should create stencil buffer
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

		return glSurfaceView;
	}

	static {
		System.loadLibrary("cocos2dcpp");
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
