//
//  STIndicator.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-9.
//
//

#include "STIndicator.h"


USING_NS_CC;

#define DEFAULT_LAYER_COLOR         ccc4(0,0,0,0)
#define DEFAULT_MARGIN              10


STIndicator* STIndicator::create(const unsigned capacity, cocos2d::CCSpriteFrame *onFrame, cocos2d::CCSpriteFrame *offFrame)
{
    STIndicator *pobSprite = new STIndicator(onFrame, offFrame);
    if (pobSprite && pobSprite->initWithCapacity(capacity))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STIndicator* STIndicator::create(const unsigned capacity, std::string onFileName, std::string offFileName)
{
    STIndicator *pobSprite = new STIndicator(onFileName, offFileName);
    if (pobSprite && pobSprite->initWithCapacity(capacity))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STIndicator* STIndicator::create(const unsigned capacity, cocos2d::CCTexture2D *onTexture2D, cocos2d::CCTexture2D *offTexture2D)
{
    STIndicator *pobSprite = new STIndicator(onTexture2D, offTexture2D);
    if (pobSprite && pobSprite->initWithCapacity(capacity))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STIndicator* STIndicator::create(const unsigned capacity, const char *onFileName, const char *offFileName)
{
    STIndicator *pobSprite = new STIndicator(onFileName, offFileName);
    if (pobSprite && pobSprite->initWithCapacity(capacity))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}




STIndicator::STIndicator(CCSpriteFrame *onFrame, CCSpriteFrame *offFrame)
{
    mp_NormalFrame = onFrame;
    mp_SelectedFrame = offFrame;
    
    CC_SAFE_RETAIN(mp_NormalFrame);
    CC_SAFE_RETAIN(mp_SelectedFrame);
    
    STIndicator(onFrame->getTexture(), offFrame->getTexture());
}

STIndicator::STIndicator(std::string onFileName, std::string offFileName)
{
    mp_NormalFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(onFileName.c_str());
    mp_SelectedFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(offFileName.c_str());
    
    CC_SAFE_RETAIN(mp_NormalFrame);
    CC_SAFE_RETAIN(mp_SelectedFrame);
}

STIndicator::STIndicator(CCTexture2D *onTexture2D, CCTexture2D *offTexture2D)
{
    CCRect rect = CCRectZero;
    rect.size = onTexture2D->getContentSize();
    mp_NormalFrame = CCSpriteFrame::createWithTexture(onTexture2D, rect);
    
    rect.size = offTexture2D->getContentSize();
    mp_SelectedFrame = CCSpriteFrame::createWithTexture(offTexture2D, rect);
    
    CC_SAFE_RETAIN(mp_NormalFrame);
    CC_SAFE_RETAIN(mp_SelectedFrame);
}

STIndicator::STIndicator(const char *onFileName, const char *offFileName)
{
    CCTexture2D *onTexture = CCTextureCache::sharedTextureCache()->addImage(onFileName);
    CCTexture2D *offTexture = CCTextureCache::sharedTextureCache()->addImage(offFileName);
    
    if (onTexture && offTexture)
    {
        CCRect rect = CCRectZero;
        rect.size = onTexture->getContentSize();
        mp_NormalFrame = CCSpriteFrame::createWithTexture(onTexture, rect);
        
        rect.size = offTexture->getContentSize();
        mp_SelectedFrame = CCSpriteFrame::createWithTexture(offTexture, rect);
        
        CC_SAFE_RETAIN(mp_NormalFrame);
        CC_SAFE_RETAIN(mp_SelectedFrame);
    }
}

STIndicator::~STIndicator()
{
    CC_SAFE_RELEASE_NULL(mp_NormalFrame);
    CC_SAFE_RELEASE_NULL(mp_SelectedFrame);
    CC_SAFE_DELETE(mp_a_Dots);
}

bool STIndicator::initWithCapacity(const unsigned capacity)
{
    bool bRet = false;
    
    do
    {
        mp_a_Dots = new CCArray();
        mPreFocusPosition = 0;
        
        float dotWith, dotHeight;
        
        for (int i = 0; i < capacity; i++)
        {
            CCSprite *dot = NULL;
            if (i == mPreFocusPosition)
                dot = CCSprite::createWithSpriteFrame(mp_SelectedFrame);
            else
                dot = CCSprite::createWithSpriteFrame(mp_NormalFrame);
            
            if (!dot)
                return bRet;
            
            dotWith = dot->getContentSize().width;
            dotHeight = dot->getContentSize().height;
            
            this->addChild(dot);
            mp_a_Dots->addObject(dot);
            
            dot->setAnchorPoint(CCPointZero);
            dot->setPosition(ccp((i * (dotWith + DEFAULT_MARGIN)), 0));
        }
        
        const float layerWidth = dotWith + (dotWith + DEFAULT_MARGIN) * (capacity - 1);
        
        CC_BREAK_IF(! CCLayerColor::initWithColor(DEFAULT_LAYER_COLOR, layerWidth, dotHeight));
        
        
        
        bRet = true;
    } while (0);
    
    return bRet;
}

void STIndicator::onTurnPage(unsigned curPageIndex)
{
    if (curPageIndex == mPreFocusPosition)
        return;
    
    CCSprite *focus = (CCSprite *)mp_a_Dots->objectAtIndex(mPreFocusPosition);
    if (focus)
        focus->setDisplayFrame(mp_NormalFrame);
    
    mPreFocusPosition = curPageIndex;
    
    focus = (CCSprite *)mp_a_Dots->objectAtIndex(curPageIndex);
    if (focus)
        focus->setDisplayFrame(mp_SelectedFrame);
}


