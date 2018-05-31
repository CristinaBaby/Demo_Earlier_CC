//
//  MyArtworksLayer.cpp
//  IMG1001
//
//  Created by houhuang on 15/11/17.
//
//

#include "MyArtworksLayer.hpp"
#include "../helper/ScreenAdapterHelper.h"
#include "../define/Globalmacro.h"
#include "../layer/Dialog.h"
#include "../scene/GameScene.h"
#include "../data/DataManager.h"
#include "../scene/ChooseGalleryScene.h"

enum
{
    st_MyArtworks_deleteBtn = 15,
};


MyArtworksLayer::~MyArtworksLayer()
{

}

MyArtworksLayer::MyArtworksLayer()
{
    m_iImageNumber = DataManager::getInstance()->m_vMyArtWorksImageNumber.size();

    m_pTableView = NULL;
    m_bIsEdit = true;
    m_pClickDeleteSprite = NULL;

}

bool MyArtworksLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) return false;
    
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 1));
    this->setContentSize(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(1024)));

    
    ScrollView* scrollView = ScrollView::create(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(950 * ScreenAdapterHelper::getRealDesignScale())));
    

    scrollView->setContentSize(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(600) * m_iImageNumber));
    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    scrollView->ignoreAnchorPointForPosition(false);
    scrollView->setAnchorPoint(Vec2(0.5, 1));
    scrollView->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    this->addChild(scrollView);
    m_pScrollView = scrollView;

    this->initContentLayer();
    m_pScrollView->setDelegate(this);
    
    if (DataManager::IsMyArtworksOffsety)
    {
        DataManager::IsMyArtworksOffsety = false;
        if (DataManager::myArtworksOffset < m_pScrollView->minContainerOffset().y)
        {
            m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
        }else if (DataManager::myArtworksOffset > m_pScrollView->maxContainerOffset().y)
        {
            m_pScrollView->setContentOffset(m_pScrollView->maxContainerOffset());
        }else
        {
            m_pScrollView->setContentOffset(Vec2(m_pScrollView->getContentOffset().x, DataManager::myArtworksOffset));
        }
    }else
    {
        m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
    }
    
    if (m_pScrollView->minContainerOffset().y > m_pScrollView->maxContainerOffset().y)
    {
        m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
    }
    
    return true;
}

void MyArtworksLayer::onExit()
{
    LayerColor::onExit();
    DataManager::getInstance()->m_vNeedLoadImage.clear();
    DataManager::getInstance()->m_bIsLoadFinish = false;
}

void MyArtworksLayer::initContentLayer()
{
    LayerColor* lContent = LayerColor::create(Color4B(0, 0, 0, 0));
    lContent->setContentSize(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(600) * m_iImageNumber + ScreenAdapterHelper::getValue(200)));
    m_pScrollView->setContainer(lContent);
    
    m_vMyArtworksSprite.clear();
    
    for (int i = 0; i < m_iImageNumber; ++i)
    {
        MyArtworksSprite* lSprite = MyArtworksSprite::create(DM_getInstance->m_vMyArtWorksImageNumber.at(DM_getInstance->m_vMyArtWorksImageNumber.size() - i - 1), DM_getInstance->m_vMyArtWorksImageName.at(DM_getInstance->m_vMyArtWorksImageNumber.size() - i - 1));
        if (i < 2)
        {
            lSprite->display();
        }
//        lSprite->display();
        lSprite->setIndex(i);
        lSprite->addListener(callfuncN_selector(MyArtworksLayer::reponseMapSprite), this);
        
        lSprite->setPosition(Vec2(lContent->getContentSize().width/2, lContent->getContentSize().height - lSprite->getContentSize().height/2-i*lSprite->getContentSize().height - ScreenAdapterHelper::getValue(20)));
        lContent->addChild(lSprite);
        
        MenuItemImage* deleteBtn = MenuItemImage::create("delete_artworks.png", "delete_artworks.png", CC_CALLBACK_1(MyArtworksLayer::doAction, this));
        deleteBtn->setPosition(Vec2(lSprite->getContentSize().width - ScreenAdapterHelper::getValue(20), lSprite->getContentSize().height - ScreenAdapterHelper::getValue(20)));
        
        Menu* lMenu = Menu::create(deleteBtn, NULL);
        lMenu->setPosition(Vec2::ZERO);
        lSprite->addChild(lMenu);
        lMenu->setTag(st_MyArtworks_deleteBtn);
        lMenu->setVisible(false);
        
        m_vMyArtworksSprite.push_back(lSprite);
    }
}

