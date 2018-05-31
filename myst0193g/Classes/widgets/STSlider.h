//
//  STSlider.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#ifndef __LVUP002__STSlider__
#define __LVUP002__STSlider__

#include "cocos2d.h"
#include "cocos-ext.h"

class STSlider : public cocos2d::extension::CCControlSlider
{
public:
    /**
     * Creates a slider with a given background sprite and a progress bar and a
     * thumb item.
     *
     * @see initWithBackgroundSprite:progressSprite:thumbMenuItem:
     */
    static STSlider* create(cocos2d::CCSprite * backgroundSprite, cocos2d::CCSprite* pogressSprite, cocos2d::CCSprite* thumbSprite);
    
    static STSlider* create(cocos2d::CCSprite * backgroundSprite, cocos2d::CCSprite* pogressSprite, cocos2d::CCSprite* thumbSprite,
                            CCObject* target, cocos2d::SEL_CallFuncN selector);
    
    STSlider();
    virtual ~STSlider();
    
    //    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    //    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    //    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    /** set the target/selector of the slider*/
    void setTarget(CCObject *target, cocos2d::SEL_CallFuncN selector);
    
protected:
    cocos2d::CCObject*          m_pListener;
    cocos2d::SEL_CallFuncN      m_pfnSelector;
};

#endif /* defined(__LVUP002__STSlider__) */
