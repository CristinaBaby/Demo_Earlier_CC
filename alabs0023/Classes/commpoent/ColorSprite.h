//
//  ColorSprite.h
//  ColorGarden
//
//  Created by lidong on 15/8/4.
//
//

#ifndef __ColorGarden__ColorSprite__
#define __ColorGarden__ColorSprite__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/GlobalMacro.h"
#include "../game/DataManager.h"
using namespace cocos2d;
using namespace std;

class ColorSprite;
class ColorSpriteDelegate {
public:
    virtual void reponseTouchEvent(ColorSprite* pColorSprite){}
};

class ColorSprite : public Sprite {
    
    Sprite* _selectSprite;
public:
    ColorSprite();
    ~ColorSprite();
    static ColorSprite* create(string filename);
    bool initWithFile(const std::string& filename);
    void responseTouchEvent(Vec2 pPoint);
    void selected(bool pBool);
    
    void setClickSpite(bool isShow);
public:
    st_property(int, _index, Index);
    st_property(ColorData, _colorData, ColorData);
    st_property(bool, _isSelected, IsSelected);
    st_property(ColorSpriteDelegate*, _delegate, Delegate);
    
    st_property(float, _oriPositionX, OriPositionX);
    st_property(float, _oriPositionY, OriPositionY);
    
public:
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
public:
    void addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow = false);

protected:
    SEL_CallFuncN _selectedCallBack;
    Node*         _target;
    Vec2    m_vFirstPoint;
};


#endif /* defined(__ColorGarden__ColorSprite__) */
