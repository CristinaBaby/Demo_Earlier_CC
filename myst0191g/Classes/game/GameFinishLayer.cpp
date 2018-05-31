//
//  GameFinishLayer.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-17.
//
//

#include "GameFinishLayer.h"
#include "../AppGlobal.h"
#include "../FileModifiers.h"
#include "../widgets/STUILayer.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/EncryptDataHelper.h"
#include "../utilities/STUserDefault.h"
#include "../utilities/STUtility.h"



USING_NS_CC;
USING_NS_ST;
using namespaceST::STUtility;
using std::string;

#define text_font_size1             fontsize(90)
#define text_font_size2             fontsize(80)

enum
{
    z_text_bg = 1, z_text = 4,
    tag_btn_home = 50, tag_btn_replay, tag_btn_next,
};


GameFinishLayer* GameFinishLayer::create(GameScene* gameScene, ScoringSystem* score, GameFinishType type)
{
    GameFinishLayer* pRet = new GameFinishLayer(score, gameScene);
    if (pRet && pRet->init(type))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return NULL;
}

bool GameFinishLayer::init(GameFinishType type)
{
    
    if(! CoverLayer::init())
        return false;
    
    // the super class set keypad enable default, but in this layer we should disable this function.
    this->setKeypadEnabled(false);
    
    //
    STUILayer* pUILayer = STUILayer::create();
    if(!pUILayer)
        return false;
    this->addChild(pUILayer);
    
    //
    m_pBackground = CCSprite::create("ui05_box.png");
    if(!m_pBackground)
        return false;
    m_pBackground->setPosition(ccp_center_screen);
    pUILayer->addChild(m_pBackground);
    
    
    switch (type)
    {
        case gamecomplete:
            
            if (! initGameComplete())
                return false;
            
            break;
            
        case gameover:
            if (! initGameOver())
                return false;
            
            break;
        case alllevelfinish:
            if (! initAllLevelFinish())
                return false;
            
            break;
            
        default:
            return false;
    }
    
    return true;
}

