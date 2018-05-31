//
//  GameUILayer.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-28.
//
//

#include "GameUILayer.h"
#include "../../AppConfig.h"
#include "../../helpers/AudioHelper.h"
#include "../../helpers/EncryptDataHelper.h"
#include "../../widgets/STTouchableSprite.h"


USING_NS_CC;
using std::string;


#define UI_BG_SHADOW_HEIGHT             (AppGlobal::IS_2048 ? 44 : 22)
#define UI_MAP_OVERLAP_HEIGHT           (AppGlobal::IS_2048 ? 30 : 15)

#define GAME_TIME_FONT_SIZE             fontsize(50)
#define GAME_HINT_QUANTITY_FONT_SIZE    fontsize(55)


enum {TAG_BUTTON_NEXT = 19, TAG_BUTTON_PREVIOUS, TAG_HINT_CHARGING, TAG_SPRITE_BUY_HINT,};

enum {z_bg = 11, z_menu = 14, z_hint_progress = 16, z_hint_quantity = 17, z_lilayer = 18, };

GameUILayer* GameUILayer::create(GameScene* parent)
{
    GameUILayer* pRet = new GameUILayer(parent, false);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

GameUILayer * GameUILayer::create(const ccColor4B& color, GameScene* parent)
{
    CCSize s = GET_CCDIRCTOR->getWinSize();
    
    GameUILayer * pLayer = new GameUILayer(parent, false);
    if(pLayer && pLayer->initWithColor(color, s.width, s.height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

GameUILayer * GameUILayer::recovery(GameScene* parent, float hintPercentage)
{
    GameUILayer* pRet = new GameUILayer(parent, true, hintPercentage);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool GameUILayer::initWithColor(const ccColor4B& color, GLfloat width, GLfloat height)
{
    bool pRet = false;
    do
    {
        // call super class method
        CC_BREAK_IF(! CCLayerColor::initWithColor(color, width, height));
        
        this->setTouchEnabled(true);
        this->ignoreAnchorPointForPosition(false);
        this->setAnchorPoint(CCPointZero);
        this->setPosition(GET_CCDIRCTOR->getVisibleOrigin());
        
        m_pUIBg = CCSprite::createWithSpriteFrameName("ui04_bar.png");
        CC_BREAK_IF(!m_pUIBg);
        m_pUIBg->setAnchorPoint(CCPointZero);
        this->addChild(m_pUIBg, z_bg);
        
        this->setContentSize(m_pUIBg->getContentSize());
        
        
        // game pause button
        STButton *pPause = STButton::createWithSpriteFrameName(string("ui04_pause.png"),
                                                               this, callfuncN_selector(GameUILayer::onPauseClicked));
        CC_BREAK_IF(!pPause);
        pPause->setPosition(ccp2(127, 106));
        m_pUIBg->addChild(pPause, z_menu);
        
        
        // game hint button
        if (mHintQuantity > 0)
        {
            m_pBtnHint = STButton::createWithSpriteFrameName(string("ui04_hint.png"),
                                                             this, callfuncN_selector(GameUILayer::onHintClicked));
        }
        else
        {
            m_pBtnHint = STButton::createWithSpriteFrameName(string("ui04_hint_u.png"),
                                                             this, callfuncN_selector(GameUILayer::onHintClicked));
        }
        CC_BREAK_IF(!m_pBtnHint);
        m_pBtnHint->setPosition(ccp2(1915, 120));
        m_pUIBg->addChild(m_pBtnHint, z_menu);
        
        // ---------------------------------------------------------------------
        m_pHintPT = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("ui04_hint_u.png"));
        CC_BREAK_IF(!m_pHintPT);
        m_pHintPT->setType(kCCProgressTimerTypeBar);
        m_pHintPT->setMidpoint(ccp(0, 1));
        //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
        m_pHintPT->setBarChangeRate(ccp(0, 1));
        
        m_pUIBg->addChild(m_pHintPT, z_hint_progress);
        m_pHintPT->setPosition(m_pBtnHint->getPosition());
        m_pHintPT->setTag(TAG_HINT_CHARGING);
        m_pHintPT->setVisible(false);
        // ----------------------------------------------------------------------
        
        // hint quantity
        m_pHintQuantityBg = CCSprite::createWithSpriteFrameName("ui04_number.png");
        CC_BREAK_IF(!m_pHintQuantityBg);
        m_pHintQuantityBg->setAnchorPoint(ccp(0, 0));
        m_pHintQuantityBg->setPosition(ccp2(1915, 140));
        m_pUIBg->addChild(m_pHintQuantityBg, z_hint_quantity);
        
        
        m_pHintQuantity = CCLabelTTF::create("", font_Helvetica_Neue, GAME_HINT_QUANTITY_FONT_SIZE);
        CC_BREAK_IF(! m_pHintQuantity);
        m_pHintQuantityBg->addChild(m_pHintQuantity);
        m_pHintQuantity->setPosition(ccp_get_center(m_pHintQuantityBg->getContentSize())+ccp2(-1.1, 3.5));
        m_pHintQuantity->setHorizontalAlignment(kCCTextAlignmentCenter);
        
        if (mHintQuantity > 0)
        {
            CCString* number = CCString::createWithFormat("%d", mHintQuantity);
            m_pHintQuantity->setString(number->getCString());
        }
        else if (mHintQuantity == 0)
        {
            CCSprite *plus = CCSprite::createWithSpriteFrameName("ui04_plus.png");
            CC_BREAK_IF(!plus);
            m_pHintQuantityBg->addChild(plus);
            plus->setPosition(ccp_get_center(m_pHintQuantityBg->getContentSize()));
            plus->setTag(TAG_SPRITE_BUY_HINT);
        } 
        
        
        
        if (isHintCharging)
        {
            recreateHintCharging();
        }
        
        // create a layer include texts or images.
        pLabelImageLayer = STLILayer::create(pGameScene);
        CC_BREAK_IF(!pLabelImageLayer);
        m_pUIBg->addChild(pLabelImageLayer, z_lilayer);
        
        
        m_pGameTime = CCLabelTTF::create("", font_Helvetica_Neue, GAME_TIME_FONT_SIZE);
        CC_BREAK_IF(! m_pGameTime);
        m_pGameTime->setPosition(ccp_fixed_X(m_pUIBg->getContentSize().width/2, 230));
        m_pGameTime->setHorizontalAlignment(kCCTextAlignmentCenter);
        
        m_pUIBg->addChild(m_pGameTime, z_lilayer);
        
        pRet = true;
    } while (0);
    
    return pRet;
}

void GameUILayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority+1, true); // swallows touches.
}

void GameUILayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

void GameUILayer::refreshContent(int objIdx)
{
    pLabelImageLayer->refresh(objIdx);
}

unsigned GameUILayer::getRandomIdxInPage()
{
    return pLabelImageLayer->getRandomIndexInPage();
}

void GameUILayer::updateGameTime(std::string timeString)
{
    m_pGameTime->setString(timeString.c_str());
}

void GameUILayer::onPauseClicked(CCNode*)
{
    AudioHelper::getInstance()->playSound(sound_button);
    pGameScene->pauseGame();
}

void GameUILayer::onHintClicked(CCNode* btn)
{
    // hint available
    if (mHintQuantity > 0)
    {
        AudioHelper::getInstance()->playSound(sound_game_hint);
        
        pGameScene->onTips();
        // turn to first page, we will show the first object.
        pLabelImageLayer->turnToFirstPage();
        hintCharging();
        
#ifndef DEBUG_HINT
        
        EncryptDataHelper::setHintQuantity(key_hint_quantity, --mHintQuantity);
        if (mHintQuantity == 0)
        {
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ui04_hint_u.png");
            m_pBtnHint->setDisplayFrame(frame);
            m_pBtnHint->setNormalSpriteFrame(frame);
            
            CCSprite *plus = CCSprite::createWithSpriteFrameName("ui04_plus.png");
            m_pHintQuantityBg->addChild(plus);
            plus->setPosition(ccp_get_center(m_pHintQuantityBg->getContentSize()));
            plus->setTag(TAG_SPRITE_BUY_HINT);
            m_pHintQuantity->setString("");
        }
        else
        {
            m_pHintQuantity->setString(CCString::createWithFormat("%d", mHintQuantity)->getCString());
        }
        
#endif
        
    }
    else
    {
        AudioHelper::getInstance()->playSound(sound_button);
        // pop deluxe layer
        pGameScene->pauseGame(this);
    }
    
}

void GameUILayer::hintCharging()
{
    m_pBtnHint->setEnabled(false);
    m_pHintPT->setVisible(true);
    isHintCharging = true;
    
    // --------------------------------------------------------------
    CCAction* action = NULL;
    
#ifdef DEBUG_HINT
    action =  CCSequence::create(CCProgressFromTo::create(1, 100, 0),
                                 CCCallFuncN::create(this, callfuncN_selector(GameUILayer::hintFull)),
                                 NULL);
    
#else
    
    action =  CCSequence::create(CCProgressFromTo::create(pGameScene->getHintRechargeTime(), 100, 0),
                                 CCCallFuncN::create(this, callfuncN_selector(GameUILayer::hintFull)),
                                 NULL);
#endif
    // --------------------------------------------------------------
    
    m_pHintPT->runAction(action);
    
}

bool GameUILayer::recreateHintCharging()
{
    bool bRet = false;
    
    do
    {
        float during = mHintPercentageBefore * 0.01f * pGameScene->getHintRechargeTime();
        if (during <= 0)
            return true;
        
        m_pBtnHint->setEnabled(false);
        m_pHintPT->setVisible(true);
        
        CCAction* action = NULL;
        
#ifdef DEBUG_HINT
        action =  CCSequence::create(CCProgressFromTo::create(1, mHintPercentageBefore, 0),
                                     CCCallFuncN::create(this, callfuncN_selector(GameUILayer::hintFull)),
                                     NULL);
#else
        action =  CCSequence::create(CCProgressFromTo::create(during, mHintPercentageBefore, 0),
                                     CCCallFuncN::create(this, callfuncN_selector(GameUILayer::hintFull)),
                                     NULL);
#endif
        
        
        m_pHintPT->runAction(action);
        
        m_pHintPT->pauseSchedulerAndActions();
        
        isHintCharging = true;
        
        bRet = true;
    } while (0);
    
    return bRet;
}

void GameUILayer::hintFull(CCNode* node)
{
    node->setVisible(false);
    
    m_pBtnHint->setEnabled(true);
    
    isHintCharging = false;
}

void  GameUILayer::pauseHintCharging()
{
    if (!isHintCharging)
        return;
    
    if (m_pHintPT)
        m_pHintPT->pauseSchedulerAndActions();
}

void  GameUILayer::resumeHintCharging()
{
    if (!isHintCharging)
        return;
    
    if (m_pHintPT)
        m_pHintPT->resumeSchedulerAndActions();
}

float GameUILayer::getHintChargingPercentage()
{
    if (isHintCharging && m_pHintPT)
        return m_pHintPT->getPercentage();
    
    return  0;
}



void GameUILayer::purchaseSuccessful(const char* pid)
{
    // only handle hint5 sku.
    if (strcmp(pid, st_iap_sku_hint_5) == 0)
    {
        m_pHintQuantityBg->removeChildByTag(TAG_SPRITE_BUY_HINT);
        
        mHintQuantity += 5;
        CCString* number = CCString::createWithFormat("%d", mHintQuantity);
        m_pHintQuantity->setString(number->getCString());
        
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ui04_hint.png");
        m_pBtnHint->setDisplayFrame(frame);
        m_pBtnHint->setNormalSpriteFrame(frame);
    }
}

void GameUILayer::purchaseFailed(const char *pid, int errorCode)
{
}

void GameUILayer::restoreSuccessful(const char* pid)
{
    purchaseSuccessful(pid);
}

void GameUILayer::restoreFailed(const char* pid, int errorCode)
{
}



GameUILayer::GameUILayer(GameScene* parent, bool isResume, float hintPercentage /*= 0.0f*/)
: pLabelImageLayer(NULL), isHintCharging(isResume), mHintPercentageBefore(hintPercentage),
m_pHintPT(NULL)
{
    pGameScene = parent;
    mHintQuantity = EncryptDataHelper::getHintQuantity(key_hint_quantity);
}

GameUILayer::~GameUILayer()
{
}

bool GameUILayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // return OpenGL coord
	CCPoint touchPoint = pTouch->getLocation();
	// convert to parent view space coord
	CCPoint realPoint = this->getParent()->convertToNodeSpace(touchPoint);
	// base on parent view space coord
	CCRect rect = this->boundingBox();
    
	return rect.containsPoint(realPoint);
}





