//
//  LoadingScene.cpp
//  HH002
//
//  Created by houhuang on 15/8/26.
//
//

#include "LoadingScene.h"
#include "../helper/ScreenAdapterHelper.h"
#include "ChooseGalleryScene.h"

//#include "../IOS_Android_Include/RewardedAds.h"
#include "../crossplatformapi/headers/ads/RewardedAds.h"
#include "../compoment/UIButton.h"
#include "../helper/MusicHelper.h"
#include "../DownLoad/DownLoadPage.h"
#include "../crossplatformapi/headers/ads/AdsManager.h"

LoadingScene::~LoadingScene()
{

}

LoadingScene::LoadingScene()
{
    
}

bool LoadingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    

    //降低default场景加载时间，提高效率
    DataManager::getInstance()->initCacheTexture();
    DataManager::getInstance()->initColorGalleryText();
    DataManager::getInstance()->initLoadingAnimate();
    DataManager::getInstance()->initFlurryDatas();
    DataManager::getInstance()->readMyArtworksDatas();
    DataManager::getInstance()->readMyPostsDatas();
    DataManager::getInstance()->readFavoriteDatas();
//    DownLoadPage::getInstance()->readInspitationDatas();
    
    for (int i = 0 ; i < DownLoadPage::getInstance()->m_vPostImageId.size(); i++)
    {
        log("===== %d",DownLoadPage::getInstance()->m_vPostImageId.at(i));
    }
    
    Director::getInstance()->getTextureCache()->addImage("noise.png");
    Director::getInstance()->getTextureCache()->addImage("tietu_d.png");
    Director::getInstance()->getTextureCache()->addImage("crayon3.jpg");
    Director::getInstance()->getTextureCache()->addImage("pencil_disp2.jpg");
    Director::getInstance()->getTextureCache()->addImage("penciltest.jpg");
    Director::getInstance()->getTextureCache()->addImage("tietu_b_gai.jpg");
    Director::getInstance()->getTextureCache()->addImage("watercolor1.jpg");
//
    Sprite* bg = Sprite::create("loading.png");
    ScreenAdapterHelper::setPosition(bg,Vec2(384, 512));
    bg->setScale(ScreenAdapterHelper::getRealDesignScale());
    this->addChild(bg);
//
    MusicHelper::m_bIsPlayBackgroudMusic = UserDefault::getInstance()->getBoolForKey(st_IsPlayBackgrioundMusic, true);
    
    MusicHelper::getInstance()->playMultipleMusic_loop();
    MusicHelper::getInstance()->pauseMusic();

    this->playAnimate();
    this->scheduleOnce(schedule_selector(LoadingScene::gotoNextScene), 3.3f);
    
//    UIButton* back = UIButton::create("btn_back.png");
//    ScreenAdapterHelper::setPosition(back, Vec2(384, 512), KVBoarderTypeTop, KHBoarderTypeLeft);
//    back->initListener(this, callfuncN_selector(LoadingScene::doAction));
//    this->addChild(back);

    AdsManager::getInstance()->removeAds(kTypeBannerAds);
    
    return true;
}

void LoadingScene::doAction(Node* ref)
{
    RewardedAds::getInstance()->show();
}

void LoadingScene::gotoNextScene(float dt)
{
    Scene* scene = ChooseGalleryScene::scene();
    Director::getInstance()->replaceScene(scene);
}

void LoadingScene::playAnimate()
{
    for (int i = 0; i < DataManager::getInstance()->m_vLoadingAnimate.size(); ++i)
    {
        Sprite* lSprite = Sprite::createWithSpriteFrameName(DataManager::getInstance()->m_vLoadingAnimate.at(i));
        int offset = 0.0f;
        if (i > 4)
        {
            offset = 50.0f;
        }
        ScreenAdapterHelper::setPosition(lSprite, Vec2(offset + 150 + 50 * i, 512));
        lSprite->setOpacity(0);
        this->addChild(lSprite);
        
        lSprite->runAction(Sequence::create(DelayTime::create(0.3 * i), FadeIn::create(0.3f), NULL) );
    }
}