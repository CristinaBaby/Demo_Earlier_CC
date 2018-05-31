//
//  FavoriteLayer.cpp
//  IMG1001
//
//  Created by houhuang on 15/12/10.
//
//

#include "FavoriteLayer.hpp"
#include "../helper/ScreenAdapterHelper.h"
#include "../define/Globalmacro.h"
#include "../layer/Dialog.h"
#include "../scene/GameScene.h"
#include "../data/DataManager.h"
#include "../scene/ChooseGalleryScene.h"
#include "../DownLoad/DownLoadPage.h"

enum
{
    st_MyArtworks_deleteBtn = 15,
};


FavoriteLayer::~FavoriteLayer()
{
    this->setScrollViewOffset();
    DownLoadPage::getInstance()->m_vNeedGetLikeImage.clear();
}

FavoriteLayer::FavoriteLayer()
{
    m_iImageNumber = (int)DM_getInstance->m_vMyFavoriteDatas.size();
    
    m_pTableView = NULL;
    m_bIsEdit = true;
    m_pClickDeleteSprite = NULL;
    
    
}

bool FavoriteLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) return false;
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 1));
    this->setContentSize(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(1024)));
    
    DownLoadPage::getInstance()->m_vFavoriteInfo.clear();
    
    if (m_iImageNumber == 0)
    {
        Sprite* lSprite = Sprite::create("pop3.png");
        lSprite->setPosition(Vec2(this->getContentSize().width/2, ScreenAdapterHelper::getValue(512)/ScreenAdapterHelper::getRealDesignScale() + ScreenAdapterHelper::getValue(100)));
        this->addChild(lSprite);
 
        Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["FavoriteEmpty"], st_font_AR, ScreenAdapterHelper::getValue(35), Size(lSprite->getContentSize().width * 0.92, lSprite->getContentSize().height), TextHAlignment::LEFT, TextVAlignment::CENTER);
        label->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
        lSprite->addChild(label);
        label->setColor(Color3B::BLACK);
        
    }
    
    ScrollView* scrollView = ScrollView::create(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(950 * ScreenAdapterHelper::getRealDesignScale())));
    
    
    scrollView->setContentSize(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(680) * m_iImageNumber));
    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    scrollView->ignoreAnchorPointForPosition(false);
    scrollView->setAnchorPoint(Vec2(0.5, 1));
    scrollView->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    
//    scrollView->isDragging();
    
    this->addChild(scrollView);
    m_pScrollView = scrollView;
    
    if (DataManager::favoriteOffset > 0)
    {
        DataManager::favoriteOffset = scrollView->minContainerOffset().y;
    }
    
    this->initContentLayer();
    m_pScrollView->setDelegate(this);

    
    return true;
}

void FavoriteLayer::onExit()
{
    LayerColor::onExit();
    DataManager::getInstance()->m_vNeedLoadImage.clear();
    DataManager::getInstance()->m_bIsLoadFinish = false;
}

