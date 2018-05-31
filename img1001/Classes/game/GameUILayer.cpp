//
//  GameUILayer.cpp
//  ColorFree
//
//  Created by houhuang on 15/8/28.
//
//

#include "GameUILayer.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../data/DataManager.h"
#include "../compoment/ColorSprite.h"
#include "../layer/Dialog.h"
#include "../scene/GameScene.h"
#include "GameLayer.h"
#include "../scene/ChooseMapScene.h"
#include "../scene/EditScene.h"
#include "../layer/TutorialLayer.h"
#include "../scene/MyArtworksScene.hpp"
#include "../scene/ChooseGalleryScene.h"

enum
{
    st_gameUILayer_resentlyColor1_tag = 0,
    st_gameUILayer_resentlyColor2_tag = 1,
    st_gameUILayer_resentlyColor3_tag = 2,
    
    st_gameUILayer_button_back = 10,
    st_gameUILayer_button_reset,
    st_gameUILayer_button_undo,
    st_gameUILayer_button_edit,
    
    st_gameUILayer_pen = 50,
    
};

GameUILayer::~GameUILayer()
{
    m_pRensentlyColor.clear();
    NotificationCenter::getInstance()->removeObserver(this, st_baseScene_downloadSuc_notifition);
}

GameUILayer::GameUILayer()
{
    m_pBottomMask = NULL;
    m_pTutorialLayer = NULL;
    
    m_pBackUIB = NULL;
    m_pEditUIB = NULL;

    m_bIsShowDialog = false;
    m_bIsReset = false;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameUILayer::waitDownLoadSucNotification), st_baseScene_downloadSuc_notifition, NULL);
}

GameUILayer* GameUILayer::create()
{
    GameUILayer* layer = new GameUILayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool GameUILayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    
    this->setIndex(DataManager::getInstance()->m_pCurrentImage.ID);
    this->initUI();
    
    return true;
}

