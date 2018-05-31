//
//  GameScene.cpp
//  HH002
//
//  Created by houhuang on 15/8/28.
//
//

#include "GameScene.h"
#include "../game/GameUILayer.h"
#include "../game/GameLayer.h"
#include "../crossplatformapi/headers/ads/AdsManager.h"

enum
{
    st_gameLayer_tag = 20,
    st_gameUILayer_TAG,
};

GameScene::~GameScene()
{
    DataManager::m_iCurrentClickNumber = 0;
    DataManager::m_iCurrentCancelNumber = 0;
}

bool GameScene::init()
{
    if (!BaseScene::init())
    {
        return false;
    }
    
    
    DM_getInstance->initFramesData();
    DM_getInstance->updataFramesDataBase(false);
    
    DM_getInstance->initFilterData();
    DM_getInstance->updataFilterDataBase(false);
    
    DM_getInstance->initFontTypeInfoDatas("FontTypeInfo.csv");
    
    DataManager::m_iCurrentClickNumber = 0;
    DataManager::m_iCurrentCancelNumber = 0;
    
    m_iIndex = DataManager::getInstance()->m_iCurrentGalleryID;
    m_sGallery = DataManager::getInstance()->m_pCurrentImage.imageName;
    
    GameLayer* lGameLayer = GameLayer::create();
    this->addChild(lGameLayer, 9, st_gameLayer_tag);
    
    GameUILayer* lGameUILyer = GameUILayer::create();
    this->addChild(lGameUILyer, 10, st_gameUILayer_TAG);
    
#ifndef DEBUG_ADS
    AdsManager::getInstance()->showAds(kTypeBannerAds);
#endif
    
    return true;
}

Scene* GameScene::create()
{
    Scene* scene = Scene::create();
    GameScene* layer = new GameScene();
    if (layer && layer->init())
    {
        layer->autorelease();
        scene->addChild(layer);
        return scene;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void GameScene::onEnterTransitionDidFinish()
{
    BaseScene::onEnterTransitionDidFinish();

    this->scheduleOnce(schedule_selector(GameScene::delayeRemoveTextureCache), 5.0f);
}

void GameScene::pauseGame()
{

}

void GameScene::delayeRemoveTextureCache(float dt)
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

GameLayer* GameScene::getGameLayer()
{
    GameLayer* lGameLayer = dynamic_cast<GameLayer*>(this->getChildByTag(st_gameLayer_tag));
    return lGameLayer;
}

GameUILayer* GameScene::getGameUILayer()
{
    GameUILayer* lGameUILayer = dynamic_cast<GameUILayer*>(this->getChildByTag(st_gameUILayer_TAG));
    return lGameUILayer;
}

int GameScene::getIndex()
{
    return m_iIndex;
}

std::string GameScene::getGalleryName()
{
    return m_sGallery;
}