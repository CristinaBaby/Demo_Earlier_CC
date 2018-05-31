//
//  GameScene.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#ifndef __LVUP002__GameScene__
#define __LVUP002__GameScene__

#include <cocos2d.h>
#include <string>
#include "../AppGlobal.h"
#include "../game/ScoringSystem.h"
#include "../modules/STIABDelegate.h"
#include "../helpers/GameSaveHelper.h"

#define MAX_DISPLAY_NAME_SIZE               16
#define DISAPPEAR_ANIMATION_TIME            0.4

class GameMapLayer;
class GameUILayer;

class GameScene : public cocos2d::CCScene, public STIABDelegate
{
    GameMapLayer            *m_pGameMapLayer;
    GameUILayer             *m_pGameUILayer;
    
    bool                    isGamePaused;
    bool                    isGameFinished;
    
    ScoringSystem           *m_pScoringSystem;
    
    // indicator current level's position
    unsigned                curlevelIndicator;
    
    //
    cocos2d::CCArray        *m_pAvailableLevels;
    
    bool                    mbMayRateUs;
    // already played game levels
    static unsigned         s_PlayedLevels;
    
public:
    const SearchMode        mSearchMode;
    
    // the identifer of current level
    std::string             stLevelId;
    // current level's resuorces root directory
    std::string             stLevelResRoot;
    
    // the index of skill bonus object.
    static int              s_uSkillBonusIndex;
    
    cocos2d::CCDictionary        *stAllHiddObjectsTexts;
    
public:
    static GameScene* create(const unsigned levelID, SearchMode sm);
    static GameScene* recovery();
    
    GameScene(const unsigned levelID, SearchMode sm);
    virtual ~GameScene();
    
    virtual bool init(bool isRecovery);
    
    virtual void onEnter();
    virtual void onExit();
    
public:
    void purchaseSuccessful(const char* pid);
    void purchaseFailed(const char* pid, int errorCode);
    void restoreSuccessful(const char* pid);
    void restoreFailed(const char* pid, int errorCode);
    
public:
    /** get all available objects on the map */
    cocos2d::CCArray* getHiddObjs();
    
    void refreshUIContent(unsigned objIdx);
    
    void onTips();
    
    void resumeGame();
    
    void pauseGame(STIABDelegate* delegate = NULL);
    
    void replayGame();
    
    void finishGame(bool isComplete);
    
    void returnToHome();
    
    void nextLevel();
    
    void clear();
    
    float getHintRechargeTime();
    
    void interruptByError();
    void updateSkillBonus(bool isGet);
    
private:
    //
    bool build(bool isRecovery);
    
    bool loadObjectsName();
    
    void onPauseLayerDismiss(CCNode* );
    void onDeluxeLayerClosed(CCNode*);
    
    void unlockAllLevel(const char* pid);
    
    // schedule this every 1 second.
    void onTimeUpdate(float delta = 0.0f);
    
    bool _pauseInternal();
    bool _resumeInternal();
    
private:
    int  mHitBonusNum;
    bool mbIsHitBonus;
    // continuous correct hit number
    unsigned mContinuousCorrectHit;
    bool mbIsSkillBonus;
    
    void enableHitBonus();
    void disableHitBonus(float delta = 0.0f);
    void enableSkillBonus();
    void disableSkillBonus(float delta = 0.0f);
    
private:
    static GameSaveHelper         *s_pGameSaveHelper;

};

#endif /* defined(__LVUP002__GameScene__) */