void GameUILayer::initUI()
{
    Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    
    Sprite* logo_line = Sprite::create("seperator-2.png");
    ScreenAdapterHelper::setPosition(logo_line, Vec2(384, 100),KVBoarderTypeTop,KHBoarderTypeNone);
    this->addChild(logo_line);

    ColorScrollLayerMask* topMask = ColorScrollLayerMask::create(Size(ScreenAdapterHelper::getValue(1000), ScreenAdapterHelper::getValue(200)));
    ScreenAdapterHelper::setPosition(topMask, Vec2(384, 100), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(topMask);
    
    for (int i = 0; i < 3; i ++)
    {
        Sprite* dot = Sprite::create("dot.png");
        ScreenAdapterHelper::setPosition(dot, Vec2(768/4 *  (i+1), 50), KVBoarderTypeTop, KHBoarderTypeNone);
        this->addChild(dot);
    }
    
    UIButton* back = UIButton::create("btn_back.png");
    ScreenAdapterHelper::setPosition(back, Vec2(768/8, 50), KVBoarderTypeTop, KHBoarderTypeNone);
    back->initListener(this, callfuncN_selector(GameUILayer::doAction));
    back->setTag(st_gameUILayer_button_back);
    this->addChild(back);
    m_pBackUIB = back;
    
    UIButton* reset = UIButton::create("btn_reset.png");
    ScreenAdapterHelper::setPosition(reset, Vec2(768/8 * 3, 50), KVBoarderTypeTop, KHBoarderTypeNone);
    reset->initListener(this, callfuncN_selector(GameUILayer::doAction));
    reset->setTag(st_gameUILayer_button_reset);
    this->addChild(reset);
    
    UIButton* undo = UIButton::create("btn_undo.png");
    ScreenAdapterHelper::setPosition(undo, Vec2(768/8 * 5, 50), KVBoarderTypeTop, KHBoarderTypeNone);
    undo->initListener(this, callfuncN_selector(GameUILayer::doAction));
    undo->setTag(st_gameUILayer_button_undo);
    this->addChild(undo);
    
    UIButton* edit = UIButton::create("btn_done.png");
    ScreenAdapterHelper::setPosition(edit, Vec2(768/8 * 7, 50), KVBoarderTypeTop, KHBoarderTypeNone);
    edit->initListener(this, callfuncN_selector(GameUILayer::doAction));
    edit->setTag(st_gameUILayer_button_edit);
    this->addChild(edit);
    m_pEditUIB = edit;
    
    m_pTutorialLayer = nullptr;
    
    if (!DataManager::getInstance()->isShowingTutorial())
    {
        DataManager::getInstance()->setIsShowingTutorial(true);

        m_pTutorialLayer = TutorialLayer::create();
        this->addChild(m_pTutorialLayer, 100);
        
    }

    
    //最近使用的三种颜色
//    for (int i = 0; i < 3; i ++)
//    {
////        log("%d  %d  %d",DataManager::getInstance()->m_vRecentUseColor.at(0).R,DataManager::getInstance()->m_vRecentUseColor.at(0).G,DataManager::getInstance()->m_vRecentUseColor.at(0).B);
//        ColorSprite* colorSprite = ColorSprite::create("rounded_color.png", "rounded_stroke.png");
//        ScreenAdapterHelper::setPosition(colorSprite, Vec2(470 + 85*i, 260),KVBoarderTypeBottom, KHBoarderTypeNone);
//        this->addChild(colorSprite);
//        ColorData colorData = DataManager::getInstance()->m_vRecentUseColor.at(i);
//        
//        colorSprite->setColor(Color3B(colorData.R, colorData.G, colorData.B));
//        colorSprite->setColorData(colorData);
//        colorSprite->setTag(i);
//        
//        colorSprite->addListener(callfuncN_selector(GameUILayer::reponseTouchResentlyColor), this, true);
//        
//        
//        Sprite* clickColor = Sprite::create("clicked_color.png");
//        clickColor->setPosition(Vec2(colorSprite->getContentSize().width/2, colorSprite->getContentSize().height/2));
//        clickColor->setVisible(false);
//        colorSprite->addChild(clickColor);
//        clickColor->setTag(st_gameUILayer_pen);
//        
//        Sprite* pen = Sprite::create("clicked-color-icon.png");
//        pen->setPosition(Vec2(clickColor->getContentSize().width/2, clickColor->getContentSize().height/2));
//        clickColor->addChild(pen);
//        
//        
//        m_pRensentlyColor.push_back(colorSprite);
//    }
    
//    log("DataManager::getInstance()->m_iSelectedColorIndex=%d",DataManager::getInstance()->m_iSelectedColorIndex);
    
//    m_pRensentlyColor.at(DataManager::getInstance()->m_iSelectedColorIndex)->getChildByTag(st_gameUILayer_pen)->setVisible(true);
    
    m_pBottomMask= ColorScrollLayerMask::create(Size(ScreenAdapterHelper::getValue(1000), ScreenAdapterHelper::getValue(610)));
    m_pBottomMask->ignoreAnchorPointForPosition(false);
    m_pBottomMask->setAnchorPoint(Vec2(0.5, 1));
    ScreenAdapterHelper::setPosition(m_pBottomMask, Vec2(384, 185 + 270 + 3), KVBoarderTypeBottom, KHBoarderTypeNone);
    this->addChild(m_pBottomMask);
    m_vBottomOrginalPos = m_pBottomMask->getPosition();
    
    
    //色板
    ColorScrollLayer* layer = ColorScrollLayer::create(Size(0, 0));
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2(0.5, 1));
    ScreenAdapterHelper::setPosition(layer, Vec2(384, 130.4 + 270),KVBoarderTypeBottom, KHBoarderTypeNone);
    this->addChild(layer);
    m_pColorScrollLayer = layer;
    
    ColorScrollLayerMask* layerMask = ColorScrollLayerMask::create(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(210)));
    layerMask->setAnchorPoint(Vec2(0.5, 1));
    ScreenAdapterHelper::setPosition(layerMask, Vec2(384, 122.5),KVBoarderTypeBottom,KHBoarderTypeNone);
    this->addChild(layerMask);
    
    
}

