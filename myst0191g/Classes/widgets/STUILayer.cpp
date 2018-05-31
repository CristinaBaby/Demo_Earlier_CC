//
//  STUILayer.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-24.
//
//

#include "STUILayer.h"
#include "AppGlobal.h"

USING_NS_CC;

STUILayer* STUILayer::create()
{
    STUILayer* pRet = new STUILayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

STUILayer * STUILayer::create(const ccColor4B& color, GLfloat width, GLfloat height)
{
    STUILayer * pLayer = new STUILayer();
    if( pLayer && pLayer->initWithColor(color,width,height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

STUILayer * STUILayer::create(const ccColor4B& color)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    STUILayer * pLayer = new STUILayer();
    
    if(pLayer && pLayer->initWithColor(color, s.width, s.height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

bool STUILayer::initWithColor(const ccColor4B& color, GLfloat width, GLfloat height)
{
    bool pRet = CCLayerColor::initWithColor(color, width, height);
    
    this->setScale(AppGlobal::UI_SCALE);
    
    return pRet;
}


