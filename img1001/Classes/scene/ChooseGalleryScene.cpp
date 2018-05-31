 //
//  ChooseGalleryScene.cpp
//  HH002
//
//  Created by houhuang on 15/8/26.
//
//

#include "ChooseGalleryScene.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../data/DataManager.h"
#include "cocos-ext.h"
#include "../define/Globalmacro.h"

#include "../crossplatformapi/headers/ads/AdsManager.h"
#include "../ad3_2/AdCross/AdLoadingLayerDoll.h"
#include "../layer/ColorScrollLayer/ColorScrollLayerMask.h"
#include "../define/AppConfigs.h"
#include "../layer/Dialog.h"
#include "../modules/STSystemFunction.h"

#include "../crossplatformapi/headers/analytics/Analytics.h"
#include "../helper/MusicHelper.h"
#include "../utilities/Common.h"
#include "../DownLoad/DownLoadPage.h"
#include "../layer/ColorScrollLayer/ColorScrollLayerMask.h"
#include "../compoment/UIButton.h"
#include "FavoriteScene.hpp"

using namespace cocos2d::extension;

#define scroll_width 600

enum
{
    st_Dialog_tag = 99,
    st_menu_galery_tag,
    st_menu_inspiration_tag,
    st_menu_reset_tag,
};

Scene* ChooseGalleryScene::scene()
{
    Scene* scene = Scene::create();
    ChooseGalleryScene* layer = new ChooseGalleryScene();
    
    if (layer && layer->init() && layer->initWithColor(Color4B(255, 255, 255, 255)))
    {
        layer->autorelease();
        scene->addChild(layer);
        return scene;
    }else
    {
        delete layer;
        layer = nullptr;
        return NULL;
    }
    
}

ChooseGalleryScene::~ChooseGalleryScene()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

ChooseGalleryScene::ChooseGalleryScene()
{
    m_pTitleLabel = NULL;
    m_pGalleryBtnLabel = NULL;
    m_pInspirationBtnLabel = NULL;
    
    m_pMenu = NULL;
    m_pMusicMenu = NULL;
    m_pFavMenu = NULL;
    
    m_pInspirationLayer = NULL;
    m_pScrollViewLayer = NULL;
}

void ChooseGalleryScene::updateScene()
{
    DataManager::getInstance()->updateDownLoadResource();
    if (m_pScrollViewLayer)
    {
        m_pScrollViewLayer->removeFromParentAndCleanup(true);
        m_pScrollViewLayer = NULL;
    }
    
    if (DataManager::m_bIsGalleryLayer)
    {
        this->createGalleryLayer();
    }
}

