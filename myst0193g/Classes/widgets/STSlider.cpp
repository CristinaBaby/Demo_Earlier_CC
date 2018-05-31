//
//  STSlider.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#include "STSlider.h"

USING_NS_CC;

STSlider* STSlider::create(CCSprite * backgroundSprite, CCSprite* pogressSprite, CCSprite* thumbSprite)
{
    return create(backgroundSprite, pogressSprite, thumbSprite, NULL, NULL);
}

STSlider* STSlider::create(CCSprite * backgroundSprite, CCSprite* pogressSprite, CCSprite* thumbSprite,
                 CCObject* target, SEL_CallFuncN selector)
{
    STSlider *pRet = new STSlider();
    pRet->initWithSprites(backgroundSprite, pogressSprite, thumbSprite);
    pRet->setTarget(target, selector);
    pRet->autorelease();
    return pRet;
}

void STSlider::setTarget(CCObject *rec, SEL_CallFuncN selector)
{
    m_pListener = rec;
    m_pfnSelector = selector;
}

void STSlider::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCControlSlider::ccTouchEnded(pTouch, pEvent);
    
    if (m_pListener && m_pfnSelector)
    {
        (m_pListener->*m_pfnSelector)(this);
    }
}

STSlider::STSlider() : m_pListener(NULL), m_pfnSelector(NULL)
{
}

STSlider::~STSlider()
{
}