void FavoriteLayer::initContentLayer()
{
    LayerColor* lContent = LayerColor::create(Color4B(0, 0, 0, 0));
    lContent->setContentSize(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(680) * m_iImageNumber + ScreenAdapterHelper::getValue(200)));
    m_pScrollView->setContainer(lContent);
    
    m_vMyArtworksSprite.clear();
    
    for (int i = 0; i < m_iImageNumber; ++i)
    {
//        FavoriteSprite* lSprite = FavoriteSprite::create(DM_getInstance->m_vMyFavoriteDatas.at(m_iImageNumber - i - 1), DM_getInstance->m_vMyFavoriteLikeNum.at(m_iImageNumber - i - 1));
        FavoriteSprite* lSprite = FavoriteSprite::create(DM_getInstance->m_vMyFavoriteDatas.at(m_iImageNumber - i - 1), 0);
        lSprite->setPosition(Vec2(lContent->getContentSize().width/2, lContent->getContentSize().height - ScreenAdapterHelper::getValue(680)/2 - i*ScreenAdapterHelper::getValue(680) ));
        lContent->addChild(lSprite);
        
        if (abs(DataManager::favoriteOffset + lSprite->getPosition().y) < ScreenAdapterHelper::getValue(3000))
        {
            lSprite->display();
        }
        lSprite->setIndex(i);
        //        lSprite->display();
        
//        log("%f ",DataManager::favoriteOffset + lSprite->getPosition().y);

        
        MenuItemImage* deleteBtn = MenuItemImage::create("delete_artworks.png", "delete_artworks.png", CC_CALLBACK_1(FavoriteLayer::doAction, this));
        deleteBtn->setPosition(Vec2(lSprite->getContentSize().width - ScreenAdapterHelper::getValue(20), lSprite->getContentSize().height - ScreenAdapterHelper::getValue(20)));
        
        Menu* lMenu = Menu::create(deleteBtn, NULL);
        lMenu->setPosition(Vec2::ZERO);
        lSprite->addChild(lMenu);
        lMenu->setTag(st_MyArtworks_deleteBtn);
        lMenu->setVisible(false);
        
        m_vMyArtworksSprite.push_back(lSprite);
//        log("%f",lSprite->getPosition().y);
    }
    
    if (m_iImageNumber <= 2)
    {
        m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
        return;
    }
    
    if (DataManager::isFavoriteOffset)
    {
        m_pScrollView->setContentOffset(Vec2(m_pScrollView->getContentOffset().x, DataManager::favoriteOffset));
        
    }else
    {
        m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
    }

//    log("scrollView content offsetY = %f",m_pScrollView->getContentOffset().y);
}

void FavoriteLayer::scrollViewDidScroll(ScrollView* view)
{
    Vec2 offset = m_pScrollView->getContentOffset();
    
    for (auto lSprite : m_vMyArtworksSprite)
    {
        float endX = lSprite->getPosition().y + offset.y;
        
        if (endX > ScreenAdapterHelper::getValue(-600) * 10 && endX < ScreenAdapterHelper::getValue(600) * 10)
        {
            lSprite->display();
        }else
        {
            lSprite->hideThis();
        }
    }
    
//    if (m_pScrollView->getContentOffset().y < m_pScrollView->minContainerOffset().y)
//    {
//        m_pScrollView->setContentOffset(m_pScrollView->maxContainerOffset());
//    }
}

void FavoriteLayer::scrollViewDidZoom(ScrollView* view)
{
    
}

void FavoriteLayer::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();;
}


void FavoriteLayer::doAction(Ref* ref)
{
    MenuItemImage* lMenuItemImage = dynamic_cast<MenuItemImage*>(ref);
    m_pClickDeleteSprite = dynamic_cast<FavoriteSprite*>(lMenuItemImage->getParent()->getParent());
    
    STAlertDialog* lDialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["SureDelete"], DM_getInstance->getLanguageMes()["Later"], DM_getInstance->getLanguageMes()["Delete"]);
    lDialog->setLeftListener(this, callfuncN_selector(FavoriteLayer::removeDialog));
    lDialog->setRightListener(this, callfuncN_selector(FavoriteLayer::deleteMapImage));
    //    ScreenAdapterHelper::setPosition(lDialog, Vec2(384, 512));
    this->getParent()->addChild(lDialog, 100);
}

void FavoriteLayer::hideMapDeleteBtn()
{
    
    m_bIsEdit = true;
    //    float offset = m_pTableView->getContentOffset().x;
    DataManager::m_bIsShowMyArtworksDeleteBtn = false;
    //    m_pTableView->reloadData();
    //    m_pTableView->setContentOffset(Vec2(m_pTableView->getContentOffset().x, offset));
    for (int i=0; i<m_vMyArtworksSprite.size(); i++)
    {
        FavoriteSprite* lSprite = dynamic_cast<FavoriteSprite*>(m_vMyArtworksSprite.at(i));
        if (lSprite)
        {
            Menu* lMenu = dynamic_cast<Menu*>(lSprite->getChildByTag(st_MyArtworks_deleteBtn));
            lMenu->setVisible(false);
        }
        
    }
}

