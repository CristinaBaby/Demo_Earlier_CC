//
//  AdLoadingLayerBase.cpp
//
//  Created by tanshoumei on 15/6/10.
//
//

#include "AdLoadingLayerBase.h"
#include "SimpleAudioEngine.h"
#include "../AppConfig.h"
using namespace CocosDenshion;

AdLoadingLayerBase* AdLoadingLayerBase::s_currentInstance = NULL;

AdLoadingLayerBase::AdLoadingLayerBase():
_adLoadType(0),
_adShowing(false),
_bLoadingCanRemove(false),
_isRequestingLoadAd(false),
_adLoadDone(false)
{
}

bool AdLoadingLayerBase::init()
{
    CCLayer::init();
    
    m_pTarget = NULL;
    m_pfnSelector = NULL;
    
    _adLoadType = 0;
    _loadingMinTime = 0;
    _loadingMaxTime = 5;
    
    return true;
}

void AdLoadingLayerBase::onExit()
{
    CCLayer::onExit();
    
    _ads.setAdsDelegate(NULL);
    
    s_currentInstance = NULL;
    
}

void AdLoadingLayerBase::onEnter()
{
    CCLayer::onEnter();
    
    s_currentInstance = this;
    
    gettimeofday(&_timeEnter, NULL);
    
    schedule(schedule_selector(AdLoadingLayerBase::_timeCheckSchedule));
    
    _ads.setAdsDelegate(this);
    
    loadAd();
}

void AdLoadingLayerBase::_timeCheckSchedule(float dt)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    float lTimePasses = now.tv_sec - _timeEnter.tv_sec;
    if ( ((lTimePasses >= _loadingMinTime) && _adLoadDone))
    {
        _adLoadInTime();
    }
    else if (lTimePasses >=_loadingMaxTime)
    {
        _adLoadTimeOut();
    }
    
}

void AdLoadingLayerBase::loadAd(bool abIscross)
{
    _adLoadType = abIscross;
    
    _isRequestingLoadAd = false;
    
    if(abIscross)
    {
        bool ret = _ads.showCrosspromo();
        if(!ret)
        {
            _isRequestingLoadAd = true;
        }
    }
    else
    {
        bool ret = _ads.showInterstitial();
        if(!ret)
        {
            _isRequestingLoadAd = true;
            _ads.preloadInterstitial();
        }
        else
        {
            _adLoadTimeOut();
        }
    }
}

void AdLoadingLayerBase::setLoadingDoneCallback(cocos2d::CCObject *target, SEL_CallFunc callback)
{
    m_pTarget = target;
    m_pfnSelector = callback;
}

void AdLoadingLayerBase::onInterstitialAdFailed(int errorCode, int type)
{
    if(_adLoadType == type)
        _adLoadDone = true;
}

void AdLoadingLayerBase::onInterstitialShown(int type)
{
    if(_adLoadType == type)
    {
        _adLoadDone = true;
        _adShowing  = true;
    }
}

void AdLoadingLayerBase::onInterstitialAdLoaded(int type)
{
    if(_adLoadType == type && _isRequestingLoadAd)
        _ads.showInterstitial();
}

void AdLoadingLayerBase::onInterstitialDismissed(int type)
{
    if(_adLoadType == type)
    {
        if(_bLoadingCanRemove)
        {
            _taskDone();
            return;
        }
        _adShowing = false;
    }
}

void AdLoadingLayerBase::_adLoadInTime()
{
    //广告已关闭或加载失败
    if(!_adShowing)
    {
        _taskDone();
    }
    //广告正在显示
    else
    {
        _bLoadingCanRemove = true;
    }
}

void AdLoadingLayerBase::_adLoadTimeOut()
{
    //广告已关闭或加载失败
    if(!_adShowing)
    {
        _taskDone();
    }
    //广告正在显示
    else
    {
        _bLoadingCanRemove = true;
    }
}

void AdLoadingLayerBase::_taskDone()
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(st_pauselayer_request_ad_notification);
    
    unschedule(schedule_selector(AdLoadingLayerBase::_timeCheckSchedule));
    removeFromParent();
    if(m_pTarget && m_pfnSelector)
    {
        (m_pTarget->*m_pfnSelector)();
    };
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotifyAdLoadingLayerDone);
}