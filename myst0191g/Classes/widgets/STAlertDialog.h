//
//  STAlertDialog.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-22.
//
//

#ifndef __LVUP002__STAlertDialog__
#define __LVUP002__STAlertDialog__

#include <cocos2d.h>
#include "CoverLayer.h"

class STAlertDialog : public CoverLayer
{
public:
    STAlertDialog();
    virtual ~STAlertDialog();
    
    static STAlertDialog* show(cocos2d::CCNode* container, cocos2d::CCNode* text,
                        cocos2d::CCNode* leftBtn, cocos2d::CCNode* rightBtn,
                        cocos2d::CCObject* leftTarget, cocos2d::SEL_CallFuncN leftSelector,
                        cocos2d::CCObject* rightTarget, cocos2d::SEL_CallFuncN rightSelector);
    
    static STAlertDialog* show(cocos2d::CCNode* container, cocos2d::CCNode* text, cocos2d::CCNode* button,
                        cocos2d::CCObject* target, cocos2d::SEL_CallFuncN selector);
    
    static STAlertDialog* show(cocos2d::CCSpriteFrame* container, cocos2d::CCSpriteFrame* button,
                        std::string text, std::string leftBtnText, std::string rightBtnText,
                        cocos2d::CCObject* leftTarget, cocos2d::SEL_CallFuncN leftSelector,
                        cocos2d::CCObject* rightTarget, cocos2d::SEL_CallFuncN rightSelector);
    
    static STAlertDialog* show(cocos2d::CCSpriteFrame* container, cocos2d::CCSpriteFrame* button,
                        std::string text, std::string btnText,
                        cocos2d::CCObject* leftTarget, cocos2d::SEL_CallFuncN leftSelector);
    
    static STAlertDialog* show(cocos2d::CCNode* parent, std::string text, std::string btnText);
    
    static STAlertDialog* show(cocos2d::CCNode* parent, std::string text, std::string leftBtnText, std::string rightBtnText);
    
    static STAlertDialog* show(cocos2d::CCSpriteFrame* container,
                        cocos2d::CCSpriteFrame* leftButton, cocos2d::CCSpriteFrame* rightButton,
                        std::string text, std::string leftBtnText, std::string rightBtnText,
                        cocos2d::CCObject* leftTarget, cocos2d::SEL_CallFuncN leftSelector,
                        cocos2d::CCObject* rightTarget, cocos2d::SEL_CallFuncN rightSelector);
    
    
    void setLeftButtonListener (cocos2d::CCObject* target, cocos2d::SEL_CallFuncN selector);
    void setRightButtonListener(cocos2d::CCObject* target, cocos2d::SEL_CallFuncN selector);
    
    void setMessageTextColor(const cocos2d::ccColor4B& color);
    void setButtonTextColor(const cocos2d::ccColor4B& color);
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color);
    
//    virtual bool initWithTexts1(std::string text, std::string btnText);
    virtual bool initWithTexts(std::string text, std::string leftText, std::string rightText = "");
    
private:
    cocos2d::CCNode*                m_pParentNode;
    cocos2d::CCSpriteFrame*         m_pBackgroundFrame;
    cocos2d::CCSpriteFrame*         m_pButtonFrame;
    cocos2d::CCSpriteFrame*         m_pButtonSelectedFrame;
    cocos2d::CCLabelTTF*            m_pMessage;
    cocos2d::CCLabelTTF*            m_pLeftBtnText;
    cocos2d::CCLabelTTF*            m_pRightBtnText;
    
protected:
    cocos2d::CCObject*              m_pLeftListener;
    cocos2d::SEL_CallFuncN          m_pLeftSelector;
    cocos2d::CCObject*              m_pRightListener;
    cocos2d::SEL_CallFuncN          m_pRightSelector;
    
private:
    void onButtonClicked(cocos2d::CCObject* btn);
};

#endif /* defined(__LVUP002__STAlertDialog__) */
