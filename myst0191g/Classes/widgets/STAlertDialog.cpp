//
//  STAlertDialog.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-22.
//
//

#include "STAlertDialog.h"
#include "STUILayer.h"
#include "STButtonMenu.h"
#include "../AppGlobal.h"

USING_NS_CC;
using std::string;

enum { tag_button_left = 20, tag_button_right,};

//#define DEFAULT_TEXTURE_SHEET       "load-home.plist"
#define DEFAULT_BG_NAME             "ui10_popup.png"
#define DEFAULT_BUTTON_NAME1        "ui10_popup_button.png"
#define DEFAULT_BUTTON_NAME2        "ui10_popup_button_h.png"

#define DEFAULT_TEXT_COLOR          ccWHITE
#define DEFAULT_MESSAGE_COLOR       ccWHITE
#define TEXT_POSITION_Y             330
#define BUTTON_POSITION_Y           65
#define TEXT_ZONE_SIZE              CCSizeMake2(1170, 225)
#define MESSAGE_FONT_SIZE           fontsize(70)
#define BUTTON_FONT_SIZE            fontsize(70)
#define BUTTON_SHADOW_SIZE          ccp2(2, 2)


STAlertDialog::STAlertDialog()
: m_pLeftListener(NULL), m_pLeftSelector(NULL), m_pRightListener(NULL),m_pRightSelector(NULL),
//m_pBatchNode(NULL),
m_pBackgroundFrame(NULL), m_pButtonFrame(NULL), m_pButtonSelectedFrame(NULL),
m_pMessage(NULL), m_pLeftBtnText(NULL), m_pRightBtnText(NULL)
{
    // load sprite sheet
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(DEFAULT_TEXTURE_SHEET);
    
    m_pBackgroundFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(DEFAULT_BG_NAME);
    CC_SAFE_RETAIN(m_pBackgroundFrame);
    m_pButtonFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(DEFAULT_BUTTON_NAME1);
    CC_SAFE_RETAIN(m_pButtonFrame);
    m_pButtonSelectedFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(DEFAULT_BUTTON_NAME2);
    CC_SAFE_RETAIN(m_pButtonSelectedFrame);
}

STAlertDialog::~STAlertDialog()
{
    CC_SAFE_RELEASE(m_pBackgroundFrame);
    CC_SAFE_RELEASE(m_pButtonFrame);
    CC_SAFE_RELEASE(m_pButtonSelectedFrame);
}

STAlertDialog* STAlertDialog::show(CCNode* container, CCNode* text,
                                   CCNode* leftBtn, CCNode* rightBtn,
                                   CCObject* leftTarget, SEL_CallFuncN leftSelector,
                                   CCObject* rightTarget, SEL_CallFuncN rightSelector)
{
    return NULL;
}

STAlertDialog* STAlertDialog::show(CCNode* container, CCNode* text, CCNode* button,
                                   CCObject* target, SEL_CallFuncN selector)
{
    return NULL;
}

STAlertDialog* STAlertDialog::show(CCSpriteFrame* container, CCSpriteFrame* button,
                                   std::string text, std::string leftBtnText, std::string rightBtnText,
                                   CCObject* leftTarget, SEL_CallFuncN leftSelector,
                                   CCObject* rightTarget, SEL_CallFuncN rightSelector)
{
    return NULL;
}

STAlertDialog* STAlertDialog::show(CCSpriteFrame* container, CCSpriteFrame* button,
                                   std::string text, std::string btnText,
                                   CCObject* leftTarget, SEL_CallFuncN leftSelector)
{
    return NULL;
}

STAlertDialog* STAlertDialog::show(CCSpriteFrame* container,
                                   CCSpriteFrame* leftButton, CCSpriteFrame* rightButton,
                                   std::string text, std::string leftBtnText, std::string rightBtnText,
                                   CCObject* leftTarget, SEL_CallFuncN leftSelector,
                                   CCObject* rightTarget, SEL_CallFuncN rightSelector)
{
    return NULL;
}

STAlertDialog* STAlertDialog::show(CCNode* parent, std::string text, std::string btnText)
{
    STAlertDialog *pDialog = new STAlertDialog();
    if (pDialog && pDialog->initWithTexts(text, btnText))
    {
        pDialog->autorelease();
        
        parent->addChild(pDialog, INT_MAX);
        
        return pDialog;
    }
    
    CC_SAFE_DELETE(pDialog);
    
    return NULL;
}

STAlertDialog* STAlertDialog::show(CCNode* parent, std::string text, std::string leftBtnText, std::string rightBtnText)
{
    STAlertDialog *pDialog = new STAlertDialog();
    if (pDialog && pDialog->initWithTexts(text, leftBtnText, rightBtnText))
    {
        pDialog->autorelease();
        
        parent->addChild(pDialog, INT_MAX);
        
        return pDialog;
    }
    
    CC_SAFE_DELETE(pDialog);
    
    return NULL;
}


void STAlertDialog::setLeftButtonListener (CCObject* target, SEL_CallFuncN selector)
{
    m_pLeftListener = target;
    m_pLeftSelector = selector;
}

