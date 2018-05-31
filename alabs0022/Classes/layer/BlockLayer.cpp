//
//  BlockLayer.cpp
//  ColorGarden
//
//  Created by lidong on 15/8/4.
//
//

#include "BlockLayer.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../helper/ControlHelper.h"
#include "../commpoent/ColorSprite.h"
#include "../game/DataManager.h"
#include "../define/ResoureMacro.h"
#include "../game/GameUILayer.h"
#include "../helper/DeviceHelper.h"


bool BlockLayer::init()
{
    if(!LayerColor::init()) return false;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->bool{
        return true;
    };
    listener->onTouchMoved = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    listener->onTouchEnded = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto layer = LayerColor::create(Color4B(0, 0, 0, 0), 1024, 1024);
    ScreenAdapterHelper::setPosition(layer, Vec2(384, 645));
    
    auto rotateSprite = Sprite::create("load_rotate_icon.png");
    rotateSprite->setPosition(layer->getPosition());
    addChild(rotateSprite);
    
    RotateBy* lRotateBy=RotateBy::create(1.0f, 360.0f);
    RepeatForever* lRepeatForever=RepeatForever::create(lRotateBy);
    rotateSprite->runAction(lRepeatForever);

    return true;
}