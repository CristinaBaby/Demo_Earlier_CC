//
//  GameMapLayer.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#include <string>
#include "GameMapLayer.h"
#include "GameMapSprite.h"
#include "../../AppConfig.h"
#include "../../AppGlobal.h"
#include "../../FileModifiers.h"
#include "../../AnimationPacker/AnimatePacker.h"
#include "../../helpers/AudioHelper.h"


USING_NS_CC;
USING_NS_CC_EXT;
using std::string;


enum {tag_GameMapSprite = 50, tag_anim_error_fadeout = 60,};

/**
 *  Helper macro that convert the tmx file's coord to MapsLayer's coord, and the sprite's anchor point must be point {0.5, 0.5}
 *
 *  NOTE:  the sprite's anchor point must be point point {0.5, 0.5}
 */
#define st_convert_tmx2map_point(X, Y, size) (\
(AppGlobal::IS_2048)? \
ccp(X * 2  + ((CCSize)(size)).width / 2, Y * 2 + ((CCSize)(size)).height / 2) : \
ccp(X + ((CCSize)(size)).width / 2, Y + ((CCSize)(size)).height / 2))

#define ccp_map_horizontal_center(Y)    ccp_horizontal_center(Y * 2)

/**
 * z-order of this layer
 */
enum
{
	z_bg = 0, z_anims = 3, z_objs = 5, z_particles = 100,
};

#define CC_INVALID_INDEX 0xffffffff


GameMapLayer* GameMapLayer::create(GameScene* parent)
{
    GameMapLayer *pRet = new GameMapLayer(parent);
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

GameMapLayer* GameMapLayer::recovery(GameScene *parent, CCArray *leftObjs, CCArray *removedObjs)
{
    GameMapLayer *pRet = new GameMapLayer(parent, leftObjs, removedObjs);
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

bool GameMapLayer::init()
{
    bool pRet = false;
    
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        
		// load tiled map file
        string stMapPath = pGameScene->stLevelResRoot;
        stMapPath.append(st_file_seperator).append(pGameScene->stLevelId).append(st_map_suffix);
        
		stTmxTiledMap = CCTMXTiledMap::create(stMapPath.c_str());
        CC_BREAK_IF(!stTmxTiledMap);
		// parse background info
		CC_BREAK_IF(! getBackground());
        // parse animation on bg
        CC_BREAK_IF(! getBgAnimations());
		// parse hidden objects info
		CC_BREAK_IF(! getHiddObjects());
        
        // error tap image
        m_pError = CCSprite::createWithSpriteFrameName("ui04_wrong.png");
        CC_BREAK_IF(!m_pError);
        this->addChild(m_pError, z_particles);
        m_pError->setVisible(false);
        
        // return contains MAX_DISPLAY_NAME_SIZE objects' list
        subNamesList();
        
        // recieve key events
        setKeypadEnabled(true);
        
        mIsRecovery = false;
        
        pRet = true;
    } while (0);
    
    return pRet;
}

bool GameMapLayer::getBackground()
{
	CCTMXObjectGroup *pGroup = stTmxTiledMap->objectGroupNamed(st_map_group_bg_name);
	// check if the group is null
	if(! pGroup)
    {
        CCLOGERROR("Can't find a group named BGObjects!");
        return false;
    }
    
	// get all objects contained in object group
	CCArray *pObjects = pGroup->getObjects();
	if(! pObjects)
    return false;
    
	CCDictionary *dic = NULL;
	CCObject *obj = NULL;
	CCARRAY_FOREACH(pObjects, obj)
	{
		// get the dictionary of each object in group
		dic = dynamic_cast<CCDictionary*>(obj);
        
		// hidden objects' picture name exclude path and suffix
		string name = dynamic_cast<CCString*>(dic->objectForKey(st_map_obj_name))->m_sString;
        
		// pucture's path name
		string spFileName = pGameScene->stLevelResRoot;
		//  append suffix
		spFileName.append(st_file_seperator).append(name).append(st_suffix_jpg);
        
        
		mpMapBg = CCSprite::create(spFileName.c_str());
		if(!mpMapBg)
        {
            CCLOGERROR("Can't create background sprite! %s", spFileName.c_str());
            return false;
        }
        
        mpMapBg->setPosition(CCPointZero);
        
        break;
	}
    
    
    /* create a scroll view to move map */
    m_pScrollView = STMapScrollView::create(this, GET_CCDIRCTOR->getVisibleSize(), mpMapBg);
    if (! m_pScrollView)
    return false;
    m_pScrollView->setDirection(kCCScrollViewDirectionBoth);
    m_pScrollView->setPosition(GET_CCDIRCTOR->getVisibleOrigin());
    
    this->addChild(m_pScrollView, z_bg);
    
	return true;
}

bool GameMapLayer::getBgAnimations()
{
    CCTMXObjectGroup *pGroup = stTmxTiledMap->objectGroupNamed(st_map_group_anim_name);
	// check if the group is null
	if(! pGroup)
    {
        CCLOG("this level have doesn't have any animation.");
        return true;
    }
    
    // get all objects contained in object group
	CCArray *pObjects = pGroup->getObjects();
	if(! pObjects)
    return false;
    
    CCDictionary *dic = NULL;
	CCObject *obj = NULL;
	CCARRAY_FOREACH(pObjects,obj)
	{
		// get the dictionary of each object in group
		dic = dynamic_cast<CCDictionary*>(obj);
        
		// animation name
		string animName = dynamic_cast<CCString*>(dic->objectForKey(st_map_obj_name))->m_sString;
        // the first frame of this animation
        string animFirstFrame = dynamic_cast<CCString*>(dic->objectForKey(st_map_anim_ff))->m_sString;
        // this animation's file create by tools
        string animTargetFile = dynamic_cast<CCString*>(dic->objectForKey(st_map_anim_tf))->m_sString;
        
        // animation's coord
		float x = dynamic_cast<CCString*>(dic->objectForKey(st_map_obj_x))->floatValue();
		float y = dynamic_cast<CCString*>(dic->objectForKey(st_map_obj_y))->floatValue();
        
        // create animation
        mAnimTFPath = pGameScene->stLevelResRoot;
        mAnimTFPath.append(st_file_seperator).append(animTargetFile);
        //In function of your layer class
        AnimatePacker::getInstance()->loadAnimations(mAnimTFPath.c_str());
        CCSprite *sprite = CCSprite::createWithSpriteFrameName(animFirstFrame.c_str());
        // if sprite is not create correct, go on
        if(!sprite)
        {
            CCLOGERROR("can't load sprite %s", animFirstFrame.c_str());
            continue;
        }
        sprite->setAnchorPoint(CCPointZero);
        sprite->setPosition(ccp2_1024(x, y));
        sprite->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getSequence(animName.c_str())));
        mpMapBg->addChild(sprite, z_anims);
	}
    
    return true;
}

