//
//  ChooseMapScene.cpp
//  HH002
//
//  Created by houhuang on 15/8/27.
//
//

#include "ChooseMapScene.h"
#include "../define/Globalmacro.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../layer/ScrollMapLayer_ScrollView.h"
#include "../scene/ChooseGalleryScene.h"
#include "../layer/LoadLayer.h"
#include "../layer/ColorScrollLayer/ColorScrollLayerMask.h"
#include "../helper/MusicHelper.h"
//#include "../IOS_Android_Include/AdsManager.h"
#include "../crossplatformapi/headers/ads/AdsManager.h"
#include "../DownLoad/DownLoadPage.h"

Scene* ChooseMapScene::scene(int index, const string& galleryName)
{
    Scene* scene = Scene::create();
    ChooseMapScene* layer = new ChooseMapScene();
    
    if (layer && layer->init(index, galleryName))
    {
        layer->autorelease();
        scene->addChild(layer);
        return scene;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

ChooseMapScene::ChooseMapScene()
{
    m_pLoadLayer = NULL;
    m_pBackUIbtn = NULL;
}

void ChooseMapScene::updateScene()
{
//    if (_scrollLayer)
//    {
//        _scrollLayer->removeFromParentAndCleanup(true);
//        _scrollLayer = NULL;
//    }
//    
//    ScrollMapLayer_ScrollView* mapLayer = ScrollMapLayer_ScrollView::create(this->getIndex());
//    ScreenAdapterHelper::setPosition(mapLayer, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
//    _scrollLayer=mapLayer;
//    this->addChild(_scrollLayer);
    
//    Scene* scene = ChooseGalleryScene::scene();
//    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
}

bool ChooseMapScene::init(int index, const string& galleryName)
{
    if (!BaseScene::init())
    {
        return false;
    }

    _index=index;

    Sprite* bg = Sprite::create("bg.png");
    bg->setAnchorPoint(Vec2(0.5, 1));
    bg->setScale(ScreenAdapterHelper::getRealDesignScale() * 1.05);
    ScreenAdapterHelper::setPosition(bg, Vec2(384, -( ScreenAdapterHelper::getRealDesignScale() - 1) * 100 * 1.05), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(bg);
    
    ScrollMapLayer_ScrollView* mapLayer = ScrollMapLayer_ScrollView::create(this->getIndex());
    ScreenAdapterHelper::setPosition(mapLayer, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
    _scrollLayer=mapLayer;
    this->addChild(_scrollLayer);
    
    //屏蔽menu栏scrollview事件
    ColorScrollLayerMask* topMask = ColorScrollLayerMask::create(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(110)));
    topMask->setAnchorPoint(Vec2(0.5, 0));
    ScreenAdapterHelper::setPosition(topMask, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(topMask);
    
//    Label* title = Label::createWithTTF(DM_getInstance->getLanguageMes()[galleryName.c_str()], st_font_AR, ScreenAdapterHelper::getValue(50));
    Label* title = Label::createWithTTF(galleryName.c_str(), st_font_AR, ScreenAdapterHelper::getValue(50));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    ScreenAdapterHelper::setPosition(title, Vec2(384,50),KVBoarderTypeTop,KHBoarderTypeNone);
    title->setColor(Color3B(0, 0, 0));
    this->addChild(title);
    
    
    Sprite* logo_line = Sprite::create("seperator-2.png");
    ScreenAdapterHelper::setPosition(logo_line, Vec2(384, 110),KVBoarderTypeTop,KHBoarderTypeNone);
    this->addChild(logo_line);
    
    
    m_pBackUIbtn = UIButton::create("btn_back.png");
    m_pBackUIbtn->setAnchorPoint(Vec2(0, 0.5));
    ScreenAdapterHelper::setPosition(m_pBackUIbtn, Vec2(60, 55), KVBoarderTypeTop, KHBoarderTypeLeft);
    m_pBackUIbtn->initListener(this, callfuncN_selector(ChooseMapScene::doAction));
    this->addChild(m_pBackUIbtn);
    
    m_pBackUIbtn->setButtonEnable(false);
    this->scheduleOnce(schedule_selector(ChooseMapScene::resumeButtonEnable), 0.7f);

    MenuItemSprite* lMenuSprite = MenuItemSprite::create(Sprite::create("btn_left.png"), NULL, CC_CALLBACK_1(ChooseMapScene::reponseButton, this));
    lMenuSprite->setAnchorPoint(Vec2(1, 1));
//    lMenuSprite->setScale(1.5f);
    ScreenAdapterHelper::setPosition(lMenuSprite, Vec2(25, 25), KVBoarderTypeTop, kHBoarderTypeRight);
    
    if (MusicHelper::m_bIsPlayBackgroudMusic)
    {
        lMenuSprite->setNormalImage(Sprite::create("music.png"));
        MusicHelper::getInstance()->resumeMusic();
    }else
    {
        lMenuSprite->setNormalImage(Sprite::create("music2.png"));
    }
    
    Menu* lMenu = Menu::create(lMenuSprite, NULL);
    lMenu->setPosition(Vec2::ZERO);
    this->addChild(lMenu);
    
    DataManager::m_bIsMyArtworksInGameScene = false;
    
#ifndef DEBUG_ADS
    AdsManager::getInstance()->showAds(kTypeBannerAds);
#endif
    
    return true;
}



void ChooseMapScene::reponseButton(Ref* ref)
{
    MenuItemSprite* lMenuSprite = (MenuItemSprite*)ref;
    
    if (MusicHelper::m_bIsPlayBackgroudMusic)
    {
        MusicHelper::m_bIsPlayBackgroudMusic = false;
        lMenuSprite->setNormalImage(Sprite::create("music2.png"));
        
        MusicHelper::getInstance()->pauseMusic();
        
    }else
    {
        MusicHelper::m_bIsPlayBackgroudMusic = true;
        lMenuSprite->setNormalImage(Sprite::create("music.png"));
        
        MusicHelper::getInstance()->resumeMusic();

    }
}


void ChooseMapScene::resumeButtonEnable(float dt)
{
    m_pBackUIbtn->setButtonEnable(true);
}

void ChooseMapScene::doAction(Node* node)
{
    _scrollLayer->stopLoading();
    m_pBackUIbtn->setButtonEnable(false);

    DataManager::getInstance()->m_iAdsCount++;
    
    Scene* scene = ChooseGalleryScene::scene();
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));

}
