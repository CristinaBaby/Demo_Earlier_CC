//
//  LoadingScene.h
//  HIDD
//
//  Created by lidong on 15-3-12.
//
//

#ifndef __HIDD__LoadingScene__
#define __HIDD__LoadingScene__

#include <stdio.h>
#include "BaseLayer.h"
#include "../define/GlobalMacro.h"

USING_NS_CC;
using namespace std;
class LoadingScene : public BaseLayer{

public:
    bool init();
    SCENE_FUNC(LoadingScene);
    void loadUI();

    void gotoNextScene(float dt);
};

#endif /* defined(__HIDD__LoadingScene__) */
