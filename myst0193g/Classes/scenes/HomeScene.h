//
//  HomeScene.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-18.
//
//

#ifndef __LVUP002__HomeScene__
#define __LVUP002__HomeScene__

#include "cocos2d.h"
#include "../widgets/STUILayer.h"

class HomeLayer : public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* scene(bool save = false);
    
    static HomeLayer* create(bool savegame)
    {
        HomeLayer *pRet = new HomeLayer(savegame);
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
    
    HomeLayer(bool savegame);
    virtual ~HomeLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
private:
    // indicate whether we will show resume game button.
    bool haveSavedGame;
    
    // except background, all nodes will add to this
    STUILayer* pUILayer;
    
    // normal mode
    bool initWithoutResume();
    // have saved game
    bool initWithResume();
    
    //
    void onButtonClicked(cocos2d::CCObject*);
    
public:
    //
    void resetLayoutToWithoutResume();
    
private:
    // The back key clicked
    virtual void keyBackClicked();
    
    void quit(cocos2d::CCNode*);
};

#endif /* defined(__LVUP002__HomeScene__) */
