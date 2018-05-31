//
//  LayerButton.cpp
//  IMG1001
//
//  Created by houhuang on 15/9/16.
//
//

#include "LayerButton.h"

LayerButton* LayerButton::create(Size size, Color4B color)
{
    LayerButton* layer = new LayerButton();
    if (layer && layer->init(size, color))
    {
        layer->autorelease();
        return layer;
    }

    CC_SAFE_DELETE(layer);
    return NULL;
}

LayerButton::~LayerButton()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

bool LayerButton::init(Size size, Color4B color)
{
    if (!LayerColor::initWithColor(color)) return false;

    this->setContentSize(size);

    return true;
}

void LayerButton::addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow /* = false */)
{
    _selectedCallBack = callBackFun;
    _target = target;
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(LayerButton::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerButton::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerButton::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(LayerButton::onTouchCancelled, this);
    listener->setSwallowTouches(isSwallow);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool LayerButton::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    m_vFirstPoint = lPos;
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    if (this->boundingBox().containsPoint(lPos))
    {
            
        return true;
    }
    
    return false;
}

void LayerButton::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}

void LayerButton::onTouchEnded(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    //    lPos = Director::getInstance()->convertToGL(lPos);
    //    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    float distance = sqrt(pow(lPos.x - m_vFirstPoint.x, 2) + pow(lPos.y - m_vFirstPoint.y, 2));
    if (distance < 10)
    {
        //        this->selected(true);
        if (_target && _selectedCallBack)
        {
            (_target->*_selectedCallBack)(this);
        }
    }
}

void LayerButton::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}
