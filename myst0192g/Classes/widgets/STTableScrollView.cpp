//
//  STTableScrollView.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-9.
//
//

#include "STTableScrollView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TableListener : public CCTableViewDelegate
{
    STTableClickedLisenter *mp_lisenter;
    CCTouch *pTouch;
public:
    TableListener(STTableClickedLisenter *lisenter) : pTouch(NULL)
    {
        mp_lisenter = lisenter;
    }
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    
    void setTouch(CCTouch *touch)
    {
        pTouch = touch;
        pTouch->retain();
    }
};

void TableListener::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    if (pTouch)
    {
        STPage* cellPage = dynamic_cast<STPage*>(cell);
        CCNode* pItemCell = cellPage->onClickedPage(pTouch);
        
        if (pItemCell)
        {
            ItemCell* item = dynamic_cast<ItemCell*>(pItemCell->getUserObject());
            
            mp_lisenter->onPageClicked(pItemCell, item->getItemCellIndex());
        }
    }
    
    CC_SAFE_RELEASE(pTouch);
}

/* ============================================================================== */


STTableScrollView::STTableScrollView(STTableDelegate* delegate)
: l_OnPageClicked(NULL), p_mTableListener(NULL)
{
    p_PageDelegate = delegate;
}

STTableScrollView::~STTableScrollView()
{
    CC_SAFE_DELETE(p_mTableListener);
}

STTableScrollView* STTableScrollView::create(STTableDelegate* delegate, CCSize size, CCNode *container /* = NULL */)
{
    STTableScrollView* pRet = new STTableScrollView(delegate);
    if (pRet && pRet->init(size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool STTableScrollView::init(CCSize size, CCNode *container)
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(!CCTableView::initWithViewSize(size, container));
        
        setDataSource(this);
        _updateCellPositions();
        _updateContentSize();
        
        bRet = true;
    } while (0);
    
    return bRet;
}

CCSize STTableScrollView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return this->cellSizeForTable(table);
    
}

CCSize STTableScrollView::cellSizeForTable(CCTableView *table)
{
    if (!p_PageDelegate)
    {
        return CCSizeZero;
    }
    else
    {
        return p_PageDelegate->getPageSize();
    }
}

CCTableViewCell* STTableScrollView::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    STPage* page = dynamic_cast<STPage*>( table->dequeueCell() );
    
    if (NULL == page)
    {
        if (p_PageDelegate)
        {
            page = p_PageDelegate->getANewPageAtIndex(idx);
        }
    }
    else
    {
        page->reset();
    }
    
    page->refreshPage(idx);
    
    return page;
}

unsigned int STTableScrollView::numberOfCellsInTableView(CCTableView *table)
{
    if (p_PageDelegate)
    {
       return p_PageDelegate->getCapacity();
    }
    
    return 0;
}

void STTableScrollView::setOnPageClickListener(STTableClickedLisenter *lisenter)
{
    l_OnPageClicked = lisenter;
    
    CC_SAFE_DELETE(p_mTableListener);
    
    p_mTableListener = new TableListener(l_OnPageClicked);
    this->setDelegate(p_mTableListener);
    
}

void STTableScrollView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (p_mTableListener)
    {
        p_mTableListener->setTouch(pTouch);
    }
    
    CCTableView::ccTouchEnded(pTouch, pEvent);
}


