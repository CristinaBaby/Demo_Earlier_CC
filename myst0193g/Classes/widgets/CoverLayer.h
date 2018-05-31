#ifndef _COVERLAYER_H_
#define _COVERLAYER_H_

#include "cocos2d.h"

#define kCoverLayerContentTag				100000000
#define kCoverLayerTag						0xceaea999
#define kCoverLayerAnimationTime            0.4f

// class that implements a black colored layer that will cover the whole screen
// and eats all touches except within the dialog box child
class CoverLayer: public cocos2d::CCLayerColor
{
public:
	CoverLayer();
	virtual ~CoverLayer();

	virtual void onEnter();
	virtual void onExit();

	virtual bool init();
    virtual bool initWithColor(const cocos2d::ccColor4B& color);
	virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

	// android back key handler
	virtual void keyBackClicked();

	CREATE_FUNC(CoverLayer);

	static CoverLayer* createWithColor(cocos2d::ccColor4B color);
    
    /** set the target/selector of close button, when layer closed call this selector */
    void setCloseCallback(CCObject *target, cocos2d::SEL_CallFuncN selector);
    
protected:
    cocos2d::CCObject*           m_pListener;
    cocos2d::SEL_CallFuncN       m_pfnSelector;
};

#endif