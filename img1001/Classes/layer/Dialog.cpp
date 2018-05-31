//
//  Dialog.cpp
//  HH002
//
//  Created by houhuang on 15/8/27.
//
//

#include "Dialog.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../define/Globalmacro.h"
#include "../data/DataManager.h"

#pragma mark - Dialog

enum
{
    st_Alert_Left_Tag = 10,
    st_Alert_Right_tag,
    
    st_DIALOG_TAG = 99
};

bool Dialog::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    LayerColor* lLayerColor = LayerColor::create(Color4B::BLACK);
    this->addChild(lLayerColor);
    lLayerColor->setOpacity(180);
    m_pLayerColor = lLayerColor;
    
    this->setTouchEnable(true);
    
    return true;
}

void Dialog::setTouchEnable(bool pBool)
{
    m_bIsTouchEnable = pBool;
    if (m_bIsTouchEnable)
    {
        this->addTouchListener();
    }
    else
    {
        this->removeTouchListener();
    }
}

void Dialog::addTouchListener()
{
    this->removeTouchListener();
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Dialog::onTouchBegan, this);
    listener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Dialog::removeTouchListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

bool Dialog::onTouchBegan(Touch* touch, Event* unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    lPos = Director::getInstance()->convertToGL(lPos);
    
    if (this->getBoundingBox().containsPoint(lPos))
    {
        return true;
    }

    return false;
}

void Dialog::doAction(Ref* pSender)
{

}

Dialog::~Dialog()
{
    this->removeTouchListener();
}


#pragma mark - TargetDialog

bool TargetDialog::init()
{
    if (!Dialog::init())
    {
        return false;
    }

    
    
    this->startListener();
    
    return true;
}

TargetDialog::TargetDialog():_leftTarget(NULL),_left_selector(NULL),_rightTarget(NULL),_right_selector(NULL)
{

}

void TargetDialog::removeDialog(Node* pNode)
{
    pNode->removeFromParentAndCleanup(true);
    
    Dialog* lDialog = (Dialog*)this->getChildByTag(st_DIALOG_TAG);
    if (lDialog)
    {
        return;
    }
    
    this->startListener();
    
}

void TargetDialog::setLeftListener(Ref* pLeftTarget, SEL_CallFuncN pLeftSelector)
{
    _leftTarget = pLeftTarget;
    _left_selector = pLeftSelector;
}

void TargetDialog::setRightListener(Ref* pRightTarget, SEL_CallFuncN pRightSelector)
{
    _rightTarget = pRightTarget;
    _right_selector = pRightSelector;
}

void TargetDialog::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        Dialog* lDialog = (Dialog*)this->getChildByTag(st_DIALOG_TAG);
        if (lDialog)
        {
            return;
        }
        if (_right_selector && _rightTarget)
        {
            (_rightTarget->*_right_selector)(this);
        }
    }
}

