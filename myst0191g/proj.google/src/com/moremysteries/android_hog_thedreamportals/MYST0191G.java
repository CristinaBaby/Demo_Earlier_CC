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
package com.moremysteries.android_hog_thedreamportals;

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

public class MYST0191G extends Cocos2dxActivity implements NewsBlastListener {
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
			setPublicKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAoasMUOv7GMvBbH59Gkqj5i9wRwKy87TnyMG7609jzkhZO9I2RFc/8S9buGET8BFY7fYplXpfgEzhAj/H+ssJL5Ad98DVJ/PwrhWccXr4t5jsmUBYWPBc7pXfeQ+vbI9PaGarJjS/P0ndASFhFLwo71If4jlYRo7HI0CKzikjCMoHJjzgxNV4UdzifB6Djekrqovl8YFK8i5o9rx7MU470z59S0rBGg1jqR8RCk1OI7lNzqP7l6ydidqybRU9Lg4+X2/Yx8D2EjwXrjUdPTAe4Wv1jeCZSeYBQKXwYFWtTVtAlrIwsB4XESLw+40QIQIkCOs0NRb1ansWbMgsMU2gKwIDAQAB");
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
		// MYST0191G should create stencil buffer
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
