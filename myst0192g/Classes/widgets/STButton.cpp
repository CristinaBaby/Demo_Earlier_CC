//
//  STButton.cpp
//  CHUPS003
//
//  Created by Steven.Xc.Tian on 13-10-31.
//
//

#include "STButton.h"

USING_NS_CC;
using std::string;


const float STButton::CLICK_INTERVAL = 0.17f;
bool STButton::s_b_DelayEnable = false;


STButton* STButton::createWithSpriteFrame(CCSpriteFrame *normalFrame, CCSpriteFrame *selectedFrame, CCSpriteFrame *disableFrame,
                                          CCObject *target /*= NULL*/, SEL_CallFuncN selector /*= NULL*/)
{
    STButton *pobSprite = new STButton(normalFrame, selectedFrame, disableFrame, target, selector);
    if (pobSprite && pobSprite->initWithSpriteFrame(normalFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STButton* STButton::createWithSpriteFrameName(std::string normalFileName, std::string selectedFileName, std::string disableFileName,
                                              cocos2d::CCObject *target /*= NULL*/, cocos2d::SEL_CallFuncN selector /*= NULL*/)
{
    STButton *pobSprite = new STButton(normalFileName, selectedFileName, disableFileName, target, selector);
    if (pobSprite && pobSprite->initWithSpriteFrameName(normalFileName.c_str()))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STButton* STButton::createWithSpriteFrame(cocos2d::CCSpriteFrame *normalFrame, cocos2d::CCSpriteFrame *selectedFrame,
                                          cocos2d::CCObject *target /*= NULL*/, cocos2d::SEL_CallFuncN selector/*= NULL*/)
{
    return createWithSpriteFrame(normalFrame, selectedFrame, NULL, target, selector);
}

STButton* STButton::createWithSpriteFrame(cocos2d::CCSpriteFrame *normalFrame, cocos2d::CCObject *target /*= NULL*/, cocos2d::SEL_CallFuncN selector/*= NULL*/)
{
    return createWithSpriteFrame(normalFrame, NULL, NULL, target, selector);
}

STButton* STButton::createWithSpriteFrameName(std::string normalFileName, std::string selectedFileName,
                                              CCObject *target /*= NULL*/, SEL_CallFuncN selector /*= NULL*/)
{
    return createWithSpriteFrameName(normalFileName, selectedFileName, "", target, selector);
}

STButton* STButton::createWithSpriteFrameName(std::string normalFileName, CCObject *target /*= NULL*/, SEL_CallFuncN selector /*= NULL*/)
{
    return createWithSpriteFrameName(normalFileName, "", "", target, selector);
}




STButton::STButton(CCSpriteFrame *normalFrame, CCSpriteFrame *selectedFrame,
                   CCSpriteFrame *disableFrame, CCObject *target, SEL_CallFuncN selector)
{
    mp_NormalFrame = normalFrame;
    mp_SelectedFrame = selectedFrame;
    mp_DisableFrame = disableFrame;
    
    CC_SAFE_RETAIN(mp_NormalFrame);
    CC_SAFE_RETAIN(mp_SelectedFrame);
    CC_SAFE_RETAIN(mp_DisableFrame);
    
    this->setOnClickedListener(target, selector);
    
    m_iTouchPriority = kCCMenuHandlerPriority;
}

STButton::STButton(string normalFileName, string selectedFileName, string disableFileName,
                   CCObject *target, SEL_CallFuncN selector)
{
    mp_NormalFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(normalFileName.c_str());
    
    selectedFileName.empty() ?
    mp_SelectedFrame = NULL :
    mp_SelectedFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(selectedFileName.c_str());
    
    disableFileName.empty() ?
    mp_DisableFrame = NULL :
    mp_DisableFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(disableFileName.c_str());
    
    CC_SAFE_RETAIN(mp_NormalFrame);
    CC_SAFE_RETAIN(mp_SelectedFrame);
    CC_SAFE_RETAIN(mp_DisableFrame);
    
    this->setOnClickedListener(target, selector);
    
    m_iTouchPriority = kCCMenuHandlerPriority;
}


STButton::~STButton()
{
    CC_SAFE_RELEASE_NULL(mp_NormalFrame);
    CC_SAFE_RELEASE_NULL(mp_SelectedFrame);
    CC_SAFE_RELEASE_NULL(mp_DisableFrame);
}

void STButton::setEnabled(bool value)
{
    if (m_bEnabled == value)
        return;
    
    m_bEnabled = value;
    
    if (mp_DisableFrame)
    {
        m_bEnabled ?
        this->setDisplayFrame(mp_NormalFrame) :
        this->setDisplayFrame(mp_DisableFrame);
    }
}

void STButton::resumeEnable(float)
{
    s_b_DelayEnable = false;
}

void STButton::setNormalSpriteFrame(CCSpriteFrame *normalFrame)
{
    CC_SAFE_RELEASE_NULL(mp_NormalFrame);
    mp_NormalFrame = normalFrame;
    CC_SAFE_RETAIN(mp_NormalFrame);
}

void STButton::setSelectedSpriteFrame(CCSpriteFrame *selectedFrame)
{
    CC_SAFE_RELEASE_NULL(mp_SelectedFrame);
    mp_SelectedFrame = selectedFrame;
    CC_SAFE_RETAIN(mp_SelectedFrame);
}

void STButton::setDisableSpriteFrame(CCSpriteFrame *disableFrame)
{
    CC_SAFE_RELEASE_NULL(mp_DisableFrame);
    mp_DisableFrame = disableFrame;
    CC_SAFE_RETAIN(mp_DisableFrame);
}

void STButton::onExit()
{
    STTouchableSprite::onExit();
    
    // when this node is exit, but it still block the touch, we need resume the status
    s_b_DelayEnable = false;
}

bool STButton::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!m_bEnabled || s_b_DelayEnable)
        return false;
    
    bool inSprite = isHitSprite(pTouch);
    
    if (inSprite)
    {
        s_b_DelayEnable = true;
        
        if (mp_SelectedFrame)
        {
            this->setDisplayFrame(mp_SelectedFrame);
        }
        
    }
    else
    {
        s_b_DelayEnable = false;
    }
    
    return inSprite;
}

void STButton::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (mp_SelectedFrame)
    {
        isHitSprite(pTouch) ?
        this->setDisplayFrame(mp_SelectedFrame):
        this->setDisplayFrame(mp_NormalFrame) ;
    }
}

void STButton::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (mp_SelectedFrame)
        this->setDisplayFrame(mp_NormalFrame);
    
    STTouchableSprite::ccTouchEnded(pTouch, pEvent);
    
    this->scheduleOnce(schedule_selector(STButton::resumeEnable), CLICK_INTERVAL);
}

void STButton::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (mp_SelectedFrame)
        this->setDisplayFrame(mp_NormalFrame);
    
    this->scheduleOnce(schedule_selector(STButton::resumeEnable), CLICK_INTERVAL);
}