void GameUILayer::reponseMaskAnimation(bool pBool)
{
    if (pBool)
    {
        m_pBottomMask->runAction(EaseExponentialOut::create(MoveTo::create(0.3f, m_vBottomOrginalPos + Vec2(0, -ScreenAdapterHelper::getValue(270)))));
    }
    else
    {
        m_pBottomMask->runAction(EaseExponentialIn::create(MoveTo::create(0.3f, m_vBottomOrginalPos)));
    }
}

void GameUILayer::reponseTouchResentlyColor(Node* pSender)
{
    m_pColorScrollLayer->deleteCurrentColor();
    
    ColorSprite* lSprite = (ColorSprite*)pSender;
    DataManager::getInstance()->m_cSelectedColor = lSprite->getColorData();
    
//    for (int i = 0; i < m_pRensentlyColor.size(); ++i)
//    {
//        if (m_pRensentlyColor.at(i)->getChildByTag(st_gameUILayer_pen))
//        {
//            m_pRensentlyColor.at(i)->getChildByTag(st_gameUILayer_pen)->setVisible(false);
//        }
//    }
    DataManager::getInstance()->m_iSelectedColorIndex = lSprite->getTag();
    lSprite->getChildByTag(st_gameUILayer_pen)->setVisible(true);
}

void GameUILayer::updateResentlyColor()
{
//    for (int i = 0; i < 3; i++)
//        {
//            ColorData colorData = DataManager::getInstance()->m_vRecentUseColor.at(i);
//            m_pRensentlyColor.at(i)->setColorData(colorData);
//            m_pRensentlyColor.at(i)->setColor(Color3B(colorData.R, colorData.G, colorData.B));
//        }
//    
//    for (int i = 0; i < m_pRensentlyColor.size(); ++i)
//    {
//        if (m_pRensentlyColor.at(i)->getChildByTag(st_gameUILayer_pen))
//        {
//            m_pRensentlyColor.at(i)->getChildByTag(st_gameUILayer_pen)->setVisible(false);
//        }
//    }
//    
//    m_pRensentlyColor.at(0)->getChildByTag(st_gameUILayer_pen)->setVisible(true);
    
}

