//
//  UIButton.cpp
//  ColorFree
//
//  Created by houhuang on 15/8/28.
//
//

#include "UIButton.h"

UIButton* UIButton::create(const char* image)
{
    UIButton* lNode = new UIButton();
    if (lNode && lNode->init(image))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;
}

bool UIButton::init(const char* image)
{
    if (!Node::init())
    {
        return false;
    }
    
    _isClick = false;
    
    _normalSprite = Sprite::create(image);
    this->setContentSize(_normalSprite->getContentSize());
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    _normalSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(_normalSprite);
    _normalSprite->setVisible(true);
    
    
    _selectSprite = Sprite::create(image);
    _selectSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    _selectSprite->setColor(Color3B(96, 96, 96));
    _selectSprite->setOpacity(120);
    this->addChild(_selectSprite);
    _selectSprite->setVisible(false);
    
    this->addTouchListener(true);
    
    return true;
}

void UIButton::initListener(Ref* target, SEL_CallFuncN callBack)
{
    _target = target;
    _selector = callBack;
}

void UIButton::addTouchListener(bool isSwallow /* =true */)
{
    this->removeTouchListener();
    
    auto dispatcher = Director::Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(UIButton::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(UIButton::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(UIButton::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(UIButton::onTouchCancelled, this);
    listener->setSwallowTouches(isSwallow);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void UIButton::removeTouchListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

void UIButton::setButtonEnable(bool pBool)
{
    if (pBool)
    {
        this->addTouchListener(pBool);
    }else
    {
        this->removeTouchListener();
    }
}

bool UIButton::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    if (this->getBoundingBox().containsPoint(lPos))
    {
        _isClick = true;
        _normalSprite->setVisible(false);
        _selectSprite->setVisible(true);
        return true;
    }
    
    return false;
}

void UIButton::onTouchMoved(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    if (this->getBoundingBox().containsPoint(lPos))
    {
        _isClick = true;
        _normalSprite->setVisible(false);
        _selectSprite->setVisible(true);
    }else
    {
        _isClick = false;
        _normalSprite->setVisible(true);
        _selectSprite->setVisible(false);
    }
}

void UIButton::onTouchEnded(Touch *touch, Event *unused_event)
{
    _normalSprite->setVisible(true);
    _selectSprite->setVisible(false);
    
    if (_isClick)
    {
        if (_target && _selector)
        {
            (_target->*_selector)(this);
        }
    }
    
}

void UIButton::onTouchCancelled(Touch *touch, Event *unused_event)
{

}