bool GameMapLayer::getHiddObjects()
{
    // packed texture path
    string hiddObjsFramePlist = pGameScene->stLevelResRoot;
    hiddObjsFramePlist.append(st_file_seperator).append(pGameScene->stLevelId);
    
    string hiddObjsImage = hiddObjsFramePlist;
    hiddObjsImage.append(st_map_hidd_objs_pvr_ccz);
    
    
    /*
     *  for some reason .pvr.ccz format can't reproduct accurate color,
     *  so we use .png format instead.
     *  when .pvr.ccz file doesn't exist, find .png file instead.
     */
    std::string fullpathname = CCFileUtils::sharedFileUtils()->fullPathForFilename(hiddObjsImage.c_str());
    
    if (! CCFileUtils::sharedFileUtils()->isFileExist(fullpathname)) {
        
        hiddObjsImage = hiddObjsFramePlist;
        hiddObjsImage.append(st_map_hidd_objs_png);
    }
    
    hiddObjsFramePlist.append(st_map_hidd_objs_suffix);
    
    // add sprite sheet via .plist file
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(hiddObjsFramePlist.c_str(), hiddObjsImage.c_str());
    
    // get tmx object group
	CCTMXObjectGroup *pGroup = stTmxTiledMap->objectGroupNamed(st_map_group_obj_name);
	// check if the group is null
    if(! pGroup)
    {
        CCLOGERROR("Can't find a group named Objects!");
        return false;
    }
    
	// get all objects contained in object group
	CCArray *pObjects = pGroup->getObjects();
	if(! pObjects)
    return false;
    
    pHiddBathNode = CCSpriteBatchNode::create(hiddObjsImage.c_str(), 32);
    if (!pHiddBathNode) return false;
    mpMapBg->addChild(pHiddBathNode, z_objs);
    
	CCDictionary *dic = NULL;
	CCObject *obj = NULL;
	CCARRAY_FOREACH(pObjects,obj)
	{
		// get the dictionary of each object in group
		dic = (CCDictionary*)obj;
        
		// hidden objects' coord
		float x = dynamic_cast<CCString*>(dic->objectForKey(st_map_obj_x))->floatValue();
		float y = dynamic_cast<CCString*>(dic->objectForKey(st_map_obj_y))->floatValue();
        
		// hidden objects' display name as uique id
		CCString *objID = dynamic_cast<CCString*>(dic->objectForKey(st_map_obj_id));
        
        // if this id included in removed list, we need not create sprite.
        if(objID && (isInStringList(m_pRemovedSprites, objID) != CC_INVALID_INDEX))
        {
            continue;
        }
        
		// hidden objects' picture name exclude path and suffix under
        string name = objID->m_sString;
        name.append("_w.png");
        
        
        CCLog("NAME=%s",name.c_str());
        GameMapSprite* sprite = GameMapSprite::createWithSpriteFrameName(name.c_str(), this);
        //        STTouchableSprite* sprite = STTouchableSprite::create("tutorial_skip.png");
        if (!sprite)
        {
            CCLOGERROR("Can't load %s sprite!", name.c_str());
            continue;
            // return false;
        }
        
        sprite->setPosition(st_convert_tmx2map_point(x, y, sprite->getContentSize()));
        sprite->setUserObject(objID);
        sprite->setTag(tag_GameMapSprite);
        sprite->setOnClickedListener(this, callfuncN_selector(GameMapLayer::onHiddObjectClicked));
        // add this sprite to background
        pHiddBathNode->addChild(sprite);
		
		// add displayName in the target list if this is a new level
        if (!mIsRecovery)
        {
            targetNamesList->addObject(objID);
        }
		
	}
    
	return true;
}

