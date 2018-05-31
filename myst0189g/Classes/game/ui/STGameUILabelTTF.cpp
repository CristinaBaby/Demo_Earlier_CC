//
//  STGameUILabelTTF.cpp
//  MysteryGuardians
//
//  Created by Steven.Xc.Tian on 13-6-27.
//
//

#include "STGameUILabelTTF.h"
#include "../../AppGlobal.h"

USING_NS_CC;

#define font_size   45

enum
{
    disappear_action = 11,
    move_action,
    remove_directly_action,
};

STGameUILabelTTF* STGameUILabelTTF::create(const char *string, const CCSize& dimensions)
{
    
    STGameUILabelTTF *pRet = new STGameUILabelTTF();
    
    if (pRet && pRet->initWithString(string, font_Helvetica_Neue, fontsize(font_size), dimensions,
                                     kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter))
    {
        pRet->autorelease();
        
        pRet->isInit = true;
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void STGameUILabelTTF::onEnter()
{
    CCNode::onEnter();
    
    // if this is not first time enter this node, and not resume game
    // play move left animation
//    if (!isInit /* && !stGameScene->getSavedGameFlag() */)
//    {
//        // release this node's reference, which add it in removedirectly() method
//        // when reference is equals to 1, skip this step
//        if(this->retainCount() > 1)
//            this->release();
//        
//        moveTowardsLeft();
//    }
//    else
//    {
//        isInit = false;
//    }
    
}

void STGameUILabelTTF::removedirectly()
{
    if(isRemoving)
        return;
    
    // for play fadeinanimation  smoothly
    setOpacity(0);
    
    // keep this node avoid it is deleted after call removeFromParent() method
    this->retain();
    
    removeFromParent();
}

void STGameUILabelTTF::removeWithAnimation()
{
    if(isRemoving)
        return;
    
    /* complete remove this node, never use it. */
    
    isRemoving = true;
    CCFiniteTimeAction* action =
    CCSequence::create(
                       CCSpawn::create(CCScaleTo::create(0.5f, 1.5f), CCFadeOut::create(0.5f), NULL),
                       CCCallFunc::create(this , callfunc_selector(STGameUILabelTTF::removeFromParent)),
                       NULL);
    action->setTag(disappear_action);
    runAction(action);
}

void STGameUILabelTTF::moveTowardsLeft()
{
    if(isRemoving)
        return;
    
    stopActionByTag(move_action);
    
    CCFiniteTimeAction* action =  CCSequence::create(CCDelayTime::create(0.5f),
                                                     CCSpawn::create(
                                                                     CCMoveTo::create(0.5f, stDestnationPoint),
                                                                     CCFadeIn::create(0.5f), NULL),
                                                     NULL);
    action->setTag(move_action);
    runAction(action);
}

void STGameUILabelTTF::setDestinationPoint(const CCPoint& destination)
{
    stDestnationPoint = destination;
}

const CCPoint& STGameUILabelTTF::getDestinationPoint()
{
    return stDestnationPoint;
}

void STGameUILabelTTF::setGameScene(GameScene* scene)
{
    stGameScene = scene;
}
