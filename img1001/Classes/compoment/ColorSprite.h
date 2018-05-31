//
//  ColorSprite.h
//  ColorFree
//
//  Created by houhuang on 15/8/31.
//
//

#ifndef __ColorFree__ColorSprite__
#define __ColorFree__ColorSprite__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../data/DataManager.h"
USING_NS_CC;

class ColorSprite : public Sprite
{
public:
    static ColorSprite* create(const std::string fileName, const std::string borderFile);
    bool init(const std::string fileName, const std::string borderFile);
    
public:
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
public:
    void addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow = false);
    void selected(bool isColorBoard);
    void setEnableTouch(bool pBool);
  
public:
    st_property(ColorData, _colorData, ColorData);
    st_property(std::string, _galleryName, GalleryName);
protected:
    Sprite* m_pSelectedSpr;
    bool    m_bIsSelected;
    Vec2    m_vFirstPoint;
    
    SEL_CallFuncN _selectedCallBack;
    Node*         _target;
    
    bool    m_bIsEnableTouch;
};

#endif /* defined(__ColorFree__ColorSprite__) */