void STAlertDialog::setRightButtonListener(CCObject* target, SEL_CallFuncN selector)
{
    m_pRightListener = target;
    m_pRightSelector = selector;
}

void STAlertDialog::setMessageTextColor(const ccColor4B& color)
{
}

void STAlertDialog::setButtonTextColor(const ccColor4B& color)
{
    
}

bool STAlertDialog::initWithColor(const ccColor4B& color)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! CoverLayer::initWithColor(color));
        
        
        
        
        
        bRet = true;
    } while (0);
    
    return bRet;
}

/*
bool STAlertDialog::initWithTexts1(string text, string btnText)
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(! CoverLayer::init());
        
        STUILayer* pUILayer = STUILayer::create();
        CC_BREAK_IF (!pUILayer);
        this->addChild(pUILayer);
        
        CC_BREAK_IF(!m_pBackgroundFrame);
        CCSprite* bg = CCSprite::createWithSpriteFrame(m_pBackgroundFrame);
        CC_BREAK_IF(!bg);
        bg->setPosition(ccp_center_screen);
        pUILayer->addChild(bg);
        
        CCPoint centerBg = ccp_get_center(bg->getContentSize());
        
        // dialog content
        m_pMessage = CCLabelTTF::create(text.c_str(), font_Helvetica_Neue, MESSAGE_FONT_SIZE,
                                        TEXT_ZONE_SIZE, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        CC_BREAK_IF(!m_pMessage);
        m_pMessage->setPosition(ccp_fixed_X(centerBg.x, 360));
        m_pMessage->setColor(DEFAULT_TEXT_COLOR);
        bg->addChild(m_pMessage);
        
        // only button
        CC_BREAK_IF(!m_pButtonFrame);
        CC_BREAK_IF(!m_pButtonSelectedFrame);
        
        CCMenuItemSprite* btnLeft = CCMenuItemSprite::create(CCSprite::createWithSpriteFrame(m_pButtonFrame),
                                                             CCSprite::createWithSpriteFrame(m_pButtonSelectedFrame),
                                                             this, menu_selector(STAlertDialog::onButtonClicked));
        CC_BREAK_IF(!btnLeft);
        btnLeft->setAnchorPoint(ccp(.5f, 0));
        btnLeft->setTag(tag_button_left);
        
        
        btnLeft->setPosition(ccp_fixed_X(centerBg.x, BUTTON_POSITION_Y));
        
        // button text
        m_pLeftBtnText = CCLabelTTF::create(btnText.c_str(), font_Helvetica_Neue, BUTTON_FONT_SIZE,
                                            btnLeft->getContentSize(), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        CC_BREAK_IF(!m_pLeftBtnText);
        
        // because button image have shadow, we need adjust text position.
        CCPoint pos = ccp_get_center(m_pLeftBtnText->getContentSize()) + BUTTON_SHADOW_SIZE;
        
        m_pLeftBtnText->setPosition(pos);
        m_pLeftBtnText->setColor(DEFAULT_TEXT_COLOR);
        btnLeft->addChild(m_pLeftBtnText);
        
        
        //
        STButtonMenu* pMenu = STButtonMenu::create(btnLeft, NULL);
        CC_BREAK_IF(!pMenu);
        bg->addChild(pMenu);
        //
        //  this settings layer have the same priority as menu, so we use "kCCMenuHandlerPriority - 1",
        //  ensure this menu can receieve the touch event before other menus and this layer itself.
        //
        pMenu->setTouchPriority(kCCMenuHandlerPriority - 1);
        
        bRet = true;
    } while (0);
    
    return bRet;
}
*/

