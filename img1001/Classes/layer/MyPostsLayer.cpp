//
//  MyPostsLayer.cpp
//  IMG1001
//
//  Created by houhuang on 15/11/17.
//
//

#include "MyPostsLayer.hpp"
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


MyPostsLayer::~MyPostsLayer()
{
    this->setScrollViewOffset();
    DownLoadPage::getInstance()->m_vNeedGetLikeImage.clear();
}

MyPostsLayer::MyPostsLayer()
{
    m_iImageNumber = (int)DM_getInstance->m_vMyPostImage.size();

    m_pTableView = NULL;
    m_bIsEdit = true;
    m_pClickDeleteSprite = NULL;

}

bool MyPostsLayer::init()
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
        
        Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["PostEmpty"], st_font_AR, ScreenAdapterHelper::getValue(35), Size(lSprite->getContentSize().width * 0.92, lSprite->getContentSize().height), TextHAlignment::LEFT, TextVAlignment::CENTER);
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
    this->addChild(scrollView);
    m_pScrollView = scrollView;
    
    if (DataManager::postOffset > 0)
    {
        DataManager::postOffset = scrollView->minContainerOffset().y;
    }

    this->initContentLayer();
    m_pScrollView->setDelegate(this);
    
    return true;
}

void MyPostsLayer::onExit()
{
    LayerColor::onExit();
    DataManager::getInstance()->m_vNeedLoadImage.clear();
    DataManager::getInstance()->m_bIsLoadFinish = false;
}

void MyPostsLayer::initContentLayer()
{
    LayerColor* lContent = LayerColor::create(Color4B(0, 0, 0, 0));
    lContent->setContentSize(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(680) * m_iImageNumber + ScreenAdapterHelper::getValue(200)));
    m_pScrollView->setContainer(lContent);
    
    m_vMyArtworksSprite.clear();
    
    for (int i = 0; i < m_iImageNumber; ++i)
    {
//        log("%lu",DownLoadPage::getInstance()->m_vPostImageId.size());
        FavoriteSprite* lSprite;
        if (DownLoadPage::getInstance()->m_vPostImageId.size() > i)
        {
            lSprite = FavoriteSprite::create(DM_getInstance->m_vMyPostImage.at(m_iImageNumber - 1 - i), DownLoadPage::getInstance()->m_vPostImageId.at(DownLoadPage::getInstance()->m_vPostImageId.size() - 1 - i));
        }else
        {
            lSprite = FavoriteSprite::create(DM_getInstance->m_vMyPostImage.at(m_iImageNumber - 1 - i), -1);
        }
        
//        MyArtworksSprite* lSprite = MyArtworksSprite::create(DM_getInstance->m_vMyPostImage.at(m_iImageNumber - 1 - i));

        
        lSprite->setPosition(Vec2(lContent->getContentSize().width/2, lContent->getContentSize().height - ScreenAdapterHelper::getValue(680)/2-i*ScreenAdapterHelper::getValue(680)));
        lContent->addChild(lSprite);

        if (abs(DataManager::postOffset + lSprite->getPosition().y) < ScreenAdapterHelper::getValue(3000))
        {
            lSprite->display();
        }
        //        lSprite->display();
        lSprite->setIndex(i);
        //        lSprite->addListener(callfuncN_selector(MyPostsLayer::reponseMapSprite), this);
        
        MenuItemImage* deleteBtn = MenuItemImage::create("delete_artworks.png", "delete_artworks.png", CC_CALLBACK_1(MyPostsLayer::doAction, this));
        deleteBtn->setPosition(Vec2(lSprite->getContentSize().width - ScreenAdapterHelper::getValue(20), lSprite->getContentSize().height - ScreenAdapterHelper::getValue(20)));
        
        Menu* lMenu = Menu::create(deleteBtn, NULL);
        lMenu->setPosition(Vec2::ZERO);
        lSprite->addChild(lMenu);
        lMenu->setTag(st_MyArtworks_deleteBtn);
        lMenu->setVisible(false);
        
        m_vMyArtworksSprite.push_back(lSprite);
    }
    
    if (m_iImageNumber <= 2)
    {
        m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
        return;
    }
    
    if (DataManager::isPostOffset)
    {
        m_pScrollView->setContentOffset(Vec2(m_pScrollView->getContentOffset().x, DataManager::postOffset));
        
    }else
    {
        m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
    }
    
}

