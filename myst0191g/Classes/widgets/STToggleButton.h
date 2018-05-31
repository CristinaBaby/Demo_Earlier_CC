//
//  STToggleButton.h
//  CHUPS003
//
//  Created by Steven.Xc.Tian on 13-11-1.
//
//

#ifndef __CHUPS003__STToggleButton__
#define __CHUPS003__STToggleButton__

#include <cocos2d.h>
#include "STButton.h"

class STToggleButton : public STButton
{
protected:
    bool mb_Checked;
    
public:
    STToggleButton(cocos2d::CCSpriteFrame *onFrame, cocos2d::CCSpriteFrame *offFrame,
                   cocos2d::CCObject *target = NULL, cocos2d::SEL_CallFuncN selector = NULL);
    STToggleButton(std::string onFileName, std::string offFileName,
                   cocos2d::CCObject *target = NULL, cocos2d::SEL_CallFuncN selector = NULL);
    virtual ~STToggleButton();
    
    static STToggleButton* create(cocos2d::CCSpriteFrame *onFrame, cocos2d::CCSpriteFrame *offFrame,
                                  cocos2d::CCObject *target = NULL, cocos2d::SEL_CallFuncN selector = NULL);
    
    static STToggleButton* createWithSpriteFrameName(std::string onFileName, std::string offFileName,
                                               cocos2d::CCObject *target = NULL, cocos2d::SEL_CallFuncN selector = NULL);
    
    /**
     *  set this button is checked or not.
     */
    void setChecked(bool checked);
    bool isChecked(){ return mb_Checked; };
    
    virtual void setEnabled(bool value);
    
protected:
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
};


#endif /* defined(__CHUPS003__STToggleButton__) */