void FavoriteLayer::showMapDeleteBtn()
{
    m_bIsEdit = false;
    //    float offset = m_pTableView->getContentOffset().x;
    DataManager::m_bIsShowMyArtworksDeleteBtn = true;
    //    m_pTableView->reloadData();
    //    m_pTableView->setContentOffset(Vec2(m_pTableView->getContentOffset().x, offset));
    
    for (int i=0; i<m_vMyArtworksSprite.size(); i++)
    {
        FavoriteSprite* lSprite = dynamic_cast<FavoriteSprite*>(m_vMyArtworksSprite.at(i));
        if (lSprite)
        {
            Menu* lMenu = dynamic_cast<Menu*>(lSprite->getChildByTag(st_MyArtworks_deleteBtn));
            lMenu->setVisible(true);
        }
        
    }
}

void FavoriteLayer::removeDialog(Node* pSender)
{
    STAlertDialog* lDialog = (STAlertDialog*)pSender;
    lDialog->removeFromParentAndCleanup(true);
}


void FavoriteLayer::deleteMapImage(Node* node)
{
    for (int i = 0; i < m_vMyArtworksSprite.size(); ++i)
    {
        FavoriteSprite* lSprite = m_vMyArtworksSprite.at(i);
        if (lSprite->getIndex() == m_pClickDeleteSprite->getIndex())
        {
            std::vector<FavoriteSprite*>::iterator iter = m_vMyArtworksSprite.begin() + i;
            m_vMyArtworksSprite.erase(iter);
//            this->deleteSaveImage(lSprite->getImageFullPath());
            
            for (int i = 0; i < m_vMyArtworksSprite.size(); ++i)
            {
                FavoriteSprite* lSprite = m_vMyArtworksSprite.at(i);
                if (lSprite->getIndex() < m_pClickDeleteSprite->getIndex())
                {
                    lSprite->setPosition(Vec2(lSprite->getPosition().x, lSprite->getPosition().y - ScreenAdapterHelper::getValue(680)));
                }
            }
        }
        
        
    }
    
    DM_getInstance->deleteFavoriteImage(m_pClickDeleteSprite->getImageName());
    m_pClickDeleteSprite->removeFromParentAndCleanup(true);
    m_pClickDeleteSprite = NULL;
    
    m_pScrollView->setContentSize(Size(m_pScrollView->getContentSize().width, m_pScrollView->getContentSize().height - ScreenAdapterHelper::getValue(680)));
    m_pScrollView->setContentOffset(Vec2(m_pScrollView->getContentOffset().x, m_pScrollView->getContentOffset().y + ScreenAdapterHelper::getValue(680)));
    
    this->removeDialog(node);
    
    if (m_vMyArtworksSprite.empty())
    {
        Sprite* lSprite = Sprite::create("pop3.png");
        lSprite->setPosition(Vec2(this->getContentSize().width/2, ScreenAdapterHelper::getValue(512)/ScreenAdapterHelper::getRealDesignScale() + ScreenAdapterHelper::getValue(100)));
        this->addChild(lSprite);
        
        Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["FavoriteEmpty"], st_font_AR, ScreenAdapterHelper::getValue(35), Size(lSprite->getContentSize().width * 0.92, lSprite->getContentSize().height), TextHAlignment::LEFT, TextVAlignment::CENTER);
        label->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
        lSprite->addChild(label);
        label->setColor(Color3B::BLACK);
    }
}

void FavoriteLayer::deleteSaveImage(const string& imageName)
{
    
    
    if (FileUtils::getInstance()->isFileExist(imageName))
    {
        remove(imageName.c_str());
        Director::getInstance()->getTextureCache()->removeTextureForKey(imageName);
    }
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(imageName.c_str());
    
}

void FavoriteLayer::setScrollViewOffset()
{
    DataManager::isFavoriteOffset = true;
    DataManager::favoriteOffset = m_pScrollView->getContentOffset().y;
}