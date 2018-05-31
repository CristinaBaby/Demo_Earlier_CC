//
//  HomeScene.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-18.
//
//

#include "HomeScene.h"
#include "ChooseModeScene.h"
#include "GuideScene.h"
#include "GameScene.h"
#include "LevelMapScene.h"
#include "../AppGlobal.h"
#include "../AppConfig.h"
#include "../FileModifiers.h"
#include "../layers/DeluxeLayer.h"
#include "../layers/SettingsLayer.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/AdHelper.h"
#include "../helpers/EncryptDataHelper.h"
#include "../widgets/STButtonMenu.h"
#include "../widgets/STAlertDialog.h"
#include "../modules/AnalyticX.h"
#include "../modules/STSystemFunction.h"
#include "../utilities/STUtility.h"

USING_NS_CC;

enum
{
    btn_play = 100, btn_deluxe, btn_setting, btn_moregames, btn_resume,
};

/* the z-order of layers */
enum
{
    z_homelayer = 1,
    z_functionlayer = 40,
};

CCScene* HomeLayer::scene(bool save)
{
    CCScene* pScene = CCScene::create();
    if (pScene)
    {
        // set current scene tag
        pScene->setTag(tag_scene_home);
        
        HomeLayer* p_hLayer = HomeLayer::create(save);
        if (p_hLayer)
        {
            pScene->addChild(p_hLayer, z_homelayer);
        }
    }
    
    return pScene;
}

bool HomeLayer::init()
{
    bool pRet = false;
    
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
        
        CCSprite* pBg = CCSprite::create("ui01_menu_bg.jpg");
        CC_BREAK_IF(!pBg);
        pBg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        this->addChild(pBg, 0);
        
        // create a scaled ui layer
        pUILayer = STUILayer::create();
        CC_BREAK_IF(!pUILayer);
        this->addChild(pUILayer, 10);
        
        if (haveSavedGame)
        {
            CC_BREAK_IF(! initWithResume());
        }
        else
        {
            CC_BREAK_IF(! initWithoutResume());
        }
        
        this->setKeypadEnabled(true);


        
        pRet = true;
    } while (0);
    
    return pRet;
}

bool HomeLayer::initWithoutResume()
{
    bool pRet = false;
    
    do
    {
        // logo
        CCSprite* pLogo = CCSprite::createWithSpriteFrameName("ui01_logo.png");
        CC_BREAK_IF(!pLogo);
        pLogo->setAnchorPoint(ccp(.5, 1));
        pLogo->setPosition(ccp_horizontal_center(1500));
        // add pLogoBg to plogoSBN
        pUILayer->addChild(pLogo);
        
        // play button
        CCMenuItemSprite* pBtnPlay = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui01_btn_play.png"),
                                                              CCSprite::createWithSpriteFrameName("ui01_btn_play_h.png"),
                                                              this, menu_selector(HomeLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnPlay);
        pBtnPlay->setTag(btn_play);
        pBtnPlay->setPosition(ccp_horizontal_center(727));
        pBtnPlay->setPosition(pBtnPlay->getPosition() + ccp(10, 0));
        
        // Deluxe button
        CCMenuItemSprite* pBtnDeluxe = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui01_btn_deluxe.png"),
                                                                 CCSprite::createWithSpriteFrameName("ui01_btn_deluxe_h.png"),
                                                                 this, menu_selector(HomeLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnDeluxe);
        pBtnDeluxe->setTag(btn_deluxe);
        pBtnDeluxe->setPosition(ccp_horizontal_center(452));
        
        // Settings button
        CCMenuItemSprite* pBtnSettings = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui01_btn_settings.png"),
                                                                  CCSprite::createWithSpriteFrameName("ui01_btn_settings_h.png"),
                                                                  this, menu_selector(HomeLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnSettings);
        pBtnSettings->setTag(btn_setting);
        pBtnSettings->setPosition(ccp_horizontal_center(210));
        
        // more button
        CCMenuItemSprite* pBtnMore = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui01_btn_more.png"),
                                                              CCSprite::createWithSpriteFrameName("ui01_btn_more_h.png"),
                                                              this, menu_selector(HomeLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnMore);
        pBtnMore->setScale(AppGlobal::UI_SCALE);
        pBtnMore->setAnchorPoint(ccp(1, 0));
        pBtnMore->setTag(btn_moregames);
        CCRect vRect = VISIBLE_RECT;
        float delta = get_offset(35);
        pBtnMore->setPosition(ccp(vRect.getMaxX()-delta, vRect.getMinY()+delta));
        STButtonMenu* pMenMore = STButtonMenu::create(pBtnMore, NULL);
        // should always on the right-bottom corner of screen
        this->addChild(pMenMore);
        
        // create a button menu
        STButtonMenu* pMenu = STButtonMenu::create(pBtnPlay, pBtnDeluxe, pBtnSettings,  NULL);
        CC_BREAK_IF(!pMenu);
        pMenu->setPosition(CCPointZero);
        pUILayer->addChild(pMenu);
        
        pRet = true;
    } while (0);
    
    return pRet;
}

