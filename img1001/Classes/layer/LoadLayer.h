//
//  LoadLayer.h
//  IMG1001
//
//  Created by houhuang on 15/9/23.
//
//

#ifndef __IMG1001__LoadLayer__
#define __IMG1001__LoadLayer__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class LoadLayer : public LayerColor
{
public:
    CREATE_FUNC(LoadLayer);
    bool init();
    
    ~LoadLayer();
public:
    void addListener();
    void removeListener();
    
    void setTouchEnabled(bool pBool);
};

#endif /* defined(__IMG1001__LoadLayer__) */
