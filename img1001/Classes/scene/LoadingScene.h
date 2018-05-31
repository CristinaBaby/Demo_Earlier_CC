//
//  LoadingScene.h
//  HH002
//
//  Created by houhuang on 15/8/26.
//
//

#ifndef __HH002__LoadingScene__
#define __HH002__LoadingScene__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../compoment/MapSmallSprite.hpp"
USING_NS_CC;

class LoadingScene : public Layer
{
public:
    bool init();
    SCENE_FUNC(LoadingScene);
    
    LoadingScene();
    ~LoadingScene();
    void gotoNextScene(float dt);
    
    void playAnimate();

    void doAction(Node* ref);
};

#endif /* defined(__HH002__LoadingScene__) */
