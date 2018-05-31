//
//  GamePauseLayer.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-8.
//
//

#include "GamePauseLayer.h"
#include "../AppGlobal.h"
#include "../FileModifiers.h"
#include "../helpers/AdHelper.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/PurchaseHelper.h"
#include "../layers/DeluxeLayer.h"
#include "../layers/SettingsLayer.h"
#include "../widgets/STAlertDialog.h"
#include "../AppConfig.h"


USING_NS_CC;
using std::string;


float bgMoveOffset = 0.f;
float bgWidth      = 0.f;

GamePauseLayer* GamePauseLayer::create(GameScene* gameScene)
{
    GamePauseLayer *pRet = new GamePauseLayer(gameScene);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool GamePauseLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! CoverLayer::init());
        
        // the super class set keypad enable default, but in this layer we should disable this function.
        this->setKeypadEnabled(false);
        
        CCSpriteBatchNode *pBatchNode = CCSpriteBatchNode::create("in-game.pvr.ccz");
        CC_BREAK_IF(!pBatchNode);
        this->addChild(pBatchNode);
        
        m_pPauseBG = CCSprite::createWithSpriteFrameName("ui06_pause_bg.png");
        CC_BREAK_IF(!m_pPauseBG);
        pBatchNode->addChild(m_pPauseBG);
        m_pPauseBG->setScale(AppGlobal::UI_SCALE);
        
        bgWidth = m_pPauseBG->getContentSize().width;
        bgMoveOffset = fabsf(bgWidth*AppGlobal::UI_SCALE - bgWidth);
        
        m_pPauseBG->setAnchorPoint(ccp(1, 0));
        m_pPauseBG->setPosition(VISIBLE_RECT.origin);
        
        
        
        // play button
        pBtnPlay = STButton::createWithSpriteFrameName(string("ui06_play.png"), string("ui06_play_h.png"),
                                                                 this, callfuncN_selector(GamePauseLayer::onPlayClicked));
        CC_BREAK_IF(!pBtnPlay);
        pBtnPlay->setAnchorPoint(ccp(0, .5));
        pBtnPlay->setPosition(ccp2(150, 1300));
        m_pPauseBG->addChild(pBtnPlay);
        const int touchPriority = pBtnPlay->getTouchPriority() - 1;
        pBtnPlay->setTouchPriority(touchPriority);
        
        // replay button
        pBtnReplay = STButton::createWithSpriteFrameName(string("ui06_replay.png"), string("ui06_replay_h.png"),
                                                                 this, callfuncN_selector(GamePauseLayer::onReplayClicked));
        CC_BREAK_IF(!pBtnReplay);
        pBtnReplay->setAnchorPoint(ccp(0, .5));
        pBtnReplay->setPosition(ccp2(150, 945));
        m_pPauseBG->addChild(pBtnReplay);
        pBtnReplay->setTouchPriority(touchPriority);
        
        
        // home button
        pBtnHome = STButton::createWithSpriteFrameName(string("ui06_menu.png"), string("ui06_menu_h.png"),
                                                                   this, callfuncN_selector(GamePauseLayer::onHomeClicked));
        CC_BREAK_IF(!pBtnHome);
        pBtnHome->setAnchorPoint(ccp(0, .5));
        pBtnHome->setPosition(ccp2(150, 590));
        m_pPauseBG->addChild(pBtnHome);
        pBtnHome->setTouchPriority(touchPriority);
        
        
        // setting button
        pBtnSetting = STButton::createWithSpriteFrameName(string("ui06_settings.png"), string("ui06_settings_h.png"),
                                                                 this, callfuncN_selector(GamePauseLayer::onSettingClicked));
        CC_BREAK_IF(!pBtnSetting);
        pBtnSetting->setAnchorPoint(ccp(0, .5));
        pBtnSetting->setPosition(ccp2(150, 215));
        m_pPauseBG->addChild(pBtnSetting);
        pBtnSetting->setTouchPriority(touchPriority);
        

        // hide the bottom banner ad view
//        AdHelper::setAdsVisibility(false);
        

        CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(GamePauseLayer::showBannerAd), st_pauselayer_request_ad_notification, NULL);
        

        bRet = true;
    } while (0);
    
    return bRet;
}

void GamePauseLayer::showBannerAd(CCObject* pObject)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
    CCLOG("---------------excute xxx AdHelper::requestAds");
    AdHelper::requestAds();
#endif

}
                                                                      
                                                                      
                                                                      
void GamePauseLayer::onEnter()
{
    CoverLayer::onEnter();
    
    AdHelper::requestAds();
    
    CCAction *action = CCMoveBy::create(ANIMATION_DURING, ccp(bgWidth*AppGlobal::UI_SCALE, 0));
    m_pPauseBG->runAction(action);
    
}

void GamePauseLayer::onExit()
{

    AdHelper::removeAds();
    
    CoverLayer::onExit();
}

void GamePauseLayer::onPlayClicked(CCNode*)
{
    AudioHelper::getInstance()->playSound(sound_button);
    m_pGameScene->resumeGame();
}

void GamePauseLayer::onReplayClicked(CCNode *)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    setButtonsEnable(false);
    
    std::string message = AppGlobal::stAllStrings->valueForKey(strings_restart_content)->m_sString;
    std::string btnTextL = AppGlobal::stAllStrings->valueForKey(strings_button_yes)->m_sString;
    std::string btnTextR = AppGlobal::stAllStrings->valueForKey(strings_button_no)->m_sString;
    
    STAlertDialog* dialog = STAlertDialog::show(this, message, btnTextL, btnTextR);
    dialog->setLeftButtonListener(this, callfuncN_selector(GamePauseLayer::restartGame));
    dialog->setRightButtonListener(this, callfuncN_selector(GamePauseLayer::onSettingLayerClosed));
    
    AdHelper::removeAds();
    
}

void GamePauseLayer::restartGame(CCObject*)
{
    m_pGameScene->replayGame();
}

void GamePauseLayer::onHomeClicked(CCNode *)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    mbResumeBannerAd = false;
    
    m_pGameScene->returnToHome();
}

void GamePauseLayer::onSettingClicked(CCNode *)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    SettingsLayer* setting = SettingsLayer::createWithColor(ccc4(0, 0, 0, 0), true);
    setting->setCloseCallback(this, callfuncN_selector(GamePauseLayer::onSettingLayerClosed));
    this->addChild(setting, INT_MAX);
    
    setButtonsEnable(false);
  //  AdHelper::setRectAdsVisibility(false);
    

    
}

void GamePauseLayer::onSettingLayerClosed(CCNode* layer)
{
    setButtonsEnable(true);
    
    // show rect ads
  //  AdHelper::setRectAdsVisibility(true);
}

void GamePauseLayer::setButtonsEnable(bool boo)
{
    pBtnPlay->setEnabled(boo);
    pBtnReplay->setEnabled(boo);
    pBtnHome->setEnabled(boo);
    pBtnSetting->setEnabled(boo);
}

void GamePauseLayer::popDeluxeLayer()
{
    DeluxeLayer *deluxe = DeluxeLayer::create();
    deluxe->setCloseCallback(this, callfuncN_selector(GamePauseLayer::onSettingLayerClosed));
    this->addChild(deluxe, INT_MAX);
    
   // AdHelper::setRectAdsVisibility(false);
}

GamePauseLayer::GamePauseLayer(GameScene* gameScene)
: mbResumeBannerAd(true)
{
    m_pGameScene = gameScene;
}

GamePauseLayer::~GamePauseLayer()
{
     CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, st_pauselayer_request_ad_notification);
}