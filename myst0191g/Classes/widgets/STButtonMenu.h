//
//  STButtonMenu.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-19.
//
//

#ifndef __LVUP002__STButtonMenu__
#define __LVUP002__STButtonMenu__

#include "cocos2d.h"

class STButtonMenu : public cocos2d::CCMenu
{
public:
    STButtonMenu(){}
    virtual ~STButtonMenu(){}
    
    static STButtonMenu* create();
    
    static STButtonMenu* create(cocos2d::CCMenuItem* item, ...);
    
    static STButtonMenu* createWithArray(cocos2d::CCArray* pArrayOfItems);
    
    static STButtonMenu* createWithItem(cocos2d::CCMenuItem* item);
    
    static STButtonMenu* createWithItems(cocos2d::CCMenuItem *firstItem, va_list args);
    
    
    virtual bool initWithArray(cocos2d::CCArray* pArrayOfItems);
    //
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
protected:
    /**
     * this is used to avoid tap button frequently during a short time.
     * once you tap a button include this menu, it will be set disable, until touch ended or cancelled.
     */
    void resumeEnable(float);
    
};

#endif /* defined(__LVUP002__STButtonMenu__) */
