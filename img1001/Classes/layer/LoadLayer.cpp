//
//  LoadLayer.cpp
//  IMG1001
//
//  Created by houhuang on 15/9/23.
//
//

#include "LoadLayer.h"
#include "../helper/ScreenAdapterHelper.h"

LoadLayer::~LoadLayer()
{
    this->removeListener();
}

bool LoadLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 80))) return false;
    
    this->setTouchEnabled(true);
    
    Sprite* lSprite = Sprite::create("load_rotate_icon.png");
    ScreenAdapterHelper::setPosition(lSprite, Vec2(384, 512));
    this->addChild(lSprite);
    
    lSprite->runAction(RepeatForever::create(RotateBy::create(1.0, 250)));
    
    
    return true;
}

void LoadLayer::setTouchEnabled(bool pBool)
{
    if (pBool)
    {
        this->addListener();
    }else
    {
        this->removeListener();
    }
}

void LoadLayer::removeListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

void LoadLayer::addListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this](Touch* touch, Event* event)
    {
        return true;
    };
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}