bool ChooseGalleryScene::init()
{
    if (!BaseScene::init())
    {
        return false;
    }
    
//    DownLoadPage::getInstance()->m_vPostImageId.clear();
    
    DataManager::getInstance()->updateDownLoadResource();
    
//    AdsManager::getInstance()->preloadAds(kTypeBannerAds);
    AdsManager::getInstance()->removeAds(kTypeBannerAds);
    
    DataManager::m_bIsShowMyArtworksDeleteBtn = false;

    Sprite* bg = Sprite::create("bg.png");
    bg->setAnchorPoint(Vec2(0.5, 1));
    bg->setScale(ScreenAdapterHelper::getRealDesignScale() * 1.05);
    ScreenAdapterHelper::setPosition(bg, Vec2(384, -( ScreenAdapterHelper::getRealDesignScale() - 1) * 100 * 1.05), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(bg);

    //屏蔽menu栏scrollview事件
    ColorScrollLayerMask* topMask = ColorScrollLayerMask::create(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(110)));
    topMask->setAnchorPoint(Vec2(0.5, 0));
    ScreenAdapterHelper::setPosition(topMask, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(topMask, 1);
    
    Label* title = Label::createWithTTF("", st_font_AR, ScreenAdapterHelper::getValue(50));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    ScreenAdapterHelper::setPosition(title, Vec2(384,50),KVBoarderTypeTop,KHBoarderTypeNone);
    title->setColor(Color3B(236, 64, 122));
    this->addChild(title, 2);
    m_pTitleLabel = title;

    Sprite* logo_line = Sprite::create("seperator-2.png");
    ScreenAdapterHelper::setPosition(logo_line, Vec2(384, 110),KVBoarderTypeTop,KHBoarderTypeNone);
    this->addChild(logo_line, 2);
 
    this->scheduleOnce(schedule_selector(ChooseGalleryScene::loadingAds), 0.4f);
    
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(ChooseGalleryScene::onKeyPressed, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    MenuItemSprite* lMenuSprite = MenuItemSprite::create(Sprite::create("btn_left.png"), NULL, CC_CALLBACK_1(ChooseGalleryScene::reponseButton, this));
    lMenuSprite->setAnchorPoint(Vec2(1, 1));
//    lMenuSprite->setScale(1.5f);
    ScreenAdapterHelper::setPosition(lMenuSprite, Vec2(35, 35), KVBoarderTypeTop, kHBoarderTypeRight);
    
    MenuItemSprite* resetBtn = MenuItemSprite::create(Sprite::create("music.png"), NULL, CC_CALLBACK_1(ChooseGalleryScene::reponseButton, this));
    resetBtn->setAnchorPoint(Vec2(1, 1));
    resetBtn->setTag(st_menu_reset_tag);
    //    lMenuSprite->setScale(1.5f);
    ScreenAdapterHelper::setPosition(resetBtn, Vec2(65, 35), KVBoarderTypeTop, KHBoarderTypeLeft);
    resetBtn->setVisible(false);
    resetBtn->setEnabled(false);
    
#ifdef TEST_VERSION
    resetBtn->setVisible(true);
    resetBtn->setEnabled(true);
#endif
    
    if (MusicHelper::m_bIsPlayBackgroudMusic)
    {
        lMenuSprite->setNormalImage(Sprite::create("music.png"));
        MusicHelper::getInstance()->resumeMusic();
        
    }else
    {
        lMenuSprite->setNormalImage(Sprite::create("music2.png"));
    }
    
    Menu* lMenu = Menu::create(lMenuSprite, resetBtn, NULL);
    lMenu->setPosition(Vec2::ZERO);
    this->addChild(lMenu, 2);
    m_pMusicMenu = lMenu;
    lMenu->setVisible(false);
    lMenu->setEnabled(false);
    
    
    UIButton* lFavirate = UIButton::create("Persona-icon.png");
    lFavirate->initListener(this, callfuncN_selector(ChooseGalleryScene::reponseFavBtn));
    lFavirate->setAnchorPoint(Vec2(1, 1));
    //    lMenuSprite->setScale(1.5f);
    ScreenAdapterHelper::setPosition(lFavirate, Vec2(25, 25), KVBoarderTypeTop, kHBoarderTypeRight);
    m_pFavMenu = lFavirate;
    this->addChild(lFavirate, 2);
    lFavirate->setVisible(false);
    lFavirate->setButtonEnable(false);
    
    int lLoadTime=Common::getInitLoadGameTime();
    int lTime=Common::getTimeDistance(lLoadTime);
    int lDay=lTime/(24*3600);
    if(lDay>=1 && DataManager::m_bEnableRateUs)
    {
        STSystemFunction sf;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        sf.rating(apple_id, DM_getInstance->getLanguageMes()["RateUsContent"].c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        sf.rating();
#endif
        
        Common::saveInitLoadGameTime();
    }
    
    if (DataManager::m_bIsEnableCheck)
    {
        DownLoadPage::getInstance()->updateNewVersionCsv();
    }
    
    
    ColorScrollLayerMask* btn_bg = ColorScrollLayerMask::create(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(80)));
//    LayerColor* btn_bg = LayerColor::create(Color4B::WHITE, ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(80));
    btn_bg->ignoreAnchorPointForPosition(false);
    btn_bg->setAnchorPoint(Vec2(0.5, 0));
    ScreenAdapterHelper::setPosition(btn_bg, Vec2(384, 0) , KVBoarderTypeBottom, KHBoarderTypeNone);
    this->addChild(btn_bg, 100);
    
    Sprite* vLine = Sprite::create("pop_seperator2.png");
    vLine->setPosition(Vec2(btn_bg->getContentSize().width/2, btn_bg->getContentSize().height/2));
    btn_bg->addChild(vLine);
    
    MenuItemImage* galleryBtn = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(ChooseGalleryScene::doAction, this));
    galleryBtn->setScale(1.5f);
    galleryBtn->setPosition(Vec2(btn_bg->getContentSize().width/4, btn_bg->getContentSize().height/2));
    galleryBtn->setTag(st_menu_galery_tag);
    
    Label* galleryLabel = Label::createWithTTF(DM_getInstance->getLanguageMes()["Gallery"], st_font_AR, ScreenAdapterHelper::getValue(30));
    galleryLabel->setPosition(Vec2(galleryBtn->getContentSize().width/2, galleryBtn->getContentSize().height/2));
    galleryBtn->addChild(galleryLabel);
    galleryLabel->setColor(Color3B::BLACK);
    m_pGalleryBtnLabel = galleryLabel;
    
    
    MenuItemImage* insBtn = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(ChooseGalleryScene::doAction, this));
    insBtn->setScale(1.5f);
    insBtn->setPosition(Vec2(btn_bg->getContentSize().width * 0.75, btn_bg->getContentSize().height/2));
    insBtn->setTag(st_menu_inspiration_tag);
    
    Label* insLabel = Label::createWithTTF(DM_getInstance->getLanguageMes()["Inspiration"], st_font_AR, ScreenAdapterHelper::getValue(30));
    insLabel->setPosition(Vec2(insBtn->getContentSize().width/2, insBtn->getContentSize().height/2));
    insBtn->addChild(insLabel);
    insLabel->setColor(Color3B::BLACK);
    m_pInspirationBtnLabel = insLabel;
    
    Menu* menu = Menu::create(galleryBtn, insBtn, NULL);
    menu->setPosition(Vec2::ZERO);
    btn_bg->addChild(menu);
    m_pMenu = menu;
 
    return true;
}

