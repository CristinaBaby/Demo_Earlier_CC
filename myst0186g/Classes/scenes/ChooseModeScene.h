//
//  ChooseModeScene.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#ifndef __LVUP002__ChooseModeScene__
#define __LVUP002__ChooseModeScene__

#include "cocos2d.h"
#include "../AppGlobal.h"
#include "../widgets/STButton.h"

class ChooseModeLayer : cocos2d::CCLayer
{
    STButton        *pBtnPic,   *pBtnSil,   *pBtnWord;
    
//    GameMode        mGameMode;
    SearchMode      mSearchMode;
    bool            mbIsBlitzLocked;
    unsigned        mLevelID;
    
public:
    static cocos2d::CCScene* scene(const unsigned levelID);
    
    static ChooseModeLayer* create(const unsigned levelID)
    {
        ChooseModeLayer *pRet = new ChooseModeLayer(levelID);
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
    
    
    ChooseModeLayer(const unsigned levelID);
    virtual ~ChooseModeLayer();
    
    virtual bool init();
    
private:
    void onClicked(cocos2d::CCNode*);
    void onBackClicked(CCNode*);

};

#endif /* defined(__LVUP002__ChooseModeScene__) */
