//
//  GameFinishLayer.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-17.
//
//

#ifndef __LVUP002__GameFinishLayer__
#define __LVUP002__GameFinishLayer__

#include "ScoringSystem.h"
#include "../scenes/GameScene.h"
#include "../widgets/CoverLayer.h"
#include "../widgets/STButton.h"

typedef enum
{ gameover, gamecomplete, alllevelfinish}
GameFinishType;

class GameFinishLayer  : public CoverLayer
{
    GameScene               *m_pGameScene;
    ScoringSystem           *m_pScoringSystem;
    cocos2d::CCSprite       *m_pBackground;
    STButton                *pBtnHome, *pBtnReplay, *pBtnNext;
    
public:
    static GameFinishLayer* create(GameScene* gameScene, ScoringSystem* score, GameFinishType type);
    
    GameFinishLayer(ScoringSystem* score, GameScene* gameScene);
    virtual ~GameFinishLayer();
    
    virtual bool init(GameFinishType type);
    virtual void onExit();
    
    void setButtonsEnable(bool enable);
    
private:
    bool initGameComplete();
    bool initGameOver();
    bool initAllLevelFinish();
    
    void onButtonClicked(CCNode*);
    
    std::string getHighScoreKey();
    
    void saveFinishMode();
};

#endif /* defined(__LVUP002__GameFinishLayer__) */
