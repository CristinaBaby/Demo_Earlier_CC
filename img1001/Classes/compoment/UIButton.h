//
//  UIButton.h
//  ColorFree
//
//  Created by houhuang on 15/8/28.
//
//

#ifndef __ColorFree__UIButton__
#define __ColorFree__UIButton__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class UIButton : public Node
{
public:
    static UIButton* create(const char* image);
    bool init(const char* image);
    
public:
    void initListener(Ref* target, SEL_CallFuncN callBack);
    
    void addTouchListener(bool isSwallow = true);
    void removeTouchListener();
    void setButtonEnable(bool pBool);
    
public:
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
public:
    SEL_CallFuncN _selector;
    Ref* _target;

    Sprite* _normalSprite;
    Sprite* _selectSprite;
    
    bool _isClick;
};

#endif /* defined(__ColorFree__UIButton__) */
