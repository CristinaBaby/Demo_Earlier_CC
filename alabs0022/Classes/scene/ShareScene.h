//
//  ShareScene.h
//  ColorGarden
//
//  Created by lidong on 15/8/6.
//
//

#ifndef __ColorGarden__ShareScene__
#define __ColorGarden__ShareScene__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/GlobalMacro.h"
#include "BaseLayer.h"
USING_NS_CC;


class GameScene;
class ShareScene : public BaseLayer {
  
    Menu* _menu;
    Sprite* _frameSprite;
    string _imagePath;
public:
    bool init(int pIndex, GameScene* pGameScene);
    static Scene* scene(int pIndex,GameScene* pGameScene);
    void doAction(Ref* pSender);
    void loadUI();
    void delayTimeExcute(float dt);
    void saveShareImage();
    bool createDirectory(const char *path);

    void onEnterTransitionDidFinish();
    void delayeSaveImage(float dt);
public:
    st_property(int, _index, Index);
    st_property(GameScene*, _gameScene,GameScene);
    Image * _saveImage = nullptr;
};

#endif /* defined(__ColorGarden__ShareScene__) */
