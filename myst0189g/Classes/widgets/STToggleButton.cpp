//
//  STToggleButton.cpp
//  CHUPS003
//
//  Created by Steven.Xc.Tian on 13-11-1.
//
//

#include "STToggleButton.h"

USING_NS_CC;



STToggleButton* STToggleButton::create(cocos2d::CCSpriteFrame *onFrame, cocos2d::CCSpriteFrame *offFrame,
                                       cocos2d::CCObject *target /*= NULL*/, cocos2d::SEL_CallFuncN selector /*= NULL*/)
{
    STToggleButton *pobSprite = new STToggleButton(onFrame, offFrame, target, selector);
    if (pobSprite && pobSprite->initWithSpriteFrame(onFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STToggleButton* STToggleButton::createWithSpriteFrameName(std::string onFileName, std::string offFileName,
                                                          cocos2d::CCObject *target /*= NULL*/, cocos2d::SEL_CallFuncN selector /*= NULL*/)
{
    STToggleButton *pobSprite = new STToggleButton(onFileName, offFileName, target, selector);
    if (pobSprite && pobSprite->initWithSpriteFrameName(onFileName.c_str()))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}


STToggleButton::STToggleButton(cocos2d::CCSpriteFrame *onFrame, cocos2d::CCSpriteFrame *offFrame, CCObject *target, SEL_CallFuncN selector)
: STButton(onFrame, offFrame, NULL, target, selector), mb_Checked(false)
{
}

STToggleButton::STToggleButton(std::string onFileName, std::string offFileName, CCObject *target, SEL_CallFuncN selector)
: STButton(onFileName, offFileName, "", target, selector), mb_Checked(false)
{
}

STToggleButton::~STToggleButton()
{
}



void STToggleButton::setChecked(bool checked)
{
    if (checked == mb_Checked)
        return;
    
    mb_Checked = checked;
    
    mb_Checked ?
    this->setDisplayFrame(mp_SelectedFrame) :
    this->setDisplayFrame(mp_NormalFrame);
}

void STToggleButton::setEnabled(bool value)
{
    if (m_bEnabled == value)
        return;
    
    m_bEnabled = value;
    
    /*
     if (mp_DisableFrame)
     {
     m_bEnabled ?
     this->setDisplayFrame(mp_NormalFrame) :
     this->setDisplayFrame(mp_DisableFrame);
     }
     */
}

bool STToggleButton::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_bEnabled || s_b_DelayEnable)
        return false;
    
    s_b_DelayEnable = isHitSprite(pTouch);
    
    return s_b_DelayEnable;
}

void STToggleButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
}

void STToggleButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(isHitSprite(pTouch))
    {
        this->setChecked(!mb_Checked);
        
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
    }
    
    this->scheduleOnce(schedule_selector(STToggleButton::resumeEnable), CLICK_INTERVAL);
}

void STToggleButton::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    this->scheduleOnce(schedule_selector(STToggleButton::resumeEnable), CLICK_INTERVAL);
}


