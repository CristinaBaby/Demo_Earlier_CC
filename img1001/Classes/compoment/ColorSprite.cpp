//
//  ColorSprite.cpp
//  ColorFree
//
//  Created by houhuang on 15/8/31.
//
//

#include "ColorSprite.h"
#include "../helper/ScreenAdapterHelper.h"

ColorSprite* ColorSprite::create(const std::string fileName, const std::string borderFile)
{
    ColorSprite* lSprite = new ColorSprite();
    if (lSprite && lSprite->init(fileName, borderFile))
    {
        lSprite->autorelease();
        return lSprite;
    }
    
    CC_SAFE_DELETE(lSprite);
    return NULL;
}

bool ColorSprite::init(const std::string fileName, const std::string borderFile)
{
    if (!Sprite::initWithFile(fileName))
    {
        return false;
    }

    m_bIsEnableTouch = true;
    
    Sprite* lSprite = Sprite::create(borderFile);
    lSprite->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
    this->addChild(lSprite);
    return true;
}

void ColorSprite::addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow /* = false */)
{
    _selectedCallBack = callBackFun;
    _target = target;
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(ColorSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ColorSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ColorSprite::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(ColorSprite::onTouchCancelled, this);
    listener->setSwallowTouches(isSwallow);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool ColorSprite::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    m_vFirstPoint = lPos;
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    if (m_bIsEnableTouch)
    {
        if (this->boundingBox().containsPoint(lPos))
        {
            
            return true;
        }
    }

    return false;
}

void ColorSprite::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}

void ColorSprite::onTouchEnded(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
//    lPos = Director::getInstance()->convertToGL(lPos);
//    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    float distance = sqrt(pow(lPos.x - m_vFirstPoint.x, 2) + pow(lPos.y - m_vFirstPoint.y, 2));
    if (distance < ScreenAdapterHelper::getValue(15))
    {
//        this->selected(true);
        if (_target && _selectedCallBack)
        {
            (_target->*_selectedCallBack)(this);
        }
    }
}

void ColorSprite::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void ColorSprite::selected(bool isColorBoard)
{
//    if (isColorBoard)
//    {
//        ScaleTo* lScaleTo=ScaleTo::create(0.2f, 1.1f);
//        EaseBackOut* lEackOut=EaseBackOut::create(lScaleTo);
//        ScaleTo* lScaleTo2=ScaleTo::create(0.1f, 1.0);
//        EaseBackIn* lEaseIn=EaseBackIn::create(lScaleTo2);
//        Sequence* lAction=Sequence::create(lEackOut,lEaseIn, NULL);
//        this->runAction(lAction);
//    }
}

void ColorSprite::setEnableTouch(bool pBool)
{
    m_bIsEnableTouch = pBool;
}