void GameMapLayer::subNamesList()
{
#ifndef DEBUG_ALL_OBJECTS
    // if this is recovery one, target names list already created, so we don't recreate.
	if(!mIsRecovery && targetNamesList && (targetNamesList->count() >= MAX_DISPLAY_NAME_SIZE) )
	{
        // ensure the result is not same as before
        srand(fabs(clock()));
        
		// random remove the additional objects
		while( targetNamesList->count() > MAX_DISPLAY_NAME_SIZE )
		{
			targetNamesList->fastRemoveObject(targetNamesList->randomObject());
		}
        
        //        // rand sequence
        //        Util::stArrayShuffle(targetNamesList);
	}
#endif
    
	// get all children of tiled map
	CCArray *children =  pHiddBathNode->getChildren();
	if(children && children->count() > 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(children, child)
		{
			CCNode* pChild = (CCNode*) child;
			// echo and find GameMapSprite
			if ( pChild && (pChild->getTag() == tag_GameMapSprite) )
			{
				GameMapSprite* gms = dynamic_cast<GameMapSprite*>(pChild);
				// if this sprite is the target list,set it can be removed.
				CCString *name =  dynamic_cast<CCString*>(gms->getUserObject());
				if(name && (isInStringList(targetNamesList, name) != CC_INVALID_INDEX))
				{
					gms->setIsTarget(true);
				}
			}
		} // end CCARRAY_FOREACH
	}
}

// when resume game, the CCString object changed, and we need judge the value only.
int GameMapLayer::isInStringList(CCArray* list, CCString* target)
{
    int index = 0;
    
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(list, pObj)
    {
        CCString* id = (CCString*)pObj;
        if (id->m_sString == target->m_sString)
        {
            return index;
        }
        
        index ++;
    }
    
    return CC_INVALID_INDEX;
}

