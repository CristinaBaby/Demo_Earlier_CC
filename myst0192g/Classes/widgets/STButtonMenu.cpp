//
//  STButtonMenu.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-19.
//
//

#include "STButtonMenu.h"

#define CLICK_INTERVAL  0.25

USING_NS_CC;

STButtonMenu* STButtonMenu::create()
{
    return create(NULL, NULL);
}

STButtonMenu* STButtonMenu::create(CCMenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    STButtonMenu *pRet = STButtonMenu::createWithItems(item, args);
    
    va_end(args);
    
    return pRet;
}

STButtonMenu* STButtonMenu::createWithArray(CCArray* pArrayOfItems)
{
    STButtonMenu *pRet = new STButtonMenu();
    if (pRet && pRet->initWithArray(pArrayOfItems))
    {
        pRet->setPosition(CCPointZero);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

STButtonMenu* STButtonMenu::createWithItem(CCMenuItem* item)
{
    return STButtonMenu::create(item, NULL);
}

STButtonMenu* STButtonMenu::createWithItems(CCMenuItem *firstItem, va_list args)
{
    CCArray* pArray = NULL;
    if( firstItem )
    {
        pArray = CCArray::create(firstItem, NULL);
        CCMenuItem *i = va_arg(args, CCMenuItem*);
        while(i)
        {
            pArray->addObject(i);
            i = va_arg(args, CCMenuItem*);
        }
    }
    
    return STButtonMenu::createWithArray(pArray);
}

bool STButtonMenu::initWithArray(CCArray* pArrayOfItems)
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(! CCMenu::initWithArray(pArrayOfItems));
        
        /*
         *  the CoverLayer have the same priority as menu, so we use "kCCMenuHandlerPriority - 1",
         *  ensure this menu can receieve the touch event before other menus and CoverLayer.
         
            // this->setTouchPriority(kCCMenuHandlerPriority);
        */
        
        bRet = true;
        
    } while (0);
    
    return bRet;
}

void STButtonMenu::resumeEnable(float)
{
    this->setEnabled(true);
}

bool STButtonMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    const bool flag = CCMenu::ccTouchBegan(touch, event);
    
    // if this flag is true, means user clicked one button, we disable this CCMenu,
    // and enable it after CLICK_INTERVAL second(s).
    if (flag)
    {
        this->setEnabled(false);
    }
    
    return flag;
}

void STButtonMenu::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCMenu::ccTouchEnded(touch, event);
    scheduleOnce(schedule_selector(STButtonMenu::resumeEnable), CLICK_INTERVAL);
}

void STButtonMenu::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
    CCMenu::ccTouchCancelled(touch, event);
    scheduleOnce(schedule_selector(STButtonMenu::resumeEnable), CLICK_INTERVAL);
}

void STButtonMenu::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
     CCMenu::ccTouchMoved(touch, event);
}


