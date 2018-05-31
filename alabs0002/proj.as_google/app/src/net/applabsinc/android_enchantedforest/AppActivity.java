/****************************************************************************
 * Copyright (c) 2008-2010 Ricardo Quesada
 * Copyright (c) 2010-2012 cocos2d-x.org
 * Copyright (c) 2011      Zynga Inc.
 * Copyright (c) 2013-2014 Chukong Technologies Inc.
 * <p/>
 * http://www.cocos2d-x.org
 * <p/>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * <p/>
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * <p/>
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ****************************************************************************/
package net.applabsinc.android_enchantedforest;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.common.android.IapResult;
import com.common.android.OnSetupListener;
import com.common.android.PlatformCode;
import com.common.android.ads.jni.BannerAdsJni;
import com.common.android.ads.jni.InterstitialAdsJni;
import com.common.android.ads.jni.CrosspromoAdsJni;
import com.common.android.ads.jni.RewardedAdsJni;
import com.common.android.analyticscenter.AnalyticsCenterJni;
import com.common.android.newsblast.NewsBlast;
import com.common.android.analyticscenter.AnalyticsCenter;
import com.common.android.newsblast.ErrorCode;
import com.common.android.newsblast.NewsBean;
import com.common.android.newsblast.NewsBlastListener;
import com.common.android.jni.STInstagramFunction;

import android.os.Bundle;
import android.util.Log;

public class AppActivity extends Cocos2dxActivity implements NewsBlastListener {

    private NewsBlast mNewsBlast;
    private boolean isNewsBlastShowing = false;
    private static boolean isShowNesblast = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

/*====================Google IAP setting begin====================*/
//Google版没有用到IAP就删除以下代码
        if (getPlatformCode() == PlatformCode.GOOGLEPLAY) {
            setPublicKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAq2LSzGgJfgjz20B5bfefJoPoQcaaW4KIG+zigURql2KLip+dP0r7o4dBxiig1l88eUv1KCzRNe8rmlCRqNiJV7jr5cpz3UUzcedMtKxaWWPNSH7dNEENURAWQ37sGhWBNUGRx3pD+rH+CXmV3UPRsx1cSZLmQF4qbRkJ67BRs9gVpGWbRQNhLu6aLLiP3uyxx0qcmS+jPk5ZCjF1FBtnqBdaKbFmCoGJgwgNsJ/nlDITdxmoLkQXAppqnrr09j85MF3Aq8QKXXCToMdInXO15dD/gYJr8V+PeGEiw9mY994TEkxme8RLqFhfVMTSfIhAaCcJhCAccYLl2ugSjyKDQQIDAQAB");
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
/*====================Google IAP setting end====================*/

/*==============最新广告加初始化方式，根据自己用到的广告选择加载。==============*/
        BannerAdsJni.getInstance(this);
        InterstitialAdsJni.getInstance(this);
        CrosspromoAdsJni.getInstance(this);
        RewardedAdsJni.getInstance(this);
//		RectAdsJni.getInstance(this);

        // 初始 newsblast
        mNewsBlast = new NewsBlast(this, getPlatformCode());
        mNewsBlast.registerListener(this);

        STInstagramFunction.setup(this);

        //财务统计相关,参数:用到哪些统计就给哪些
        AnalyticsCenterJni.getInstace().setDebugMode(getDebugMode());
        AnalyticsCenterJni.getInstace().setPlateform(getPlatformCode());
        AnalyticsCenterJni.getInstace().startSession(this, AnalyticsCenter.ANALYTICS | AnalyticsCenter.GOOGLE | AnalyticsCenter.FLURRY | AnalyticsCenter.QUANTCAST);

        //init JNI enviroment
        setupNativeEnvironment();

        //set more game layout id
//        MoreGamesActivityForJNI.ACTIVITY_LAYOUT_ID = R.layout.activity_more_games;
//        MoreGamesActivityForJNI.WEBVIEW_ID = R.id.more_game_view;
//        MoreGamesActivityForJNI.TITLE_PROGRESS_ID = R.id.title_progress;


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
    protected void onResume() {
        super.onResume();
        //make sure locked screen not use method
        if (!isNewsBlastShowing && isShowNesblast) {
            //  resume rerequest new blast
            mNewsBlast.continueNews();
            mNewsBlast.doNewsBlast_always();
            isNewsBlastShowing = true;
            isShowNesblast = false;
        }
    }

    @Override
    protected void onPause() {
        mNewsBlast.breakOff();
        super.onPause();
    }

    // ----------- implements NewsBlastListener -----------
    @Override
    public void onMessage(NewsBean message) {

    }

    @Override
    public void onClose() {

    }

    @Override
    public void onRedirectAndClose() {

    }

    @Override
    public void onError(ErrorCode code) {
        if (getDebugMode())
            Log.w("", "NEWSBLAST onError:" + code);
    }
}
