//
//  GameUILayer.cpp
//  ColorGarden
//
//  Created by lidong on 15/8/3.
//
//

#include "GameUILayer.h"
#include "../helper/ControlHelper.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../scene/ChooseLevelScene.h"
#include "../layer/ScrollColorLayer.h"
#include "../scene/GameScene.h"
#include "../game/GameLayer.h"
#include "../scene/ShareScene.h"
#include "../layer/Dialog.h"
#include "../define/ResoureMacro.h"
#include "../layer/ColorScrollLayer/ColorScrollLayer.h"
#include "../crossplatformapi/headers/ads/BannerAds.h"
#include "../helper/DeviceHelper.h"

enum
{
   eGameUI_MenuItem_Back_Tag=10,
   eGameUI_MenuItem_Reset_Tag,
   eGameUI_MenuItem_Undo_Tag,
   eGameUI_MenuItem_Share_Tag,
   K_DIALOG_TAG=999
};

bool GameUILayer::init(int pIndex)
{
    if(!Layer::init()) return false;

    this->setIndex(pIndex);
    
    this->loadUI();
    
    _isShowUndo = false;
    
    return true;
}

GameUILayer* GameUILayer::create(int pIndex)
{
    GameUILayer *pRet = new GameUILayer();
    if (pRet && pRet->init(pIndex))
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

void GameUILayer::loadUI()
{
//    BannerAds::getInstance()->remove();
    LayerColor* layerColor=LayerColor::create(Color4B(49, 49, 49, 255),this->getContentSize().width,ScreenAdapterHelper::getValue(90));
    this->addChild(layerColor);
    layerColor->setScale(ScreenAdapterHelper::getRealDesignScale());
    layerColor->setPosition(Vec2(0, this->getContentSize().height-ScreenAdapterHelper::getValue(90)));
    
    float lY=995 - ScreenAdapterHelper::getRealDesignScale() * 17;
    
    MenuItemSprite* lMenuItemBack=ControlHelper::getMenuItem("back.png", eGameUI_MenuItem_Back_Tag, CC_CALLBACK_1(GameUILayer::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemBack, Vec2(60, lY),true,false,false,false);
    
    MenuItemSprite* lMenuItemReset=ControlHelper::getMenuItem("reset.png", eGameUI_MenuItem_Reset_Tag, CC_CALLBACK_1(GameUILayer::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemReset, Vec2(270, lY),true,false,false,false);
    
    MenuItemSprite* lMenuItemUndo=ControlHelper::getMenuItem("undo.png", eGameUI_MenuItem_Undo_Tag, CC_CALLBACK_1(GameUILayer::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemUndo, Vec2(480, lY),true,false,false,false);
    
    MenuItemSprite* lMenuItemShare=ControlHelper::getMenuItem("next.png", eGameUI_MenuItem_Share_Tag, CC_CALLBACK_1(GameUILayer::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemShare, Vec2(690, lY),true,false,false,false);
    
    lMenuItemBack->setScale(0.8f);
    lMenuItemReset->setScale(0.8f);
    lMenuItemUndo->setScale(0.8f);
    lMenuItemShare->setScale(0.8f);
    
    _menu=Menu::create(lMenuItemBack,lMenuItemReset,lMenuItemUndo,lMenuItemShare, NULL);
    _menu->setAnchorPoint(Vec2(0, 0));
    _menu->setPosition(Vec2(0, 0));
    this->addChild(_menu);
    
//    ScrollColorLayer* layer=ScrollColorLayer::create();
//    layer->setIndex(this->getIndex());
//
//    this->addChild(layer);
    
    ColorScrollLayer* layer = ColorScrollLayer::create(Size(0, 0));
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2(0.5, 0));
    ScreenAdapterHelper::setPosition(layer, Vec2(384, -20), false, true, false, false);
    if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Pad)
    {
        ScreenAdapterHelper::setPosition(layer, Vec2(384, -50), false, true, false, false);
    }
    
//    layer->setPosition(Vec2(<#float xx#>, 0));
    
    this->addChild(layer);
    
    _shadeLayer=NULL;
    
    if(!DataManager::getInstance()->isShowingTutorial())
    {
        DataManager::getInstance()->setIsShowingTutorial(true);
        _shadeLayer=LayerColor::create(Color4B(0, 0, 0, 180));
        this->addChild(_shadeLayer);
        
        Sprite* lTutorialSprite=ControlHelper::AddSprite("tutorial.png", _shadeLayer);
        ScreenAdapterHelper::setPosition(lTutorialSprite, Vec2(384, 512));
    }
}

void GameUILayer::loadScrollView()
{
    
}

void GameUILayer::doAction(Ref* pSender)
{
    this->removeTutorial();
    
    MenuItemSprite* lMenuItem=(MenuItemSprite*)pSender;
    switch (lMenuItem->getTag()) {
        case eGameUI_MenuItem_Back_Tag:
        {
            _menu->setEnabled(false);
            
            GameScene* lGameScene= (GameScene*)this->getParent();
            lGameScene->getGameLayer()->saveImage();
            Director::getInstance()->replaceScene(ChooseLevelScene::scene(this->getIndex()));
        }
            break;
        case eGameUI_MenuItem_Reset_Tag:
        {
            GameScene* lGameScene= (GameScene*)this->getParent();
            lGameScene->getGameLayer()->enableFillNode(false);
            
            STAlertDialog* lDialog=STAlertDialog::create("Are you sure you want to reset this painting? It cannot be recovered! ", "Cancel", "Reset");
            lDialog->setLeftListener(this, callfuncN_selector(GameUILayer::removeDialog));
            lDialog->setRightListener(this, callfuncN_selector(GameUILayer::resetGame));

            this->addChild(lDialog,0,K_DIALOG_TAG);
           
        }
            break;
        case eGameUI_MenuItem_Undo_Tag:
        {
            _menu->setEnabled(false);
            GameScene* lGameScene= (GameScene*)this->getParent();
            lGameScene->getGameLayer()->undo();
            scheduleOnce(schedule_selector(GameUILayer::enableMenu), 0.3f);
        }
            break;
        case eGameUI_MenuItem_Share_Tag:
        {
            _menu->setEnabled(false);
            
            GameScene* lGameScene= dynamic_cast<GameScene*>(getParent());
            lGameScene->getGameLayer()->saveImage();
            lGameScene -> stopTouchEvent();
            
            Director::getInstance()->pushScene(ShareScene::scene(this->getIndex(),lGameScene));
            
            this->scheduleOnce(schedule_selector(GameUILayer::enableMenu), 0.5f);
        }
            break;
        default:
            break;
    }
}

void GameUILayer::enableMenu(float dt)
{
    _menu->setEnabled(true);
}
void GameUILayer::removeTutorial()
{
    if(_shadeLayer!=NULL)
    {
        _shadeLayer->removeFromParentAndCleanup(true);
        _shadeLayer=NULL;
    }
}

void GameUILayer::removeDialog(Node* pRef)
{

    pRef->removeFromParentAndCleanup(true);

    CallFunc* lFunc=CallFunc::create(CC_CALLBACK_0(GameUILayer::enableFillNode, this));
    this->runAction(lFunc);
   
}

void GameUILayer::removeUndoSprite(Node* pNode)
{
    _isShowUndo = false;
    pNode->removeFromParentAndCleanup(true);
}

void GameUILayer::resetGame(Node* pRef)
{
    pRef->removeFromParentAndCleanup(true);
    

    this->scheduleOnce(schedule_selector(GameUILayer::reset), 0.0f);
    
//    CallFunc* lFunc=CallFunc::create(CC_CALLBACK_0(GameUILayer::reset,this));
//    this->runAction(lFunc);

}

void GameUILayer::enableFillNode()
{
    GameScene* lGameScene= (GameScene*)this->getParent();
    lGameScene->getGameLayer()->enableFillNode(true);
}

void GameUILayer::reset(float dt)
{
    GameScene* lGameScene= (GameScene*)this->getParent();
    lGameScene->getGameLayer()->reset();
    
    lGameScene->getGameLayer()->enableFillNode(true);
}

void GameUILayer::undoEndPrompt()
{
    if (_isShowUndo)
    {
        return;
    }
    
    _isShowUndo = true;
    
//    Dialog* lDialog = Dialog::create();
//    this->addChild(lDialog);
//    lDialog->setToucEnable(true);
//    lDialog->setTag(333);
    
    Sprite* lSprite=ControlHelper::AddSprite("popbg.png", this);
    ScreenAdapterHelper::setPosition(lSprite, Vec2(384, 620));

    Label* label=ControlHelper::getLabel(st_font_futumd, "Undo limit reached", st_dialog_text_font_size);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
    lSprite->addChild(label);

    DelayTime* lDelayTime2=DelayTime::create(1.0);
    Sequence* lAction2=Sequence::create(lDelayTime2,Hide::create(), NULL);
    label->runAction(lAction2);
    
    DelayTime* lDelayTime=DelayTime::create(1.0);
    FadeOut* lFadeOut=FadeOut::create(0.5f);
    CallFuncN* lFuncN=CallFuncN::create(CC_CALLBACK_1(GameUILayer::removeUndoSprite, this));
    Sequence* lAction=Sequence::create(lDelayTime,lFadeOut,lFuncN, NULL);
    lSprite->runAction(lAction);
    

    
}

