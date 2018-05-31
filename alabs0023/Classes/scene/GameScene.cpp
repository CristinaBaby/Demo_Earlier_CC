//
//  GameScene.cpp
//  ColorGarden
//
//  Created by lidong on 15/7/31.
//
//

#include "GameScene.h"
#include "../helper/ControlHelper.h"
#include "../game/GameUILayer.h"
#include "../game/GameLayer.h"
#include "BlockLayer.h"
#include "../modules/STSystemFunction.h"
#include "../crossplatformapi/headers/ads/AdsManager.h"

enum
{
  eGameScene_GameLayer_Tag=10,
  eGameScene_GameUILayer_Tag
};

enum
{
  eGameScene_GameLayer_Zorder=10,
  eGameScene_GameUILayer_Zorder
};

Scene* GameScene::createScene(int pIndex)
{
    
    Scene* lScene=Scene::create();
    GameScene *pRet = new GameScene();
    if (pRet && pRet->init(pIndex))
    {
        pRet->autorelease();
        lScene->addChild(pRet);
        return lScene;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }

}

bool GameScene::init(int pIndex)
{
    if(!BaseLayer::init()) return false;
    

    GameLayer* lGameLayer=GameLayer::create(pIndex);
    this->addChild(lGameLayer,eGameScene_GameLayer_Zorder,eGameScene_GameLayer_Tag);
    
    GameUILayer* lUiLayer=GameUILayer::create(pIndex);
    this->addChild(lUiLayer, eGameScene_GameUILayer_Zorder, eGameScene_GameUILayer_Tag);
    
    _blockLayer = BlockLayer::create();
    addChild(_blockLayer, 20);
    
    int rateUs = UserDefault::getInstance()->getIntegerForKey(st_rateUs_count, 0);
    if (rateUs >= 4)
    {
        UserDefault::getInstance()->setIntegerForKey(st_rateUs_count, 0);
        UserDefault::getInstance()->destroyInstance();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        STSystemFunction sf;
        sf.rating(apple_id, "Your suggestion will help us improving our apps! Please let us know what you want!");
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        STSystemFunction sf;
        sf.rating();
#endif
        
    }
    
    return true;
}


void GameScene::pauseGame()
{
   
}

void GameScene::stopTouchEvent()
{

}

void GameScene::resumeTouchEvent()
{
    
}

GameUILayer* GameScene::getGameUILayer()
{
    GameUILayer* lGameUILayer=dynamic_cast<GameUILayer*>(this->getChildByTag(eGameScene_GameUILayer_Tag));
    return lGameUILayer;
}

GameLayer* GameScene::getGameLayer()
{
    GameLayer* lGameLayer = dynamic_cast<GameLayer*>(this->getChildByTag(eGameScene_GameLayer_Tag));
    return lGameLayer;
}


void GameScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}
void GameScene::unblock()
{
    removeChild(_blockLayer);
}