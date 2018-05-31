//
//  AdLoadingLayerDoll.cpp
//  AdsDemo
//
//  Created by zengwenbin on 15/8/31.
//
//

#include "AdLoadingLayerDoll.h"
#include "../../helper/ScreenAdapterHelper.h"


bool AdLoadingLayerDoll::init(){
    if(!AdLoadingLayerBase::init()){
        return false;
    }
  
    Sprite* loading = Sprite::create("loading.png");
    ScreenAdapterHelper::setPosition(loading, Vec2(384, 512));
    loading->setScale(ScreenAdapterHelper::getRealDesignScale());
    this->addChild(loading);
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
