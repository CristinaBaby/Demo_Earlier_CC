//
//  ColorAdsManager.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "ColorAdsManager.h"
#include "AdsLoadingLayer.h"


static ColorAdsManager *_sharedColorAds = nullptr;

ColorAdsManager* ColorAdsManager::getInstance()
{
    if (! _sharedColorAds)
    {
        _sharedColorAds = new ColorAdsManager();
    }
    
    return _sharedColorAds;
}

void ColorAdsManager::destroyInstance()
{
    xScheduler->unscheduleAllForTarget(this);
    CC_SAFE_DELETE(_sharedColorAds);
}

ColorAdsManager::ColorAdsManager()
{
    _showed = false;
    _shared = false;
    xScheduler->scheduleUpdate(this, 0, false);
}

void ColorAdsManager::share()
{
    share(false);
}

void ColorAdsManager::share(bool forSave)
{
    if (forSave) {
        if (!_showed) {
            _showed = true;
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true, Director::getInstance()->getRunningScene(), 100);
        }
    }
    else
    {
        _shared = true;
    }
}

void ColorAdsManager::back(Scene* popupAdsScene)
{
    if (!_showed && _shared) {
        //展示广告
        _showed = true;
        _popupAdsScene = popupAdsScene;
    }
    
    _shared = false;
}

void ColorAdsManager::update(float dt)
{
    if (_popupAdsScene) {
        if (_popupAdsScene == Director::getInstance()->getRunningScene()) {
            _popupAdsScene = nullptr;
            
            AdLoadingLayerBase::loadingDoneCallback = CC_CALLBACK_0(ColorAdsManager::loadFinish, this);
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true, _popupAdsScene, 100);
        }
    }
}

void ColorAdsManager::loadFinish()
{
    setShowed(true);
}