bool HomeLayer::initWithResume()
{
    bool pRet = false;
    
    do
    {
        // logo
        CCSprite* pLogo = CCSprite::createWithSpriteFrameName("ui01_logo.png");
        CC_BREAK_IF(!pLogo);
        pLogo->setAnchorPoint(ccp(.5, 1));
        pLogo->setPosition(ccp_horizontal_center(1500));
        // add pLogoBg to plogoSBN
        pUILayer->addChild(pLogo);
        
        // resume button
        CCMenuItemSprite* pBtnResume = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui01_btn_resume.png"),
                                                                CCSprite::createWithSpriteFrameName("ui01_btn_resume_h.png"),
                                                                this, menu_selector(HomeLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnResume);
        pBtnResume->setTag(btn_resume);
        pBtnResume->setPosition(ccp_horizontal_center(725));
        pBtnResume->setPosition(pBtnResume->getPosition() + ccp(10, 0));
        
        // new game button
        CCMenuItemSprite* pBtnNew = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui01_btn_new_game.png"),
                                                             CCSprite::createWithSpriteFrameName("ui01_btn_new_game_h.png"),
                                                             this, menu_selector(HomeLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnNew);
        pBtnNew->setTag(btn_play);
        pBtnNew->setPosition(ccp_horizontal_center(498));
        
        // Deluxe button
        CCMenuItemSprite* pBtnDeluxe = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui01_btn_deluxe.png"),
                                                                CCSprite::createWithSpriteFrameName("ui01_btn_deluxe_h.png"),
                                                                this, menu_selector(HomeLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnDeluxe);
        pBtnDeluxe->setTag(btn_deluxe);
        pBtnDeluxe->setPosition(ccp_horizontal_center(313));
        
        // Settings button
        CCMenuItemSprite* pBtnSettings = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui01_btn_settings.png"),
                                                                  CCSprite::createWithSpriteFrameName("ui01_btn_settings_h.png"),
                                                                  this, menu_selector(HomeLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnSettings);
        pBtnSettings->setTag(btn_setting);
        pBtnSettings->setPosition(ccp_horizontal_center(132));
        
        // more button
        CCMenuItemSprite* pBtnMore = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui01_btn_more.png"),
                                                              CCSprite::createWithSpriteFrameName("ui01_btn_more_h.png"),
                                                              this, menu_selector(HomeLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnMore);
        pBtnMore->setScale(AppGlobal::UI_SCALE);
        pBtnMore->setAnchorPoint(ccp(1, 0));
        pBtnMore->setTag(btn_moregames);
        CCRect vRect = VISIBLE_RECT;
        float delta = get_offset(35);
        pBtnMore->setPosition(ccp(vRect.getMaxX()-delta, vRect.getMinY()+delta));
        STButtonMenu* pMenMore = STButtonMenu::create(pBtnMore, NULL);
        // should always on the right-bottom corner of screen
        this->addChild(pMenMore);
        
        // create a button menu
        STButtonMenu* pMenu = STButtonMenu::create(pBtnResume, pBtnNew, pBtnDeluxe, pBtnSettings, NULL);
        CC_BREAK_IF(!pMenu);
        pMenu->setPosition(CCPointZero);
        pUILayer->addChild(pMenu);
        
        pRet = true;
    } while (0);
    
    return pRet;
}

