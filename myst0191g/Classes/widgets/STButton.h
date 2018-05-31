//
//  STButton.h
//  CHUPS003
//
//  Created by Steven.Xc.Tian on 13-10-31.
//
//

#ifndef __CHUPS003__STButton__
#define __CHUPS003__STButton__

#include "STTouchableSprite.h"

class STButton : public STTouchableSprite
{
protected:
    cocos2d::CCSpriteFrame *mp_NormalFrame;
    cocos2d::CCSpriteFrame *mp_SelectedFrame;
    cocos2d::CCSpriteFrame *mp_DisableFrame;
    
    cocos2d::CCLabelTTF    *mp_Lable;
    
public:
    static STButton* createWithSpriteFrame(cocos2d::CCSpriteFrame *normalFrame, cocos2d::CCSpriteFrame *selectedFrame,
                                           cocos2d::CCSpriteFrame *disableFrame,
                                           cocos2d::CCObject *target = NULL, cocos2d::SEL_CallFuncN selector = NULL);
    
    static STButton* createWithSpriteFrame(cocos2d::CCSpriteFrame *normalFrame, cocos2d::CCSpriteFrame *selectedFrame,
                                           cocos2d::CCObject *target = NULL, cocos2d::SEL_CallFuncN selector = NULL);
    
    static STButton* createWithSpriteFrame(cocos2d::CCSpriteFrame *normalFrame, cocos2d::CCObject *target = NULL, cocos2d::SEL_CallFuncN selector = NULL);
    
    static STButton* createWithSpriteFrameName(std::string normalFileName, std::string selectedFileName, std::string disableFileName,
                                               cocos2d::CCObject *target = NULL, cocos2d::SEL_CallFuncN selector = NULL);
    
    static STButton* createWithSpriteFrameName(std::string normalFileName, std::string selectedFileName,
                                               cocos2d::CCObject *target = NULL, cocos2d::SEL_CallFuncN selector = NULL);
    
    static STButton* createWithSpriteFrameName(std::string normalFileName, cocos2d::CCObject *target = NULL, cocos2d::SEL_CallFuncN selector = NULL);
    
public:
    STButton(cocos2d::CCSpriteFrame *normalFrame, cocos2d::CCSpriteFrame *selectedFrame,
             cocos2d::CCSpriteFrame *disableFrame,
             cocos2d::CCObject *target, cocos2d::SEL_CallFuncN selector);
    
    STButton(std::string normalFileName, std::string selectedFileName, std::string disableFileName,
             cocos2d::CCObject *target, cocos2d::SEL_CallFuncN selector);
    
    virtual ~STButton();
    
    virtual void onExit();
    
    virtual void setEnabled(bool value);
    
    void setNormalSpriteFrame(cocos2d::CCSpriteFrame *normalFrame);
    void setSelectedSpriteFrame(cocos2d::CCSpriteFrame *selectedFrame);
    void setDisableSpriteFrame(cocos2d::CCSpriteFrame *disableFrame);
    
protected:
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
protected:
    static const float CLICK_INTERVAL;
    
    static bool s_b_DelayEnable;
    /**
     * this is used to avoid tap button frequently during a short time.
     * once you tap a button include this menu, it will be set disable, until touch ended or cancelled.
     */
    void resumeEnable(float);
};

#endif /* defined(__CHUPS003__STButton__) */
