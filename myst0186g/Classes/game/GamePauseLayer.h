//
//  GamePauseLayer.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-8.
//
//

#ifndef __LVUP002__GamePauseLayer__
#define __LVUP002__GamePauseLayer__

#include <cocos2d.h>
#include "../modules/STIABDelegate.h"
#include "../scenes/GameScene.h"
#include "../widgets/CoverLayer.h"
#include "../widgets/STButton.h"

class GamePauseLayer    : public CoverLayer
{
    GameScene*          m_pGameScene;
    bool                mbResumeBannerAd;
    STButton *pBtnPlay, *pBtnReplay, *pBtnHome, *pBtnSetting;
    cocos2d::CCSprite   *m_pPauseBG;
    
public:
    static GamePauseLayer* create(GameScene* gameScene);
    
    GamePauseLayer(GameScene* gameScene);
    virtual ~GamePauseLayer();
    
    virtual bool init();
    virtual void onEnter();
	virtual void onExit();
    
    void popDeluxeLayer();
    void showBannerAd(CCObject* pObject);
private:
    void onPlayClicked(CCNode*);
    //
    void onReplayClicked(CCNode*);
    //
    void onHomeClicked(CCNode*);
    //
    void onSettingClicked(CCNode*);
    // when setting layer closed
    void onSettingLayerClosed(CCNode*);
    
    void restartGame(CCObject*);
    
    void setButtonsEnable(bool boo);
    
};

#endif /* defined(__LVUP002__GamePauseLayer__) */
