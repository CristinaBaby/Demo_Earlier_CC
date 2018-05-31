//
//  FavoriteScene.cpp
//  IMG1001
//
//  Created by houhuang on 15/12/2.
//
//

#include "FavoriteScene.hpp"
#include "../helper/ScreenAdapterHelper.h"
#include "ChooseGalleryScene.h"
#include "../layer/MyPostsLayer.hpp"
#include "../crossplatformapi/headers/ads/AdsManager.h"
#include "../layer/FavoriteLayer.hpp"
#include "../DownLoad/DownLoadPage.h"

enum
{
    st_Favorite_btn_edit_tag = 11,
    st_Favorite_btn_favorite_tag,
    st_Favorite_btn_post_tag,
    st_Favorite_postLayer_tag,
};

FavoriteScene::~FavoriteScene()
{

}

FavoriteScene::FavoriteScene()
{
    m_pTitleLabel = NULL;
    m_pBackUIbtn = NULL;
    m_bIsEditButton = true;
}

bool FavoriteScene::init()
{
    if (!BaseScene::init()) return false;
    
    AdsManager::getInstance()->removeAds(kTypeBannerAds);
    
    Sprite* bg = Sprite::create("bg.png");
    bg->setAnchorPoint(Vec2(0.5, 1));
    bg->setScale(ScreenAdapterHelper::getRealDesignScale() * 1.05);
    ScreenAdapterHelper::setPosition(bg, Vec2(384, -( ScreenAdapterHelper::getRealDesignScale() - 1) * 100 * 1.05), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(bg);
    
    //屏蔽menu栏scrollview事件
    ColorScrollLayerMask* topMask = ColorScrollLayerMask::create(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(110)));
    topMask->setAnchorPoint(Vec2(0.5, 0));
    ScreenAdapterHelper::setPosition(topMask, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(topMask, 1);
    
    Label* title = Label::createWithTTF("", st_font_AR, ScreenAdapterHelper::getValue(50));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    ScreenAdapterHelper::setPosition(title, Vec2(384,50),KVBoarderTypeTop,KHBoarderTypeNone);
    title->setColor(Color3B(236, 64, 122));
    this->addChild(title, 2);
    m_pTitleLabel = title;
    
    Sprite* logo_line = Sprite::create("seperator-2.png");
    ScreenAdapterHelper::setPosition(logo_line, Vec2(384, 110),KVBoarderTypeTop,KHBoarderTypeNone);
    this->addChild(logo_line, 2);
    
    m_pBackUIbtn = UIButton::create("btn_back.png");
    m_pBackUIbtn->setAnchorPoint(Vec2(0, 0.5));
    ScreenAdapterHelper::setPosition(m_pBackUIbtn, Vec2(60, 55), KVBoarderTypeTop, KHBoarderTypeLeft);
    m_pBackUIbtn->initListener(this, callfuncN_selector(FavoriteScene::gotoScene));
    this->addChild(m_pBackUIbtn, 2);
    
    MenuItemImage* edit = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(FavoriteScene::doAction, this));
    edit->ignoreAnchorPointForPosition(false);
    edit->setAnchorPoint(Vec2(1, 0.5));
    ScreenAdapterHelper::setPosition(edit, Vec2(40, 55), KVBoarderTypeTop, kHBoarderTypeRight);
    edit->setTag(st_Favorite_btn_edit_tag);
    
    Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["Edit"], st_font_AR, ScreenAdapterHelper::getValue(35));
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(edit->getContentSize().width/2, edit->getContentSize().height/2));
    edit->addChild(label);
    m_pEditLabel = label;
    
    Menu* lMenu = Menu::create(edit, NULL);
    lMenu->setPosition(Vec2::ZERO);
    this->addChild(lMenu, 2);
    
    
    
    
    ColorScrollLayerMask* btn_bg = ColorScrollLayerMask::create(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(80)));
    //    LayerColor* btn_bg = LayerColor::create(Color4B::WHITE, ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(80));
    btn_bg->ignoreAnchorPointForPosition(false);
    btn_bg->setAnchorPoint(Vec2(0.5, 0));
    ScreenAdapterHelper::setPosition(btn_bg, Vec2(384, 0) , KVBoarderTypeBottom, KHBoarderTypeNone);
    this->addChild(btn_bg, 100);
    m_pBtnBG = btn_bg;
    
    Sprite* vLine = Sprite::create("pop_seperator2.png");
    vLine->setPosition(Vec2(btn_bg->getContentSize().width/2, btn_bg->getContentSize().height/2));
    btn_bg->addChild(vLine);
    
    MenuItemImage* favoriteBtn = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(FavoriteScene::doAction, this));
    favoriteBtn->setScale(1.5f);
    favoriteBtn->setPosition(Vec2(btn_bg->getContentSize().width/4, btn_bg->getContentSize().height/2));
    favoriteBtn->setTag(st_Favorite_btn_favorite_tag);
    
    Label* favoriteLabel = Label::createWithTTF(DM_getInstance->getLanguageMes()["Favorites"], st_font_AR, ScreenAdapterHelper::getValue(30));
    favoriteLabel->setPosition(Vec2(favoriteBtn->getContentSize().width/2, favoriteBtn->getContentSize().height/2));
    favoriteBtn->addChild(favoriteLabel);
    favoriteLabel->setColor(Color3B::BLACK);
    m_pFavoriteBtnLabel = favoriteLabel;
    
    
    MenuItemImage* postBtn = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(FavoriteScene::doAction, this));
    postBtn->setScale(1.5f);
    postBtn->setPosition(Vec2(btn_bg->getContentSize().width * 0.75, btn_bg->getContentSize().height/2));
    postBtn->setTag(st_Favorite_btn_post_tag);
    
    Label* postLabel = Label::createWithTTF(DM_getInstance->getLanguageMes()["Posts"], st_font_AR, ScreenAdapterHelper::getValue(30));
    postLabel->setPosition(Vec2(postBtn->getContentSize().width/2, postBtn->getContentSize().height/2));
    postBtn->addChild(postLabel);
    postLabel->setColor(Color3B::BLACK);
    m_pPostBtnLabel = postLabel;
    
    
    Menu* menu = Menu::create(favoriteBtn, postBtn, NULL);
    menu->setPosition(Vec2::ZERO);
    btn_bg->addChild(menu);
    m_pMenu = menu;
    
    return true;
}

