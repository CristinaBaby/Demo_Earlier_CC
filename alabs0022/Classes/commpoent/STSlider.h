//
//  STSlider.h
//  HIDD
//
//  Created by lidong on 15-4-7.
//
//

#ifndef __HIDD__STSlider__
#define __HIDD__STSlider__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/GlobalMacro.h"

USING_NS_CC;
using namespace std;

class STSlider;
class STSliderDelegate  {
public:
   virtual void sliderChangeValue(STSlider* pSlider){}
};

class STSlider : public Node {
protected:
    float _progressLength;
    Sprite* _progressSprite;
    Sprite* _buttonSprite;
    float _minX;
    float _maxX;
public:
    st_property(float, _minValue, MinValue);
    st_property(float, _maxValue, MaxValue);
    st_property(float, _value, Value);
    st_property(STSliderDelegate*, _delegate, Delegate);
public:
    bool init(string pBgImage,string pProgressImage,string pBtnImage);
    static STSlider* create(string pBgImage,string pProgressImage,string pBtnImage);
    void changePosition(Vec2 pProgressVec,Vec2 pBtnVec);
    void changeValue(float pValue);
    
    bool isContainPosition(Vec2 pPoint);
    void moveX(float dX);
    void excuteBackData();
};

#endif /* defined(__HIDD__STSlider__) */