void TargetDialog::startListener()
{
    this->stopListener();
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(TargetDialog::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _keyListener = listener;
}

void TargetDialog::stopListener()
{
    if (_keyListener != NULL)
    {
        _eventDispatcher->removeEventListener(_keyListener);
        _keyListener = NULL;
    }
}


#pragma mark - UnlockLayer

STAlertDialog::STAlertDialog()
{

}

STAlertDialog* STAlertDialog::create(std::string pText, std::string pLeftBtnName, std::string pRightBtnName)
{
    STAlertDialog* layer = new STAlertDialog();
    if (layer && layer->init(pText, pLeftBtnName, pRightBtnName))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool STAlertDialog::init(std::string pText, std::string pLeftBtnName, std::string pRightBtnName)
{
    if (!TargetDialog::init())
    {
        return false;
    }
    
    this->setTouchEnable(true);
    _index = 0;
    
    Sprite* lBgSprite = Sprite::create("pop.png");
    ScreenAdapterHelper::setPosition(lBgSprite, Vec2(384, 512));
    this->addChild(lBgSprite);
    
    Sprite* hLine = Sprite::create("pop_seperator1.png");
    hLine->setPosition(Vec2(lBgSprite->getContentSize().width/2, ScreenAdapterHelper::getValue(110)));
    lBgSprite->addChild(hLine);
    
    
    Label* label = Label::createWithTTF(pText.c_str(), st_font_AR, ScreenAdapterHelper::getValue(40), Size(lBgSprite->getContentSize().width * 0.85, lBgSprite->getContentSize().height), TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    label->setColor(Color3B::BLACK);
    lBgSprite->addChild(label);
//    label->setDimensions(lBgSprite->getContentSize().width, lBgSprite->getContentSize().height);
    label->setPosition(Vec2(lBgSprite->getContentSize().width/2, lBgSprite->getContentSize().height/3 * 2));
    
    if (pRightBtnName.empty())
    {
        MenuItemImage* lLeftItem = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(STAlertDialog::doAction,this));
        lLeftItem->setPosition(Vec2(lBgSprite->getContentSize().width/2, ScreenAdapterHelper::getValue(70)));
        lLeftItem->setScale(1.5);
        lLeftItem->setTag(st_Alert_Left_Tag);
        
        Label* lLeftLabel = Label::createWithTTF(pLeftBtnName.c_str(), st_font_PTS, ScreenAdapterHelper::getValue(30));
        lLeftLabel->setColor(Color3B::BLACK);
        lLeftLabel->setPosition(Vec2(lLeftItem->getContentSize().width/2, lLeftItem->getContentSize().height/2));
        lLeftItem->addChild(lLeftLabel);
        
        m_pMenu = Menu::create(lLeftItem,NULL);
        lBgSprite->addChild(m_pMenu);
        m_pMenu->setPosition(Vec2::ZERO);
        
    }
    else
    {
        Sprite* vLine = Sprite::create("pop_seperator2.png");
        vLine->setPosition(Vec2(lBgSprite->getContentSize().width/2, ScreenAdapterHelper::getValue(70)));
        lBgSprite->addChild(vLine);
        
        
        MenuItemImage* lLeftItem = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(STAlertDialog::doAction,this));
        lLeftItem->setPosition(Vec2(lBgSprite->getContentSize().width/4 + ScreenAdapterHelper::getValue(20), ScreenAdapterHelper::getValue(70)));
        lLeftItem->setScale(1.5);
        lLeftItem->setTag(st_Alert_Left_Tag);
        
        Label* lLeftLabel = Label::createWithTTF(pLeftBtnName.c_str(), st_font_PTS, ScreenAdapterHelper::getValue(30));
        lLeftLabel->setColor(Color3B::BLACK);
        lLeftLabel->setPosition(Vec2(lLeftItem->getContentSize().width/2, lLeftItem->getContentSize().height/2));
        lLeftItem->addChild(lLeftLabel);
        
        
        MenuItemImage* lRightItem = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(STAlertDialog::doAction,this));
        lRightItem->setPosition(Vec2(lBgSprite->getContentSize().width/4 * 3 - ScreenAdapterHelper::getValue(20), ScreenAdapterHelper::getValue(70)));
        lRightItem->setScale(1.5);
        lRightItem->setTag(st_Alert_Right_tag);
        
        Label* lRightLabel = Label::createWithTTF(pRightBtnName.c_str(), st_font_PTS, ScreenAdapterHelper::getValue(30));
        lRightLabel->setColor(Color3B::BLACK);
        lRightLabel->setPosition(Vec2(lRightItem->getContentSize().width/2, lRightItem->getContentSize().height/2));
        lRightItem->addChild(lRightLabel);
        
        if (strcmp(pRightBtnName.c_str(), DM_getInstance->getLanguageMes()["Buy"].c_str()) == 0)
        {
            lRightLabel->setColor(Color3B(255, 69, 100));
        }

        m_pMenu = Menu::create(lLeftItem, lRightItem, NULL);
        lBgSprite->addChild(m_pMenu);
        m_pMenu->setPosition(Vec2::ZERO);
    }
    
    
    return true;
}

