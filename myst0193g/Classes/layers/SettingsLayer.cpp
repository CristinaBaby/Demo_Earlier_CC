//
//  SettingsLayer.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#include "SettingsLayer.h"
#include "../AppGlobal.h"
#include "../AppConfig.h"
#include "../FileModifiers.h"
#include "../helpers/AudioHelper.h"
#include "../modules/STSystemFunction.h"
#include "../scenes/HomeScene.h"
#include "../utilities/STUserDefault.h"
#include "../widgets/STAlertDialog.h"
#include "../widgets/STUILayer.h"


USING_NS_CC;
USING_NS_CC_EXT;
using std::string;

enum
{
    tag_slider_music = 50, tag_slider_sound,
};

SettingsLayer* SettingsLayer::create(bool onlyOneButton)
{
    SettingsLayer *pRet = new SettingsLayer(onlyOneButton);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

SettingsLayer* SettingsLayer::createWithColor(cocos2d::ccColor4B color, bool onlyOneButton)
{
    SettingsLayer *pRet = new SettingsLayer(onlyOneButton);
    if (pRet && pRet->initWithColor(color))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool SettingsLayer::initWithColor(const cocos2d::ccColor4B& color)
{
    bool pRet = false;
    
    do
    {
        CC_BREAK_IF(! CoverLayer::initWithColor(color));
        
        // add body
        CC_BREAK_IF(!createFrame());
        
        pRet = true;
    } while (0);
    
    return pRet;
}

bool SettingsLayer::createFrame()
{
    STUILayer* pUILayer = STUILayer::create();
    if (!pUILayer) return false;
    
    CCSpriteBatchNode *pBatchNode = CCSpriteBatchNode::create("pop-window.pvr.ccz");
    if (!pBatchNode) return false;
    pUILayer->addChild(pBatchNode);
    
    // frame
    CCSprite* pFrame = CCSprite::createWithSpriteFrameName("ui05_box.png");
    if (!pFrame) return false;
    // in the center of screen
    pFrame->setPosition(ccp_center_screen);
    pBatchNode->addChild(pFrame);
    
    // text
    CCSprite* pText = CCSprite::createWithSpriteFrameName("ui07_settings.png");
    if (!pText) return false;
    pText->setPosition(ccp_fixed_X(pFrame->getContentSize().width / 2, 1100));
    pFrame->addChild(pText);
    
    // music icon and sound icon
    CCSprite* pMusicIcon = CCSprite::createWithSpriteFrameName("ui07_music.png");
    if (!pMusicIcon) return false;
    pMusicIcon->setAnchorPoint(ccp(0, .5f));
    pMusicIcon->setPosition(ccp2(230, 775));
    pFrame->addChild(pMusicIcon);
    
    // music slider bar bg
    CCSprite* pMusicBg = CCSprite::createWithSpriteFrameName("ui07_bar_bg.png");
    if (!pMusicBg) return false;
    pMusicBg->setAnchorPoint(ccp(0, .5f));
    pMusicBg->setPosition(ccp2(490, 775));
    pFrame->addChild(pMusicBg);
    
    // slider bar
    pMusicSlider = STSlider::create(CCSprite::createWithSpriteFrameName("ui07_line.png"),
                                    CCSprite::createWithSpriteFrameName("ui07_bar.png"),
                                    CCSprite::createWithSpriteFrameName("ui07_button.png"),
                                    this, callfuncN_selector(SettingsLayer::onSliderChanged));
    if (!pMusicSlider) return false;
    // ensure this slider can receieve the touch event same as button menu.
    // this settings layer have the same priority as menu, so we use "kCCMenuHandlerPriority - 1"
    pMusicSlider->setTouchPriority(kCCMenuHandlerPriority - 1);
    pMusicSlider->setTag(tag_slider_music);
    pMusicSlider->setMinimumValue(0.f);
    pMusicSlider->setMaximumValue(1.f);
    pMusicSlider->setValue(AudioHelper::getInstance()->getMusicVolume());
    pMusicSlider->setAnchorPoint(ccp(0, .5f));
    pMusicSlider->setPosition(ccp2(714, 937));
    pUILayer->addChild(pMusicSlider);
    
    // -------------------------------------------------------------------------
    
    CCSprite* pSoundIcon = CCSprite::createWithSpriteFrameName("ui07_sound.png");
    if (!pSoundIcon) return false;
    pSoundIcon->setAnchorPoint(ccp(0, .5f));
    pSoundIcon->setPosition(ccp2(230, 517));
    pFrame->addChild(pSoundIcon);
    
    // sound slider bar bg
    CCSprite* pSoundBg = CCSprite::createWithSpriteFrameName("ui07_bar_bg.png");
    if (!pSoundBg) return false;
    pSoundBg->setAnchorPoint(ccp(0, .5f));
    pSoundBg->setPosition(ccp2(490, 517));
    pFrame->addChild(pSoundBg);
    
    // slider bar
    pSoundSlider = STSlider::create(CCSprite::createWithSpriteFrameName("ui07_line.png"),
                                    CCSprite::createWithSpriteFrameName("ui07_bar.png"),
                                    CCSprite::createWithSpriteFrameName("ui07_button.png"),
                                    this, callfuncN_selector(SettingsLayer::onSliderChanged));
    if (!pSoundSlider) return false;
    // ensure this slider can receieve the touch event same as button menu.
    // this settings layer have the same priority as menu, so we use "kCCMenuHandlerPriority - 1"
    pSoundSlider->setTouchPriority(kCCMenuHandlerPriority - 1);
    pSoundSlider->setTag(tag_slider_sound);
    pSoundSlider->setMinimumValue(0.f);
    pSoundSlider->setMaximumValue(1.f);
    pSoundSlider->setValue(AudioHelper::getInstance()->getSoundVolume());
    pSoundSlider->setAnchorPoint(ccp(0, .5f));
    pSoundSlider->setPosition(ccp2(714, 678));
    pUILayer->addChild(pSoundSlider);
    

    
    pBtnClose = STButton::createWithSpriteFrameName(string("ui07_close.png"), string("ui07_close_h.png"),
                                                           this, callfuncN_selector(SettingsLayer::onCloseClicked));
    if (!pBtnClose) return false;
    pBtnClose->setAnchorPoint(ccp(1, 1));
    float offset = get_offset(35);
    pBtnClose->setPosition(ccp(pFrame->getContentSize().width-offset, pFrame->getContentSize().height-offset));
    pFrame->addChild(pBtnClose);
    const int touchPriority = pBtnClose->getTouchPriority() - 1;
    pBtnClose->setTouchPriority(touchPriority);

    
    // share button
    pBtnShare = STButton::createWithSpriteFrameName(string("ui07_tell_a_friend.png"), string("ui07_tell_a_friend_h.png"),
                                                           this, callfuncN_selector(SettingsLayer::onShareClicked));
    if (!pBtnShare) return false;
    pFrame->addChild(pBtnShare);
    pBtnShare->setTouchPriority(touchPriority);
    // only have a tell a friend button
    if (isOneButton)
    {
        pBtnShare->setPosition(ccp_fixed_X(pFrame->getContentSize().width / 2, 224));
    }
    else
    {
        pBtnShare->setAnchorPoint(ccp(0, 0.5));
        pBtnShare->setPosition(ccp2(900, 224));
        
        // delete button
        pBtnDelete = STButton::createWithSpriteFrameName(string("ui07_delete.png"), string("ui07_delete_h.png"),
                                                                this, callfuncN_selector(SettingsLayer::onDeleteClicked));
        if (!pBtnDelete) return false;
        pBtnDelete->setAnchorPoint(ccp(0, 0.5));
        pBtnDelete->setPosition(ccp2(170, 224));
        pFrame->addChild(pBtnDelete);
        pBtnDelete->setTouchPriority(touchPriority);
    }
    
    
    // add setting frame
    this->addChild(pUILayer);
    
    // ----------------------------
    // load volume from file
    //        STLoadFloatFromUD();
    
    return true;
}

void SettingsLayer::onShareClicked(CCNode* button)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    std::string subject = AppGlobal::stAllStrings->valueForKey(strings_email_subject)->m_sString;
    
    std::string body;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    body = AppGlobal::stAllStrings->valueForKey(strings_email_body)->m_sString;
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    body.append("Hey!\n")
    .append("\nI am playing this awesome hidden object game - ")
    .append(project_name)
    .append("!\nI think you will like this also!\n")
    .append("Get it NOW!\n");
    
#if (ST_TARGET_MARKET == ST_TARGET_MARKET_AMAZON)
    body.append("http://www.amazon.com/gp/mas/dl/android?p=")
    .append(st_android_package_name).append("\n");
#else
    body.append("https://play.google.com/store/apps/details?id=")
    .append(st_android_package_name).append("\n");
#endif
    
#endif
    
    
    STSystemFunction* sf = new STSystemFunction();
    sf->sendEmail(subject.c_str(), body.c_str());
    
    CC_SAFE_DELETE(sf);
    
}

void SettingsLayer::onDeleteClicked(CCNode* button)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    std::string message = AppGlobal::stAllStrings->valueForKey(strings_delete_content)->m_sString;
    std::string btnTextL = AppGlobal::stAllStrings->valueForKey(strings_button_yes)->m_sString;
    std::string btnTextR = AppGlobal::stAllStrings->valueForKey(strings_button_no)->m_sString;
    
    STAlertDialog* dialog = STAlertDialog::show(this, message, btnTextL, btnTextR);
    dialog->setLeftButtonListener(this, callfuncN_selector(SettingsLayer::deleteUserData));
    dialog->setCloseCallback(this, callfuncN_selector(SettingsLayer::onAlertDialogClosed));
    
    setButtonsEnable(false);
}

