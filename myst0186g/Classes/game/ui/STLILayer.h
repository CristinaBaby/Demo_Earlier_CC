//
//  STLILayer.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-8.
//
//

#ifndef __LVUP002__STLILayer__
#define __LVUP002__STLILayer__

#include <cocos2d.h>
#include "../../AppGlobal.h"
#include "../../scenes/GameScene.h"
#include "../../widgets/STPageController.h"

class STLILayer :   public cocos2d::CCLayerColor, public STTableDelegate, public STPageControllerListener
{
    // game scene
    GameScene* mpGameScene;
    
    STPageController* mp_PageController;
    
    cocos2d::CCMenuItemSprite *pBtnPrevious, *pBtnNext;
    
    // this is used to store sprite frames in image mode.
    cocos2d::CCArray* mp_a_SpriteFrameArray;
    
public:
    static STLILayer* create(GameScene* gameScene);
    /** creates a CCLayer with color. Width and height are the window size. */
    static STLILayer * create(const cocos2d::ccColor4B& color, GameScene* gameScene);
    
    STLILayer(GameScene* gameScene);
    virtual ~STLILayer();
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color, GLfloat width, GLfloat height);
    
public:
    virtual STPage* getANewPageAtIndex(unsigned index);
    
    virtual cocos2d::CCSize getPageSize();
    
    virtual unsigned getCapacity();
    
    virtual void onPageClicked(cocos2d::CCNode* view, unsigned index){};
    
    virtual void onTurnPage(unsigned focusPageIndex);
    
public:
    void refresh(int objIdx);
    
    void turnToFirstPage();
    
    /**
     * @brief get a random index of current page
     */
    unsigned getRandomIndexInPage();

private:
    //
    void onPageClicked(CCObject*);
    
    bool autoTurnPage();

};


#endif /* defined(__LVUP002__STLILayer__) */