STAlertDialog* STAlertDialog::create(std::string pTitleText, std::string pContentText, std::string pLeftBtnName, std::string pRightBtnName)
{
    STAlertDialog* layer = new STAlertDialog();
    if (layer && layer->init(pTitleText, pContentText, pLeftBtnName, pRightBtnName))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool STAlertDialog::init(std::string pTitleText, std::string pContentText, std::string pLeftBtnName, std::string pRightBtnName)
{
    if (!TargetDialog::init())
    {
        return false;
    }
    
    this->setTouchEnable(true);
    _index = 0;
    
    Sprite* lBgSprite = Sprite::create("pop.png");
    ScreenAdapterHelper::setPosition(lBgSprite, Vec2(384, 512));
    this->addChild(lBgSprite);
    
    Sprite* hLine = Sprite::create("pop_seperator1.png");
    hLine->setPosition(Vec2(lBgSprite->getContentSize().width/2, ScreenAdapterHelper::getValue(110)));
    lBgSprite->addChild(hLine);
    
    
    Label* title = Label::createWithTTF(pTitleText.c_str(), st_font_AR, ScreenAdapterHelper::getValue(40));
    title->setColor(Color3B::BLACK);
    lBgSprite->addChild(title);
    title->setPosition(Vec2(lBgSprite->getContentSize().width/2, lBgSprite->getContentSize().height/4 * 3));
    
//    Label* content = Label::createWithTTF(pContentText.c_str(), st_font_AR, ScreenAdapterHelper::getValue(33));
    Label* content = Label::createWithTTF(pContentText.c_str(), st_font_AR, ScreenAdapterHelper::getValue(33), Size(lBgSprite->getContentSize().width * 0.85, lBgSprite->getContentSize().height), TextHAlignment::CENTER, TextVAlignment::CENTER);
    content->setColor(Color3B::BLACK);
    lBgSprite->addChild(content);
//    content->setDimensions(lBgSprite->getContentSize().width, lBgSprite->getContentSize().height);
    
    content->setPosition(Vec2(lBgSprite->getContentSize().width/2, lBgSprite->getContentSize().height/2));
    
    if (pRightBtnName.empty())
    {
        MenuItemImage* lLeftItem = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(STAlertDialog::doAction,this));
        lLeftItem->setPosition(Vec2(lBgSprite->getContentSize().width/2, ScreenAdapterHelper::getValue(70)));
        lLeftItem->setScale(1.5);
        lLeftItem->setTag(st_Alert_Left_Tag);
        
        Label* lLeftLabel = Label::createWithTTF(pLeftBtnName.c_str(), st_font_PTS, ScreenAdapterHelper::getValue(30));
        lLeftLabel->setColor(Color3B::BLACK);
        lLeftLabel->setPosition(Vec2(lLeftItem->getContentSize().width/2, lLeftItem->getContentSize().height/2));
        lLeftItem->addChild(lLeftLabel);
        
        m_pMenu = Menu::create(lLeftItem,NULL);
        lBgSprite->addChild(m_pMenu);
        m_pMenu->setPosition(Vec2::ZERO);
        
    }
    else
    {
        Sprite* vLine = Sprite::create("pop_seperator2.png");
        vLine->setPosition(Vec2(lBgSprite->getContentSize().width/2, ScreenAdapterHelper::getValue(70)));
        lBgSprite->addChild(vLine);
        
        
        MenuItemImage* lLeftItem = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(STAlertDialog::doAction,this));
        lLeftItem->setPosition(Vec2(lBgSprite->getContentSize().width/4 + ScreenAdapterHelper::getValue(20), ScreenAdapterHelper::getValue(70)));
        lLeftItem->setScale(1.5);
        lLeftItem->setTag(st_Alert_Left_Tag);
        
        Label* lLeftLabel = Label::createWithTTF(pLeftBtnName.c_str(), st_font_PTS, ScreenAdapterHelper::getValue(30));
        lLeftLabel->setColor(Color3B::BLACK);
        lLeftLabel->setPosition(Vec2(lLeftItem->getContentSize().width/2, lLeftItem->getContentSize().height/2));
        lLeftItem->addChild(lLeftLabel);
        
        
        MenuItemImage* lRightItem = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(STAlertDialog::doAction,this));
        lRightItem->setPosition(Vec2(lBgSprite->getContentSize().width/4 * 3 - ScreenAdapterHelper::getValue(20), ScreenAdapterHelper::getValue(70)));
        lRightItem->setScale(1.5);
        lRightItem->setTag(st_Alert_Right_tag);
        
        Label* lRightLabel = Label::createWithTTF(pRightBtnName.c_str(), st_font_PTS, ScreenAdapterHelper::getValue(30));
        lRightLabel->setColor(Color3B::BLACK);
        lRightLabel->setPosition(Vec2(lRightItem->getContentSize().width/2, lRightItem->getContentSize().height/2));
        lRightItem->addChild(lRightLabel);
        
        if (strcmp(pRightBtnName.c_str(), DM_getInstance->getLanguageMes()["Buy"].c_str()) == 0)
        {
            lRightLabel->setColor(Color3B(255, 69, 100));
        }
        
        m_pMenu = Menu::create(lLeftItem, lRightItem, NULL);
        lBgSprite->addChild(m_pMenu);
        m_pMenu->setPosition(Vec2::ZERO);
    }
    
    
    return true;
}

void STAlertDialog::doAction(Ref* pSender)
{
   // m_pMenu->setEnabled(false);
    Dialog::doAction(pSender);
    
    MenuItemImage* lmenuItem = (MenuItemImage*)pSender;
    switch (lmenuItem->getTag())
    {
        case st_Alert_Left_Tag:
        {
            if (_leftTarget && _left_selector)
            {
                (_leftTarget->*_left_selector)(this);
            }
        }
            break;
        case st_Alert_Right_tag:
        {
            if (_right_selector && _rightTarget)
            {
                (_rightTarget->*_right_selector)(this);
            }
        }
            break;
        default:
            break;
    }
    
}





