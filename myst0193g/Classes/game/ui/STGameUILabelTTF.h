//
//  STGameUILabelTTF.h
//  MysteryGuardians
//
//  Created by Steven.Xc.Tian on 13-6-27.
//
//

#ifndef __MysteryGuardians__STGameUILabelTTF__
#define __MysteryGuardians__STGameUILabelTTF__

#include "cocos2d.h"
#include "../../scenes/GameScene.h"

NS_CC_BEGIN

class STGameUILabelTTF : public CCLabelTTF
{
    bool isRemoving;
    // first create
    bool isInit;
    
    CCPoint stDestnationPoint;
    
public:
    STGameUILabelTTF() : isRemoving(false), isInit(true), stGameScene(NULL)
    {
    }
    
    static STGameUILabelTTF* create(const char *string, const CCSize& dimensions);
    
    virtual void onEnter();

public:
    /**
     * remove this node with animation. complete remove this node, never reuse it.
     * @return this node's position. when a new node will replace it, need set this node's position
     */
    void removeWithAnimation();
    
    /**
     * when this is the last one of array, remove it directly, and you must use reset() 
     * method also when you reuse this node next time.
     */
    void removedirectly();
    
    /**
     * when this node is reuse, it will be play a move animation, this method is to set the destination,
     * if the animation is interrupted, we can get the correct posion.
     */
    void setDestinationPoint(const CCPoint& destination);
    
    const CCPoint& getDestinationPoint();
    
    void setGameScene(GameScene* scene);
    
private:
    /**
     * when this node replace other node, call it.
     */
    void moveTowardsLeft();
    
    GameScene* stGameScene;
};

NS_CC_END

#endif /* defined(__MysteryGuardians__STGameUILabelTTF__) */