void GameMapLayer::onHiddObjectClicked(GameMapSprite* hiddObj)
{
    CCString* objID = dynamic_cast<CCString*>(hiddObj->getUserObject());
    
    const int idx =  isInStringList(targetNamesList, objID);
    if (idx == CC_INVALID_INDEX)
    return;
    
    
    // remove from list
    //    targetNamesList->fastRemoveObject(objID);
    targetNamesList->fastRemoveObjectAtIndex(idx);
    // add to removed list
    m_pRemovedSprites->addObject(objID);
    
    hiddObj->fadeoutAndRemove();
    
    // play particle
    CCParticleSystem*  m_emitter = NULL;
    
    if (GameScene::s_uSkillBonusIndex == idx)
    {
        pGameScene->updateSkillBonus(true);
        
        m_emitter = CCParticleSystemQuad::create("particles/skill_bonus.plist");
        
        AudioHelper::getInstance()->playSound(sound_game_skill_bonus);
    }
    else
    {
        pGameScene->updateSkillBonus(false);
        // create a particle anmiation
        m_emitter = CCParticleSystemQuad::create("particles/tap_correct.plist");
        
//        CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage("particles/fire.png");
//        m_emitter->setTexture(texture);
        
        
        AudioHelper::getInstance()->playSound(sound_game_correct);
    }
    
	// auto remove particle after play over
	m_emitter->setAutoRemoveOnFinish(true);
	// add to parent layer
	mpMapBg->addChild(m_emitter);
	// set positon in center of sprite
	m_emitter->setPosition(ccp(hiddObj->boundingBox().getMidX(),hiddObj->boundingBox().getMidY()));
    
    
    // inform game ui layer, must place after check skill bonus.
    pGameScene->refreshUIContent(idx);
}

// call this method from GameMapSprite, this just for call game over and complete layer after animation finish.
void GameMapLayer::removeHiddObject(GameMapSprite* hiddObj)
{
    if (hiddObj)
    {
        hiddObj->setVisible(false);
    }
    
    if (targetNamesList->count() <= 0)
    {
        pGameScene->finishGame(true);
    }
    
}

static bool isFirst = true;
void GameMapLayer::spreadTouch(bool isTouchEnd, CCTouch *pTouch, bool isCancel /*= false*/)
{
    if (!isTouchEnd)
    {
        if (isFirst)
        {
            m_pScrollView->ccTouchBegan(pTouch, NULL);
            isFirst = false;
        }
        else
        {
            m_pScrollView->ccTouchMoved(pTouch, NULL);
        }
    }
    else
    {
        if (isCancel)
            m_pScrollView->ccTouchCancelled(pTouch, NULL);
        else
            m_pScrollView->ccTouchEnded(pTouch, NULL);
        
        isFirst = true;
    }
}

void GameMapLayer::showHint()
{
    CCString* objID = (CCString*)targetNamesList->objectAtIndex(0);
    if (NULL == objID)
    return;
    
    // get all children of tiled map
	CCArray *children =  pHiddBathNode->getChildren();
	if(children && children->count() > 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(children, child)
		{
			CCNode* pChild = (CCNode*) child;
			// echo and find GameMapSprite
			if ( pChild && (pChild->getTag() == tag_GameMapSprite) )
			{
				GameMapSprite* gms = (GameMapSprite*) pChild;
				// if this sprite is the target list,set it can be removed.
				CCString *name = dynamic_cast<CCString*>(gms->getUserObject());
				if(name && (objID->m_sString == name->m_sString))
				{
                    pHiddBathNode->boundingBox();
                    CCPoint point = gms->getPosition();
					playHintParticle(point);
                    
                    m_pScrollView->scrollByHint(point);
                    
                    break;
				}
			}
		} // end CCARRAY_FOREACH
	}
}

void GameMapLayer::playHintParticle(CCPoint pos)
{
    // create a particle anmiation
    CCParticleSystem*  m_emitter = CCParticleSystemQuad::create("particles/hint.plist");
    // auto remove particle after play over
    m_emitter->setAutoRemoveOnFinish(true);
    // add to parent layer
    mpMapBg->addChild(m_emitter, z_particles);
    // set positon in center of sprite
    m_emitter->setPosition(this->convertToNodeSpace(pos));
}

