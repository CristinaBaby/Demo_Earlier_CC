//
//  GameUILayer.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-28.
//
//

#ifndef __LVUP002__GameUILayer__
#define __LVUP002__GameUILayer__

#include "STLILayer.h"
#include "../../modules/STIABDelegate.h"
#include "../../scenes/GameScene.h"
#include "../../widgets/STButton.h"


class GameUILayer : public cocos2d::CCLayerColor, public STIABDelegate
{
    GameScene* pGameScene;
    STLILayer* pLabelImageLayer;
    
    cocos2d::CCLabelTTF *m_pGameTime, *m_pHintQuantity;
    
    STButton *m_pBtnHint;
    
    cocos2d::CCSprite *m_pUIBg, *m_pHintQuantityBg;
    
    bool isHintCharging;
    
    cocos2d::CCProgressTimer* m_pHintPT;
    
    const float mHintPercentageBefore;
    unsigned    mHintQuantity;
    
public:
    
    static GameUILayer* create(GameScene* parent);
    /** creates a CCLayer with color. Width and height are the window size. */
    static GameUILayer * create(const cocos2d::ccColor4B& color, GameScene* parent);
    
    static GameUILayer * recovery(GameScene* parent, float hintPercentage);
    
    GameUILayer(GameScene* parent, bool isResume, float hintPercentage = 0.0f);
    virtual ~GameUILayer();
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color, GLfloat width, GLfloat height);
    
    virtual void onEnter();
    virtual void onExit();
    
    // default implements are used to call script callback if exist
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
public:
    void refreshContent(int objIdx);
    
    void updateGameTime(std::string timeString);

    unsigned getRandomIdxInPage();
    
    void pauseHintCharging();
    void resumeHintCharging();
    
    float getHintChargingPercentage();
    
private:
    void onPauseClicked(CCNode*);
    //
    void onHintClicked(CCNode*);
    //
    void hintCharging();
    
    void hintFull(CCNode* node);
    
    bool recreateHintCharging();
    
public:
    void purchaseSuccessful(const char* pid);
    void purchaseFailed(const char *pid, int errorCode);
    void restoreSuccessful(const char* pid);
    void restoreFailed(const char* pid, int errorCode);
    
};

#endif /* defined(__LVUP002__GameUILayer__) */
