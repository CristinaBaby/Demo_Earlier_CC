//
//  ScreenAdapterHelper.cpp
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#include "ScreenAdapterHelper.h"
#include "DeviceHelper.h"
static cocos2d::Size _screenSize;
static cocos2d::Size _designSize;
static cocos2d::Size _realDesignSize;

static cocos2d::Vec2 _topLeftOffset;
static cocos2d::Vec2 _designOffset;

static float _realDesignScale;

void ScreenAdapterHelper::setupScreenAdapter(cocos2d::Size pScreenSize,cocos2d::Size pDesignSize)
{
    _screenSize=pScreenSize;
    _designSize=pDesignSize;
    
    CCLOG("sw=%f sh=%f",pScreenSize.width,pScreenSize.height);
    
    float lScaleX=_screenSize.width/_designSize.width;
    float lScaleY=_screenSize.height/_designSize.height;
    
    float lScale=0.0f;
    if(lScaleX>lScaleY)
    {
        lScale=lScaleX/lScaleY;
        // _topLeftOffset=ccp(_designSize.width*(lScale-1)/2.0f,0);
    }
    else
    {
        lScale=lScaleY/lScaleX;
        // _topLeftOffset=ccp(0,_designSize.height*(lScale-1)/2.0f);
    }
    _realDesignScale=lScale;
    
    _realDesignSize=cocos2d::Vec2(_designSize.width*lScale, _designSize.height*lScale);
    _designOffset=cocos2d::Vec2((_realDesignSize.width-_designSize.width)/2,(_realDesignSize.height-_designSize.height)/2);

}
Size& ScreenAdapterHelper::getRealDesignSize() //缩放后屏幕真正的尺寸
{
    return _realDesignSize;
}
Size& ScreenAdapterHelper::getDesignSize()    //匹配输入的尺寸
{
     return _designSize;
}
float ScreenAdapterHelper::getRealDesignScale()
{
    return _realDesignScale;
}
void ScreenAdapterHelper::setPosition(Node* pNode,cocos2d::Vec2 pDesignPoint,bool bTop,bool bBottom,bool bLeft,bool bRight)
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
void ScreenAdapterHelper::setPosition_real(Node* pNode,cocos2d::Vec2 pDesignPoint,bool bTop,bool bBottom,bool bLeft,bool bRight)
{
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

Vec2 ScreenAdapterHelper::getTargetPoint(cocos2d::Vec2 pPoint,bool bTop, bool bBottom, bool bLeft)
{
    if(DeviceHelper::getInstance()->getIs2048Device())
    {
        pPoint=pPoint*2;
    }
    
    float posX = pPoint.x + _designOffset.x;
    float posY = pPoint.y + _designOffset.y;
    if(bTop)
        posY += _designOffset.y;
    else if(bBottom)
        posY -= _designOffset.y;
    
    if(bLeft)
        posX -= _designOffset.x;
    
    return Vec2(posX,posY);
}
float ScreenAdapterHelper::getValue(float value)
{
    if(DeviceHelper::getInstance()->getIs2048Device())
    {
        value=value*2;
    }
    
    return value;
}
cocos2d::Vec2 ScreenAdapterHelper::getPoint(const cocos2d::Vec2& pPoint)
{
    cocos2d::Vec2 lPoint=pPoint;
    if(DeviceHelper::getInstance()->getIs2048Device())
    {
        lPoint=lPoint*2;
    }
    return lPoint;
}
