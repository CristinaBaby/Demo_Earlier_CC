//
//  ChooseModeScene.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#include "ChooseModeScene.h"
#include "GameScene.h"
#include "LevelMapScene.h"
#include "../widgets/STUILayer.h"
#include "../helpers/AudioHelper.h"

USING_NS_CC;
using std::string;

enum
{
    tag_btn_pic = 50, tag_btn_sil, tag_btn_word,
};


CCScene* ChooseModeLayer::scene(const unsigned levelID)
{
    CCScene* pScene = CCScene::create();
    if (pScene)
    {
        ChooseModeLayer* pLayer = ChooseModeLayer::create(levelID);
        if (pLayer)
        {
            pScene->addChild(pLayer);
        }
    }
    
    return pScene;
}

bool ChooseModeLayer::init()
{
    bool pRet = false;

    do
    {
        CC_BREAK_IF(!CCLayer::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
        
        CCSprite* pBg = CCSprite::create("ui01_menu_bg.jpg");
        CC_BREAK_IF(!pBg);
        pBg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        this->addChild(pBg, 0);
        
        // create a scaled ui layer
        STUILayer* pUILayer = STUILayer::create();
        CC_BREAK_IF(!pUILayer);
        this->addChild(pUILayer, 5);
        
        CCSpriteBatchNode* pSBN = CCSpriteBatchNode::create("choose-map.pvr.ccz");
        CC_BREAK_IF(!pSBN);
        pUILayer->addChild(pSBN);
        
        
        pBtnPic = STButton::createWithSpriteFrameName(string("ui02_button.png"), string("ui02_button_h.png"),
                                                      this, callfuncN_selector(ChooseModeLayer::onClicked));
        CC_BREAK_IF(!pBtnPic);
        pBtnPic->setTag(tag_btn_pic);
        pBtnPic->setAnchorPoint(ccp(.5, 0));
        pBtnPic->setPosition(ccp2(490, 260));
        pSBN->addChild(pBtnPic);
        
        CCSprite *pic = CCSprite::createWithSpriteFrameName("ui02_picture.png");
        CC_BREAK_IF(!pic);
        pic->setAnchorPoint(CCPointZero);
        pBtnPic->addChild(pic);
        
        
        pBtnSil = STButton::createWithSpriteFrameName(string("ui02_button.png"), string("ui02_button_h.png"),
                                                      this, callfuncN_selector(ChooseModeLayer::onClicked));
        CC_BREAK_IF(!pBtnSil);
        pBtnSil->setTag(tag_btn_sil);
        pBtnSil->setAnchorPoint(ccp(.5, 0));
        pBtnSil->setPosition(ccp2(1024, 260));
        pSBN->addChild(pBtnSil);
        
        CCSprite *sil = CCSprite::createWithSpriteFrameName("ui02_silhouette.png");
        CC_BREAK_IF(!sil);
        sil->setAnchorPoint(CCPointZero);
        pBtnSil->addChild(sil);
        
        
        pBtnWord = STButton::createWithSpriteFrameName(string("ui02_button.png"), string("ui02_button_h.png"),
                                                       this, callfuncN_selector(ChooseModeLayer::onClicked));
        CC_BREAK_IF(!pBtnWord);
        pBtnWord->setTag(tag_btn_word);
        pBtnWord->setAnchorPoint(ccp(.5, 0));
        pBtnWord->setPosition(ccp2(1553, 260));
        pSBN->addChild(pBtnWord);
        
        CCSprite *word = CCSprite::createWithSpriteFrameName("ui02_word.png");
        CC_BREAK_IF(!word);
        word->setAnchorPoint(CCPointZero);
        pBtnWord->addChild(word);
        
        
        // logo
        CCSprite* pLogo = CCSprite::createWithSpriteFrameName("ui02_choose_mode.png");
        CC_BREAK_IF(!pLogo);
        pLogo->setAnchorPoint(ccp(.5, 1));
        pLogo->setPosition(ccp_horizontal_center(1405));
        pSBN->addChild(pLogo);
        
        STButton *pBackBtn = STButton::createWithSpriteFrameName(string("ui03_back.png"), string("ui03_back_h.png"), this,
                                                             callfuncN_selector(ChooseModeLayer::onBackClicked));
        CC_BREAK_IF(!pBackBtn);
        this->addChild(pBackBtn, 5);
        pBackBtn->setScale(AppGlobal::UI_SCALE);
        pBackBtn->setAnchorPoint(ccp(0, 1));
        CCRect vRect = VISIBLE_RECT;
        pBackBtn->setPosition(ccp((vRect.getMinX()+get_offset(90)),
                                  (vRect.getMaxY()-get_offset(30))));
        
        
        pRet = true;
    } while (0);
    
    return pRet;
}


void ChooseModeLayer::onClicked(cocos2d::CCNode* button)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    CCNode* btn = dynamic_cast<CCNode*>(button);
    switch (btn->getTag())
    {
        case tag_btn_pic:
            CCLOG("Picture");
            mSearchMode = Picture;
            break;
            
        case tag_btn_sil:
            CCLOG("Silhouette");
            mSearchMode = Silhouette;
            break;
            
        case tag_btn_word:
            CCLOG("Word");
            mSearchMode = Name;
            break;
            
        default:
            mSearchMode = Name;
            break;
    }
    
    CCScene* pGameScene = GameScene::create(mLevelID, mSearchMode);
    GET_CCDIRCTOR->replaceScene(pGameScene);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("choose-map.plist");
}

void ChooseModeLayer::onBackClicked(CCNode*)
{
    GET_CCDIRCTOR->replaceScene(LevelMapLayer::scene());
}


ChooseModeLayer::ChooseModeLayer(const unsigned levelID)
: mSearchMode(Name), mLevelID(levelID)
{
}

ChooseModeLayer::~ChooseModeLayer()
{
}


