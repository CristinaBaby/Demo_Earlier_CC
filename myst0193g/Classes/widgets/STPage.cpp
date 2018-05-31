//
//  STPage.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-9.
//
//

#include "STPage.h"

USING_NS_CC;


STPage* STPage::create()
{
    STPage* pRet = new STPage();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    if (pRet && pRet->initWithColor(ccc4(0,0,0,0), s.width, s.height))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

STPage * STPage::create(const ccColor4B& color)
{
    STPage * pLayer = new STPage();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    if(pLayer && pLayer->initWithColor(color, s.width, s.height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

bool STPage::initWithColor(const ccColor4B& color, GLfloat width, GLfloat height)
{
    bool bRet = false;
    do
    {
        pBaseLayer = CCLayerColor::create(color, width, height);
        CC_BREAK_IF(!pBaseLayer);
        this->addChild(pBaseLayer);
        
        this->setContentSize(pBaseLayer->getContentSize());
        
        bRet = true;
    } while (0);
    
    return bRet;
}

STPage::STPage()
{
}

STPage::~STPage()
{
}

