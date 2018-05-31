//
//  STTouchableSprite.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#include "STTouchableSprite.h"

USING_NS_CC;

STTouchableSprite* STTouchableSprite::createWithTexture(CCTexture2D *pTexture)
{
    STTouchableSprite *pobSprite = new STTouchableSprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STTouchableSprite* STTouchableSprite::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
    STTouchableSprite *pobSprite = new STTouchableSprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STTouchableSprite* STTouchableSprite::create(const char *pszFileName)
{
    STTouchableSprite *pobSprite = new STTouchableSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STTouchableSprite* STTouchableSprite::create(const char *pszFileName, const CCRect& rect)
{
    STTouchableSprite *pobSprite = new STTouchableSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STTouchableSprite* STTouchableSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    STTouchableSprite *pobSprite = new STTouchableSprite();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STTouchableSprite* STTouchableSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pFrame != NULL, msg);
#endif
    
    return createWithSpriteFrame(pFrame);
}

STTouchableSprite* STTouchableSprite::create()
{
    STTouchableSprite *pSprite = new STTouchableSprite();
    if (pSprite && pSprite->init())
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool STTouchableSprite::isHitSprite(CCTouch *touch)
{
	// return OpenGL coord
	CCPoint touchPoint = touch->getLocation();
	// convert to parent view space coord
	CCPoint realPoint = this->getParent()->convertToNodeSpace(touchPoint);
	// base on parent view space coord
	CCRect rect = this->boundingBox();
    
	return rect.containsPoint(realPoint);
}

void STTouchableSprite::setTouchPriority(int priority)
{
    if (m_iTouchPriority != priority)
    {
        m_iTouchPriority = priority;
        
		if(m_bRunning)
        {
            CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
            CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_iTouchPriority, true);
		}
    }
}

void STTouchableSprite::onEnter()
{
	// must call parten's method
	CCSprite::onEnter();
    
	// add this sprite to receive touch events, and consume this event
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_iTouchPriority, true);
}

void STTouchableSprite::onExit()
{
	// remove listener from the targeted touch delegate's list
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	//
	CCSprite::onExit();
}

void STTouchableSprite::setOnClickedListener(CCObject *rec, SEL_CallFuncN selector)
{
    m_pListener = rec;
    m_pfnSelector = selector;
}

bool STTouchableSprite::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return (m_bEnabled && isHitSprite(pTouch));
}

void STTouchableSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    // ensure when touch end the touch point is also in this sprite rect
	if(isHitSprite(pTouch))
	{
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
	}
}

void STTouchableSprite::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
}

void STTouchableSprite::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

STTouchableSprite::STTouchableSprite() : m_bEnabled(true), m_pListener(NULL), m_pfnSelector(NULL),
m_iTouchPriority(kCCMenuHandlerPriority + 1)
{
}

STTouchableSprite::~STTouchableSprite()
{
}
