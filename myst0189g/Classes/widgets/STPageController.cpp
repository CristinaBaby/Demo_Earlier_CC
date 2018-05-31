//
//  STPageController.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-14.
//
//

#include "STPageController.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define MOVE_DISTANCE_OFFSET            (1 / 7.f)

#define TAG_INDICATOR                   40

STPageController::STPageController(STTableDelegate* delegate)
: STTableScrollView(delegate),
m_pPageControllerListener(NULL), m_pIndicator(NULL), mFocusPageIndex(0), mStartPoint()
{
}

STPageController::~STPageController()
{
}

STPageController* STPageController::create(STTableDelegate* delegate, cocos2d::CCSize size, CCNode *container /*= NULL*/)
{
    STPageController* pRet = new STPageController(delegate);
    if (pRet && pRet->init(size, container))
    {
        pRet->setDirection(kCCScrollViewDirectionHorizontal);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

void STPageController::setIndicator(STIndicator* indicator)
{
    m_pIndicator = indicator;
    if (m_pIndicator)
    {
        m_pIndicator->setTag(TAG_INDICATOR);
    }
}

void STPageController::setPageControllerListener(STPageControllerListener* listener)
{
    m_pPageControllerListener = listener;
    this->setOnPageClickListener(m_pPageControllerListener);
}

bool STPageController::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    bool bRet = STTableScrollView::ccTouchBegan(pTouch, pEvent);
    if (bRet)
    {
        mStartPoint = m_pContainer->getPosition();
    }
    
    return bRet;
}

void STPageController::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    STTableScrollView::ccTouchMoved(pTouch, pEvent);
}

void STPageController::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    STTableScrollView::ccTouchCancelled(pTouch, pEvent);
    mStartPoint = CCPointZero;
}

void STPageController::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    STTableScrollView::ccTouchEnded(pTouch, pEvent);
    
    // must call this after super class method
    relocateContainer();
    
    mStartPoint = CCPointZero;
}


void STPageController::relocateContainer()
{
    // stop the bounce animation which set in super class CCScrollView
    this->unscheduleAllSelectors();
    
    
    CCPoint oldPoint, min, max;
    float newX, newY;
    
    min = this->minContainerOffset();
    max = this->maxContainerOffset();
    
    
    oldPoint = m_pContainer->getPosition();
    
    newX     = oldPoint.x;
    newY     = oldPoint.y;
    
    const float itemWidth = this->cellSizeForTable(this).width;
    const float tolerance = itemWidth * MOVE_DISTANCE_OFFSET;
    
    CCPoint moveDelta = ccpSub(mStartPoint, oldPoint);
    
    if (m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionHorizontal)
    {
        if (fabsf(moveDelta.x) < tolerance)
        {
            newX = mStartPoint.x;
        }
        else
        {
            if (moveDelta.x < 0)
            {
                this->turnPage(turnLeftPage);
            }
            else if (moveDelta.x >0)
            {
                this->turnPage(turnRightPage);
            }
        }
        
        
        newX     = MAX(newX, min.x);
        newX     = MIN(newX, max.x);
    }
    
    if (m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionVertical)
    {
        newY     = MIN(newY, max.y);
        newY     = MAX(newY, min.y);
    }
    
    if (newY != oldPoint.y || newX != oldPoint.x)
    {
        this->setContentOffset(ccp(newX, newY));
    }
}

void STPageController::turnPage(turnPageDirection dire)
{
    switch (dire)
    {
        case turnLeftPage:
        {
            if (mFocusPageIndex == 0)
                return;
            mFocusPageIndex --;
        }
            break;
            
        case turnRightPage:
        {
            if (mFocusPageIndex == (numberOfCellsInTableView(this) - 1)
                || p_PageDelegate->getCapacity() == 0)
                return;
            mFocusPageIndex ++;
        }
            break;
            
        default:
            break;
    }
    
    float delta = 0.f;
    
    if (m_eDirection == kCCScrollViewDirectionHorizontal)
    {
        
        delta = cellSizeForTable(this).width * mFocusPageIndex;
        this->setContentOffset(ccp(-delta, 0), true);
    }
    else if (m_eDirection == kCCScrollViewDirectionVertical)
    {
        delta = cellSizeForTable(this).height * mFocusPageIndex;
        this->setContentOffset(ccp(0, delta), true);
    }
    
    // inform indicator
    if (m_pIndicator)
    {
        m_pIndicator->onTurnPage(mFocusPageIndex);
    }
    
    if (m_pPageControllerListener)
    {
        m_pPageControllerListener->onTurnPage(mFocusPageIndex);
    }
}

void STPageController::turnToPage(unsigned pageIndex)
{
    if (pageIndex == mFocusPageIndex || pageIndex >= (numberOfCellsInTableView(this)))
        return;
    
    mFocusPageIndex = pageIndex;
    
    float delta = 0.f;
    if (m_eDirection == kCCScrollViewDirectionHorizontal)
    {
        delta = cellSizeForTable(this).width * mFocusPageIndex;
        this->setContentOffset(ccp(-delta, 0), true);
    }
    else if (m_eDirection == kCCScrollViewDirectionVertical)
    {
        delta = cellSizeForTable(this).height * mFocusPageIndex;
        this->setContentOffset(ccp(0, delta), true);
    }
    
    // inform indicator
    if (m_pIndicator)
    {
        m_pIndicator->onTurnPage(mFocusPageIndex);
    }
    
    if (m_pPageControllerListener)
    {
        m_pPageControllerListener->onTurnPage(mFocusPageIndex);
    }

        
}




