//
//  GameMapLayer.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#ifndef __LVUP002__GameMapLayer__
#define __LVUP002__GameMapLayer__

#include <string.h>
#include "cocos2d.h"
#include "STMapScrollView.h"
#include "../../scenes/GameScene.h"
#include "../../helpers/GameSaveHelper.h"


class GameMapSprite;

class GameMapLayer : public cocos2d::CCLayer
{
    GameScene* pGameScene;
    //
	cocos2d::CCTMXTiledMap *stTmxTiledMap;
    //
    cocos2d::CCSprite *mpMapBg;
    //
    cocos2d::CCSpriteBatchNode* pHiddBathNode;
    //
    std::string mAnimTFPath;
    
    // error tap indicator
    bool isErrorTap;
    
    // error tip sprite
    cocos2d::CCSprite* mpErrorTip;
    
    STMapScrollView* m_pScrollView;
    
    bool mIsRecovery;
    
    cocos2d::CCSprite* m_pError;
    
public:
    
    static GameMapLayer* create(GameScene* parent);
    static GameMapLayer* recovery(GameScene* parent, cocos2d::CCArray *leftObjs, cocos2d::CCArray *removedObjs);
    
    GameMapLayer(GameScene* parent);
    GameMapLayer(GameScene* parent, cocos2d::CCArray *leftObjs, cocos2d::CCArray *removedObjs);
	virtual ~GameMapLayer();
	virtual bool init();

    virtual void keyBackClicked();
    
public:
    
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, targetNamesList, DisplayNamesList);
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pRemovedSprites, RemovedNamesList);
       
    void showHint();
    
    /**
     * @brief when tap a error object, show tip
     */
    void showErrorTip(cocos2d::CCTouch *touch);
    
private:
    // scale map layer
//    void scaleMapLayer(CCTouch *touch1, CCTouch *touch2);
    
	// parse tmx tiled map file, get the background's information
	bool getBackground();
    
    // if there are animaitons on the background's layer, get it.
    bool getBgAnimations();
    
	// parse tmx tiled map file, get the hidden objects' information
	bool getHiddObjects();
    
    // create a subset of map sprites in file
	void subNamesList();
    
    /*
     * @return if return value is lesser than 0, the target not find in the list,
     *         otherwise return the target index in the list.
     */
    int isInStringList(cocos2d::CCArray* list, cocos2d::CCString* target);
    
    void onHiddObjectClicked(GameMapSprite* hiddObj);
    
    void playHintParticle(cocos2d::CCPoint pos);
    
    // scale map layer
    void scaleMapLayer(cocos2d::CCTouch *touch1, cocos2d::CCTouch *touch2);
    
public:
    // call this method from GameMapSprite
    void removeHiddObject(GameMapSprite* hiddObj);
    
    // when touch hidden object, but the action is dragging, we need pass this
    // touch to maplayer for moving.
    void spreadTouch(bool isTouchEnd, cocos2d::CCTouch *pTouch, bool isCancel = false);
    
};

#endif /* defined(__LVUP002__GameMapLayer__) */
