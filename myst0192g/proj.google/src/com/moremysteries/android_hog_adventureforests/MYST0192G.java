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
package com.moremysteries.android_hog_adventureforests;

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

public class MYST0192G extends Cocos2dxActivity implements NewsBlastListener {
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
			setPublicKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAjZvHGC+5BCiZAEQKqJ8pPIFdIxiJOyB/vUaoOiywzubheOHKV/yUJFfkVAoG2kplS9MlEecTe+MG3h3k8xGfe/HEBH0nmC+e3VNY45BEFdO/tDnbDxzL06VIH8EMu54CfzKvz1cwgLmOyd6MuvEh6WfyOBMeCiEFBifKSO7oFC0Nk/F2G12to4YnYC8+QDxE77gBmWc+iTUMXhFGWlwjBUX4tql3yAxPwhsoHdkOlsF4aNBYiEGT3uHFO7yq3kMaCQcLmSV4aKfh19Or5od5NE5Z8s+lHCZx789lbhy0SMi5bAfXYCRGlK05Yxq/bTGaiGsPs07NAjiYAwZvhMRz4wIDAQAB");
		}

		setSetupBillingListener(new OnSetupListener() {

			@Override
			public void onIabSetupFinished(IapResult result) {
				// 褰�Billing���澧����缃�瀹�������锛�浼�璋���ㄦ�ゆ�ュ�ｏ��������璐�涔版��浣�搴�褰����Billing���澧����缃���������������借�����

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

		// ���濮����JNI���澧�
		setupNativeEnvironment();

        // ���濮����JNI骞垮��妯″��
        STMopubAds.setup(this, true);
        // 濡�������Amazon骞垮��锛�璁剧疆Amazon骞垮��澶у��
        STMopubAds.getInstance().setAmazonSizeType(STAmazonAdSize.SIZE_300x250, STAmazonAdSize.SIZE_300x250);
        // 璁剧疆骞垮��Banner澶у�����浣�缃�
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

		// ���濮� newsblast
		mNewsBlast = new NewsBlast(this, getPlatformCode());
		mNewsBlast.registerListener(this);
		// NewsBlast.debug = getDebugMode();

		// 璁剧疆more game������layout id
		MoreGamesActivityForJNI.ACTIVITY_LAYOUT_ID = R.layout.activity_more_games;
		MoreGamesActivityForJNI.WEBVIEW_ID = R.id.more_game_view;
		MoreGamesActivityForJNI.TITLE_PROGRESS_ID = R.id.title_progress;
	}
	@Override
	protected void onResume() {
		super.onResume();

        ++_index;

            // 纭�淇����灞���舵��涓�涓�浼����澶�璋����
            if (!islock && !isNewsBlastShowing && _index%10 == 0) {
                // 姣�娆�resume��堕����拌�锋�� newsblast
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
		// MYST0192G should create stencil buffer
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
