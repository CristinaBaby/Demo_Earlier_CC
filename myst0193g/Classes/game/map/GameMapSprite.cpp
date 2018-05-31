//
//  GameMapSprite.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#include "GameMapSprite.h"
#include "../../AppGlobal.h"

USING_NS_CC;

GameMapSprite* GameMapSprite::createWithSpriteFrameName(const char *pszSpriteFrameName, GameMapLayer* maplayer)
{
    CCSpriteFrame *pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pSpriteFrame != NULL, msg);
#endif
    
    GameMapSprite *pobSprite = new GameMapSprite(maplayer);
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

bool GameMapSprite::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    bool inRect = STTouchableSprite::ccTouchBegan(pTouch, pEvent);
	
    return (inRect && mIsTarget);
}

void GameMapSprite::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    STTouchableSprite::ccTouchMoved(pTouch, pEvent);
    mpGameMapLayer->spreadTouch(false, pTouch);
}

void GameMapSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    mpGameMapLayer->spreadTouch(true, pTouch, isHitSprite(pTouch));
    STTouchableSprite::ccTouchEnded(pTouch, pEvent);
}

void GameMapSprite::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    mpGameMapLayer->spreadTouch(true, pTouch);
    STTouchableSprite::ccTouchCancelled(pTouch, pEvent);
}

void GameMapSprite::fadeoutAndRemove()
{
    /*
     *  we use CCSpriteBatchNode to draw sprites for fast render speed, but remove child from CCSpriteBatchNode is
     *  very slow, so we just set visibility of this sprite instead of remove it from its parent actually.
     */
    
    // set it can't recive any touch event
    setEnabled(false);
    
	// when move up and fade out animation end, remove sprite
	CCAction*  action = CCSequence::create(
                                           CCSpawn::create(CCScaleBy::create(DISAPPEAR_ANIMATION_TIME, 1.5f),
                                                           CCFadeOut::create(DISAPPEAR_ANIMATION_TIME), NULL),
                                           CCCallFuncN::create(mpGameMapLayer, callfuncN_selector(GameMapLayer::removeHiddObject)),
                                           NULL);
    
	this->runAction(action);
}

GameMapSprite::GameMapSprite(GameMapLayer* maplayer) : mIsTarget(false)
{
    mpGameMapLayer = maplayer;
}

GameMapSprite::~GameMapSprite()
{
}