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
    
    return true;
}


void GameScene::pauseGame()
{
   
}

void GameScene::stopTouchEvent()
{
    this -> getGameLayer() -> _zoomLayer -> _touchEventListener->setEnabled(false);
    this -> getGameLayer() -> _zoomLayer -> _contentNode -> setUserData((void *)0x0);
}

void GameScene::resumeTouchEvent()
{
    this -> getGameLayer() -> _zoomLayer -> _touchEventListener->setEnabled(true);
    this -> getGameLayer() -> _zoomLayer -> _contentNode -> setUserData((void *)0x1);
    
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