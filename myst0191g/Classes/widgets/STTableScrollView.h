//
//  STTableScrollView.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-9.
//
//

#ifndef __LVUP002__STTableScrollView__
#define __LVUP002__STTableScrollView__

#include <cocos2d.h>
#include "cocos-ext.h"
#include "STPage.h"

class STTableScrollView;

/**
 *
 */
class STTableDelegate
{
public:
    virtual STPage* getANewPageAtIndex(unsigned index) = 0;
    
    virtual cocos2d::CCSize getPageSize() = 0;
    
    virtual unsigned getCapacity() = 0;
};



/**
 *
 */
class STTableClickedLisenter
{
public:
    
    /**
     *  @param view. STTableScrollView object pointer
     *  @param index. the index of STTableScrollView's page, which also is the position in array you pass by.
     */
    virtual void onPageClicked(cocos2d::CCNode* view, unsigned index) = 0;
};



/**
 *
 */
class TableListener;



/**
 *
 */
class STTableScrollView  :   public cocos2d::extension::CCTableView, public cocos2d::extension::CCTableViewDataSource
{
protected:
    STTableDelegate          *p_PageDelegate;
    STTableClickedLisenter   *l_OnPageClicked;
    
    TableListener           *p_mTableListener;
    
public:
    /**
     *  @param pages is the array which only contains STPage and its subclass.
     */
    static STTableScrollView* create(STTableDelegate* delegate, cocos2d::CCSize size, CCNode *container = NULL);
    
    
    STTableScrollView(STTableDelegate* delegate);
    virtual ~STTableScrollView();
    
    bool init(cocos2d::CCSize size, CCNode *container);
    
    //    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    //    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    //    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
public:
    
    virtual cocos2d::CCSize cellSizeForTable(CCTableView *table);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
public:
    
    void setOnPageClickListener(STTableClickedLisenter *lisenter);

};

#endif /* defined(__LVUP002__STTableScrollView__) */
