//
//  LoadingScene.hpp
//  ALABS0002NEW
//
//  Created by houhuang on 16/8/8.
//
//

#ifndef LoadingScene_hpp
#define LoadingScene_hpp

#include "UITools.h"

class LoadingScene : public Scene
{
public:
    CREATE_FUNC(LoadingScene);
    bool init();

    void gotoChapterScene(float dt);
};

#endif /* LoadingScene_hpp */