void FavoriteScene::onEnterTransitionDidFinish()
{
    BaseScene::onEnterTransitionDidFinish();
    
    if (DataManager::m_bIsFavoriteLayer)
    {
        this->createFavoritelayer();
    }else
    {
        this->createPostLayer();
    }
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void FavoriteScene::gotoScene(Node* node)
{
    UIButton* button = dynamic_cast<UIButton*>(node);
    button->setButtonEnable(false);
    
    Scene* scene = ChooseGalleryScene::scene();
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
    
}

void FavoriteScene::doAction(Ref* ref)
{
    MenuItemImage* lMenu = dynamic_cast<MenuItemImage*>(ref);
    
//    auto layer;
    if (DataManager::m_bIsFavoriteLayer)
    {
        FavoriteLayer* layer = dynamic_cast<FavoriteLayer*>(this->getChildByTag(st_Favorite_postLayer_tag));
    }else
    {
        MyPostsLayer* layer = dynamic_cast<MyPostsLayer*>(this->getChildByTag(st_Favorite_postLayer_tag));
    }
    
    
    switch (lMenu->getTag())
    {
        case st_Favorite_btn_edit_tag:
        {
            if (m_bIsEditButton)
            {
                m_pBtnBG->setVisible(false);
                m_bIsEditButton = false;
                m_pEditLabel->setString(DM_getInstance->getLanguageMes()["Done"]);
                m_pBackUIbtn->setButtonEnable(false);
                m_pBackUIbtn->setVisible(false);
                if (DataManager::m_bIsFavoriteLayer)
                {
                    FavoriteLayer* layer = dynamic_cast<FavoriteLayer*>(this->getChildByTag(st_Favorite_postLayer_tag));
                    layer->showMapDeleteBtn();
                }else
                {
                    MyPostsLayer* layer = dynamic_cast<MyPostsLayer*>(this->getChildByTag(st_Favorite_postLayer_tag));
                    layer->showMapDeleteBtn();
                }
   
            }else
            {
                m_pBtnBG->setVisible(true);
                m_bIsEditButton = true;
                m_pEditLabel->setString(DM_getInstance->getLanguageMes()["Edit"]);
                m_pBackUIbtn->setButtonEnable(true);
                m_pBackUIbtn->setVisible(true);
                if (DataManager::m_bIsFavoriteLayer)
                {
                    FavoriteLayer* layer = dynamic_cast<FavoriteLayer*>(this->getChildByTag(st_Favorite_postLayer_tag));
                    layer->hideMapDeleteBtn();
                }else
                {
                    MyPostsLayer* layer = dynamic_cast<MyPostsLayer*>(this->getChildByTag(st_Favorite_postLayer_tag));
                    layer->hideMapDeleteBtn();
                }
            }
            
        }
            break;
            
        case st_Favorite_btn_favorite_tag:
        {
            if (m_bIsEditButton)
            {
                if (!DataManager::m_bIsFavoriteLayer)
                {
                    DataManager::m_bIsFavoriteLayer = true;
                    
                    this->destoryPostLayer();
                    this->createFavoritelayer();
                    FavoriteLayer* layer = dynamic_cast<FavoriteLayer*>(this->getChildByTag(st_Favorite_postLayer_tag));
                    if (m_bIsEditButton)
                    {
                        layer->hideMapDeleteBtn();
                    }else
                    {
                        layer->showMapDeleteBtn();
                    }
                }
            }
  
        }
            break;
            
        case st_Favorite_btn_post_tag:
        {

            if (m_bIsEditButton)
            {
                if (DataManager::m_bIsFavoriteLayer)
                {
                    DataManager::m_bIsFavoriteLayer = false;
                    
                    this->destoryFavoriteLayer();
                    this->createPostLayer();
                    MyPostsLayer* layer = dynamic_cast<MyPostsLayer*>(this->getChildByTag(st_Favorite_postLayer_tag));
                    if (m_bIsEditButton)
                    {
                        if (layer)
                        {
                            layer->hideMapDeleteBtn();
                        }
                    }else
                    {
                        if (layer)
                        {
                            layer->showMapDeleteBtn();
                        }
                    }
                }
            }
            
        }
            break;
            
        default:
            break;
    }
}

void FavoriteScene::createFavoritelayer()
{
    FavoriteLayer* layer = FavoriteLayer::create();
    ScreenAdapterHelper::setPosition(layer, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(layer);
    layer->setTag(st_Favorite_postLayer_tag);
    
    m_pTitleLabel->setString(DM_getInstance->getLanguageMes()["MyFavorites"]);
    m_pFavoriteBtnLabel->setColor(Color3B(236, 64, 122));
}

void FavoriteScene::destoryFavoriteLayer()
{
    FavoriteLayer* layer = dynamic_cast<FavoriteLayer*>(this->getChildByTag(st_Favorite_postLayer_tag));
    if (layer)
    {
        layer->removeFromParentAndCleanup(true);
    }
    
    m_pFavoriteBtnLabel->setColor(Color3B::BLACK);
}

void FavoriteScene::createPostLayer()
{
    MyPostsLayer* layer = MyPostsLayer::create();
    ScreenAdapterHelper::setPosition(layer, Vec2(384, 110), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(layer);
    layer->setTag(st_Favorite_postLayer_tag);
    
    m_pTitleLabel->setString(DM_getInstance->getLanguageMes()["MyPost"]);
    m_pPostBtnLabel->setColor(Color3B(236, 64, 122));
}

void FavoriteScene::destoryPostLayer()
{
    MyPostsLayer* layer = dynamic_cast<MyPostsLayer*>(this->getChildByTag(st_Favorite_postLayer_tag));
    if (layer)
    {
        layer->removeFromParentAndCleanup(true);
    }
    m_pPostBtnLabel->setColor(Color3B::BLACK);
}

void FavoriteScene::updateScene()
{

}