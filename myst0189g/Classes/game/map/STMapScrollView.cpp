//
//  STMapScrollView.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-15.
//
//

#include "STMapScrollView.h"
#include "../../AppGlobal.h"
#include "GameMapLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;


#define MOVE_INCH            7.0f/160.0f

static float convertDistanceFromPointToInch(float pointDis)
{
    float factor = ( CCEGLView::sharedOpenGLView()->getScaleX() + CCEGLView::sharedOpenGLView()->getScaleY() ) / 2;
    return pointDis * factor / CCDevice::getDPI();
}



static bool isReceiveTouch = false;
static bool isMoved        = false;

STMapScrollView::STMapScrollView(GameMapLayer* maplayer)
: mStartPoint(CCPointZero)
{
    m_pMaplayer = maplayer;
}

STMapScrollView* STMapScrollView::create(GameMapLayer* maplayer, CCSize size, CCNode* container /*= NULL*/)
{
    STMapScrollView* pRet = new STMapScrollView(maplayer);
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
    
}

STMapScrollView* STMapScrollView::create(GameMapLayer* maplayer)
{
    STMapScrollView* pRet = new STMapScrollView(maplayer);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool STMapScrollView::initWithViewSize(CCSize size, CCNode* container)
{
    if (CCScrollView::initWithViewSize(size, container))
    {
        m_fMinScale = 1.0f;
        m_fMaxScale = 1.5f;
        return true;
    }
    return false;
}


bool STMapScrollView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    isReceiveTouch = true;
    isMoved = false;
    
    mStartPoint = this->convertTouchToNodeSpace(pTouch);
    
    return CCScrollView::ccTouchBegan(pTouch, pEvent);
}

void STMapScrollView::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    isReceiveTouch = false;
    isMoved = false;
    mStartPoint = CCPointZero;
    
    CCScrollView::ccTouchCancelled(pTouch, pEvent);
}


void STMapScrollView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchMoved(pTouch, pEvent);
    
    if (m_pTouches->containsObject(pTouch))
    {
        isMoved = errorTap(pTouch);
    }
    
    checkBounds();
}


void STMapScrollView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    // if this this node receive the touch event, and not move anymore, it's a error tap
    if (isReceiveTouch && !isMoved)
    {
        m_pMaplayer->showErrorTip(pTouch);
    }
    
    CCScrollView::ccTouchEnded(pTouch, pEvent);
    
    this->unscheduleAllSelectors();
    checkBounds();
    
    isReceiveTouch = false;
    isMoved = false;
    mStartPoint = CCPointZero;
}

bool STMapScrollView::errorTap(CCTouch *pTouch)
{
    CCPoint moveDistance, newPoint;
    newPoint     = this->convertTouchToNodeSpace(pTouch);
    moveDistance = ccpSub(newPoint, mStartPoint);
    
    float dis = sqrtf(moveDistance.x*moveDistance.x + moveDistance.y*moveDistance.y);
    
    if (fabs(convertDistanceFromPointToInch(dis)) < MOVE_INCH )
    {
        return false;
    }
 
    return true;
}

void STMapScrollView::checkBounds()
{
    CCPoint oldPoint;
    float newX, newY;
    
    CCPoint  min, max;
    min = this->minContainerOffset();
    max = this->maxContainerOffset();
    
    const float gapX = fabsf(min.x);
    
    float maxX    =  0;
    float minX    = -gapX;
    
    oldPoint = m_pContainer->getPosition();
    
    newX     = oldPoint.x;
    newY     = oldPoint.y;
    
    if (m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionHorizontal)
    {
        newX    = MAX(newX, minX);
        newX    = MIN(newX, maxX);
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

void STMapScrollView::scrollByHint(cocos2d::CCPoint &pos)
{
    CCSize realBGSiz = m_pContainer->getContentSize();
    
    float realOffsetX = fabsf(pos.x / realBGSiz.width);
    float realOffsetY = fabsf(pos.y / realBGSiz.height);
    
    
    CCPoint min = this->minContainerOffset();
    CCPoint max = this->maxContainerOffset();
    
    const float gapX = fabsf(min.x);
    
    min.setPoint((-gapX * 1.5f), min.y);
    max.setPoint((gapX * 0.5f), max.y);
    
    
    CCPoint scrollRange = max - min;
    
    CCPoint posInScroll = CCPointMake((min.x + scrollRange.x * realOffsetX), (min.y + scrollRange.y * realOffsetY));
    
    CCPoint centerPoint = ccp((max.x - scrollRange.x / 2), (max.y - scrollRange.y / 2));
    
    CCPoint sgap = centerPoint - posInScroll;
    
    
    this->setContentOffset(sgap * 2);
    
    
    checkBounds();
}




