//
//  STMapScrollView.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-15.
//
//

#ifndef __LVUP002__STMapScrollView__
#define __LVUP002__STMapScrollView__

#include "cocos-ext.h"

class GameMapLayer;

class STMapScrollView : public cocos2d::extension::CCScrollView
{
    GameMapLayer* m_pMaplayer;
    cocos2d::CCPoint mStartPoint;
    
public:
    STMapScrollView(GameMapLayer* maplayer);
    virtual ~STMapScrollView(){};
    
    static STMapScrollView* create(GameMapLayer* maplayer, cocos2d::CCSize size, cocos2d::CCNode* container = NULL);
    static STMapScrollView* create(GameMapLayer* maplayer);
    
    bool initWithViewSize(cocos2d::CCSize size, CCNode* container = NULL);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void scrollByHint(cocos2d::CCPoint &pos);
    
private:
    void checkBounds();
    bool errorTap(cocos2d::CCTouch *pTouch);
};

#endif /* defined(__LVUP002__STMapScrollView__) */
