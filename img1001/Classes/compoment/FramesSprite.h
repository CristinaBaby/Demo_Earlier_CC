//
//  FramesSprite.h
//  ColorFree
//
//  Created by houhuang on 15/9/8.
//
//

#ifndef __ColorFree__FramesSprite__
#define __ColorFree__FramesSprite__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class FramesSprite : public LayerColor
{
public:
    static FramesSprite* create(string framesName);
    static FramesSprite* create();
    bool init(string framesName);
    bool init();
    
    
public:
    string getImageName_w();
};

#endif /* defined(__ColorFree__FramesSprite__) */
