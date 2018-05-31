//
//  STPageController.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-14.
//
//

#ifndef __LVUP002__STPageController__
#define __LVUP002__STPageController__

#include <cocos2d.h>
#include "STTableScrollView.h"
#include "STIndicator.h"

typedef enum
{
    turnRightPage, turnLeftPage,
}
turnPageDirection;


class STPageControllerListener : public STTableClickedLisenter
{
public:
    /**
     *  when turn page, call this method
     *
     *  @param focusPageIndex, current focus page index
     */
    virtual void onTurnPage(unsigned focusPageIndex) = 0;
};


class STPageController : public STTableScrollView
{
    cocos2d::CCPoint            mStartPoint;
    
protected:
    unsigned                    mFocusPageIndex;
    STIndicator                 *m_pIndicator;
    STPageControllerListener    *m_pPageControllerListener;
    
public:
    STPageController(STTableDelegate* delegate);
    virtual ~STPageController();
    
    static STPageController* create(STTableDelegate* delegate, cocos2d::CCSize size, CCNode *container = NULL);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
public:
    /** */
    void setIndicator(STIndicator* indicator);
    
    void setPageControllerListener(STPageControllerListener* listener);
    
    void turnPage(turnPageDirection dire);
    
    void turnToPage(unsigned pageIndex);
    
    const unsigned getFocusPageIndex()
    {
        return mFocusPageIndex;
    }
    
protected:
    void setOnPageClickListener(STTableClickedLisenter *lisenter)
    {
        STTableScrollView::setOnPageClickListener(lisenter);
    };
    
private:
    // 
    void relocateContainer();
    
};

#endif /* defined(__LVUP002__STPageController__) */
