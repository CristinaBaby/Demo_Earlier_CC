//
//  EditScene.cpp
//  ColorFree
//
//  Created by houhuang on 15/9/7.
//
//

#include "EditScene.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../define/Globalmacro.h"
#include "../data/DataManager.h"
#include "../scene/GameScene.h"
#include "../scene/ShareScene.h"
//#include "../IOS_Android_Include/AdsManager.h"
#include "../crossplatformapi/headers/ads/AdsManager.h"
#include "../define/AppConfigs.h"


enum
{
    st_edit_menu_back = 10,
    st_edit_menu_share,
    st_edit_menu_filters,
    st_edit_menu_frames,
    st_edit_menu_text,
    
    st_editLayer_tag = 50,
};

Scene* EditScene::scene(Sprite* pSprite)
{
    Scene* scene = Scene::create();
    EditScene* layer = new EditScene();
    if (layer && layer->init(pSprite))
    {
        layer->autorelease();
        scene->addChild(layer);
        return scene;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

EditScene::~EditScene()
{
    
}

EditScene::EditScene()
{

    m_pShareBtn = NULL;
    m_pBackBtn = NULL;
 
}

void EditScene::onEnter()
{
    BaseScene::onEnter();
    
    EditLayer* lEditLayer = this->getEditLayer();
    lEditLayer->updateImageFilter();
    
}

bool EditScene::init(Sprite* pSprite)
{
    if (!BaseScene::init()) return false;
    
    Sprite* topLine = Sprite::create("seperator-2.png");
    ScreenAdapterHelper::setPosition(topLine, Vec2(384, 100),KVBoarderTypeTop,KHBoarderTypeNone);
    this->addChild(topLine);
    
    Sprite* bottomLine = Sprite::create("seperator-2.png");
    ScreenAdapterHelper::setPosition(bottomLine, Vec2(384, 250),KVBoarderTypeBottom,KHBoarderTypeNone);
    this->addChild(bottomLine);

    EditLayer* lEditLayer = EditLayer::create(pSprite);
    lEditLayer->setTag(st_editLayer_tag);
    this->addChild(lEditLayer);
    
    
    this->initMenu();
    
#ifndef DEBUG_ADS
    AdsManager::getInstance()->removeAds(kTypeBannerAds);
#endif
    
    return true;
}

void EditScene::initMenu()
{
    UIButton* back = UIButton::create("btn_back.png");
    ScreenAdapterHelper::setPosition(back, Vec2(70, 50), KVBoarderTypeTop, KHBoarderTypeLeft);
    back->initListener(this, callfuncN_selector(EditScene::doAction));
    this->addChild(back);
    back->setTag(st_edit_menu_back);
    m_pBackBtn = back;
    
    UIButton* share = UIButton::create("btn_done.png");
    ScreenAdapterHelper::setPosition(share, Vec2(70, 50), KVBoarderTypeTop, kHBoarderTypeRight);
    share->initListener(this, callfuncN_selector(EditScene::doAction));
    this->addChild(share);
    share->setTag(st_edit_menu_share);
    m_pShareBtn = share;
  
}

void EditScene::doAction(Node* pNode)
{
    EditLayer* lEditLayer = this->getEditLayer();
    UIButton* button = (UIButton*)pNode;
    switch (button->getTag())
    {
        case st_edit_menu_back:
        {
//            button->setButtonEnable(false);
            m_pBackBtn->setButtonEnable(false);
            m_pShareBtn->setButtonEnable(false);
            lEditLayer->hideSignatureLayer();
            
            Scene* scene = GameScene::create();
            Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
        }
            break;
            
        case st_edit_menu_share:
        {
//            button->setButtonEnable(false);
            m_pBackBtn->setButtonEnable(false);
            m_pShareBtn->setButtonEnable(false);
            
            this->scheduleOnce(schedule_selector(EditScene::resumeShareButtonEnable), 0.5f);
            
            
            lEditLayer->sendFlurryDatas();
            lEditLayer->hideSignatureLayer();
            
            Sprite* lSprite = lEditLayer->getShareSprite();
            
            Scene* scene = ShareScene::scene(lSprite);
//            Director::getInstance()->pushScene(scene);
            Director::getInstance()->pushScene(TransitionSlideInR::create(0.35f, scene));
        }
            break;
            
        default:
            break;
    }
}

void EditScene::resumeShareButtonEnable(float dt)
{
    if (m_pShareBtn)
    {
        m_pShareBtn->setButtonEnable(true);
    }
    
    if (m_pBackBtn)
    {
        m_pBackBtn->setButtonEnable(true);
    }
}




EditLayer* EditScene::getEditLayer()
{
    EditLayer* layer = dynamic_cast<EditLayer*>(this->getChildByTag(st_editLayer_tag));
    return layer;
}


