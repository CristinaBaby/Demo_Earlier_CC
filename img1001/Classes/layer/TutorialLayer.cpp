//
//  TutorialLayer.cpp
//  IMG1001
//
//  Created by houhuang on 15/9/15.
//
//

#include "TutorialLayer.h"
#include "../helper/ScreenAdapterHelper.h"

bool TutorialLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 150))) return false;
    
    Sprite* lTotorial = Sprite::create("tutorial.png");
    ScreenAdapterHelper::setPosition(lTotorial, Vec2(384, 512));
    this->addChild(lTotorial);
    
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)
    {
        this->scheduleOnce(schedule_selector(TutorialLayer::scheduleDelaye), 0.1f);
        return true;
    };
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

TutorialLayer::~TutorialLayer()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

void TutorialLayer::scheduleDelaye(float dt)
{
    this->removeFromParentAndCleanup(true);
}