//
//  MyArtworksScene.cpp
//  IMG1001
//
//  Created by houhuang on 15/11/13.
//
//

#include "MyArtworksScene.hpp"
#include "../helper/ScreenAdapterHelper.h"
#include "../layer/ColorScrollLayer/ColorScrollLayerMask.h"
//#include "../IOS_Android_Include/AdsManager.h"
#include "../crossplatformapi/headers/ads/AdsManager.h"
#include "../scene/ChooseGalleryScene.h"
#include "../layer/MyArtworksLayer.hpp"

enum
{
    st_MyArtworksLayer_tag = 22,
};

MyArtworksScene::MyArtworksScene()
{
    m_pBackUIbtn = NULL;
    m_pEditLabel = NULL;
    
    m_bIsEditButton = true;
}

bool MyArtworksScene::init()
{
    if (!BaseScene::init()) return false;

    Sprite* bg = Sprite::create("bg.png");
    bg->setAnchorPoint(Vec2(0.5, 1));
    bg->setScale(ScreenAdapterHelper::getRealDesignScale() * 1.05);
    ScreenAdapterHelper::setPosition(bg, Vec2(384, -( ScreenAdapterHelper::getRealDesignScale() - 1) * 100 * 1.05), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(bg);
    
    
    MyArtworksLayer* layer = MyArtworksLayer::create();
    ScreenAdapterHelper::setPosition(layer, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(layer);
    layer->setTag(st_MyArtworksLayer_tag);
    
    //屏蔽menu栏scrollview事件
    ColorScrollLayerMask* topMask = ColorScrollLayerMask::create(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(110)));
    topMask->setAnchorPoint(Vec2(0.5, 0));
    ScreenAdapterHelper::setPosition(topMask, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(topMask);
    
    Label* title = Label::createWithTTF(DM_getInstance->getLanguageMes()["MyArtworks"], st_font_AR, ScreenAdapterHelper::getValue(50));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    ScreenAdapterHelper::setPosition(title, Vec2(384,50),KVBoarderTypeTop,KHBoarderTypeNone);
    title->setColor(Color3B(0, 0, 0));
    this->addChild(title);
    
    
    Sprite* logo_line = Sprite::create("seperator-2.png");
    ScreenAdapterHelper::setPosition(logo_line, Vec2(384, 110),KVBoarderTypeTop,KHBoarderTypeNone);
    this->addChild(logo_line);
    
    
    m_pBackUIbtn = UIButton::create("btn_back.png");
    m_pBackUIbtn->setAnchorPoint(Vec2(0, 0.5));
    ScreenAdapterHelper::setPosition(m_pBackUIbtn, Vec2(60, 55), KVBoarderTypeTop, KHBoarderTypeLeft);
    m_pBackUIbtn->initListener(this, callfuncN_selector(MyArtworksScene::doAction));
    this->addChild(m_pBackUIbtn);
    
    MenuItemImage* edit = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(MyArtworksScene::reponseLabel, this));
    edit->ignoreAnchorPointForPosition(false);
    edit->setAnchorPoint(Vec2(1, 0.5));
    ScreenAdapterHelper::setPosition(edit, Vec2(40, 55), KVBoarderTypeTop, kHBoarderTypeRight);
    
    Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["Edit"], st_font_AR, ScreenAdapterHelper::getValue(35));
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(edit->getContentSize().width/2, edit->getContentSize().height/2));
    edit->addChild(label);
    m_pEditLabel = label;
    
    Menu* lMenu = Menu::create(edit, NULL);
    lMenu->setPosition(Vec2::ZERO);
    this->addChild(lMenu);
    
    DataManager::m_bIsMyArtworksInGameScene = true;
    
#ifndef DEBUG_ADS
    AdsManager::getInstance()->showAds(kTypeBannerAds);
#endif
    
    return true;
}

void MyArtworksScene::onEnterTransitionDidFinish()
{

}

void MyArtworksScene::reponseLabel(Ref* ref)
{
    MyArtworksLayer* layer = dynamic_cast<MyArtworksLayer*>(this->getChildByTag(st_MyArtworksLayer_tag));
    if (m_bIsEditButton)
    {
        m_bIsEditButton = false;
        m_pEditLabel->setString(DM_getInstance->getLanguageMes()["Done"]);

        layer->showMapDeleteBtn();
        m_pBackUIbtn->setButtonEnable(false);
        m_pBackUIbtn->setVisible(false);
        
    }else
    {
        m_bIsEditButton = true;
        m_pEditLabel->setString(DM_getInstance->getLanguageMes()["Edit"]);

        layer->hideMapDeleteBtn();
        m_pBackUIbtn->setButtonEnable(true);
        m_pBackUIbtn->setVisible(true);
    }
}

void MyArtworksScene::doAction(Node* node)
{
    UIButton* lUIButton = (UIButton*)node;
    lUIButton->setButtonEnable(false);
    
    MyArtworksLayer* layer = dynamic_cast<MyArtworksLayer*>(this->getChildByTag(st_MyArtworksLayer_tag));
    layer->setScrollViewOffset();
    
    Scene* scene = ChooseGalleryScene::scene();
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
}
