//
//  ScrollMapLayer_ScrollView.cpp
//  IMG1001
//
//  Created by houhuang on 15/9/21.
//
//

#include "ScrollMapLayer_ScrollView.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../define/Globalmacro.h"
#include "../compoment/MapSprite_New.h"
#include "../compoment/ColorSprite.h"
#include "../define/AppConfigs.h"
#include "../layer/Dialog.h"
#include "../scene/GameScene.h"
#include "../helper/PurchaseHelper.h"

enum
{
    st_scrollMapLayer_Dialog = 11,
};

ScrollMapLayer_ScrollView* ScrollMapLayer_ScrollView::create(int index)
{
    ScrollMapLayer_ScrollView* layer = new ScrollMapLayer_ScrollView();
    if (layer && layer->init(index))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

ScrollMapLayer_ScrollView::ScrollMapLayer_ScrollView()
{
    m_pScrollView   = NULL;
    m_iMapCellCount = 0;
    m_sGalleryName  = "";
    
    _stInAppBilling = new STInAppBilling();
    _stInAppBilling->setIABDelegate(PurchaseHelper::getInstance());
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ScrollMapLayer_ScrollView::waitNotification), st_scrollmaplayer_notifiction, NULL);
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ScrollMapLayer_ScrollView::waitDownLoadSucNotification), st_baseScene_downloadSuc_notifition, NULL);
}

void ScrollMapLayer_ScrollView::stopLoading()
{
    Director::getInstance()->getTextureCache()->unbindAllImageAsync();
    DataManager::stopLoadThread=true;
  
}


ScrollMapLayer_ScrollView::~ScrollMapLayer_ScrollView()
{
//    this->stopLoading();
    
    CC_SAFE_DELETE(_stInAppBilling);
    NotificationCenter::getInstance()->removeObserver(this, st_scrollmaplayer_notifiction);
    
    NotificationCenter::getInstance()->removeObserver(this, st_baseScene_downloadSuc_notifition);
    
    m_pScrollView = NULL;
    m_vMapSprite.clear();
    
//    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool ScrollMapLayer_ScrollView::init(int index)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) return false;
    
  
    m_vGalleryInfo  = DataManager::getInstance()->_allMapDatas.at(index - 1);
    m_iMapCellCount = (int)m_vGalleryInfo->size();
    m_sGalleryName  = m_vGalleryInfo->at(0).galleryname;
    
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 1));
    this->setContentSize(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(1024)));
//

    
    m_pScrollView = ScrollView::create(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(950 * ScreenAdapterHelper::getRealDesignScale())));
    m_pScrollView->setContentSize(Size(ScreenAdapterHelper::getValue(700),ScreenAdapterHelper::getValue(600) * m_iMapCellCount));


    m_pScrollView->setDirection(ScrollView::Direction::VERTICAL);
    m_pScrollView->ignoreAnchorPointForPosition(false);
    m_pScrollView->setAnchorPoint(Vec2(0.5, 1));
    m_pScrollView->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    this->addChild(m_pScrollView);
    this->initContentLayer();
    
    if (DataManager::isTableOffset)
    {
        DataManager::isTableOffset = false;
        if (DataManager::tableOffset < m_pScrollView->minContainerOffset().y)
        {
            m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
        }else if (DataManager::tableOffset > m_pScrollView->maxContainerOffset().y)
        {
            m_pScrollView->setContentOffset(m_pScrollView->maxContainerOffset());
        }else
        {
            m_pScrollView->setContentOffset(Vec2(0, DataManager::tableOffset));
        }
    }
    else
    {
        m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
    }

    return true;
}

void ScrollMapLayer_ScrollView::onExit()
{
    LayerColor::onExit();
    DataManager::getInstance()->m_vNeedLoadImage.clear();
    DataManager::getInstance()->m_bIsLoadFinish = false;
}