void MyPostsLayer::scrollViewDidScroll(ScrollView* view)
{
    Vec2 offset = m_pScrollView->getContentOffset();
    
    for (auto lSprite : m_vMyArtworksSprite)
    {
        float endX = lSprite->getPosition().y + offset.y;
        
        if (endX > ScreenAdapterHelper::getValue(-600) * 6 && endX < ScreenAdapterHelper::getValue(600) * 6)
        {
            lSprite->display();
        }else
        {
            lSprite->hideThis();
        }
    }
}

void MyPostsLayer::scrollViewDidZoom(ScrollView* view)
{

}

void MyPostsLayer::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();;
}

void MyPostsLayer::reponseMapSprite(Node* ref)
{
    MyArtworksSprite* lSprite = (MyArtworksSprite*)ref;
    if (m_bIsEdit)
    {
        DataManager::getInstance()->m_pCurrentImage.ID = DM_getInstance->m_vMyArtWorksImageNumber.at(DM_getInstance->m_vMyArtWorksImageNumber.size() - lSprite->getIndex() - 1);
        DataManager::getInstance()->m_pCurrentImage.imageName = DM_getInstance->m_vMyArtWorksImageName.at(DM_getInstance->m_vMyArtWorksImageName.size() - lSprite->getIndex() - 1);
        
        DataManager::IsMyArtworksOffsety = true;
        DataManager::myArtworksOffset = m_pScrollView->getContentOffset().y;
    }
}



void MyPostsLayer::doAction(Ref* ref)
{
    MenuItemImage* lMenuItemImage = dynamic_cast<MenuItemImage*>(ref);
    m_pClickDeleteSprite = dynamic_cast<FavoriteSprite*>(lMenuItemImage->getParent()->getParent());
    
    STAlertDialog* lDialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["SureDelete"], DM_getInstance->getLanguageMes()["Later"], DM_getInstance->getLanguageMes()["Delete"]);
    lDialog->setLeftListener(this, callfuncN_selector(MyPostsLayer::removeDialog));
    lDialog->setRightListener(this, callfuncN_selector(MyPostsLayer::deleteMapImage));
    //    ScreenAdapterHelper::setPosition(lDialog, Vec2(384, 512));
    this->getParent()->addChild(lDialog, 100);
}

void MyPostsLayer::hideMapDeleteBtn()
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

void MyPostsLayer::showMapDeleteBtn()
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



void MyPostsLayer::removeDialog(Node* pSender)
{
    STAlertDialog* lDialog = (STAlertDialog*)pSender;
    lDialog->removeFromParentAndCleanup(true);
}

void MyPostsLayer::deleteMapImage(Node* node)
{
    for (int i = 0; i < m_vMyArtworksSprite.size(); ++i)
    {
        FavoriteSprite* lSprite = m_vMyArtworksSprite.at(i);
        if (lSprite->getIndex() == m_pClickDeleteSprite->getIndex())
        {
            std::vector<FavoriteSprite*>::iterator iter = m_vMyArtworksSprite.begin() + i;
            m_vMyArtworksSprite.erase(iter);
            this->deleteSaveImage(lSprite->getPostImageName());
            
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
    
    DM_getInstance->deleteMyPostsImage(m_pClickDeleteSprite->getPostImageName());
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
        
        Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["PostEmpty"], st_font_AR, ScreenAdapterHelper::getValue(35), Size(lSprite->getContentSize().width * 0.92, lSprite->getContentSize().height), TextHAlignment::LEFT, TextVAlignment::CENTER);
        label->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
        lSprite->addChild(label);
        label->setColor(Color3B::BLACK);
//        Scene* scene = ChooseGalleryScene::scene();
//        Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
    }
}

void MyPostsLayer::deleteSaveImage(const string& imageName)
{

    std::string fullPath = FileUtils::getInstance()->getWritablePath() + imageName;
    
    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
        remove(fullPath.c_str());
        Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
    }

    Director::getInstance()->getTextureCache()->removeTextureForKey(imageName.c_str());

}

void MyPostsLayer::setScrollViewOffset()
{
    DataManager::isPostOffset = true;
    DataManager::postOffset = m_pScrollView->getContentOffset().y;
}
