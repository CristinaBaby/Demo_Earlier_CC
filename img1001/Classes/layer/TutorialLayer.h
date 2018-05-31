//
//  TutorialLayer.h
//  IMG1001
//
//  Created by houhuang on 15/9/15.
//
//

#ifndef __IMG1001__TutorialLayer__
#define __IMG1001__TutorialLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../define/Globalmacro.h"
USING_NS_CC;
USING_NS_CC_EXT;

class TutorialLayer : public LayerColor
{
public:
    CREATE_FUNC(TutorialLayer);
    bool init();
    
    ~TutorialLayer();
public:
    void scheduleDelaye(float dt);

};

#endif /* defined(__IMG1001__TutorialLayer__) */
