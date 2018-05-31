//
//  ColorSprite.cpp
//  ColorGarden
//
//  Created by lidong on 15/8/4.
//
//

#include "ColorSprite.h"
#include "../helper/ScreenAdapterHelper.h"

ColorSprite* ColorSprite::create(string filename)
{
    ColorSprite* lColorSprite =new ColorSprite();
    if(lColorSprite && lColorSprite->initWithFile(filename))
    {
        lColorSprite->autorelease();
        return lColorSprite;
    }
    else
    {
        delete lColorSprite;
        lColorSprite = NULL;
        return NULL;
    }
}

ColorSprite::ColorSprite()
{

}

ColorSprite::~ColorSprite()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

bool ColorSprite::initWithFile(const std::string& filename)
{
    if (!Sprite::initWithFile(filename)) return false;
    _isSelected=false;
    _selectSprite=NULL;
    
    Sprite* bg = Sprite::create("rounded_stroke.png");
    bg->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(bg, -1);
  
    return true;
}

void ColorSprite::setClickSpite(bool isShow)
{
    if (_selectSprite)
    {
        return;
    }
    
    if (_colorData.R >= 238 && _colorData.G >= 238 && _colorData.B >= 238)
    {
        _selectSprite=Sprite::create("clickColor.png");
        _selectSprite->setColor(Color3B::BLACK);
    }else
    {
        _selectSprite=Sprite::create("clickColor.png");
    }

    _selectSprite->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
    this->addChild(_selectSprite);
    _selectSprite->setVisible(isShow);
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
    
    if (lPos.x < 55 || lPos.x > 700)
    {
        return false;
    }
//    if (m_bIsEnableTouch)
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

void ColorSprite::responseTouchEvent(Vec2 pPoint)
{
//    Vec2 lPosition=this->getParent()->convertToNodeSpace(pPoint);
//    if(this->boundingBox().containsPoint(lPosition))
    {
        this->selected(true);
        this->getDelegate()->reponseTouchEvent(this);
    }
 
}

void ColorSprite::selected(bool pBool)
{
    this->setIsSelected(pBool);
    
//    if(_selectSprite==NULL)
//    {
//        _selectSprite=Sprite::create("pencil.png");
//        _selectSprite->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
//        this->addChild(_selectSprite);
//        _selectSprite->setVisible(false);
//    }
    
    if(pBool)
    {
        _selectSprite->setVisible(true);
        _selectSprite->setOpacity(0);
        _selectSprite->runAction(FadeIn::create(0.2f));
  
//        ScaleTo* lScaleTo=ScaleTo::create(0.2f, 1.15f);
//        EaseBackOut* lEackOut=EaseBackOut::create(lScaleTo);
//        ScaleTo* lScaleTo2=ScaleTo::create(0.1f, 1.0);
//        EaseBackIn* lEaseIn=EaseBackIn::create(lScaleTo2);
//        Sequence* lAction=Sequence::create(lEackOut,lEaseIn, NULL);
//        this->runAction(lAction);
        
    }
    else
    {
        _selectSprite->setVisible(false);
    }
    
}