void ScrollMapLayer_ScrollView::initContentLayer()
{
    LayerColor* lContent = LayerColor::create();
    lContent->setContentSize(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(600) * m_iMapCellCount+ScreenAdapterHelper::getValue(200)));
    m_pScrollView->setContainer(lContent);
    
    m_vMapSprite.clear();
    
    for (int i = 0; i < m_vGalleryInfo->size(); ++i)
    {
        GalleryInfo* lGalleryInfo = &m_vGalleryInfo->at(i);
        MapSprite_New* lSprite=MapSprite_New::create();
        lSprite->setGalleryInfo(lGalleryInfo);
        
        lSprite->setPosition(Vec2(lContent->getContentSize().width/2, lContent->getContentSize().height -lSprite->getContentSize().height/2-i*lSprite->getContentSize().height));
        lContent->addChild(lSprite);
        lSprite->setIndex(lGalleryInfo->ID);
        m_vMapSprite.push_back(lSprite);
        
        lSprite->addListener(callfuncN_selector(ScrollMapLayer_ScrollView::reponseMapSprite), this);
        
//        bool isLock = lGalleryInfo->isLock;
        
        if (lGalleryInfo->isLock == true)
        {
            if (!UserDefault::getInstance()->getBoolForKey(lGalleryInfo->userdefault.c_str()))
            {
                Sprite* lock = Sprite::create("lock.png");
                lock->setAnchorPoint(Vec2(1, 0));
                lock->setPosition(Vec2(ScreenAdapterHelper::getValue(578), ScreenAdapterHelper::getValue(22)));
                lSprite->addChild(lock,10);
                lock->setTag(st_map_islock);
            }
        }
        
        if (lGalleryInfo->isNewImage && !UserDefault::getInstance()->getBoolForKey(lGalleryInfo->isUsedNewImage.c_str()))
        {
            Sprite* newIcon = Sprite::create("image_newIcon.png");
            newIcon->setAnchorPoint(Vec2(1, 1));
            newIcon->setPosition(Vec2(ScreenAdapterHelper::getValue(578), ScreenAdapterHelper::getValue(558)));
            lSprite->addChild(newIcon, 10);
        }
        
    }
}

void ScrollMapLayer_ScrollView::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    DataManager::stopLoadThread=false;
    
    for (int i=0; i<m_vMapSprite.size(); i++) {
        MapSprite_New* lNode=m_vMapSprite.at(i);
        lNode->display();
    }
}

void ScrollMapLayer_ScrollView::reponseMapSprite(Node* ref)
{
    MapSprite_New* lSprite = (MapSprite_New*)ref;

    DataManager::getInstance()->m_CurrentPurchase = lSprite->getGalleryInfo();
    
    if (lSprite->getChildByTag(st_map_islock))
    {
        string str1 = DM_getInstance->getLanguageMes()["Unlock"] ;
        string str2 = DM_getInstance->getLanguageMes()[m_sGalleryName.c_str()];
        string str3 = DM_getInstance->getLanguageMes()["Pack"];
        
        //        string str4 = "Unlock to get 5 artworks \nwith just $0.99. But it now!";
        string str = str1 + " " + str2 + str3;
        
        STAlertDialog* dialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["PurchasePack"], DM_getInstance->getLanguageMes()["Later"], DM_getInstance->getLanguageMes()["Buy"]);
//        STAlertDialog* dialog = STAlertDialog::create(str.c_str(), DM_getInstance->getLanguageMes()["PurchasePack"], DM_getInstance->getLanguageMes()["Later"], DM_getInstance->getLanguageMes()["Buy"]);
        dialog->setLeftListener(this, callfuncN_selector(ScrollMapLayer_ScrollView::removeDialog));
        dialog->setRightListener(this, callfuncN_selector(ScrollMapLayer_ScrollView::unlockMap));
        this->getParent()->addChild(dialog,100);
        dialog->setTag(st_dialog_tag);
    }
    else
    {
        if (!lSprite->getGalleryInfo()->isUsedNewImage.empty())
        {
            UserDefault::getInstance()->setBoolForKey(lSprite->getGalleryInfo()->isUsedNewImage.c_str(), true);
            UserDefault::getInstance()->destroyInstance();
        }
        
        DataManager::tableOffset = m_pScrollView->getContentOffset().y;
        string str = m_vGalleryInfo->at(lSprite->getIndex()-1).galleryname;
        
        DataManager::getInstance()->m_pCurrentImage.ID = lSprite->getIndex();
        DataManager::getInstance()->m_pCurrentImage.imageName = str;

//        log("%d %s",DataManager::getInstance()->m_pCurrentImage.ID, DataManager::getInstance()->m_pCurrentImage.imageName.c_str());
        
        Scene* scene = GameScene::create();
        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.35f, scene));
    }
    
}

