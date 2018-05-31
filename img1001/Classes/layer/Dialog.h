//
//  Dialog.h
//  HH002
//
//  Created by houhuang on 15/8/27.
//
//

#ifndef __HH002__Dialog__
#define __HH002__Dialog__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
USING_NS_CC;


class Dialog : public Layer
{
public:
    bool init();
    
    virtual void setTouchEnable(bool pBool);
    virtual void addTouchListener();
    virtual void removeTouchListener();
    
    virtual bool onTouchBegan(Touch* touch, Event* unused_event);
    void doAction(Ref* pSender);
    
    ~Dialog();
    
protected:
    bool m_bIsTouchEnable;
    LayerColor* m_pLayerColor;
};


class TargetDialog : public Dialog
{
public:
    bool init();
    TargetDialog();
    
    void removeDialog(Node* pNode);
    
public:
    void setLeftListener(Ref* pLeftTarget, SEL_CallFuncN pLeftSelector);
    void setRightListener(Ref* pRightTarget, SEL_CallFuncN pRightSelector);
    
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    
    void startListener();
    void stopListener();
    
protected:
    Ref*    _leftTarget;
    SEL_CallFuncN   _left_selector;
    
    Ref*    _rightTarget;
    SEL_CallFuncN   _right_selector;
    
    EventListenerKeyboard* _keyListener;
    
};


class STAlertDialog : public TargetDialog
{
public:
    STAlertDialog();
    
    static STAlertDialog* create(std::string pText, std::string pLeftBtnName, std::string pRightBtnName);
    bool init(std::string pText, std::string pLeftBtnName, std::string pRightBtnName);
    
    static STAlertDialog* create(std::string pTitleText, std::string pContentText, std::string pLeftBtnName, std::string pRightBtnName);
    bool init(std::string pTitleText, std::string pContentText, std::string pLeftBtnName, std::string pRightBtnName);
    
    void doAction(Ref* pSender);
    
    st_property(int, _index, Index);
protected:
    Menu* m_pMenu;
};




#endif /* defined(__HH002__Dialog__) */
