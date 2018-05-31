//
//  GameScene.h
//  HH002
//
//  Created by houhuang on 15/8/28.
//
//

#ifndef __HH002__GameScene__
#define __HH002__GameScene__

#include <stdio.h>
#include "cocos2d.h"
#include "../base/BaseScene.hpp"

USING_NS_CC;
//using namespace std;

class GameLayer;
class GameUILayer;
class GameScene : public BaseScene
{
public:
    ~GameScene();
    virtual bool init();
    static Scene* create();
    
    void onEnterTransitionDidFinish();
public:
    void pauseGame();
    void delayeRemoveTextureCache(float dt);
    
public:
    GameLayer* getGameLayer();
    GameUILayer* getGameUILayer();
    
    int getIndex();
    std::string getGalleryName();
    
public:
    int m_iIndex;
    std::string m_sGallery;
};


#endif /* defined(__HH002__GameScene__) */