bool GameFinishLayer::initGameComplete()
{
    bool bRet = false;
    
    do
    {
        AudioHelper::getInstance()->playSound(sound_game_complete);
        
        CCSpriteBatchNode* pBatchNode = CCSpriteBatchNode::create("in-game.pvr.ccz");
        CC_BREAK_IF(!pBatchNode);
        m_pBackground->addChild(pBatchNode);
        
        CCPoint bg_center = ccp_get_center(m_pBackground->getContentSize());
        
        // title
        CCSprite* title = CCSprite::createWithSpriteFrameName("ui05_game_complete.png");
        CC_BREAK_IF(! title);
        title->setPosition(ccp_fixed_X(bg_center.x, 1100));
        pBatchNode->addChild(title);
        
        // text bg
        CCSprite* textBg = CCSprite::createWithSpriteFrameName("ui05_text_bg.png");
        CC_BREAK_IF(! textBg);
        textBg->setPosition(ccp_fixed_X(bg_center.x, 650));
        pBatchNode->addChild(textBg, z_text_bg);
        
        
        const float textOffset = get_offset(340);
        // text time
        CCSprite* textTime = CCSprite::createWithSpriteFrameName("ui05_time.png");
        CC_BREAK_IF(! textTime);
        textTime->setAnchorPoint(ccp(.5, 0));
        textTime->setPosition(ccp_fixed_X((bg_center.x-textOffset), 800));
        pBatchNode->addChild(textTime, z_text);
        
        // label time
        CCLabelTTF* labelTime = CCLabelTTF::create(m_pScoringSystem->getLifeTimeString().c_str(),
                                                   font_Helvetica_Neue, text_font_size1);
        CC_BREAK_IF(! labelTime);
        labelTime->setPosition(ccp_fixed_X(bg_center.x-textOffset, 685));
        m_pBackground->addChild(labelTime, z_text);
        
        
        
        
        // text score
        CCSprite* textScore = CCSprite::createWithSpriteFrameName("ui05_score.png");
        CC_BREAK_IF(! textScore);
        textScore->setAnchorPoint(ccp(.5, 0));
        textScore->setPosition(ccp_fixed_X((bg_center.x+textOffset), 800));
        pBatchNode->addChild(textScore, z_text);
        
        // label score
        CCLabelTTF* labelScore = CCLabelTTF::create(m_pScoringSystem->getFinalScoreString().c_str(),
                                                    font_Helvetica_Neue, text_font_size1);
        CC_BREAK_IF(! labelScore);
        labelScore->setPosition(ccp_fixed_X(bg_center.x+textOffset, 685));
        m_pBackground->addChild(labelScore, z_text);
        
        
        
        
        // text high score
        CCSprite* textHighScore = CCSprite::createWithSpriteFrameName("ui05_high_score.png");
        CC_BREAK_IF(! textHighScore);
        textHighScore->setPosition(ccp_fixed_X((textBg->getContentSize().width/2), 120));
        textBg->addChild(textHighScore, z_text);
        
        // lable high score
        int highestScore = EncryptDataHelper::updateHighScore(getHighScoreKey(), m_pScoringSystem->getFinalScore());
        CCString* highestScoreString = CCString::createWithFormat("%d", highestScore);
        
        CCLabelTTF* labelHighScore = CCLabelTTF::create(highestScoreString->getCString(), font_Helvetica_Neue, text_font_size2);
        CC_BREAK_IF(! labelHighScore);
        labelHighScore->setAnchorPoint(ccp(0, .5f));
        labelHighScore->setPosition(ccp2(960, 485));
        labelHighScore->setHorizontalAlignment(kCCTextAlignmentLeft);
        m_pBackground->addChild(labelHighScore, z_text);
        
        
        
        
        // button home
        pBtnHome = STButton::createWithSpriteFrameName(string("ui06_menu.png"), string("ui06_menu_h.png"),
                                                                this, callfuncN_selector(GameFinishLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnHome);
        pBtnHome->setTag(tag_btn_home);
        pBtnHome->setAnchorPoint(ccp(0, .5f));
        pBtnHome->setPosition(ccp2(250, 210));
        pBatchNode->addChild(pBtnHome, z_text);
        const int touchPriority = pBtnHome->getTouchPriority() - 1;
        pBtnHome->setTouchPriority(touchPriority);
        
        // button replay
        pBtnReplay = STButton::createWithSpriteFrameName(string("ui05_replay.png"), string("ui05_replay_h.png"),
                                                                this, callfuncN_selector(GameFinishLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnReplay);
        pBtnReplay->setTag(tag_btn_replay);
        pBtnReplay->setPosition(ccp_fixed_X(bg_center.x, 210));
        pBatchNode->addChild(pBtnReplay, z_text);
        pBtnReplay->setTouchPriority(touchPriority);
        
        // button next
        pBtnNext = STButton::createWithSpriteFrameName(string("ui05_next.png"), string("ui05_next_h.png"),
                                                                  this, callfuncN_selector(GameFinishLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnNext);
        pBtnNext->setTag(tag_btn_next);
        pBtnNext->setAnchorPoint(ccp(1, .5f));
        const float rightMargin = m_pBackground->getContentSize().width - get_offset(250);
        pBtnNext->setPosition(ccp_fixed_X(rightMargin, 210));
        pBatchNode->addChild(pBtnNext, z_text);
        pBtnNext->setTouchPriority(touchPriority);
        
        saveFinishMode();
        
        bRet = true;
    } while (0);
    
    return bRet;
}

bool GameFinishLayer::initGameOver()
{
    bool bRet = false;
    
    do
    {
        AudioHelper::getInstance()->playSound(sound_game_over);
        
        CCSpriteBatchNode* pBatchNode = CCSpriteBatchNode::create("in-game.pvr.ccz");
        CC_BREAK_IF(!pBatchNode);
        m_pBackground->addChild(pBatchNode);
        
        CCPoint bg_center = ccp_get_center(m_pBackground->getContentSize());
        
        // title
        CCSprite* title = CCSprite::createWithSpriteFrameName("ui05_game_over.png");
        CC_BREAK_IF(! title);
        title->setPosition(ccp_fixed_X((bg_center.x-get_offset(4)), 1100));
        pBatchNode->addChild(title);
        
        CCSprite* textBg = CCSprite::createWithSpriteFrameName("ui05_text_bg.png");
        CC_BREAK_IF(! textBg);
        textBg->setScaleY(.7f);
        textBg->setAnchorPoint(ccp(.5f, 0));
        textBg->setPosition(ccp_fixed_X(bg_center.x, 470));
        pBatchNode->addChild(textBg, z_text_bg);
        
        // text score
        CCSprite* textScore = CCSprite::createWithSpriteFrameName("ui05_score.png");
        CC_BREAK_IF(! textScore);
        textScore->setAnchorPoint(ccp(.5f, 0));
        textScore->setPosition(ccp_fixed_X(bg_center.x, 720));
        pBatchNode->addChild(textScore, z_text);
        
        // label score
        CCLabelTTF* labelScore = CCLabelTTF::create(m_pScoringSystem->getFinalScoreString().c_str(),
                                                    font_Helvetica_Neue, text_font_size1);
        CC_BREAK_IF(! labelScore);
        labelScore->setAnchorPoint(ccp(.5f, .5f));
        labelScore->setPosition(ccp_fixed_X(bg_center.x, 590));
        m_pBackground->addChild(labelScore, z_text);
        
        // update highest score
        EncryptDataHelper::updateHighScore(getHighScoreKey(), m_pScoringSystem->getFinalScore());
        
        
        // button home
        pBtnHome = STButton::createWithSpriteFrameName(string("ui06_menu.png"), string("ui06_menu_h.png"),
                                                                this, callfuncN_selector(GameFinishLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnHome);
        pBtnHome->setTag(tag_btn_home);
        pBtnHome->setAnchorPoint(ccp(0, .5f));
        pBtnHome->setPosition(ccp2(0, 270));
        const int touchPriority = pBtnHome->getTouchPriority() - 1;
        pBtnHome->setTouchPriority(touchPriority);
        m_pBackground->addChild(pBtnHome, z_text);
        
        
        // layout
        float bgWidth  = m_pBackground->getContentSize().width;
        float btnWidth = pBtnHome->getContentSize().width;
        float offset   = (bgWidth - btnWidth * 2) / 3;
        
        // adjust button position
        pBtnHome->setPositionX(offset);
        
        
        // button replay
        pBtnReplay = STButton::createWithSpriteFrameName(string("ui05_replay.png"), string("ui05_replay_h.png"),
                                                                  this, callfuncN_selector(GameFinishLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnReplay);
        pBtnReplay->setTag(tag_btn_replay);
        pBtnReplay->setAnchorPoint(ccp(0, .5f));
        pBtnReplay->setPosition(ccp2(715, 270));
        pBtnReplay->setPositionX(offset * 2 + btnWidth);
        m_pBackground->addChild(pBtnReplay, z_text);
        pBtnReplay->setTouchPriority(touchPriority);
    
        
        bRet = true;
    } while (0);
    
    return bRet;
}

bool GameFinishLayer::initAllLevelFinish()
{
    bool bRet = false;
    
    do
    {
        AudioHelper::getInstance()->playSound(sound_game_complete);
        
        CCSpriteBatchNode* pBatchNode = CCSpriteBatchNode::create("in-game.pvr.ccz");
        CC_BREAK_IF(!pBatchNode);
        m_pBackground->addChild(pBatchNode);
        
        CCPoint bg_center = ccp_get_center(m_pBackground->getContentSize());
        
        // title
        CCSprite* title = CCSprite::createWithSpriteFrameName("ui05_game_complete.png");
        CC_BREAK_IF(! title);
        title->setPosition(ccp_fixed_X(bg_center.x, 1100));
        pBatchNode->addChild(title);
        
        // text bg
        CCSprite* textBg = CCSprite::createWithSpriteFrameName("ui05_text_bg.png");
        CC_BREAK_IF(! textBg);
        textBg->setPosition(ccp_fixed_X(bg_center.x, 650));
        pBatchNode->addChild(textBg, z_text_bg);
        
        
        const float textOffset = get_offset(340);
        // text time
        CCSprite* textTime = CCSprite::createWithSpriteFrameName("ui05_time.png");
        CC_BREAK_IF(! textTime);
        textTime->setAnchorPoint(ccp(.5, 0));
        textTime->setPosition(ccp_fixed_X((bg_center.x-textOffset), 800));
        pBatchNode->addChild(textTime, z_text);
        
        // label time
        CCLabelTTF* labelTime = CCLabelTTF::create(m_pScoringSystem->getLifeTimeString().c_str(),
                                                   font_Helvetica_Neue, text_font_size1);
        CC_BREAK_IF(! labelTime);
        labelTime->setPosition(ccp_fixed_X(bg_center.x-textOffset, 685));
        m_pBackground->addChild(labelTime, z_text);
        
        
        
        
        // text score
        CCSprite* textScore = CCSprite::createWithSpriteFrameName("ui05_score.png");
        CC_BREAK_IF(! textScore);
        textScore->setAnchorPoint(ccp(.5, 0));
        textScore->setPosition(ccp_fixed_X((bg_center.x+textOffset), 800));
        pBatchNode->addChild(textScore, z_text);
        
        // label score
        CCLabelTTF* labelScore = CCLabelTTF::create(m_pScoringSystem->getFinalScoreString().c_str(),
                                                    font_Helvetica_Neue, text_font_size1);
        CC_BREAK_IF(! labelScore);
        labelScore->setPosition(ccp_fixed_X(bg_center.x+textOffset, 685));
        m_pBackground->addChild(labelScore, z_text);
        
        
        
        
        // text high score
        CCSprite* textHighScore = CCSprite::createWithSpriteFrameName("ui05_high_score.png");
        CC_BREAK_IF(! textHighScore);
        textHighScore->setPosition(ccp_fixed_X((textBg->getContentSize().width/2), 120));
        textBg->addChild(textHighScore, z_text);
        
        // lable high score
        int highestScore = EncryptDataHelper::updateHighScore(getHighScoreKey(), m_pScoringSystem->getFinalScore());
        CCString* highestScoreString = CCString::createWithFormat("%d", highestScore);
        
        CCLabelTTF* labelHighScore = CCLabelTTF::create(highestScoreString->getCString(), font_Helvetica_Neue, text_font_size2);
        CC_BREAK_IF(! labelHighScore);
        labelHighScore->setAnchorPoint(ccp(0, .5f));
        labelHighScore->setPosition(ccp2(960, 485));
        labelHighScore->setHorizontalAlignment(kCCTextAlignmentLeft);
        m_pBackground->addChild(labelHighScore, z_text);
        
        
        
        
        // button home
        pBtnHome = STButton::createWithSpriteFrameName(string("ui06_menu.png"), string("ui06_menu_h.png"),
                                                                this, callfuncN_selector(GameFinishLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnHome);
        pBtnHome->setTag(tag_btn_home);
        pBtnHome->setAnchorPoint(ccp(0, .5f));
        pBtnHome->setPosition(ccp2(0, 210));
        const int touchPriority = pBtnHome->getTouchPriority() - 1;
        pBtnHome->setTouchPriority(touchPriority);
        m_pBackground->addChild(pBtnHome, z_text);
        
        
        // layout
        float bgWidth  = m_pBackground->getContentSize().width;
        float btnWidth = pBtnHome->getContentSize().width;
        float offset   = (bgWidth - btnWidth * 2) / 3;
        
        // adjust button position
        pBtnHome->setPositionX(offset);
        
        
        // button replay
        pBtnReplay = STButton::createWithSpriteFrameName(string("ui05_replay.png"), string("ui05_replay_h.png"),
                                                                  this, callfuncN_selector(GameFinishLayer::onButtonClicked));
        CC_BREAK_IF(!pBtnReplay);
        pBtnReplay->setTag(tag_btn_replay);
        pBtnReplay->setAnchorPoint(ccp(0, .5f));
        pBtnReplay->setPosition(ccp2(715, 210));
        pBtnReplay->setPositionX(offset * 2 + btnWidth);
        m_pBackground->addChild(pBtnReplay, z_text);
        pBtnReplay->setTouchPriority(touchPriority);
        
        saveFinishMode();
        
        bRet = true;
    } while (0);
    
    return bRet;
}


void GameFinishLayer::onButtonClicked(CCNode* btn)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    CCNode* button = dynamic_cast<CCNode*>(btn);
    
    switch (button->getTag())
    {
        case tag_btn_home:
        {
            m_pGameScene->returnToHome();
        }
            break;
            
        case tag_btn_replay:
        {
            m_pGameScene->replayGame();
        }
            break;
            
        case tag_btn_next:
        {
            m_pGameScene->nextLevel();
        }
            break;
            
        default:
            break;
    }
    
}

string GameFinishLayer::getHighScoreKey()
{
    string key = key_high_score_normal_prefix;
    key.append(m_pGameScene->stLevelId);
    return key;
}

void GameFinishLayer::saveFinishMode()
{
    string keyFinish = "";
   
    switch (m_pGameScene->mSearchMode)
    {
        case Picture:
        {
            keyFinish.append(key_level_picture).append(m_pGameScene->stLevelId);
            STSaveBooleanToUD(keyFinish.c_str(), true);
            break;
        }
        case Silhouette:
        {
            keyFinish.append(key_level_silhouette).append(m_pGameScene->stLevelId);
            STSaveBooleanToUD(keyFinish.c_str(), true);
            break;
        }
        case Name:
        {
            keyFinish.append(key_level_word).append(m_pGameScene->stLevelId);
            STSaveBooleanToUD(keyFinish.c_str(), true);
            break;
        }
    }
    
}

void GameFinishLayer::setButtonsEnable(bool enable)
{
    pBtnHome->setEnabled(enable);
    pBtnReplay->setEnabled(enable);
    pBtnNext->setEnabled(enable);
}

void GameFinishLayer::onExit()
{
    CoverLayer::onExit();
}


GameFinishLayer::GameFinishLayer(ScoringSystem* score, GameScene* gameScene)
{
    m_pScoringSystem    = score;
    m_pGameScene        = gameScene;
}

GameFinishLayer::~GameFinishLayer()
{
}