void MyArtworksLayer::scrollViewDidScroll(ScrollView* view)
{
    Vec2 offset = m_pScrollView->getContentOffset();
    
    for (auto lSprite : m_vMyArtworksSprite)
    {
        float endX = lSprite->getPosition().y + offset.y;
        
        if (endX > ScreenAdapterHelper::getValue(-600) * 8 && endX < ScreenAdapterHelper::getValue(600) * 8)
        {
            lSprite->display();
        }else
        {
            lSprite->hideThis();
        }
    }
}

void MyArtworksLayer::scrollViewDidZoom(ScrollView* view)
{

}

void MyArtworksLayer::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();;
}

void MyArtworksLayer::reponseMapSprite(Node* ref)
{
    MyArtworksSprite* lSprite = (MyArtworksSprite*)ref;
    if (m_bIsEdit)
    {
        DataManager::getInstance()->m_pCurrentImage.ID = DM_getInstance->m_vMyArtWorksImageNumber.at(DM_getInstance->m_vMyArtWorksImageNumber.size() - lSprite->getIndex() - 1);
        DataManager::getInstance()->m_pCurrentImage.imageName = DM_getInstance->m_vMyArtWorksImageName.at(DM_getInstance->m_vMyArtWorksImageName.size() - lSprite->getIndex() - 1);
        
//        log("%d %s",DataManager::getInstance()->m_pCurrentImage.ID, DataManager::getInstance()->m_pCurrentImage.imageName.c_str());
        
        DataManager::IsMyArtworksOffsety = true;
        DataManager::myArtworksOffset = m_pScrollView->getContentOffset().y;
        Scene* scene = GameScene::create();
        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.35f, scene));
    }
}



void MyArtworksLayer::doAction(Ref* ref)
{
    MenuItemImage* lMenuItemImage = dynamic_cast<MenuItemImage*>(ref);
    m_pClickDeleteSprite = dynamic_cast<MyArtworksSprite*>(lMenuItemImage->getParent()->getParent());
    
    STAlertDialog* lDialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["SureDelete"], DM_getInstance->getLanguageMes()["Later"], DM_getInstance->getLanguageMes()["Delete"]);
    lDialog->setLeftListener(this, callfuncN_selector(MyArtworksLayer::removeDialog));
    lDialog->setRightListener(this, callfuncN_selector(MyArtworksLayer::deleteMapImage));
    //    ScreenAdapterHelper::setPosition(lDialog, Vec2(384, 512));
    this->getParent()->addChild(lDialog, 100);
}                                    

void MyArtworksLayer::hideMapDeleteBtn()
{
    
    m_bIsEdit = true;
//    float offset = m_pTableView->getContentOffset().x;
    DataManager::m_bIsShowMyArtworksDeleteBtn = false;
//    m_pTableView->reloadData();
//    m_pTableView->setContentOffset(Vec2(m_pTableView->getContentOffset().x, offset));
    for (int i=0; i<m_vMyArtworksSprite.size(); i++)
    {
        MyArtworksSprite* lSprite = dynamic_cast<MyArtworksSprite*>(m_vMyArtworksSprite.at(i));
        if (lSprite)
        {
            Menu* lMenu = dynamic_cast<Menu*>(lSprite->getChildByTag(st_MyArtworks_deleteBtn));
            lMenu->setVisible(false);
        }
        
    }
}

