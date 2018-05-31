//
//  GameScene.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#include "GameScene.h"
#include "HomeScene.h"
#include "../AppConfig.h"
#include "../FileModifiers.h"
#include "../game/map/GameMapLayer.h"
#include "../game/ui/GameUILayer.h"
#include "../game/GamePauseLayer.h"
#include "../game/GameFinishLayer.h"
#include "../helpers/AdHelper.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/EncryptDataHelper.h"
#include "../helpers/PurchaseHelper.h"
#include "../layers/DeluxeLayer.h"
#include "../modules/AnalyticX.h"
#include "../modules/STSystemFunction.h"
#include "../utilities/STUserDefault.h"
#include "../utilities/STUtility.h"
#include "../utilities/CSVParse.h"


USING_NS_CC;
USING_NS_ST;

#define RATE_US_DELAY_1                         24
#define RATE_US_DELAY_2                         (24 * 3)

#define GAME_TIME_SCHEDULE                      schedule_selector(GameScene::onTimeUpdate)
#define HIT_BONUS_SCHEDULE                      schedule_selector(GameScene::disableHitBonus)
#define SKILL_BONUS_SCHEDULE                    schedule_selector(GameScene::disableSkillBonus)

#define HIT_BONUS_DURATION                      5

unsigned GameScene::s_PlayedLevels = 0;

enum
{
    tag_cover_layer = 20,
};

enum { z_map_layer = 1, z_ui_layer = 5, };


int                 GameScene::s_uSkillBonusIndex = INT_MIN;
GameSaveHelper*     GameScene::s_pGameSaveHelper = NULL;