bool STAlertDialog::initWithTexts(string text, string leftText, string rightText/* = ""*/)
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(! CoverLayer::init());
        
        STUILayer* pUILayer = STUILayer::create();
        CC_BREAK_IF (!pUILayer);
        this->addChild(pUILayer);
        
        CC_BREAK_IF(!m_pBackgroundFrame);
        CCSprite* bg = CCSprite::createWithSpriteFrame(m_pBackgroundFrame);
        CC_BREAK_IF(!bg);
        bg->setPosition(ccp_center_screen);
        pUILayer->addChild(bg);
        
        CCPoint centerBg = ccp_get_center(bg->getContentSize());
        
        // dialog content
        m_pMessage = CCLabelTTF::create(text.c_str(), font_Helvetica_Neue, MESSAGE_FONT_SIZE,
                                        TEXT_ZONE_SIZE, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        CC_BREAK_IF(!m_pMessage);
        m_pMessage->setPosition(ccp_fixed_X(centerBg.x, TEXT_POSITION_Y));
        m_pMessage->setColor(DEFAULT_MESSAGE_COLOR);
        bg->addChild(m_pMessage);
        
        
        /* ------- buttons --------*/
        
        CC_BREAK_IF(!m_pButtonFrame);
        CC_BREAK_IF(!m_pButtonSelectedFrame);
        
        CCArray* buttons = CCArray::create();
        
        if (rightText.empty())
        {
            // there is only one button
            CCMenuItemSprite* btnLeft = CCMenuItemSprite::create(CCSprite::createWithSpriteFrame(m_pButtonFrame),
                                                                 CCSprite::createWithSpriteFrame(m_pButtonSelectedFrame),
                                                                 this, menu_selector(STAlertDialog::onButtonClicked));
            CC_BREAK_IF(!btnLeft);
            btnLeft->setAnchorPoint(ccp(.5f, 0));
            btnLeft->setTag(tag_button_left);
            
            
            btnLeft->setPosition(ccp_fixed_X(centerBg.x, BUTTON_POSITION_Y));
            
            // button text
            m_pLeftBtnText = CCLabelTTF::create(leftText.c_str(), font_Helvetica_Neue, BUTTON_FONT_SIZE,
                                                btnLeft->getContentSize(), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            CC_BREAK_IF(!m_pLeftBtnText);
            
            // because button image have shadow, we need adjust text position.
            CCPoint pos = ccp_get_center(m_pLeftBtnText->getContentSize()) + BUTTON_SHADOW_SIZE;
            
            m_pLeftBtnText->setPosition(pos);
            m_pLeftBtnText->setColor(DEFAULT_TEXT_COLOR);
            btnLeft->addChild(m_pLeftBtnText);
            
            buttons->addObject(btnLeft);
        }
        else
        {
            CCMenuItemSprite* btnLeft = CCMenuItemSprite::create(CCSprite::createWithSpriteFrame(m_pButtonFrame),
                                                                 CCSprite::createWithSpriteFrame(m_pButtonSelectedFrame),
                                                                 this, menu_selector(STAlertDialog::onButtonClicked));
            CC_BREAK_IF(!btnLeft);
            btnLeft->setAnchorPoint(CCPointZero);
            btnLeft->setTag(tag_button_left);
            
            buttons->addObject(btnLeft);
            
            CCMenuItemSprite* btnRight = CCMenuItemSprite::create(CCSprite::createWithSpriteFrame(m_pButtonFrame),
                                                                  CCSprite::createWithSpriteFrame(m_pButtonSelectedFrame),
                                                                  this, menu_selector(STAlertDialog::onButtonClicked));
            CC_BREAK_IF(!btnRight);
            btnRight->setAnchorPoint(CCPointZero);
            btnRight->setTag(tag_button_right);
            
            buttons->addObject(btnRight);
            
            
            const float dialogWidth = bg->getContentSize().width;
            const float btnWidth = btnLeft->getContentSize().width;
            const float space = (dialogWidth - btnWidth * 2) / 3;
            
            btnLeft->setPosition(ccp_fixed_X(space, BUTTON_POSITION_Y));
            btnRight->setPosition(ccp_fixed_X((space * 2 + btnWidth), BUTTON_POSITION_Y));
            
            // left button text
            m_pLeftBtnText = CCLabelTTF::create(leftText.c_str(), font_Helvetica_Neue, BUTTON_FONT_SIZE,
                                                btnLeft->getContentSize(), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            CC_BREAK_IF(!m_pLeftBtnText);
            
            // because button image have shadow, we need adjust text position.
            CCPoint textpos = ccp_get_center(m_pLeftBtnText->getContentSize()) + BUTTON_SHADOW_SIZE;
            
            m_pLeftBtnText->setPosition(textpos);
            m_pLeftBtnText->setColor(DEFAULT_TEXT_COLOR);
            btnLeft->addChild(m_pLeftBtnText);
            
            // right button text
            m_pRightBtnText = CCLabelTTF::create(rightText.c_str(), font_Helvetica_Neue, BUTTON_FONT_SIZE,
                                                 btnLeft->getContentSize(), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            CC_BREAK_IF(!m_pRightBtnText);
            
            m_pRightBtnText->setPosition(textpos);
            m_pRightBtnText->setColor(DEFAULT_TEXT_COLOR);
            btnRight->addChild(m_pRightBtnText);
        }
        
        //
        STButtonMenu* pMenu = STButtonMenu::createWithArray(buttons);
        CC_BREAK_IF(!pMenu);
        bg->addChild(pMenu);
        /*
         *  this settings layer have the same priority as menu, so we use "kCCMenuHandlerPriority - 1",
         *  ensure this menu can receieve the touch event before other menus and this layer itself.
         */
        pMenu->setTouchPriority(kCCMenuHandlerPriority - 1);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

void STAlertDialog::onButtonClicked(CCObject* btn)
{
    CCNode* b = (CCNode*)btn;
    switch (b->getTag())
    {
        case tag_button_left:
        {
            if (m_pLeftListener && m_pLeftSelector)
            {
                (m_pLeftListener->*m_pLeftSelector)(this);
            }
        }
            break;
            
        case tag_button_right:
        {
            if (m_pRightListener && m_pRightSelector)
            {
                (m_pRightListener->*m_pRightSelector)(this);
            }
        }
            break;
            
        default:
            break;
    }
    
    // closed dialog
    this->keyBackClicked();
}



