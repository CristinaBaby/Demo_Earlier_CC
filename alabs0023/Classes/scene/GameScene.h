//
//  GameScene.h
//  ColorGarden
//
//  Created by lidong on 15/7/31.
//
//

#ifndef __ColorGarden__GameScene__
#define __ColorGarden__GameScene__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "../define/GlobalMacro.h"


USING_NS_CC;
class BlockLayer;
class GameLayer;
class GameUILayer;
class GameScene : public BaseLayer {
    
protected:
    
    void onEnter();
public:
    virtual bool init(int pIndex);
    static Scene* createScene(int pIndex);
    void pauseGame();
    void stopTouchEvent();
    void resumeTouchEvent();
    void unblock();
public:
    GameUILayer* getGameUILayer();
    GameLayer* getGameLayer();
    BlockLayer * _blockLayer;
    
};

#endif /* defined(__ColorGarden__GameScene__) */
