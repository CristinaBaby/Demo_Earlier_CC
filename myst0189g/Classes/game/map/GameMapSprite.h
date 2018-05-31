//
//  GameMapSprite.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#ifndef __LVUP002__GameMapSprite__
#define __LVUP002__GameMapSprite__

#include <cocos2d.h>
#include "GameMapLayer.h"
#include "../../widgets/STTouchableSprite.h"

class GameMapSprite : public STTouchableSprite
{
    GameMapLayer *mpGameMapLayer;
public:
    //
    static GameMapSprite* createWithSpriteFrameName(const char *pszSpriteFrameName, GameMapLayer* maplayer);
    
    GameMapSprite(GameMapLayer* maplayer);
    virtual ~GameMapSprite();
    
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
public:
	/**
     *  set whether this sprite can be removed.
     *  if this sprite's name in the list,this can be removed.
     */
	void setIsTarget(bool boo)
    {
        mIsTarget = boo;
    }
    
    bool isTarget()
    {
        return mIsTarget;
    }
    
    void fadeoutAndRemove();
    
protected:
    bool mIsTarget;
    
    //    void setParent(GameMapLayer *parent);
    
private:
	// when the targeted object is clicked,  play particle animation,
	// and fadeout, move up animation
    //	void playDisappearAnimation();
    
public:
    //	// remove sprite
    //	void disappear();
    //
    //    /*
    //     * @brief
    //     */
    //    void showHint();
};

#endif /* defined(__LVUP002__GameMapSprite__) */
