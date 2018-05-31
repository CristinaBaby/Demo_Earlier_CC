//
//  STSlider.cpp
//  HIDD
//
//  Created by lidong on 15-4-7.
//
//

#include "STSlider.h"
#include "../helper/ControlHelper.h"

bool STSlider::init(string pBgImage, string pProgressImage, string pBtnImage)
{
    if(!Node::init()) return false;
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    //load background
    Sprite* lBgSprite=ControlHelper::addSpriteWithFrame(pBgImage.c_str(), this);
    this->setContentSize(lBgSprite->getContentSize());
    lBgSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));

    //load progress
    Sprite* lProgressSprite=ControlHelper::addSpriteWithFrame(pProgressImage.c_str(),this);
    _progressSprite=lProgressSprite;
    _progressLength=lProgressSprite->getContentSize().width;
    

    
    //load Btn
    Sprite* lButtonSprite=ControlHelper::addSpriteWithFrame(pBtnImage.c_str(), this);
    _buttonSprite=lButtonSprite;
    
    
    
    return true;
}

STSlider* STSlider::create(string pBgImage, string pProgressImage, string pBtnImage)
{
    STSlider* lSlider=new STSlider();
    if(lSlider && lSlider->init(pBgImage,  pProgressImage,  pBtnImage))
    {
        lSlider->autorelease();
        return lSlider;
    }
    
    CC_SAFE_DELETE(lSlider);
    return NULL;
}

void STSlider::changePosition(cocos2d::Vec2 pProgressVec, cocos2d::Vec2 pBtnVec)
{
    _progressSprite->setPosition(this->convertToNodeSpace(pProgressVec));
    _progressSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
    float lX=_progressSprite->getPosition().x-_progressLength/2;
    _progressSprite->setPosition(Vec2(lX, _progressSprite->getPosition().y));
    
    _minX=lX;
    _maxX=lX+_progressLength;
    _buttonSprite->setPosition(this->convertToNodeSpace(pBtnVec));
}

bool STSlider::isContainPosition(cocos2d::Vec2 pPoint)
{
    //CCLOG("pPoint X=%f Y=%f",pPoint.x,pPoint.y);
    Vec2 lPoint=this->convertToNodeSpace(pPoint);
    Rect lRect=_buttonSprite->getBoundingBox();
   // CCLOG("Rect X=%f Y=%f W=%f H=%f",lRect.origin.x,lRect.origin.y,lRect.size.width,lRect.size.height);
    return _buttonSprite->getBoundingBox().containsPoint(lPoint);
}

void STSlider::moveX(float dX)
{
    float lX=_buttonSprite->getPosition().x+dX;
    lX=MAX(_minX, MIN(lX,_maxX));

    //_buttonSprite的资源有问题才会出现移动两端不平衡的问题
    
    _buttonSprite->setPosition(Vec2(lX, _buttonSprite->getPosition().y));
    float lDistanceX=_buttonSprite->getPosition().x-_minX;
    _progressSprite->setScaleX(lDistanceX/_progressLength);
    this->setValue((lDistanceX/_progressLength)*(_maxValue-_minValue));
    
//    if(_delegate!=NULL)
//    {
//        _delegate->sliderChangeValue(this);
//    }
}

void STSlider::excuteBackData()
{
    if(_delegate!=NULL)
    {
        _delegate->sliderChangeValue(this);
    }
}


void STSlider::changeValue(float pValue)
{
    float lDistanceX=pValue/(_maxValue-_minValue) * _progressLength;
    float lValue=lDistanceX+_minX;
    _buttonSprite->setPosition(lValue, _buttonSprite->getPosition().y);
    _progressSprite->setScaleX(lDistanceX/_progressLength);
    
}
