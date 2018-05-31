/*
 * ModalAlert - Customizable popup dialogs/alerts for Cocos2D-X with a dynamic size
 * 
 *
 * Customization of ModalAlert by Rombos && ParallaxShift by
 *
 * Jose Antonio Andujar Clavell, JanduSoft
 * http://www.jandusoft.com 
 * http://www.jandujar.com
 *
 * For details, visit the Rombos blog:
 * http://rombosblog.wordpress.com/2012/02/28/modal-alerts-for-cocos2d/
 *
 * Copyright (c) 2012 Hans-Juergen Richstein, Rombos
 * http://www.rombos.de
 *
 * C++ version (c) 2012 Philippe Chaintreuil, ParallaxShift
 * http://www.parallaxshift.com/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "cocos2d.h"
#include "ModalAlert.h"
#include "CoverLayer.h"
#include "BackStack.h"

USING_NS_CC;

#define kCoverLayerTag						0xceaea999
#define kAnimationTime						0.4f
#define kPopuImg							"ui/ui_modal_bg.png"
#define kPopupButtonImg						"ui/ui_modal_button.png"
#define kPopupButtonPressImg				"ui/ui_modal_button_p.png"
#define kFontStyle							"fonts/VERDANA.ttf"
#define KFontSizeText						32
#define KFontSizeButton						32
#define kMarginLeft							40
#define kMarginBottom						20
#define kSeparation							40

// Local function declarations
static void ModalAlertCloseAlert(
	CCNode *alertDialog,
	CCLayer *coverLayer,
	CCObject *doneSelectorTarget,
	SEL_CallFunc doneSelector);

// Replaces the use of blocks, since C++ doesn't have those.
//
// These would be the subclass of CCObject, but we don't get
// the handy auto-reference counting that blocks get.  So my
// hack is to add them as invisible items on the cover layer.
//
// They can't have circular references back to the layer to run
// the code that was in the blocks, so layers and dialogs are
// found and verified by tags (defines near top).
//
// Like CCMenuItem's, they don't retain the selectors they're
// going to call.
class ModalCloseAndCallBlock: public CCNode
{
public:
	ModalCloseAndCallBlock():
		selectorTarget(NULL),
		selector(NULL)
	{ }

	virtual ~ModalCloseAndCallBlock()
	{
		this->selectorTarget = NULL;
		this->selector = NULL;
	}

	virtual bool initWithOptions(
		CCLayer *coverLayer,
		CCObject *selectorTarget,
		SEL_CallFunc selector)
	{
		CC_ASSERT(coverLayer);
		CC_ASSERT( (selectorTarget && selector) || (!selectorTarget && !selector) );

		// Note that we automatically glom onto the coverlayer,
		// this is likely all that keeps us around.  We'll
		// go away when it goes away.
		CC_ASSERT(coverLayer->getTag() == kCoverLayerTag);
		coverLayer->addChild(this);

		this->setSelectorTarget(selectorTarget);
		this->setSelector(selector);

		return true;
	}

	static ModalCloseAndCallBlock* closeAndCallBlockWithOptions(
		CCLayer *coverLayer,
		CCObject *selectorTarget,
		SEL_CallFunc selector)
	{
		ModalCloseAndCallBlock *cncb = new ModalCloseAndCallBlock();
		if (!cncb)
			return NULL;
		bool success = cncb->initWithOptions(
			coverLayer,
			selectorTarget,
			selector);
		if (success)
		{
			cncb->autorelease();
		}
		else
		{
			delete cncb;
			cncb = NULL;
		}
		return cncb;
	}

	void Execute(CCNode* menu_item)
	{
		CC_UNUSED_PARAM(menu_item);

		// Parent == CoverLayer & find sibling dialog by tag.
		CCNode *parent = this->getParent();
		CC_ASSERT(parent);
		CC_ASSERT(dynamic_cast<CCLayer*>(parent) != NULL);
		CC_ASSERT(parent->getTag() == kCoverLayerTag);

		CCLayer *coverLayer = static_cast<CCLayer*>(parent);
		CC_ASSERT(coverLayer);
		CCNode *dialog = coverLayer->getChildByTag(kCoverLayerContentTag);
		CC_ASSERT(dialog);

		ModalAlertCloseAlert(
			dialog,
			coverLayer,
			this->getSelectorTarget(),
			this->getSelector() );
	}

	CC_SYNTHESIZE(CCObject*, selectorTarget, SelectorTarget);
	CC_SYNTHESIZE(SEL_CallFunc, selector, Selector);
};

// Make sure you read the comment above ModalCloseAndCallBlock
class ModalWhenDoneBlock: public CCNode
{
public:
	ModalWhenDoneBlock():
		selectorTarget(NULL),
		selector(NULL)
	{ }

	virtual ~ModalWhenDoneBlock()
	{
		this->selectorTarget = NULL;
		this->selector = NULL;
	}

	virtual bool initWithOptions(
		CCLayer *coverLayer,
		CCObject *selectorTarget,
		SEL_CallFunc selector)
	{
		CC_ASSERT(coverLayer);
		CC_ASSERT( (selectorTarget && selector) || (!selectorTarget && !selector) );

		// Note that we automatically glom onto the coverlayer,
		// this is likely all that keeps us around.  We'll
		// go away when it goes away.
		CC_ASSERT(coverLayer->getTag() == kCoverLayerTag);
		coverLayer->addChild(this);

		this->setSelectorTarget(selectorTarget);
		this->setSelector(selector);

		return true;
	}

	static ModalWhenDoneBlock* whenDone(
		CCLayer *coverLayer,
		CCObject *selectorTarget,
		SEL_CallFunc selector)
	{
		ModalWhenDoneBlock *wdb = new ModalWhenDoneBlock();
		if (!wdb)
			return NULL;
		bool success = wdb->initWithOptions(
			coverLayer,
			selectorTarget,
			selector);
		if (success)
			wdb->autorelease();
		else
		{
			delete wdb;
			wdb = NULL;
		}
		return wdb;
	}

	void Execute()
	{
		// Code we're replacing:
		//[CCCallBlock actionWithBlock:^{
		//     [coverLayer removeFromParentAndCleanup:YES];
		//     if (block) block();

		// More work than above, coverlayer should be our parent.
		CCNode *parent = this->getParent();
		CC_ASSERT(parent);
		CC_ASSERT(parent->getTag() == kCoverLayerTag);
		CC_ASSERT(dynamic_cast<CCLayer*>(parent) != NULL);
		CCLayer *coverLayer = static_cast<CCLayer*>(parent);
		CC_ASSERT(coverLayer);

		// Retain coverlayer so it doesn't go away on us.
		coverLayer->retain();

		coverLayer->removeFromParentAndCleanup(true);

		SEL_CallFunc sel = this->getSelector();
		CCObject *target = this->getSelectorTarget();
		if (sel && target)
		{
			(target->*selector)();
		}

		// Release -- there's a good chance we might be
		// deleted by this next line since being a child
		// of coverLayer is all that keeps us around,
		// so make sure "this" isn't used after it.
		coverLayer->release();
	}

	CC_SYNTHESIZE(CCObject*, selectorTarget, SelectorTarget);
	CC_SYNTHESIZE(SEL_CallFunc, selector, Selector);
};

void ModalAlertCloseAlert(
	CCNode *alertDialog,
	CCLayer *coverLayer,
	CCObject *doneSelectorTarget,
	SEL_CallFunc doneSelector)
{
	CC_ASSERT(alertDialog);
	CC_ASSERT(coverLayer);

	// Shrink dialog box
	alertDialog->runAction(CCScaleTo::create(kAnimationTime, 0.0f));

	// in parallel, fadeout and remove cover layer and execute
	// block (note: you can't use CCFadeOut since we don't start at
	// opacity 1!)
	ModalWhenDoneBlock* wdb = ModalWhenDoneBlock::whenDone(
		coverLayer,
		doneSelectorTarget,
		doneSelector);
	CC_ASSERT(wdb);

	coverLayer->runAction(
		CCSequence::create(CCFadeTo::create(kAnimationTime, 0.0f),
			CCCallFunc::create(wdb, SEL_CallFunc(&ModalWhenDoneBlock::Execute)), NULL));
}

// Text + 2 buttons + customizable containers
void ModalAlert::PopupOnLayer(
                         cocos2d::CCNode *layer,
                         char const * text,
                         char const * buttonRightText,
                         char const * buttonLeftText,
                         CCScale9Sprite* popup,
                         CCScale9Sprite* textContainer,
                         CCScale9Sprite* buttonRight,
                         CCScale9Sprite* buttonRightSelected,
                         CCScale9Sprite* buttonLeft,
                         CCScale9Sprite* buttonLeftSelected,
                         cocos2d::CCObject *buttonRigthSelectorTarget,
                         cocos2d::SEL_CallFunc buttonRigthSelector,
                         cocos2d::CCObject *buttonLeftSelectorTarget,
                         cocos2d::SEL_CallFunc buttonLeftSelector){

    // Only strict variables
    CC_ASSERT(layer);
    CC_ASSERT(text);
    CC_ASSERT(buttonRightText);
    CC_ASSERT(popup);
    CC_ASSERT(buttonRight);
    CC_ASSERT(buttonRigthSelectorTarget);
    CC_ASSERT(buttonRigthSelector);
    
    float maxWidth = 0;
    float maxHeight = 0;
    
    // Create the cover layer that "hides" the current application.
	CCLayerColor *coverLayer = CoverLayer::create();
	CC_ASSERT(coverLayer);

	// enable back key
	coverLayer->setKeypadEnabled(true);
	BackStack::sharedBackStack()->pushBackLayer(coverLayer);
    
	// Tag for later validation.
	coverLayer->setTag(kCoverLayerTag);
    
	// put to the very top to block application touches.
	layer->addChild(coverLayer, INT_MAX);
    
	// Smooth fade-in to dim with semi-transparency.
	coverLayer->runAction(CCFadeTo::create(kAnimationTime, 128));
    
	// Create the inner objects
	
    // Create the text with font
    CCLabelTTF* labelText = CCLabelTTF::create(text, kFontStyle, KFontSizeText);
    maxWidth = labelText->getContentSize().width;
    maxHeight = labelText->getContentSize().height;
    
	// Resize the container for the text
	if(textContainer != NULL){
		maxWidth += 2 * kMarginLeft;
		maxHeight += 2 * kMarginBottom;
		textContainer->setContentSize(CCSizeMake(maxWidth, maxHeight));
		textContainer->addChild(labelText);

		//Add the label to the container
		CCSize const & tsz = textContainer->getContentSize();
		CCPoint post(tsz.width, tsz.height);
		labelText->setPosition(ccpMult(post, 0.5f));
    }

	// Create the first button
	CCLabelTTF* button1Label = CCLabelTTF::create(buttonRightText ,kFontStyle, KFontSizeButton);
	CCControlButton* button1 = CCControlButton::create(button1Label, buttonRight);
    CC_ASSERT(button1);
	button1->setMargins(30, 12);
	button1->setTouchPriority(kCCMenuHandlerPriority);

    // Set the action for the first button
    // The following is to replace the Objective-C block
	// in the original.
	ModalCloseAndCallBlock *cncb = ModalCloseAndCallBlock::closeAndCallBlockWithOptions(coverLayer,
                                                buttonRigthSelectorTarget,
                                                buttonRigthSelector);
	CC_ASSERT(cncb);
    button1->addTargetWithActionForControlEvents(cncb, cocos2d::extension::SEL_CCControlHandler(&ModalCloseAndCallBlock::Execute), CCControlEventTouchDown);
    
	if(buttonRightSelected != NULL) {
		button1->setBackgroundSpriteForState(buttonRightSelected, CCControlStateHighlighted);
	}

	if( ( button1->getContentSize().width) > maxWidth) {
		maxWidth = button1->getContentSize().width;
	}

	maxHeight += kSeparation + button1->getContentSize().height;
	float buttonHeight = button1->getContentSize().height;

	CCControlButton* button2 = NULL;
	// Create the second button (optional)
	if(buttonLeft != NULL){
		CCLabelTTF *button2Label = CCLabelTTF::create(buttonLeftText ,kFontStyle, KFontSizeButton);
		button2 = CCControlButton::create(button2Label, buttonLeft);
        CC_ASSERT(button2);
		button2->setMargins(30, 12);
		button2->setTouchPriority(kCCMenuHandlerPriority);

        // Replaces Objective-C block in original code.
		cncb = ModalCloseAndCallBlock::closeAndCallBlockWithOptions(coverLayer,
                                                                  buttonLeftSelectorTarget,
                                                                  buttonLeftSelector);
		CC_ASSERT(cncb);
        button2->addTargetWithActionForControlEvents(cncb, cocos2d::extension::SEL_CCControlHandler(&ModalCloseAndCallBlock::Execute), CCControlEventTouchDown);

		if(buttonLeftSelected != NULL){
			button2->setBackgroundSpriteForState(buttonLeftSelected, CCControlStateHighlighted);
		}

		if( ( button1->getContentSize().width + button2->getContentSize().width + kSeparation ) > maxWidth){
			maxWidth = button1->getContentSize().width + button2->getContentSize().width + kSeparation;
		}

		// Height of button1 must be equal to button2 or we must change the maxHeight
		if( button2->getContentSize().height > button1->getContentSize().height){
			maxHeight -= button1->getContentSize().height;
			maxHeight += button2->getContentSize().height;
			buttonHeight = button2->getContentSize().height;
		}
	}

	maxWidth += 2 * kMarginLeft;
	maxHeight += 2 * kMarginBottom;

	//Create the popup dialog
	popup->setContentSize(CCSizeMake(maxWidth, maxHeight));

	//add components to popup
    CCSize const & sz = coverLayer->getContentSize();
	CCPoint pos(sz.width, sz.height);

	//add the text
	if(textContainer != NULL){
		popup->addChild(textContainer);
        textContainer->setPosition(ccp(maxWidth*0.5f, maxHeight - kMarginBottom - (textContainer->getContentSize().height/2)));
	}else{
		popup->addChild(labelText);
        labelText->setPosition(ccp(maxWidth*0.5f, maxHeight - kMarginBottom - (labelText->getContentSize().height/2)));
	}

	//add buttons
	if(button2!=NULL){
		//Two buttons
		popup->addChild(button1);
		popup->addChild(button2);
        button1->setPosition(ccp((button1->getContentSize().width/2) + kMarginLeft, kMarginBottom + (button1->getContentSize().height/2)));
        button2->setPosition(ccp(maxWidth -(button2->getContentSize().width/2) - kMarginLeft, kMarginBottom + (button2->getContentSize().height/2)));
	}else{
		//One Button
		popup->addChild(button1);
        button1->setPosition(ccp(maxWidth * 0.5f, kMarginBottom + (button1->getContentSize().height/2)));
	}

	//Add the popup to the cover layer
	popup->setTag(kCoverLayerContentTag);
    popup->setPosition( ccpMult(pos, 0.5f) );
    //TODO: setOpacity doesn't work on children
//	popup->setOpacity(220); // Make it a bit transparent for a cooler look.

	coverLayer->addChild(popup);

	// Open the dialog with a nice popup-effect
	popup->setScale(0.0f);
	popup->runAction(CCEaseBackOut::create(CCScaleTo::create(kAnimationTime, 1.0f)));
}

// Text + 1 button + customizable colors
void ModalAlert::PopupOnLayer(
                         cocos2d::CCNode *layer,
                         char const * text,
                         char const * buttonText,
                         CCScale9Sprite* popup,
                         CCScale9Sprite* textContainer,
                         CCScale9Sprite* button,
                         CCScale9Sprite* buttonSelected,
                         cocos2d::CCObject *buttonSelectorTarget,
                         cocos2d::SEL_CallFunc buttonSelector){
    CC_ASSERT(layer);
    CC_ASSERT(text);
    CC_ASSERT(buttonText);
    CC_ASSERT(popup);
    CC_ASSERT(button);
    CC_ASSERT(buttonSelectorTarget);
    CC_ASSERT(buttonSelector);
    
    PopupOnLayer(layer, text, buttonText, NULL, popup, textContainer, button, NULL, NULL, NULL, buttonSelectorTarget, buttonSelector, NULL, NULL);
}

// Text + 2 buttons (default colors)
void ModalAlert::PopupOnLayer(
                         cocos2d::CCNode *layer,
                         char const * text,
                         char const * buttonRightText,
                         char const * buttonLeftText,
                         cocos2d::CCObject *buttonRigthSelectorTarget,
                         cocos2d::SEL_CallFunc buttonRigthSelector,
                         cocos2d::CCObject *buttonLeftSelectorTarget,
                         cocos2d::SEL_CallFunc buttonLeftSelector){
    CC_ASSERT(layer);
    CC_ASSERT(text);
    CC_ASSERT(buttonRightText);
    CC_ASSERT(buttonLeftText);
    CC_ASSERT(buttonRigthSelectorTarget);
    CC_ASSERT(buttonRigthSelector);
    CC_ASSERT(buttonLeftSelectorTarget);
    CC_ASSERT(buttonLeftSelector);
    
    CCScale9Sprite *popup = CCScale9Sprite::create(kPopuImg);
    CCScale9Sprite *textContainer = NULL;
    CCScale9Sprite *button = CCScale9Sprite::create(kPopupButtonImg, CCRectMake(0, 0, 180, 64), CCRectMake(10, 8, 160, 48));
	CCScale9Sprite *buttonP = CCScale9Sprite::create(kPopupButtonPressImg, CCRectMake(0, 0, 180, 64), CCRectMake(10, 8, 160, 48));
	CCScale9Sprite *button2 = CCScale9Sprite::create(kPopupButtonImg, CCRectMake(0, 0, 180, 64), CCRectMake(10, 8, 160, 48));
    CCScale9Sprite *button2P = CCScale9Sprite::create(kPopupButtonPressImg, CCRectMake(0, 0, 180, 64), CCRectMake(10, 8, 160, 48));
    
    PopupOnLayer(layer, text, buttonRightText, buttonLeftText, popup, textContainer, button, buttonP, button2, button2P,
		buttonRigthSelectorTarget, buttonRigthSelector, buttonLeftSelectorTarget, buttonLeftSelector);
}

// Text + 1 button (default colors)
void ModalAlert::PopupOnLayer(
                         cocos2d::CCNode *layer,
                         char const * text,
                         char const * buttonText,
                         cocos2d::CCObject *buttonSelectorTarget,
                         cocos2d::SEL_CallFunc buttonSelector){
    CC_ASSERT(layer);
    CC_ASSERT(text);
    CC_ASSERT(buttonText);
    CC_ASSERT(buttonSelectorTarget);
    CC_ASSERT(buttonSelector);
    
	CCScale9Sprite* popup = CCScale9Sprite::create(kPopuImg, CCRectMake(0,0,50,50), CCRectMake(4,4,42,42));
    CCScale9Sprite* textContainer = NULL;
    CCScale9Sprite* button = CCScale9Sprite::create(kPopupButtonImg, CCRectMake(0, 0, 180, 64), CCRectMake(10, 8, 160, 48));
	CCScale9Sprite* buttonP = CCScale9Sprite::create(kPopupButtonPressImg, CCRectMake(0, 0, 180, 64), CCRectMake(10, 8, 160, 48));
    
    PopupOnLayer(layer, text, buttonText, NULL, popup, textContainer, button, buttonP, NULL, NULL, buttonSelectorTarget, buttonSelector, NULL, NULL);
}