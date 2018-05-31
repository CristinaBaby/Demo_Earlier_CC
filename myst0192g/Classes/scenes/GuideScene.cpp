//
//  GuideScene.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#include "GuideScene.h"
#include "LevelMapScene.h"
#include "../AppGlobal.h"
#include "../widgets/STButtonMenu.h"
#include "../helpers/AudioHelper.h"
#include "../modules/AnalyticX.h"

USING_NS_CC;

#define SWITCH_DELAY        2
#define GUIDE_PICS_NUM      5
#define TAG_SPRITE          50

static unsigned current = 1;
static bool isFinish = false;

bool GuideScene::init()
{
    bool pRet = false;
    
    do
    {
        CCRect vrect = CCRectPointSizeMake(GET_CCDIRCTOR->getVisibleOrigin(), GET_CCDIRCTOR->getVisibleSize());
        
        pUILayer = STUILayer::create();
        CC_BREAK_IF(!pUILayer);
        this->addChild(pUILayer);
        
        
        // skip button
        CCMenuItemImage* pBtnSkip = CCMenuItemImage::create("tutorial/tutorial_skip.png", "tutorial/tutorial_skip.png",
                                                            this, menu_selector(GuideScene::onSkipClicked));
        CC_BREAK_IF(!pBtnSkip);
        // on the top-right corner of screen
        pBtnSkip->setAnchorPoint(ccp(1, 1));
        pBtnSkip->setPosition(ccp((vrect.getMaxX() - 15), (vrect.getMaxY() - 15)));
        
        STButtonMenu* pMenu = STButtonMenu::create(pBtnSkip, NULL);
        CC_BREAK_IF(!pMenu);
        this->addChild(pMenu, 10);
        
        // 1st picture
        CCSprite* _1frame = CCSprite::create("tutorial/tutorial_1.jpg");
        CC_BREAK_IF(!_1frame);
        _1frame->setTag(TAG_SPRITE);
        _1frame->setPosition(ccp_center_screen);
        pUILayer->addChild(_1frame);
        
        // draw next picture every 2 seconds
        schedule(selector, SWITCH_DELAY, GUIDE_PICS_NUM - 1, SWITCH_DELAY);
        
        pRet = true;
    } while (0);
    
    return pRet;
}

void GuideScene::onSkipClicked(CCObject* obj)
{
    if (obj)
        AudioHelper::getInstance()->playSound(sound_button);
    
    this->pauseSchedulerAndActions();
    
    if (isFinish)
        AnalyticX::flurryLogEvent("Tutorial - Finish");
    else
        AnalyticX::flurryLogEvent("Tutorial - Skip");
     
    GET_CCDIRCTOR->replaceScene(CCTransitionSlideInR::create(0.5, LevelMapLayer::scene()));
}

void GuideScene::nextFrame(float)
{
    
    if (current == GUIDE_PICS_NUM)
    {
        isFinish = true;
        
        // guide over, enter choose scene
        onSkipClicked();
    }
    else
    {
        CCString *sprName = CCString::createWithFormat("tutorial/tutorial_%d.jpg", ++current);
        /*
        char sprName[50] = {0};
        sprintf(sprName, "tutorial/tutorial_%d.jpg", ++current);
        */
        
        CCSprite* _frame = CCSprite::create(sprName->getCString());
        if (!_frame)
        {
            CCLOGERROR("Can not Load sprite %s", sprName->getCString());
            
            // end this guide, jump to choose scene
            onSkipClicked();
            return;
        }
        else
        {
            _frame->setTag(TAG_SPRITE);
        }
        
        // remove the sprite which before added.
        pUILayer->removeChildByTag(TAG_SPRITE);
        
        
        // add next frame
        _frame->setPosition(ccp_center_screen);
        pUILayer->addChild(_frame);
    }
    
}

GuideScene::GuideScene() : pUILayer(NULL)
{
    selector = schedule_selector(GuideScene::nextFrame);
}

GuideScene::~GuideScene()
{
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}