void GameMapLayer::showErrorTip(CCTouch *touch)
{
    AudioHelper::getInstance()->playSound(sound_game_error);
    
    CCPoint pos = this->convertTouchToNodeSpace(touch);
    
    if (!m_pError->isVisible())
    {
        m_pError->setVisible(true);
    }
    
    m_pError->setPosition(pos);
    
    // stop the running action that started last time
    m_pError->stopActionByTag(tag_anim_error_fadeout);
    
    m_pError->setOpacity(255);
    
    CCAction* action = CCFadeOut::create(0.4f);
    action->setTag(tag_anim_error_fadeout);
    
    m_pError->runAction(action);
    
    // disable bonus mode, and reset data
    pGameScene->interruptByError();
}

void GameMapLayer::keyBackClicked()
{
    pGameScene->pauseGame();
}


/*
float st_initDistance;
float st_initScale;

void GameMapLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *)(*(pTouches->begin()));
    m_pScrollView->ccTouchBegan(touch, pEvent);
}

void GameMapLayer::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    const int tCount = pTouches->count();
    if(tCount > 1) // multitouch mode
    {
        CCSetIterator iter = pTouches->begin();
        
        // we just support two points touch at same time
        CCTouch* touch1 = (CCTouch*)(*iter); // first point
        CCTouch* touch2 = (CCTouch*)(*(++iter)); // second point
        scaleMapLayer(touch1, touch2);
    }
    else if(tCount > 0) // single touch
    {
        CCTouch *touch = (CCTouch *)(*(pTouches->begin()));
        m_pScrollView->ccTouchMoved(touch, pEvent);
    }
    else
    {
        // touch events not more than 0
    }

}

void GameMapLayer::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    st_initDistance = 0.f;
    CCTouch *touch = (CCTouch *)(*(pTouches->begin()));
    m_pScrollView->ccTouchEnded(touch, pEvent);
}

void GameMapLayer::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    st_initDistance = 0.f;
    CCTouch *touch = (CCTouch *)(*(pTouches->begin()));
    m_pScrollView->ccTouchCancelled(touch, pEvent);
}


void GameMapLayer::scaleMapLayer(CCTouch *touch1, CCTouch *touch2)
{
    if (st_initDistance <= 0) {
        CCPoint _p1 = touch1->getPreviousLocation();
        CCPoint _p2 = touch2->getPreviousLocation();
        st_initDistance = _p1.getDistance(_p2);
        st_initScale = mpMapBg->getScale();
//        st_initScale = m_pScrollView->getZoomScale();
    }
    
    CCPoint p1 = touch1->getLocation();
    CCPoint p2 = touch2->getLocation();
    float diff = p1.getDistance(p2);
    
    float scale = st_initScale * diff / st_initDistance;
    CCLOG("#### map scale %f", scale);
    
    if (scale > 1.3)
        scale = 1.3f;
    else if (scale < 1)
        scale = 1;
    
    mpMapBg->setScale(scale);
}
*/


GameMapLayer::GameMapLayer(GameScene* parent)
: mAnimTFPath(), isErrorTap(false), mpErrorTip(NULL), mpMapBg(NULL), pHiddBathNode(NULL), stTmxTiledMap(NULL)
{
    pGameScene = parent;
    
    targetNamesList = CCArray::create();
    targetNamesList->retain();
    
    m_pRemovedSprites = CCArray::create();
    m_pRemovedSprites->retain();
    
    mIsRecovery = false;
}

GameMapLayer::GameMapLayer(GameScene* parent, CCArray *leftObjs, CCArray *removedObjs)
: mAnimTFPath(), isErrorTap(false), mpErrorTip(NULL), mpMapBg(NULL), pHiddBathNode(NULL), stTmxTiledMap(NULL)
{
    pGameScene = parent;
    
    targetNamesList   = CCArray::createWithArray(leftObjs);
    targetNamesList->retain();
    
    m_pRemovedSprites = CCArray::createWithArray(removedObjs);
    m_pRemovedSprites->retain();
    
    
    mIsRecovery = true;
}

GameMapLayer::~GameMapLayer()
{
    CC_SAFE_RELEASE(targetNamesList);
    
    CC_SAFE_RELEASE(m_pRemovedSprites);
    
    if (!mAnimTFPath.empty())
    {
        AnimatePacker::getInstance()->freeAnimations(mAnimTFPath.c_str());
    }
}

