//
//  LayerButton.h
//  IMG1001
//
//  Created by houhuang on 15/9/16.
//
//

#ifndef __IMG1001__LayerButton__
#define __IMG1001__LayerButton__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class LayerButton : public LayerColor
{
public:
    ~LayerButton();
    static LayerButton* create(Size size, Color4B color);
    bool init(Size size, Color4B color);
    
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

#endif /* defined(__IMG1001__LayerButton__) */