void ScrollMapLayer_ScrollView::waitDownLoadSucNotification(Ref* ref)
{
    if (this->getParent()->getChildByTag(st_dialog_tag))
    {
        this->getParent()->getChildByTag(st_dialog_tag)->removeFromParentAndCleanup(true);
    }
}

void ScrollMapLayer_ScrollView::waitNotification(Ref* ref)
{
    DataManager::stopLoadThread = false;
    
    if (this->getParent()->getChildByTag(st_dialog_tag))
    {
        this->getParent()->getChildByTag(st_dialog_tag)->removeFromParentAndCleanup(true);
    }
    
    for (int i = 0; i < m_vMapSprite.size(); ++i)
    {
        MapSprite_New* lSprite = m_vMapSprite.at(i);
        
        if (UserDefault::getInstance()->getBoolForKey(lSprite->getGalleryInfo()->userdefault.c_str()))
        {
            if (lSprite->getChildByTag(st_map_islock))
            {
                lSprite->getChildByTag(st_map_islock)->removeFromParentAndCleanup(true);
            }
        }
        
    }
}

void ScrollMapLayer_ScrollView::removeDialog(Node* pSender)
{
    pSender->removeFromParentAndCleanup(true);
    DataManager::stopLoadThread = false;
}

void ScrollMapLayer_ScrollView::unlockMap(Node* pSender)
{
#ifdef DEBUG_IAP
    PurchaseHelper::getInstance()->purchaseSuccessful(DataManager::getInstance()->m_CurrentPurchase->Iap_ID.c_str());
#else
            _stInAppBilling->purchase(DataManager::getInstance()->m_CurrentPurchase->Iap_ID.c_str());
#endif


//    switch (DataManager::getInstance()->m_iCurrentGalleryID)
//    {
//        case 1://Animals
//        {
//            
//#ifdef DEBUG_IAP
//            PurchaseHelper::getInstance()->purchaseSuccessful(st_purchase_map_animals_iap);
//#else
//            _stInAppBilling->purchase(st_purchase_map_animals_iap);
//#endif
//        }
//            break;
//            
//        case 2://Flowers
//        {
//            
//#ifdef DEBUG_IAP
//            PurchaseHelper::getInstance()->purchaseSuccessful(st_purchase_map_flowers_iap);
//#else
//            _stInAppBilling->purchase(st_purchase_map_flowers_iap);
//#endif
//        }
//            break;
//            
//        case 3://Ladies
//        {
//            
//#ifdef DEBUG_IAP
//            PurchaseHelper::getInstance()->purchaseSuccessful(st_purchase_map_ladies_iap);
//#else
//            _stInAppBilling->purchase(st_purchase_map_ladies_iap);
//#endif
//        }
//            break;
//            
//        case 4://Mandalas
//        {
//            
//#ifdef DEBUG_IAP
//            PurchaseHelper::getInstance()->purchaseSuccessful(st_purchase_map_mandala_iap);
//#else
//            _stInAppBilling->purchase(st_purchase_map_mandala_iap);
//#endif
//        }
//            break;
//            
//        default:
//            break;
//    }
}



