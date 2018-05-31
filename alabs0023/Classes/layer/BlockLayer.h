//
//  BlockLayer.h
//  ColorGarden
//
//  Created by lidong on 15/8/4.
//
//

#ifndef __ColorGarden__BlockLayer__
#define __ColorGarden__BlockLayer__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class BlockLayer : public LayerColor{

public:
    CREATE_FUNC(BlockLayer);
    virtual bool init();
};


#endif /* defined(__ColorGarden__BlockLayer__) */