void MyArtworksLayer::showMapDeleteBtn()
{
    m_bIsEdit = false;
//    float offset = m_pTableView->getContentOffset().x;
    DataManager::m_bIsShowMyArtworksDeleteBtn = true;
//    m_pTableView->reloadData();
//    m_pTableView->setContentOffset(Vec2(m_pTableView->getContentOffset().x, offset));
    
    for (int i=0; i<m_vMyArtworksSprite.size(); i++)
    {
        MyArtworksSprite* lSprite = dynamic_cast<MyArtworksSprite*>(m_vMyArtworksSprite.at(i));
        if (lSprite)
        {
            Menu* lMenu = dynamic_cast<Menu*>(lSprite->getChildByTag(st_MyArtworks_deleteBtn));
            lMenu->setVisible(true);
        }
        
    }
}



void MyArtworksLayer::removeDialog(Node* pSender)
{
    STAlertDialog* lDialog = (STAlertDialog*)pSender;
    lDialog->removeFromParentAndCleanup(true);
}

void MyArtworksLayer::deleteMapImage(Node* node)
{
    for (int i = 0; i < m_vMyArtworksSprite.size(); ++i)
    {
        MyArtworksSprite* lSprite = m_vMyArtworksSprite.at(i);
        if (lSprite->getIndex() == m_pClickDeleteSprite->getIndex())
        {
            std::vector<MyArtworksSprite*>::iterator iter = m_vMyArtworksSprite.begin() + i;
            m_vMyArtworksSprite.erase(iter);
            this->deleteSaveImage(lSprite->getImageName(), lSprite->getImageNumber());
            
            for (int i = 0; i < m_vMyArtworksSprite.size(); ++i)
            {
                MyArtworksSprite* lSprite = m_vMyArtworksSprite.at(i);
                if (lSprite->getIndex() < m_pClickDeleteSprite->getIndex())
                {
                    lSprite->setPosition(Vec2(lSprite->getPosition().x, lSprite->getPosition().y - ScreenAdapterHelper::getValue(600)));
                }
            }
        }
        
 
    }
    
    
    DataManager::getInstance()->deleteArtworksImage(m_pClickDeleteSprite->getImageName(), m_pClickDeleteSprite->getImageNumber());
    m_pClickDeleteSprite->removeFromParentAndCleanup(true);
    m_pClickDeleteSprite = NULL;
    
    m_pScrollView->setContentSize(Size(m_pScrollView->getContentSize().width, m_pScrollView->getContentSize().height - ScreenAdapterHelper::getValue(600)));
    m_pScrollView->setContentOffset(Vec2(m_pScrollView->getContentOffset().x, m_pScrollView->getContentOffset().y + ScreenAdapterHelper::getValue(600)));
    
    this->removeDialog(node);
    
    if (m_vMyArtworksSprite.empty())
    {
        Scene* scene = ChooseGalleryScene::scene();
        Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
    }
}

void MyArtworksLayer::deleteSaveImage(const string& galleryName, int imageNumber)
{
    ColorData lColorData;
    lColorData.R = 255;
    lColorData.G = 255;
    lColorData.B = 255;
    lColorData.ID = imageNumber;
    DataManager::getInstance()->saveBackColor(galleryName, imageNumber, lColorData);
    
    stringstream ss2;
    ss2<<"";
    ss2<<galleryName;
    ss2<<imageNumber;
    ss2<<"_white.png";
    
    std::string fullPath = FileUtils::getInstance()->getWritablePath() + ss2.str();
    
    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
        remove(fullPath.c_str());
        Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
    }
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(ss2.str());
    ss2.str().clear();

}

void MyArtworksLayer::setScrollViewOffset()
{
    DataManager::IsMyArtworksOffsety = true;
    DataManager::myArtworksOffset = m_pScrollView->getContentOffset().y;
}