void GameUILayer::doAction(Node* pSender)
{
    UIButton* button = (UIButton*)pSender;
    switch (button->getTag())
    {
        case st_gameUILayer_button_back:
        {
            DataManager::m_bIsClickGameSceneBack = true;
            m_bIsShowDialog = false;
            
            if (DataManager::getInstance()->m_bIsAddMyAtrworks)
            {
                DataManager::getInstance()->m_bIsAddMyAtrworks = false;
                
                if (DataManager::m_iCurrentClickNumber > 10 || DataManager::m_iCurrentClickNumber > DataManager::m_iCurrentCancelNumber)
                {
                    DataManager::IsMyArtworksOffsety = false;
                    DM_getInstance->addImageToArtworks(DM_getInstance->m_pCurrentImage.imageName, DM_getInstance->m_pCurrentImage.ID);
                }
                else if (m_bIsReset)
                {
                    m_bIsShowDialog = true;
                }
                
            }
            
            if (DataManager::getInstance()->m_bIsAddMyAtrworks == false && DataManager::m_bIsMyArtworksInGameScene && m_bIsReset)
            {
                m_bIsShowDialog = true;
            }
            
//            button->setButtonEnable(false);
            m_pEditUIB->setButtonEnable(false);
            m_pBackUIB->setButtonEnable(false);
            
            DataManager::isTableOffset = true;
            
            //为了返回上个界面时，回到选择时的偏移量
            GameScene* lGameScene = dynamic_cast<GameScene*>(this->getParent());
            int index = lGameScene->getIndex();
            std::string gallery = lGameScene->getGalleryName();
            
            lGameScene->getGameLayer()->saveImage();
            
            
            if (DataManager::getInstance()->m_bCurrentImageIsUsed)
            {
                DataManager::getInstance()->m_bCurrentImageIsUsed = false;
                
                for (int i = 0; i < DM_FlurryDatas.size(); ++i)
                {
                    FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                    stringstream ss;
                    ss<<"";
                    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    ss<<"Picture_";
                    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    ss<<"Image_";
                    
#endif
                    ss<<DataManager::getInstance()->m_pCurrentImage.imageName;
                    ss<<DataManager::getInstance()->m_pCurrentImage.ID;
                    
                    if (strcmp(lFlurryInfo->message.c_str(), ss.str().c_str()) == 0)
                    {
                        lFlurryInfo->number++;
                    }
                }
            }
            
            if (m_bIsShowDialog && DataManager::m_bIsMyArtworksInGameScene)
            {
                STAlertDialog* dialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["ResetBack"], DM_getInstance->getLanguageMes()["Exit"], DM_getInstance->getLanguageMes()["Cancel"]);
                dialog->setLeftListener(this, callfuncN_selector(GameUILayer::returnPreScene));
                dialog->setRightListener(this, callfuncN_selector(GameUILayer::removeDialog));
                this->addChild(dialog, 100);
                return;
            }
            
    
            if (DataManager::m_bIsMyArtworksInGameScene && !DM_getInstance->m_vMyArtWorksImageName.empty())
            {
                Scene* scene = MyArtworksScene::scene();
                Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
            }else if (DataManager::m_bIsMyArtworksInGameScene && DM_getInstance->m_vMyArtWorksImageName.empty())
            {
                Scene* scene = ChooseGalleryScene::scene();
                Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
            }
            else
            {
                Scene* scene = ChooseMapScene::scene(index, gallery);
                Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
            }

        }
            break;
        case st_gameUILayer_button_edit:
        {
            DataManager::m_bIsClickGameSceneDone = true;
            
//            if (DataManager::getInstance()->m_bIsAddMyAtrworks)
//            {
//                DataManager::getInstance()->m_bIsAddMyAtrworks = false;
//            
//                if (DataManager::m_iCurrentClickNumber > 10 || DataManager::m_iCurrentClickNumber > DataManager::m_iCurrentCancelNumber)
                {
                    DataManager::IsMyArtworksOffsety = false;
                    DataManager::IsMyArtworksOffsety = false;
                    DM_getInstance->addImageToArtworks(DM_getInstance->m_pCurrentImage.imageName, DM_getInstance->m_pCurrentImage.ID);
                }
                
//            }
            
//            button->setButtonEnable(false);
            m_pEditUIB->setButtonEnable(false);
            m_pBackUIB->setButtonEnable(false);
            
            if (DataManager::getInstance()->m_bCurrentImageIsUsed)
            {
                DataManager::getInstance()->m_bCurrentImageIsUsed = false;
                
                for (int i = 0; i < DM_FlurryDatas.size(); ++i)
                {
                    FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                    stringstream ss;
                    ss<<"";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    ss<<"Picture_";
                    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    ss<<"Image_";
                    
#endif
                    ss<<DataManager::getInstance()->m_pCurrentImage.imageName;
                    ss<<DataManager::getInstance()->m_pCurrentImage.ID;
                    
                    if (strcmp(lFlurryInfo->message.c_str(), ss.str().c_str()) == 0)
                    {
                        lFlurryInfo->number++;
                    }
                }
            }
            
            
            GameScene* lGameScene = dynamic_cast<GameScene*>(this->getParent());
            lGameScene->getGameLayer()->saveImage();
            //lGameScene->getGameLayer()->saveEditImage();
            Sprite* lSprite = lGameScene->getGameLayer()->getSpriteDrawScene();
            
            
            Scene* scene = EditScene::scene(lSprite);
            lSprite->release();
//            Director::getInstance()->replaceScene(scene);
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.35f, scene));
            
        }
            break;
        case st_gameUILayer_button_reset:
        {
            GameScene* scene = dynamic_cast<GameScene*>(this->getParent());
            scene->getGameLayer()->enableFillNode(false);
            
            STAlertDialog* lDialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["ResetImage"], DM_getInstance->getLanguageMes()["Later"], DM_getInstance->getLanguageMes()["Reset"]);
            lDialog->setLeftListener(this, callfuncN_selector(GameUILayer::removeDialog));
            lDialog->setRightListener(this, callfuncN_selector(GameUILayer::resetGame));
            lDialog->setTag(st_dialog_tag);
            this->addChild(lDialog);
        }
            break;
        case st_gameUILayer_button_undo:
        {
            GameScene* lGameScene = dynamic_cast<GameScene*>(this->getParent());
            lGameScene->getGameLayer()->undo();
        }
            break;
        default:
            break;
    }
    
}

