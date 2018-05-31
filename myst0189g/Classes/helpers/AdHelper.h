//
//  AdHelper.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#ifndef __LVUP002__AdHelper__
#define __LVUP002__AdHelper__

#include "../modules/STAds.h"

class AdHelper : public STAdsDelegate
{
    AdHelper(){}
    
public:
    virtual ~AdHelper(){};
    
    static AdHelper* getInstance();
    static void destroy();
    
public:
    
    static void preloadInterstitial();
    
    static void requestInterstitialAds_Application();
    static void requestInterstitialAds();
    
    
    static void requestAds();
	static void removeAds();
    
	static void setAdsVisibility(bool visibility);
    
    static void loadLoadingLayer();
    
public:
    /* derived from STAdsDelegate */
    virtual void onBannerLoadSuccessfully();
    
	virtual void onBannerLoadFailed(int errorCode);
    
    virtual void onBannerClicked();
    
	virtual void onBannerDismissed();
    
    virtual void onInterstitialAdLoaded();
    
    virtual void onInterstitialAdFailed(int errorCode);
    
    virtual void onInterstitialShown();
    
    virtual void onInterstitialDismissed();
    
    
};

#endif /* defined(__LVUP002__AdHelper__) */