void ChooseGalleryScene::onEnterTransitionDidFinish()
{
    BaseScene::onEnterTransitionDidFinish();
    
//    DM_getInstance->saveMyArtworksDatas();
//    DM_getInstance->saveMyPostsDatas();
//    DM_getInstance->saveFavoriteDatas();
//    DownLoadPage::getInstance()->saveInspirationDatas();
    
    if (DataManager::m_bIsGalleryLayer)
    {
        this->createGalleryLayer();
    }else
    {
        this->createInspirationLayer();
    }
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void ChooseGalleryScene::reponseFavBtn(cocos2d::Node *node)
{
    UIButton* button = dynamic_cast<UIButton*>(node);
    button->setButtonEnable(false);
    
    Scene* scene = FavoriteScene::scene();
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.35f, scene));
}

void ChooseGalleryScene::doAction(cocos2d::Ref *ref)
{
//    m_pMenu->setEnabled(false);
    MenuItemImage* lMenu = dynamic_cast<MenuItemImage*>(ref);
    switch (lMenu->getTag())
    {
        case st_menu_galery_tag:
        {
            if (!DataManager::m_bIsGalleryLayer)
            {
                DataManager::m_bIsGalleryLayer = true;
                this->destoryInspirationLayer();
                this->createGalleryLayer();
//                scheduleOnce(schedule_selector(ChooseGalleryScene::resumeMenu), 0.5f);
            }
        }
            break;
            
        case st_menu_inspiration_tag:
        {
            if (DataManager::m_bIsGalleryLayer)
            {
                STSystemFunction sf;
//                if(!sf.checkNetworkAvailable() && DownLoadPage::getInstance()->m_vInspirationInfo.size() == 0 && DownLoadPage::getInstance()->m_vInsImageNameInfo.size() == 0)
//                {
//                    sf.popAlertDialog("There is something wrong with Internet connection. Please try again later.");
//                    return;
//                }
                
                if (!sf.checkNetworkAvailable())
                {
                    sf.popAlertDialog(DM_getInstance->getLanguageMes()["FBNetworkFaild"].c_str());
                    return;
                }
                
                DataManager::m_bIsGalleryLayer = false;
                this->destoryGalleryLayer();
                this->createInspirationLayer();
//                scheduleOnce(schedule_selector(ChooseGalleryScene::resumeMenu), 0.5f);
            }
        }
            break;
            
        default:
            break;
    }
}

void ChooseGalleryScene::resumeMenu(float dt)
{
    if (m_pMenu)
    {
        m_pMenu->setEnabled(true);
    }
}

