//
//  MoveMode.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__MoveMode__
#define __ColorBook__MoveMode__

#include "cocos2d.h"
#include "BaseMode.h"

class MoveMode : public BaseMode
{
public:
    MoveMode(GameLayer * layer);
    virtual void onTouchesMoved(const vector<Touch*>& touches, Event *unused_event);
    void start();
};

#endif /* defined(__ColorBook__MoveMode__) */