void GameUILayer::returnPreScene(Node* ref)
{
    if (DM_getInstance->m_vMyArtWorksImageName.empty())
    {
        Scene* scene = ChooseGalleryScene::scene();
        Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
    }else
    {
        Scene* scene = MyArtworksScene::scene();
        Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
    }
}

void GameUILayer::waitDownLoadSucNotification(Ref* ref)
{
    if (this->getChildByTag(st_dialog_tag))
    {
        this->getChildByTag(st_dialog_tag)->removeFromParentAndCleanup(true);
    }
}

void GameUILayer::removeTutorial()
{
    if (m_pTutorialLayer)
    {
        m_pTutorialLayer->removeFromParentAndCleanup(true);
        m_pTutorialLayer = nullptr;
    }
    
    auto dispacher = Director::getInstance()->getEventDispatcher();
    dispacher->removeEventListenersForTarget(this);
}

void GameUILayer::removeDialog(Node* ref)
{
    m_pBackUIB->setButtonEnable(true);
    ref->removeFromParentAndCleanup(true);
    
    CallFunc* call = CallFunc::create(CC_CALLBACK_0(GameUILayer::enableFillNode, this));
    this->runAction(call);
}

void GameUILayer::enableFillNode()
{
    GameScene* lGameScene = dynamic_cast<GameScene*>(this->getParent());
    lGameScene->getGameLayer()->enableFillNode(true);
}

void GameUILayer::resetGame(Node* ref)
{
    ref->removeFromParentAndCleanup(true);
    
    this->scheduleOnce(schedule_selector(GameUILayer::reset), 0.0f);
}

void GameUILayer::reset(float dt)
{
    m_bIsReset = true;
    DM_getInstance->deleteArtworksImage(DM_getInstance->m_pCurrentImage.imageName, DM_getInstance->m_pCurrentImage.ID);
    
    GameScene* lGameScene = dynamic_cast<GameScene*>(this->getParent());
    lGameScene->getGameLayer()->reset();
    
    lGameScene->getGameLayer()->enableFillNode(true);
}

void GameUILayer::undoEndPrompt()
{
    Sprite* lSprite = Sprite::create("popbg.png");
    ScreenAdapterHelper::setPosition(lSprite, Vec2(384, 620));
    this->addChild(lSprite);
    
    Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["Undo"], st_font_AR, ScreenAdapterHelper::getValue(30));
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
    lSprite->addChild(label);
    
    DelayTime*  lDelayTime = DelayTime::create(1.0f);
    Sequence*   lSeq = Sequence::create(lDelayTime, Hide::create(), NULL);
    label->runAction(lSeq);
    
    DelayTime*  lDelayTime2 = DelayTime::create(1.0f);
    FadeOut*    lFadeOut = FadeOut::create(0.5f);
    CallFuncN*  lCallFun = CallFuncN::create(CC_CALLBACK_1(GameUILayer::removeDialog, this));
    Sequence*   lSeq2 = Sequence::create(lDelayTime2, lFadeOut, lCallFun, NULL);
    
    lSprite->runAction(lSeq2);
}

void GameUILayer::gotoEditScene()
{

}