void HomeLayer::resetLayoutToWithoutResume()
{
    pUILayer->removeAllChildren();
    
    initWithoutResume();
}

void HomeLayer::onButtonClicked(CCObject* btn)
{
    CCNode* pBtn = dynamic_cast<CCNode*>(btn);
    if (!pBtn)
    return;
    
    AudioHelper::getInstance()->playSound(sound_button);
    
    switch (pBtn->getTag())
    {
        case btn_play:
        {
            bool firstFlag = CCUserDefault::sharedUserDefault()->getBoolForKey(key_first_play_game, true);
            if (firstFlag)
            {
                CCUserDefault::sharedUserDefault()->setBoolForKey(key_first_play_game, false);
                
                // --------------------------------------------------------------------------
                // if first play this game, we will record this time for check if we will popup rate us dialog.
                // and other operation will occur in GameScene
                
                long timestamp = namespaceST::STUtility::getCurrentTimeSeconds();
                // CCLOG("timestamp %ld ", timestamp);
                EncryptDataHelper::setRateUsTimestamp(key_rate_us_timestamp, timestamp);
                // --------------------------------------------------------------------------
                
                CCScene* pGuideScene = GuideScene::create();
                if (pGuideScene)
                {
                    GET_CCDIRCTOR->replaceScene(pGuideScene);
                }
                
            }
            else
            {
                CCScene* pScene = LevelMapLayer::scene();
                if (pScene)
                {
                    GET_CCDIRCTOR->replaceScene(pScene);
                }
            }
            
        }
        break;
        
        case btn_deluxe:
        {
            DeluxeLayer *pDeluxe = DeluxeLayer::create();
            this->addChild(pDeluxe, INT_MAX);
            pDeluxe->setTag(z_functionlayer);
        }
        break;
        
        case btn_setting:
        {
            SettingsLayer *pSettings = SettingsLayer::create(false);
            // put to the very top to block application touches.
            this->addChild(pSettings, INT_MAX);
            pSettings->setTag(z_functionlayer);
        }
        break;
        
        case btn_moregames:
        {
            STSystemFunction* sf = new STSystemFunction();
            sf->showMoreGame();
            CC_SAFE_DELETE(sf);
        }
        break;
        
        case btn_resume:
        {
            GameScene *pGame = GameScene::recovery();
            if (pGame)
            {
                GET_CCDIRCTOR->replaceScene(pGame);
            }
        }
        break;
        
        default:
        break;
    }
}

void HomeLayer::onEnter()
{
    CCLayer::onEnter();
}

void HomeLayer::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
    
#ifndef DEBUG_REMOVE_ADS
    static bool isFirstLoadingAd=true;
    if(isFirstLoadingAd)
    {
        isFirstLoadingAd=false;
    }
    else
    {
        AdHelper::requestInterstitialAds();
    }

#endif
    
}

void HomeLayer::onExit()
{
    CCLayer::onExit();
}

void HomeLayer::keyBackClicked()
{
    // check if any function layer on the top.
    CCNode* layer = this->getChildByTag(z_functionlayer);
    if (NULL != layer)
        return;
    
    
    std::string message = AppGlobal::stAllStrings->valueForKey(strings_exit_content)->m_sString;
    std::string btnTextL = AppGlobal::stAllStrings->valueForKey(strings_button_yes)->m_sString;
    std::string btnTextR = AppGlobal::stAllStrings->valueForKey(strings_button_no)->m_sString;
    
    STAlertDialog* dialog = STAlertDialog::show(this, message, btnTextL, btnTextR);
    dialog->setLeftButtonListener(this, callfuncN_selector(HomeLayer::quit));
    dialog->setTag(z_functionlayer);
    
}

void HomeLayer::quit(CCNode*)
{
    STSystemFunction* sf = new STSystemFunction();
    sf->endSession();
    CC_SAFE_DELETE(sf);
    
    AnalyticX::flurryEndSession();
    
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

HomeLayer::HomeLayer(bool savegame)
{
    haveSavedGame = savegame;
}

HomeLayer::~HomeLayer()
{
}