//
//  AdLoadingLayerBase.h
//
//  Created by tanshoumei on 15/6/10.
//
//

#pragma once

#include <stdio.h>
#include <iostream>
#include "cocos2d.h"
//#include "../../IOS_Android_Include/AdsManager.h"
#include "../../crossplatformapi/headers/ads/AdsManager.h"
#include "../../define/Globalmacro.h"

using namespace cocos2d;
using namespace cocos2d::ui;

typedef enum
{
   eInterstitial_Normal_Type,
   eInterstitial_Rward_Type
}Ad_Interstitial_Type;

class AdLoadingLayerBase: public Layer, public AdsManagerDelegate
{
public:
    //用于判断当前是否有AdLoadingLayer(其子类也算)的实例存在.不为空，则可重复使用。
    //注意这不是单例，使用要进行空判断。
    //使用场景: 比如正在AdLoading界面，用户点击了后面，再返回，此时回来界面上是有AdLoadingLayer的实例的，可重复使用，当然你也可以选择重新创建，Depends on U.
    static AdLoadingLayerBase* s_currentInstance;
    
    template<class T>
    static void showLoading(bool isCross, Node* parent = nullptr, int zorder = 0)
    {
        if (!AdsManager::getInstance()->isFullScreenShowing())
        {
            auto call = [parent, zorder, isCross]()
            {
                if(!AdLoadingLayerBase::s_currentInstance)
                {
                    auto adLoadingLayer = T::create();
                    if (!parent)
                    {
                        Director::getInstance()->getRunningScene()->addChild(adLoadingLayer, zorder);
                    }
                    else
                    {
                        parent->addChild(adLoadingLayer, zorder);
                    }
                }
                s_adShowAction = nullptr;
                AdLoadingLayerBase::s_currentInstance->loadAd(isCross);
            };
            s_adShowAction = Sequence::create(CallFunc::create(call), NULL);
            //防止android黑屏
            Director::getInstance()->getRunningScene()->runAction(s_adShowAction);
        };
    }
    template<class T>
    static void showLoading_Reward(bool isCross, Node* parent = nullptr, int zorder = 0,int butIndex=0)
    {
      
        
        if (!AdsManager::getInstance()->isFullScreenShowing())
        {
            auto call = [parent, zorder, isCross,butIndex]()
            {
                if(!AdLoadingLayerBase::s_currentInstance)
                {
                    auto adLoadingLayer = T::create();
                
                    
                    if (!parent)
                    {
                        Director::getInstance()->getRunningScene()->addChild(adLoadingLayer, zorder);
                    }
                    else
                    {
                        parent->addChild(adLoadingLayer, zorder);
                    }
                }
                s_adShowAction = nullptr;
                AdLoadingLayerBase::s_currentInstance->loadReward(isCross);
                
                AdLoadingLayerBase::s_currentInstance->setButIndex(butIndex);
            };
            s_adShowAction = Sequence::create(CallFunc::create(call), NULL);
            //��叉��android榛�灞�
            Director::getInstance()->getRunningScene()->runAction(s_adShowAction);
        };
    }
    
    
    CREATE_FUNC(AdLoadingLayerBase);
    
    ~AdLoadingLayerBase();
    
    virtual void updateLayer(Ad_Interstitial_Type type){};
    
    virtual bool init();
    
    void onExit();
    
    void onEnter() override;
        
    void loadAd(bool abIscross = false);
    void loadReward(bool abIscross=false);
    void setSwallowTouch(bool value);
    
#pragma mark --ad delegate--

    virtual void onAdsLoaded(ADS_TYPE adType);
    virtual void onAdsLoadFailed(std::string error, ADS_TYPE adType);
    virtual void onAdsExpanded(ADS_TYPE adType);
    virtual void onAdsCollapsed(ADS_TYPE adType);
#pragma mark --ad delegate end--
       void onRewardedCollapsed(std::string unitId, int amount, bool bIsSkipped);
protected:
    /*
     为什么不直接用schedule update(1. update比较常用，如果子类也用，容易产生混乱。2.此处对时效性要求不高，也不需要每帧检测)
     **/
    virtual void _timeCheckSchedule(float dt);
    
    //广告按时加载出来
    virtual void _adLoadInTime();
    
    //广告超时还未加载出来
    virtual void _adLoadTimeOut();

    //本界面任务完成
    virtual void _taskDone();
public:
    //
    static std::function<void()> loadingDoneCallback;
    
    
    
public:
    //Loading界面最多停留的时间(秒)
    CC_SYNTHESIZE(float, _loadingMaxTime, LoadingMaxTime);
    
    CC_SYNTHESIZE(float, _loadingMinTime, LoadingMinTime);
    
protected:
    static Action* s_adShowAction;
    
    EventListenerTouchOneByOne* _eventListenerTouch;
    
    timeval _timeEnter;
    
    
    //0第三方 1交叉
    ADS_TYPE _adLoadType = kTypeInterstitialAds;
    
    //广告是否在显示
    bool _adShowing = false;
    
    bool _isRequestingLoadAd = false;
    
    bool _bLoadingCanRemove = false;    
    
    //广告加载完成与否(失败成功回调都算完成)
    bool _adLoadDone = false;
    
//HH
public:
//    int _useObjectsIndex;
    st_property(int, _butIndex, ButIndex);
    
};