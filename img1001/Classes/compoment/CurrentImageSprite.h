//
//  CurrentImageSprite.h
//  ColorFree
//
//  Created by houhuang on 15/9/8.
//
//

#ifndef __ColorFree__CurrentImageSprite__
#define __ColorFree__CurrentImageSprite__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class CurrentImageSprite : public LayerColor
{
public:
    CREATE_FUNC(CurrentImageSprite);
    bool init();
    
public:
    std::string getImageName_w();
};

#endif /* defined(__ColorFree__CurrentImageSprite__) */
