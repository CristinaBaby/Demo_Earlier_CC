//
//  AdsLoadingLayer.cpp
//  SpotDifferences
//
//  Created by maxiang on 6/12/15.
//
//

#include "AdsLoadingLayer.h"
#include "AdsManager.h"
#include "DeviceHelper.h"


bool AdsLoadingLayer::init()
{
    if (!AdLoadingLayerBase::init()) {
        return false;
    }

    bool rcode = false;
    
    do {
        loadStudio();

        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        rcode = true;
    } while (0);
    
    
    return rcode;
}

void AdsLoadingLayer::loadStudio()
{
    string filename;
    if(DeviceHelper::getInstance()->getIs2048Device())
    {
        filename = "ui-2048X1536/loading.png";
    }
    else
    {
        filename = "ui-1024X768/loading.png";
    }
    
    auto bg = Sprite::create(filename);
    bg->setPosition(getContentSize()/2);
    addChild(bg);
    
    auto scaleX = getContentSize().width / bg->getContentSize().width;
    auto scaleY = getContentSize().height / bg->getContentSize().height;
    bg->setScale(scaleX > scaleY ? scaleX : scaleY);
}

void AdsLoadingLayer::onEnter(){
    AdLoadingLayerBase::onEnter();
    if (Director::getInstance()->getRunningScene()->getName() != "Home") {
        AdsManager::getInstance()->pauseAds(kTypeBannerAds);
    }
}

void AdsLoadingLayer::onExit(){
    AdLoadingLayerBase::onExit();
    if (Director::getInstance()->getRunningScene()->getName() != "Home") {
        AdsManager::getInstance()->resumeAds(kTypeBannerAds);
    }
}