void SettingsLayer::deleteUserData(CCObject*)
{
    // reset xml
    namespaceST::STUserDefault::getInstance()->reset();
    
    // reset sound and music
    AudioHelper::getInstance()->reset();
    pMusicSlider->setValue(AudioHelper::getInstance()->getMusicVolume());
    pSoundSlider->setValue(AudioHelper::getInstance()->getSoundVolume());
    
    
    // when running scene is home scene, we will reset home layer, and remove saved game
    CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
    if (tag_scene_home == pScene->getTag())
    {
        HomeLayer* pHomeLayer = dynamic_cast<HomeLayer*>(this->getParent());
        if (!pHomeLayer) return;
        pHomeLayer->resetLayoutToWithoutResume();
        
        
    }

    scheduleOnce(schedule_selector(SettingsLayer::onCloseClicked), 0.05);
}

void SettingsLayer::onCloseClicked(CCNode* button)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    CoverLayer::keyBackClicked();
}

void SettingsLayer::onAlertDialogClosed(CCObject*)
{
    setButtonsEnable(true);
}

void SettingsLayer::setButtonsEnable(bool isEnable)
{
    pMusicSlider->setEnabled(isEnable);
    pSoundSlider->setEnabled(isEnable);
    pBtnClose->setEnabled(isEnable);
    pBtnShare->setEnabled(isEnable);
    pBtnDelete->setEnabled(isEnable);
}

void SettingsLayer::onSliderChanged(CCNode *slider)
{
    if (slider)
    {
        STSlider* pSSl = dynamic_cast<STSlider*>(slider);
        const float volume = pSSl->getValue();
        
        switch (pSSl->getTag())
        {
            case tag_slider_music:
                //                CCLOG("music volume is %f", volume);
                AudioHelper::getInstance()->changeMusicVolume(volume);
                break;
                
            case tag_slider_sound:
                //                CCLOG("sound volume is %f", volume);
                AudioHelper::getInstance()->changeSoundVolume(volume);
                break;
                
            default:
                break;
        }
    }
}

SettingsLayer::SettingsLayer(bool onlyOneButton)
: pBtnDelete(NULL)
{
    isOneButton = onlyOneButton;
}

SettingsLayer::~SettingsLayer()
{
}

