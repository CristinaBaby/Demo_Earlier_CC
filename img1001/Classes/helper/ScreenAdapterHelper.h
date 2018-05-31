//
//  ScreenAdapterHelper.h
//  HH002
//
//  Created by houhuang on 15/8/25.
//
//

#ifndef __HH002__ScreenAdapterHelper__
#define __HH002__ScreenAdapterHelper__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

/************坐标设置************/
enum VerticalBoarderType
{
    KVBoarderTypeNone   =   1 << 0,
    KVBoarderTypeTop    =   1 << 1,
    KVBoarderTypeBottom =   1 << 2,
};

enum HerizontalBoarderType
{
    KHBoarderTypeNone   =   1 << 0,
    KHBoarderTypeLeft   =   1 << 3,
    kHBoarderTypeRight  =   1 << 4,
};

class ScreenAdapterHelper
{
public:
    static void setupScreenAdapter(Size pScreenSize, Size pDesignSize);
    
    static Size& getRealDesignSize();
    static Size& getDesignSize();
    static float getRealDesignScale();
    static float getValue(float value);
    
    static void setPosition(Node* node, Vec2 pDesignPoint, VerticalBoarderType vType = KVBoarderTypeNone, HerizontalBoarderType hType = KHBoarderTypeNone);
    
    static void setPosition_offset(Node* pNode,Vec2 pDesignPoint,bool bTop=false,bool bBottom=false,bool bLeft=false,bool bRight=false);
};

#endif /* defined(__HH002__ScreenAdapterHelper__) */
