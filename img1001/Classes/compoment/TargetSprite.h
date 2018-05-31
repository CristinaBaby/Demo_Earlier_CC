//
//  TargetSprite.h
//  IMG1001
//
//  Created by houhuang on 15/9/23.
//
//

#ifndef __IMG1001__TargetSprite__
#define __IMG1001__TargetSprite__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
USING_NS_CC;
class TargetSprite : public Sprite {
    
    
public:
    static TargetSprite* create(const std::string& filename);
    void onEnterTransitionDidFinish();
public:
    st_property(Sprite*, _activitySprite, ActivitySprite);
};

#endif /* defined(__IMG1001__TargetSprite__) */
