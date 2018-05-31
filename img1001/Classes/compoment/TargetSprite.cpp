//
//  TargetSprite.cpp
//  IMG1001
//
//  Created by houhuang on 15/9/23.
//
//

#include "TargetSprite.h"
TargetSprite* TargetSprite::create(const std::string& filename)
{
    TargetSprite *sprite = new (std::nothrow) TargetSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void TargetSprite::onEnterTransitionDidFinish()
{
    Sprite::onEnterTransitionDidFinish();
    if(_activitySprite!=NULL)
    {
        _activitySprite->removeFromParentAndCleanup(true);
        _activitySprite=NULL;
    }
}
