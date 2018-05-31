//
//  SwallowListenerLayer.cpp
//  IMG1001
//
//  Created by houhuang on 15/12/1.
//
//

#include "SwallowListenerLayer.hpp"

SwallowListenerLayer* SwallowListenerLayer::create(const Color4B& color)
{
    SwallowListenerLayer* layer = new SwallowListenerLayer();
    if (layer && layer->init(color))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

SwallowListenerLayer::~SwallowListenerLayer()
{
    this->setTouchEnable(false);
}

bool SwallowListenerLayer::init(const Color4B& color)
{
    if (!LayerColor::initWithColor(color)) return false;
    
    
    return true;
}

void SwallowListenerLayer::setTouchEnable(bool pBool)
{
    if (pBool)
    {
        this->addTouchListener();
    }else
    {
        this->removeTouchListener();
    }
}

void SwallowListenerLayer::addTouchListener()
{
    this->removeTouchListener();
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(SwallowListenerLayer::onTouchBegan, this);
    listener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SwallowListenerLayer::removeTouchListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

bool SwallowListenerLayer::onTouchBegan(Touch* touch, Event* unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    lPos = Director::getInstance()->convertToGL(lPos);
    
    if (this->getBoundingBox().containsPoint(lPos))
    {
        return true;
    }
    
    return false;
}