GameScene *GameScene::create(const unsigned levelID, SearchMode sm)
{
    // this is a new game! delete game data before saved
    CC_SAFE_DELETE(s_pGameSaveHelper);
    
    GameScene *pRet = new GameScene(levelID, sm);
    if (pRet && pRet->init(false))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

GameScene* GameScene::recovery()
{
    GameScene *pRet = new GameScene(s_pGameSaveHelper->mLevelIndex, s_pGameSaveHelper->mSearchMode);
    if (pRet && pRet->init(true))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool GameScene::init(bool isRecovery)
{
    bool pRet = false;
    
    do
    {
        CC_BREAK_IF(! CCScene::init());
        CC_BREAK_IF(! build(isRecovery));
        
        if (isRecovery)
        {
            this->pauseGame();
        }
        
        this->setTag(tag_scene_game);
        
        pRet = true;
    } while (0);
    
    return pRet;
}

bool GameScene::build(bool isRecovery)
{
    bool bRet = false;
    
    // load sprite sheet
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("in-game.plist");
    
    if (isRecovery)
    {
        do
        {
            // get this level's identifer
            stLevelId = s_pGameSaveHelper->mLevelId;
            CCLOG("recovery level is %s", stLevelId.c_str());
            // get level's resource path
            stLevelResRoot = s_pGameSaveHelper->mLevelResRoot;

            if (mSearchMode == Name)
                loadObjectsName();
            
            // GameMapLayer
            m_pGameMapLayer = GameMapLayer::recovery(this, s_pGameSaveHelper->m_pRemainedObjects, s_pGameSaveHelper->m_pRemovedObjects);
            CC_BREAK_IF(!m_pGameMapLayer);
            this->addChild(m_pGameMapLayer, z_map_layer);
            
            // GameUILayer
            m_pGameUILayer = GameUILayer::recovery(this, s_pGameSaveHelper->mHintPercent);
            CC_BREAK_IF(!m_pGameUILayer);
            this->addChild(m_pGameUILayer, z_ui_layer);
            m_pGameUILayer->updateGameTime(m_pScoringSystem->getLifeTimeString());
            
            
            bRet = true;
        } while (0);
        
    }
    else
    {
        // first init code
        do
        {
            CCDictionary* pLevelInfo = dynamic_cast<CCDictionary*>(m_pAvailableLevels->objectAtIndex(curlevelIndicator));
            if (!pLevelInfo)
            {
                CCLOGERROR("Can't get level informations!");
                return false;
            }
            
            // get this level's identifer
            stLevelId = dynamic_cast<CCString*>(pLevelInfo->objectForKey(st_level_res_id))->m_sString;
            
            // get level's resource path
            std::string folderID = dynamic_cast<CCString*>(pLevelInfo->objectForKey(st_level_folder_id))->m_sString;
            stLevelResRoot = st_maps_root;
            stLevelResRoot.append(st_file_seperator).append(folderID);
            
            CCLOG("current is %s", folderID.c_str());
            
            if (mSearchMode == Name)
                loadObjectsName();
            
            // GameMapLayer
            m_pGameMapLayer = GameMapLayer::create(this);
            CC_BREAK_IF(!m_pGameMapLayer);
            this->addChild(m_pGameMapLayer, z_map_layer);
            
            // GameUILayer
            m_pGameUILayer = GameUILayer::create(this);
            CC_BREAK_IF(!m_pGameUILayer);
            this->addChild(m_pGameUILayer, z_ui_layer);
            m_pGameUILayer->updateGameTime(m_pScoringSystem->getLifeTimeString());
            
            bRet = true;
        } while (0);
        
    }
    
    // clear saved game data.
    CC_SAFE_DELETE(s_pGameSaveHelper);
    
    return bRet;
}

bool GameScene:: loadObjectsName()
{
    stAllHiddObjectsTexts = CCDictionary::create();
    CC_SAFE_RETAIN(stAllHiddObjectsTexts);
    
    string textsPath = string(stLevelResRoot).append(st_file_seperator).append(st_disname_file);
    
    /* get real objects name from csv file */
    CSVParse* csv = CSVParse::create(textsPath.c_str());
    if (!csv)
    {
        CCLOGERROR("Can't load CSV file: %s", textsPath.c_str());
        return false;
    }
    unsigned rown = csv->getRows();
    
    // exclude the first row which is the column's name
    for (int r = 1; r < rown; r++)
    {
        const char* key = csv->getDatas(r, st_disname_id);
        CCString* value = CCString::create(csv->getDatas(r, st_disname_Name));
        // add names to dictionary
        stAllHiddObjectsTexts->setObject(value, key);
    }
    
    CC_SAFE_RELEASE(csv);
    
    return true;
}


void GameScene::onEnter()
{
    CCScene::onEnter();
    
    AudioHelper::getInstance()->playMusic(music_game);
    
    // start time schedule
    if (!isGamePaused)
        this->schedule(GAME_TIME_SCHEDULE, 1.f);
}

void GameScene::onExit()
{
    AudioHelper::getInstance()->stopMusic();
    PurchaseHelper::getInstance()->setDelegate(NULL);
    
    CCScene::onExit();
}

CCArray* GameScene::getHiddObjs()
{
    return m_pGameMapLayer->getDisplayNamesList();
}

void GameScene::onTips()
{
    m_pGameMapLayer->showHint();
}

bool GameScene::_pauseInternal()
{
    if (isGamePaused)
        return false;
    isGamePaused = true;
    
    this->unschedule(GAME_TIME_SCHEDULE);
    
    m_pGameUILayer->pauseHintCharging();
    
    return true;
}

bool GameScene::_resumeInternal()
{
    if (!isGamePaused)
        return false;
    isGamePaused = false;
    
    // start time schedule
    this->schedule(GAME_TIME_SCHEDULE, 1.f);
    
    m_pGameUILayer->resumeHintCharging();
    
    return true;
}

void GameScene::pauseGame(STIABDelegate* delegate/*=NULL*/)
{
    // only have one "tag_cover_layer" layer.
    if (this->getChildByTag(tag_cover_layer)) return;
    
    // ------------------------------------------
    // solve Bugs #14333
    if (m_pGameMapLayer->getDisplayNamesList()->count() <= 0)
    {
        this->finishGame(true);
    }
    // ------------------------------------------
    
    if (!_pauseInternal())
        return;
    
    GamePauseLayer* pPause = GamePauseLayer::create(this);
    pPause->setTag(tag_cover_layer);
    this->addChild(pPause, INT_MAX);
    
    if (delegate)
    {
        pPause->popDeluxeLayer();
        PurchaseHelper::getInstance()->setDelegate(delegate);
    }
}

void GameScene::finishGame(bool isComplete)
{
    if (!_pauseInternal())
        return;
    
    isGameFinished = true;
    
    // this is used to stop bonus, and calculate score.
    if (mbIsHitBonus)
    {
        this->unschedule(HIT_BONUS_SCHEDULE);
        this->disableHitBonus();
    }
    
    if (mbIsSkillBonus)
    {
        this->unschedule(SKILL_BONUS_SCHEDULE);
        this->disableSkillBonus(-1);
    }
    
    GameFinishLayer* finishLayer = NULL;
    
    if (isComplete)
    {
        // add time bonus scores.
        m_pScoringSystem->timeBonus();
        
        // it means all level is unlocked, and current is last one.
        if (curlevelIndicator == AppGlobal::stAllLevelsInformation->count()-1)
        {
            finishLayer = GameFinishLayer::create(this, m_pScoringSystem, alllevelfinish);
        }
        else
        {
            finishLayer = GameFinishLayer::create(this, m_pScoringSystem, gamecomplete);
        }
        
        // record completed level numbers
        unsigned levelComplete = STLoadIntegerFromUD(key_complete_flag, 0);
        levelComplete = MAX(levelComplete, MIN((curlevelIndicator+1), (m_pAvailableLevels->count())));
        STSaveIntegerToUD(key_complete_flag, levelComplete);
        // ------------------------------
    }
    else
    {
        finishLayer = GameFinishLayer::create(this, m_pScoringSystem, gameover);
    }
    
    finishLayer->setTag(tag_cover_layer);
    this->addChild(finishLayer, INT_MAX);
    
    
    // -----------------------------
    // rate us
    
    s_PlayedLevels ++;
    
    if (mbMayRateUs && s_PlayedLevels == 5)
    {
        mbMayRateUs = false;
        
        std::string message1 = AppGlobal::stAllStrings->valueForKey(strings_rate_us1)->m_sString;
        std::string message2 = AppGlobal::stAllStrings->valueForKey(strings_rate_us2)->m_sString;
        message1.append(", ").append(message2);
        
        STSystemFunction* sf = new STSystemFunction();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        sf->rating(apple_id, message1.c_str());
        
#else
        sf->rating();
#endif
        
        CC_SAFE_DELETE(sf);
        
        
        // when finish this popup, we need resave this time for next call if we need.
        // if user have already rate us, and system fuction may not popup
        long timestamp = namespaceST::STUtility::getCurrentTimeSeconds();
        // CCLOG("timestamp %ld ", timestamp);
        EncryptDataHelper::setRateUsTimestamp(key_rate_us_timestamp, timestamp);
        
        CCUserDefault::sharedUserDefault()->setBoolForKey(key_rate_us_never, false);
    }
    
    
    // pop up ads
    if (s_PlayedLevels == 5)
    {
        // recount
        s_PlayedLevels = 0;
#ifndef DEBUG_REMOVE_ADS
        AdHelper::requestInterstitialAds();
#endif
    }
}

void GameScene::resumeGame()
{
    if (!_resumeInternal())
        return;
    
    CoverLayer* pCoverLayer = dynamic_cast<CoverLayer*>(this->getChildByTag(tag_cover_layer));
    if (pCoverLayer)
    {
        pCoverLayer->keyBackClicked();
    }
}

void GameScene::replayGame()
{
    clear();
    
        CC_SAFE_DELETE(m_pScoringSystem);
        m_pScoringSystem = ScoringSystem::create();
        m_pScoringSystem->retain();

    
    if(build(false))
    {
        this->schedule(GAME_TIME_SCHEDULE, 1.f);
    }
    else
    {
        CCAssert(0>1, "Game Scene init error!");
    }
}

void GameScene::nextLevel()
{
    curlevelIndicator ++;
    
    // loops free level
    if (curlevelIndicator >= m_pAvailableLevels->count())
    {
        // next level is locked, pop up purchase layer
        if (!PurchaseHelper::isPurchased)
        {
            DeluxeLayer *pDeluxe = DeluxeLayer::create();
            if (pDeluxe)
            {
                PurchaseHelper::getInstance()->setDelegate(this);

                GameFinishLayer *node = (GameFinishLayer*)this->getChildByTag(tag_cover_layer);
                if (node)
                {
                    node->setButtonsEnable(false);
                    node->setVisible(false);
                }
                
                pDeluxe->setCloseCallback(this, callfuncN_selector(GameScene::onDeluxeLayerClosed));
                this->addChild(pDeluxe, INT_MAX);
            }
        }
        
        // reset indicator
        curlevelIndicator--;
    }
    else
    {
        replayGame();
    }
}

void GameScene::onDeluxeLayerClosed(CCNode*)
{
    GameFinishLayer *node = (GameFinishLayer*)this->getChildByTag(tag_cover_layer);
    if (node)
    {
        node->setButtonsEnable(true);
        node->setVisible(true);
    }
}

void GameScene::returnToHome()
{
    CCScene *pScene = NULL;
    
    CC_SAFE_RELEASE(s_pGameSaveHelper);
    
    // game not finish, we need save data.
    if (!isGameFinished)
    {
        s_pGameSaveHelper = new GameSaveHelper(m_pGameMapLayer->getDisplayNamesList(),
                                               m_pGameMapLayer->getRemovedNamesList(),
                                               mSearchMode, stLevelId, stLevelResRoot,
                                               m_pScoringSystem, m_pGameUILayer->getHintChargingPercentage(), curlevelIndicator);
        pScene = HomeLayer::scene(true);
    }
    else
    {
        pScene = HomeLayer::scene();
    }
    
    GET_CCDIRCTOR->replaceScene(pScene);
}

void GameScene::onTimeUpdate(float delta)
{
    // when game paused, do not update this
    if (isGamePaused)
        return;
    
    // update game time
    bool isOver = m_pScoringSystem->updateLifetime();
    
    // update game ui label
    m_pGameUILayer->updateGameTime(m_pScoringSystem->getLifeTimeString());
    
    if (isOver)
    {
        finishGame(false);
    }
    
}

void GameScene::refreshUIContent(unsigned objIdx)
{
    // hit bonus
    enableHitBonus();
    
    // skill bonus
    enableSkillBonus();
    
    m_pGameUILayer->refreshContent(objIdx);
    
}

void GameScene::enableHitBonus()
{
    mHitBonusNum++;
    
    // if hint bonus not started, start it.
    if(!mbIsHitBonus)
    {
        mbIsHitBonus = true;
        this->scheduleOnce(HIT_BONUS_SCHEDULE, HIT_BONUS_DURATION);
    }
}

void GameScene::disableHitBonus(float delta)
{
    mbIsHitBonus = false;
    
    // calculate the score
    m_pScoringSystem->hitBonus(mHitBonusNum);
    
    // reset data
    mHitBonusNum = 0;
}

void GameScene::enableSkillBonus()
{
    mContinuousCorrectHit++;
    
    // if user continuous tap 5 correct object, enable skill bonus mode
    if (!mbIsSkillBonus && (mContinuousCorrectHit >= 5))
    {
        mbIsSkillBonus = true;
        
        // reset skill mode, whether get the bonus or not.
        mContinuousCorrectHit = 0;
        
        s_uSkillBonusIndex = m_pGameUILayer->getRandomIdxInPage();
        
        this->scheduleOnce(SKILL_BONUS_SCHEDULE, HIT_BONUS_DURATION);
    }
}

void GameScene::disableSkillBonus(float delta)
{
    // we will disable skill bonus, and refresh UI.
    mbIsSkillBonus = false;
    
    // reset skill mode, whether get the bonus or not.
    mContinuousCorrectHit = 0;
    
    s_uSkillBonusIndex = INT_MIN;
    
    if (delta >= 0)
        m_pGameUILayer->refreshContent(-1);
}

void GameScene::updateSkillBonus(bool isGet)
{
    if (mbIsSkillBonus)
    {
        this->unschedule(SKILL_BONUS_SCHEDULE);
        this->disableSkillBonus(-1);
    }
    
    // get skill bonus, we increase score.
    if (isGet)
    {
        m_pScoringSystem->skillBonus();
        
        // revise continuous hit number. after this method,
        // mContinuousCorrectHit will increase 1
        mContinuousCorrectHit = -1;
    }
    
}

void GameScene::interruptByError()
{
    // disrupt hit bonus schedule.
    if (mbIsHitBonus)
        this->unschedule(HIT_BONUS_SCHEDULE);
    
    if (mbIsSkillBonus)
        this->unschedule(SKILL_BONUS_SCHEDULE);
    
    this->disableHitBonus();
    this->disableSkillBonus();
    
}

void GameScene::clear()
{
    stLevelId.clear();
    stLevelResRoot.clear();
    isGamePaused = false;
    mHitBonusNum = 0;
    mContinuousCorrectHit = 0;
    mbIsHitBonus = false;
    mbIsSkillBonus = false;
    isGameFinished = false;
    
    s_uSkillBonusIndex = INT_MIN;
    
    this->unscheduleAllSelectors();
    this->removeAllChildren();
}

float GameScene::getHintRechargeTime()
{
    return m_pScoringSystem->getRechargeTime();
}

GameScene::GameScene(const unsigned levelID, SearchMode sm)
: mSearchMode(sm), stLevelId(), stLevelResRoot(), m_pGameMapLayer(NULL), m_pGameUILayer(NULL),
isGamePaused(false), curlevelIndicator(levelID), mHitBonusNum(0), mContinuousCorrectHit(0),
mbIsHitBonus(false), mbIsSkillBonus(false), isGameFinished(false), stAllHiddObjectsTexts(NULL)
{
    if (!s_pGameSaveHelper)
    {
        m_pScoringSystem = ScoringSystem::create();
    }
    else
    {
        m_pScoringSystem = s_pGameSaveHelper->m_pScore;
    }
    m_pScoringSystem->retain();
    
    /* create a array with all available levels */
    unsigned freeLevelsNum = 0;
    
#ifdef DEBUG_ALL_LEVELS
    
    freeLevelsNum = dynamic_cast<CCString*>(AppGlobal::stMapsConfig->objectForKey(st_maps_config_capacity))->intValue();
#else
    
    freeLevelsNum = PurchaseHelper::isPurchased ?
    dynamic_cast<CCString*>(AppGlobal::stMapsConfig->objectForKey(st_maps_config_capacity))->intValue() :
    dynamic_cast<CCString*>(AppGlobal::stMapsConfig->objectForKey(st_maps_config_free))->intValue();
    
#endif
    
    
    // create a level list
    m_pAvailableLevels = new CCArray();
    
    // add elements to stAvailableLevels
    CCObject* obj = NULL;
    unsigned i = 0;
    
    CCARRAY_FOREACH(AppGlobal::stAllLevelsInformation, obj)
    {
        if (i++ >= freeLevelsNum)
            break;
        
        m_pAvailableLevels->addObject(obj);
    }
    
    // -------------------------------------------------------
    // rate us
    mbMayRateUs = false;
    
    long nowTimestamp = namespaceST::STUtility::getCurrentTimeSeconds();
    long beforeTimestamp = EncryptDataHelper::getRateUsTimestamp(key_rate_us_timestamp);
    int during = namespaceST::STUtility::getHoursByTimeInterval(beforeTimestamp, nowTimestamp);
    /*
     CCLOG("########timestamp2 %ld ", nowTimestamp);
     CCLOG("########timestamp 3 %ld", beforeTimestamp);
     CCLOG("#########during %d hours", during);
     */
    
    bool brun = CCUserDefault::sharedUserDefault()->getBoolForKey(key_rate_us_never, true);
    if (brun)
    {
        if (during >= RATE_US_DELAY_1)
        {
            mbMayRateUs = true;
        }
    }
    else
    {
        if (during >= RATE_US_DELAY_2)
        {
            mbMayRateUs = true;
        }
    }
    
}

GameScene::~GameScene()
{
    CC_SAFE_RELEASE(m_pScoringSystem);
    CC_SAFE_DELETE(m_pAvailableLevels);
    CC_SAFE_RELEASE_NULL(stAllHiddObjectsTexts);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("in-game.plist");
}


void GameScene::unlockAllLevel(const char* pid)
{
    if (strcmp(pid, st_iap_sku_unlock_all) == 0 || strcmp(pid, st_iap_sku_ad) == 0)
    {
        unsigned freeLevelsNum = dynamic_cast<CCString*>
        (AppGlobal::stMapsConfig->objectForKey(st_maps_config_free))->intValue();
        
        
        for (; freeLevelsNum<AppGlobal::stAllLevelsInformation->count(); freeLevelsNum++)
        {
            m_pAvailableLevels->addObject(AppGlobal::stAllLevelsInformation->objectAtIndex(freeLevelsNum));
        }
    }
}

void GameScene::purchaseSuccessful(const char* pid)
{
    unlockAllLevel(pid);
}

void GameScene::purchaseFailed(const char* pid, int errorCode)
{
}

void GameScene::restoreSuccessful(const char* pid)
{
    unlockAllLevel(pid);
}

void GameScene::restoreFailed(const char* pid, int errorCode)
{
}