void ChooseGalleryScene::createGalleryLayer()
{
    ScrollGalleryLayer_ScrollView* scrollLayer = ScrollGalleryLayer_ScrollView::create();
    ScreenAdapterHelper::setPosition(scrollLayer, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(scrollLayer);
    m_pScrollViewLayer = scrollLayer;
    
    m_pTitleLabel->setString(DM_getInstance->getLanguageMes()["Gallery"]);
    m_pMusicMenu->setVisible(true);
    m_pMusicMenu->setEnabled(true);
    m_pFavMenu->setVisible(false);
    m_pFavMenu->setButtonEnable(false);
    
    if (m_pGalleryBtnLabel)
    {
        m_pGalleryBtnLabel->setColor(Color3B(236, 64, 122));
    }
  
}

void ChooseGalleryScene::destoryGalleryLayer()
{
    if (m_pScrollViewLayer)
    {
        m_pScrollViewLayer->removeFromParentAndCleanup(true);
        m_pScrollViewLayer = NULL;
    }
    
    if (m_pGalleryBtnLabel)
    {
        m_pGalleryBtnLabel->setColor(Color3B::BLACK);
    }
}

void ChooseGalleryScene::createInspirationLayer()
{
    InspirationLayer* lLayer = InspirationLayer::create();
    ScreenAdapterHelper::setPosition(lLayer, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(lLayer);
    m_pInspirationLayer = lLayer;
    
    m_pTitleLabel->setString(DM_getInstance->getLanguageMes()["Inspiration"]);
    m_pFavMenu->setVisible(true);
    m_pFavMenu->setButtonEnable(true);
    m_pMusicMenu->setVisible(false);
    m_pMusicMenu->setEnabled(false);
    
    if (m_pInspirationBtnLabel)
    {
        m_pInspirationBtnLabel->setColor(Color3B(236, 64, 122));
    }
}

void ChooseGalleryScene::destoryInspirationLayer()
{
    if (m_pInspirationLayer)
    {
        m_pInspirationLayer->removeFromParentAndCleanup(true);
        m_pInspirationLayer = NULL;
    }
    
    if (m_pInspirationBtnLabel)
    {
        m_pInspirationBtnLabel->setColor(Color3B::BLACK);
    }
}

void ChooseGalleryScene::reponseButton(Ref* ref)
{
    MenuItemSprite* lMenuSprite = (MenuItemSprite*)ref;
    
    if (lMenuSprite->getTag() == st_menu_reset_tag)
    {
        DownLoadPage::reset(NULL);
        if (DataManager::m_bIsGalleryLayer)
        {
//            this->updateScene();
            
            Sprite* lSprite = Sprite::create("pop_download.png");
            ScreenAdapterHelper::setPosition(lSprite, Vec2(384, 512));
            this->addChild(lSprite, 102);
            
            Label* label = Label::createWithTTF("Delete Suc", st_font_AR, ScreenAdapterHelper::getValue(30));
            label->setColor(Color3B::BLACK);
            label->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
            lSprite->addChild(label);
        }
        return;
    }
    
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

void ChooseGalleryScene::loadingAds(float dt)
{
#ifndef DEBUG_ADS
    if (DataManager::getInstance()->m_iAdsCount > 9 || DataManager::getInstance()->m_bIsAds)
    {
        DataManager::getInstance()->m_iAdsCount = 0;
        DataManager::getInstance()->m_bIsAds = false;
        AdLoadingLayerDoll::showLoading<AdLoadingLayerDoll>(false);
    }
    
//    AdsManager::getInstance()->showAds(kTypeBannerAds);
#endif
}


void ChooseGalleryScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (!DataManager::m_bIsGalleryLayer)
    {
        return;
    }
    
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        Dialog* lDialog = (Dialog*)this->getChildByTag(st_Dialog_tag);
        if (lDialog)
        {
            return;
        }else
        {
            STAlertDialog* lAlerDialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["QuitGame"], DM_getInstance->getLanguageMes()["NO"], DM_getInstance->getLanguageMes()["Yes"]);
            lAlerDialog->setLeftListener(this, callfuncN_selector(ChooseGalleryScene::removeDialog));
            lAlerDialog->setRightListener(this, callfuncN_selector(ChooseGalleryScene::quit));
            lAlerDialog->setTag(st_Dialog_tag);
            this->addChild(lAlerDialog, 100);
        }
    }
}

void ChooseGalleryScene::quit(Node* pNode)
{
    DM_getInstance->saveMyArtworksDatas();
//    DM_getInstance->saveMyPostsDatas();
//    DM_getInstance->saveFavoriteDatas();
//    DownLoadPage::getInstance()->saveInspirationDatas();
    
    STSystemFunction* sf = new STSystemFunction();
    sf->endSession();
    CC_SAFE_DELETE(sf);
    
    UserDefault::getInstance()->setBoolForKey(st_IsPlayBackgrioundMusic, MusicHelper::m_bIsPlayBackgroudMusic);
    UserDefault::getInstance()->destroyInstance();

//    AnalyticX::flurryEndSession();
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void ChooseGalleryScene::removeDialog(Node* pNode)
{
    Dialog* lDialog = (Dialog*)this->getChildByTag(st_Dialog_tag);
    if (lDialog)
    {
        lDialog->removeFromParentAndCleanup(true);
    }
}










