//
//  AdLoadingLayerDoll.cpp
//  AdsDemo
//
//  Created by zengwenbin on 15/8/31.
//
//

#include "AdLoadingLayerDoll.h"
#include "../../helper/ScreenAdapterHelper.h"
//#include "../../IOS_Android_Include/AdsManager.h"
#include "../../crossplatformapi/headers/ads/AdsManager.h"

AdLoadingLayerDoll::~AdLoadingLayerDoll()
{
//    AdsManager::getInstance()->showAds(kTypeBannerAds);
}

bool AdLoadingLayerDoll::init(){
    if(!AdLoadingLayerBase::init()){
        return false;
    }
  
//    AdsManager::getInstance()->removeAds(kTypeBannerAds);
    
    Sprite* loading = Sprite::create("loading.png");
    ScreenAdapterHelper::setPosition(loading, Vec2(384, 512));
    loading->setScale(ScreenAdapterHelper::getRealDesignScale());
    this->addChild(loading);
    
    Sprite* loading_label = Sprite::create("loading_text.png");
    loading_label->setPosition(Vec2(loading->getContentSize().width/2, loading->getContentSize().height/2));
    loading->addChild(loading_label);
    
    _loadSprite = loading;
    _loadSprite->setVisible(false);
    
    return true;
}

void AdLoadingLayerDoll::updateLayer(Ad_Interstitial_Type type)
{
    if (type == eInterstitial_Rward_Type)
    {
        _loadSprite->setVisible(false);
    }
    else if (type == eInterstitial_Normal_Type)
    {
        _loadSprite->setVisible(true);
    }
   
}
