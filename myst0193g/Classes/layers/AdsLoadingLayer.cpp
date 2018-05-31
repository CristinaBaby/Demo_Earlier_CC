//
//  AdsLoadingLayer.cpp
//  SpotDifferences
//
//  Created by maxiang on 6/12/15.
//
//

#include "AdsLoadingLayer.h"


#include "../widgets/STUILayer.h"
#include "../utilities/STUtility.h"
#include "../AppGlobal.h"
#include "../FileModifiers.h"
#include "../helpers/AdHelper.h"
bool AdsLoadingLayer::init()
{
    if(!AdLoadingLayerBase::init()) return false;
    
    
    bool rcode = false;
    
    do {
        

    
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("load-home.plist");
        
          CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
        
        CCSprite *p_Bg = CCSprite::create("ui01_menu_bg.jpg");
        CC_BREAK_IF(!p_Bg);
        p_Bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        this->addChild(p_Bg);
        
        
        STUILayer* uiLayer = STUILayer::create();
        CC_BREAK_IF(!uiLayer);
        this->addChild(uiLayer);
        
//        //
//        CCSprite* progressBG = CCSprite::createWithSpriteFrameName("ui09_bar_bg.png");
//        progressBG->setPosition(ccp_horizontal_center(690));
//        progressBG->setAnchorPoint(ccp(.5, .5));
//        uiLayer->addChild(progressBG);
        
        
        CCSprite* text = CCSprite::createWithSpriteFrameName("ui09_loading.png");
        text->setPosition(ccp_horizontal_center(600));
        uiLayer->addChild(text);
        
       this->setTouchEnabled(true);
        
        rcode = true;
    } while (0);
    
    return rcode;
}
#pragma mark - Touch Event

void AdsLoadingLayer::registerWithTouchDispatcher(void){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
}

bool AdsLoadingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    return true;
}


