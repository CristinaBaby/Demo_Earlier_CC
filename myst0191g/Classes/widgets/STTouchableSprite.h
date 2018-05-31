//
//  STTouchableSprite.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#ifndef __LVUP002__STTouchableSprite__
#define __LVUP002__STTouchableSprite__

#include <cocos2d.h>

/**
 *  @brief the special sprite can receive touch events. if you set selector, it will call the selector when touch ended.
 */
class STTouchableSprite : public cocos2d::CCSprite, public cocos2d::CCTargetedTouchDelegate
{
protected:
    /** whether or not the menu will receive events */
    bool m_bEnabled;
    
public:
    
    static STTouchableSprite* create();
    
    static STTouchableSprite* create(const char *pszFileName);
    
    static STTouchableSprite* create(const char *pszFileName, const cocos2d::CCRect& rect);
    
    static STTouchableSprite* createWithTexture(cocos2d::CCTexture2D *pTexture);
    
    static STTouchableSprite* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect);
    
    static STTouchableSprite* createWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame);
    
    static STTouchableSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
    
    STTouchableSprite(void);
    virtual ~STTouchableSprite(void);
    
    virtual void onEnter();
    virtual void onExit();
    
protected:
    /*
     * @brief check if touch point in this sprite's rect
     */
	bool isHitSprite(cocos2d::CCTouch *touch);
    
    //
    CCObject*                   m_pListener;
    cocos2d::SEL_CallFuncN      m_pfnSelector;
    
    int m_iTouchPriority;

protected:
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
public:
    /** set the target/selector of the sprite*/
    virtual void setOnClickedListener(CCObject *target, cocos2d::SEL_CallFuncN selector);
    
    virtual bool isEnabled() { return m_bEnabled; }
    virtual void setEnabled(bool value) { m_bEnabled = value; };
    
    /** priority of the touch events. Default is kCCMenuHandlerPriority */
    virtual void setTouchPriority(int priority);
    virtual int getTouchPriority(){ return m_iTouchPriority; };
};

#endif /* defined(__LVUP002__STTouchableSprite__) */
