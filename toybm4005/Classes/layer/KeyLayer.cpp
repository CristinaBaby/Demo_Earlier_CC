//
//  KeyLayer.cpp
//  ColorGarden
//
//  Created by lidong on 15/8/19.
//
//

#include "KeyLayer.h"
#include "../layer/Dialog.h"
#include "../modules/STSystemFunction.h"
#include "Analytics.h"
enum
{
    K_DIALOG_TAG=999
};


bool KeyLayer::init()
{
    if(!Layer::init()) return false;
    
    this->startListener();
    
    return true;
}

void KeyLayer::startListener()
{
    this->stopListener();
    EventListenerKeyboard* listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(KeyLayer::onKeyPressed,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    _listener=listener;
}
void KeyLayer::stopListener()
{
    if(_listener!=NULL)
    {
        _eventDispatcher->removeEventListener(_listener);
        _listener=NULL;
    }
}

void KeyLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    CCLOG("keyCode=%d",keyCode);
    
    if(keyCode== EventKeyboard::KeyCode::KEY_BACK)
    {
        Dialog* lDialog=(Dialog*)this->getChildByTag(K_DIALOG_TAG);
        if(lDialog)
        {
            return;
        }
        STAlertDialog*lAlertDialog=STAlertDialog::create("Are you sure you want to quit the game?","Yes", "No");
        lAlertDialog->setLeftListener(this,callfuncN_selector(KeyLayer::quit));
        lAlertDialog->setRightListener(this, callfuncN_selector(KeyLayer::removeDialog));
        this->addChild(lAlertDialog,0,K_DIALOG_TAG);
    }
}

void KeyLayer::quit(Node* pRef)
{
    
    STSystemFunction* sf = new STSystemFunction();
    sf->endSession();
    CC_SAFE_DELETE(sf);
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void KeyLayer::removeDialog(Node* pRef)
{
    pRef->removeFromParentAndCleanup(true);
    
}

