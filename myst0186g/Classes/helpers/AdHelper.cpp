//
//  AdHelper.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#include "AdHelper.h"
#include "PurchaseHelper.h"
#include "../scenes/GameScene.h"
#include "../AppConfig.h"
#include "../layers/AdsLoadingLayer.h"

#define k_adLoadingLayer_zorder INT_MAX

USING_NS_CC;

static AdHelper* sp_AdHelper = 0;

AdHelper* AdHelper::getInstance()
{
	if (!sp_AdHelper)
	{
		sp_AdHelper = new AdHelper();
	}
    
	return sp_AdHelper;
}

void AdHelper::destroy()
{
	if (sp_AdHelper)
	{
		delete sp_AdHelper;
        
		sp_AdHelper = 0;
	}
}

void AdHelper::preloadInterstitial()
{
    STAds* ad = new STAds();
    ad->preloadInterstitial();
    CC_SAFE_DELETE(ad);
}

void AdHelper::loadLoadingLayer()
{
    if(!AdLoadingLayerBase::s_currentInstance)
    {
        AdsLoadingLayer* adLoadingLayer = AdsLoadingLayer::create();
        AdHelper::removeAds();
        CCDirector::sharedDirector()->getRunningScene()->addChild(adLoadingLayer,k_adLoadingLayer_zorder);
    }
    else
    {
        AdLoadingLayerBase::s_currentInstance->loadAd();
    }

}

void AdHelper::requestInterstitialAds_Application()
{
    if (!STAds::isInterstitialShowing)
    {
        AdHelper::loadLoadingLayer();
        
//        CCCallFunc* lFunc=CCCallFunc::create(this, callfunc_selector(AdHelper::loadLoadingLayer));
//        CCSequence* lAction=CCSequence::create(lFunc,NULL);
//        CCDirector::sharedDirector()->getRunningScene()->runAction(lAction, NULL));
    }

}
void AdHelper::requestInterstitialAds()
{
    if (PurchaseHelper::isAdPurchased) return;
    
    if(!AdLoadingLayerBase::s_currentInstance)
    {
        AdsLoadingLayer* adLoadingLayer = AdsLoadingLayer::create();
        CCDirector::sharedDirector()->getRunningScene()->addChild(adLoadingLayer,k_adLoadingLayer_zorder);

    }
    else
    {
        AdLoadingLayerBase::s_currentInstance->loadAd();
    }
}


void AdHelper::requestAds()
{
    if (PurchaseHelper::isAdPurchased) return;
    
    CCLOG("request Ad------");
    
    STAds* ad = new STAds();
    ad->requestAds();
    ad->setAdsDelegate(AdHelper::getInstance());
    CC_SAFE_DELETE(ad);
}

void AdHelper::removeAds()
{
    CCLOG("remove Ad------");
    
    STAds* ad = new STAds();
    ad->removeAds();
    CC_SAFE_DELETE(ad);
}



void AdHelper::onBannerLoadSuccessfully()
{
}

void AdHelper::onBannerLoadFailed(int errorCode)
{
}

void AdHelper::onBannerClicked()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    if (scene->getTag() == tag_scene_game)
    {
        GameScene* gs = dynamic_cast<GameScene*>(scene);
        gs->pauseGame();
    }
    
#endif
    
}

void AdHelper::onBannerDismissed()
{
}

void AdHelper::onInterstitialAdLoaded()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    if (scene->getTag() == tag_scene_game)
    {
        GameScene* gs = dynamic_cast<GameScene*>(scene);
        gs->pauseGame();
    }
    
#endif
    
}

void AdHelper::onInterstitialAdFailed(int errorCode)
{
}

void AdHelper::onInterstitialShown()
{
}

void AdHelper::onInterstitialDismissed()
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(st_resume_response_menu_touch_notification);
    

}


// --------------------------------------------



