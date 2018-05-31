//
//  ScreenAdapterHelper.cpp
//  HH002
//
//  Created by houhuang on 15/8/25.
//
//

#include "ScreenAdapterHelper.h"
#include "DeviceHelper.h"


static Size _screenSize;
static Size _designSize;
static Size _realDesignSize;

static Vec2 _designOffset;

static float _realDesignScale;


void ScreenAdapterHelper::setupScreenAdapter(Size pScreenSize, Size pDesignSize)
{
    _screenSize = pScreenSize;
    _designSize = pDesignSize;
    
    float lScaleX = _screenSize.width / _designSize.width;
    float lScaleY = _screenSize.height / _designSize.height;
    
    float lScale = 0.0f;
    if (lScaleX > lScaleY)
    {
        lScale = lScaleY / lScaleX;
    }else
    {
        lScale = lScaleY / lScaleX;
    }
    
    _realDesignScale = lScale;
    
    _realDesignSize = Vec2(_designSize.width * _realDesignScale, _designSize.height * _realDesignScale);
    
    _designOffset = Vec2((_realDesignSize.width - _designSize.width) / 2, (_realDesignSize.height - _designSize.height)/2);
}

Size& ScreenAdapterHelper::getRealDesignSize()
{
    return _realDesignSize;
}

Size& ScreenAdapterHelper::getDesignSize()
{
    return _designSize;
}

float ScreenAdapterHelper::getRealDesignScale()
{
    return _realDesignScale;
}

float ScreenAdapterHelper::getValue(float value)
{
    if (DeviceHelper::getInstance()->getIs2048Device())
    {
        value = value * 2;
    }
    
    return value;
}

void ScreenAdapterHelper::setPosition(Node* node, Vec2 pDesignPoint, VerticalBoarderType vType, HerizontalBoarderType hType)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 originPoint = Director::getInstance()->getVisibleOrigin();
    
    if (DeviceHelper::getInstance()->getIs2048Device())
    {
        pDesignPoint *= 2;
    }
    
    if (vType & KVBoarderTypeNone)
    {
        pDesignPoint.y = pDesignPoint.y + _designOffset.y;
//        pDesignPoint.y = pDesignPoint.y + originPoint.y;
    }
    else if (vType & KVBoarderTypeTop)
    {
        pDesignPoint.y = visibleSize.height +originPoint.y - pDesignPoint.y;
    }
    else if (vType & KVBoarderTypeBottom)
    {
        pDesignPoint.y = pDesignPoint.y + originPoint.y;
    }
    
    if (hType & KHBoarderTypeNone)
    {
        pDesignPoint.x = pDesignPoint.x + _designOffset.x;
//        pDesignPoint.x = pDesignPoint.x + originPoint.x;
    }
    else if (hType & KHBoarderTypeLeft)
    {
        pDesignPoint.x = pDesignPoint.x + originPoint.x;
    }
    else if (hType & kHBoarderTypeRight)
    {
        pDesignPoint.x = visibleSize.width + originPoint.x - pDesignPoint.x;
    }
    
    if (node != nullptr)
    {
        node->setPosition(pDesignPoint);
    }
}

void ScreenAdapterHelper::setPosition_offset(Node* pNode,Vec2 pDesignPoint,bool bTop,bool bBottom,bool bLeft,bool bRight)
{
    if(DeviceHelper::getInstance()->getIs2048Device())
    {
        pDesignPoint=pDesignPoint*2;
    }
    float posX = pDesignPoint.x + _designOffset.x;
    float posY = pDesignPoint.y + _designOffset.y;
    if(bTop)
        posY += _designOffset.y;
    else if(bBottom)
        posY -= _designOffset.y;
    
    if(bLeft)
        posX -= _designOffset.x;
    
    else if(bRight)
    {
        posX += _designOffset.x;
    }
    
    pNode->setPosition(Vec2(posX, posY));
}
