//
//  ScreenAdapterHelper.h
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#ifndef __HIDD__ScreenAdapterHelper__
#define __HIDD__ScreenAdapterHelper__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;


class ScreenAdapterHelper {
    
public:
    static void setupScreenAdapter(cocos2d::Size pScreenSize,cocos2d::Size pDesignSize);
    static cocos2d::Size& getRealDesignSize(); //缩放后屏幕真正的尺寸
    static cocos2d::Size& getDesignSize();     //匹配输入的尺寸
    static float getRealDesignScale();
    static void setPosition(Node* pNode,cocos2d::Vec2 pDesignPoint,bool bTop=false,bool bBottom=false,bool bLeft=false,bool bRight=false);
    static void setPosition_real(Node* pNode,cocos2d::Vec2 pDesignPoint,bool bTop=false,bool bBottom=false,bool bLeft=false,bool bRight=false);
    static cocos2d::Vec2 getTargetPoint(cocos2d::Vec2 pPoint,bool bTop=false, bool bBottom=false, bool bLeft=false);
    static  float getValue(float value);
    static cocos2d::Vec2 getPoint(const cocos2d::Vec2& pPoint);
};


#endif /* defined(__HIDD__ScreenAdapterHelper__) */
