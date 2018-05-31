//
//  BlockLayer.cpp
//  ColorGarden
//
//  Created by luoxiaopeng on 15/8/4.
//
//

#include "BlockLayer.h"

bool BlockLayer::init()
{
    if(!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) return false;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->bool{
        return true;
    };
    listener->onTouchMoved = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    listener->onTouchEnded = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void BlockLayer::setLoadingPosition(const Vec2 &pos)
{
    auto rotateSprite = Sprite::create("other/load_rotate_icon.png");
    rotateSprite->setPosition(pos);
    addChild(rotateSprite);
    
    RotateBy* lRotateBy = RotateBy::create(1.0f, 360.0f);
    RepeatForever* lRepeatForever=RepeatForever::create(lRotateBy);
    rotateSprite->runAction(lRepeatForever);
}