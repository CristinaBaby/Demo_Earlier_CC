//
//  InspirationLayer.cpp
//  IMG1001
//
//  Created by houhuang on 15/11/17.
//
//

#include "InspirationLayer.hpp"
#include "../helper/ScreenAdapterHelper.h"
#include "../define/Globalmacro.h"
#include "../layer/Dialog.h"
#include "../scene/GameScene.h"
#include "../data/DataManager.h"
#include "../scene/ChooseGalleryScene.h"
#include "../DownLoad/DownLoadPage.h"
#include "../modules/STSystemFunction.h"
#include "../compoment/MyArtworksSprite.hpp"
#include "../scene/ChooseGalleryScene.h"

enum
{
    st_MyArtworks_deleteBtn = 15,
    st_Cell_children_tag,
};


InspirationLayer::~InspirationLayer()
{

    DownLoadPage::getInstance()->m_mNeedDownloadImage.clear();
    DownLoadPage::getInstance()->m_bIsFinishDownloadImage = false;
    DownLoadPage::getInstance()->_isRequestingAsk = false;
//    DataManager::getInstance()->m_vNeedLoadImage.clear();
    
    NotificationCenter::getInstance()->removeObserver(this, st_inspirationLayer_downLoadJson_fai);
    NotificationCenter::getInstance()->removeObserver(this, st_inspirationLayer_firstDownLoadJson_suc);
    NotificationCenter::getInstance()->removeObserver(this, st_inspirationLayer_downLoadJson_suc);

}

InspirationLayer::InspirationLayer()
{
    m_iImageNumber = 0;
    
    m_pTableView = NULL;
    m_bIsEdit = true;
    m_bIsNetwork = false;
    m_pLoadingLayer = NULL;
    m_bIsShowLoading = true;
    m_fTime = 1.0f;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(InspirationLayer::waitDownloadJsonSuc), st_inspirationLayer_firstDownLoadJson_suc, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(InspirationLayer::waitDownloadJsonFai), st_inspirationLayer_downLoadJson_fai, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(InspirationLayer::updateContentSize), st_inspirationLayer_downLoadJson_suc, NULL);
    
}

bool InspirationLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) return false;

    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 1));
    this->setContentSize(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(1024)));
    
    STSystemFunction sf;
    if(!sf.checkNetworkAvailable())
    {

//        this->waitDownloadJsonFai(NULL);
        
    }else
    {
        DownLoadPage::getInstance()->firstDownLoadInspirationInfo();
        
    }
//
    return true;
}

void InspirationLayer::onExit()
{
    LayerColor::onExit();
    DataManager::getInstance()->m_vNeedLoadImage.clear();
    DataManager::getInstance()->m_bIsLoadFinish = false;
}

void InspirationLayer::loadTableView()
{
    float lScale = ScreenAdapterHelper::getRealDesignScale();
    float lY = 0.0f;
    if(lScale==1 || lScale==2)
    {
        lY = ScreenAdapterHelper::getValue(860) * ScreenAdapterHelper::getRealDesignScale();
    }else
    {
        lY = ScreenAdapterHelper::getValue(885) * ScreenAdapterHelper::getRealDesignScale();
    }
    
    STTableView *table = STTableView::create(this, Size(ScreenAdapterHelper::getValue(700), lY));
    
    table->setDirection(ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 1));
    table->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    this->addChild(table);
    table->setDelegate(this);
   table->reloadData();
//    table->setContentOffset(Vec2(table->getContentOffset().x, table->maxContainerOffset().y));
    
    m_pTableView = table;
}


Size InspirationLayer::cellSizeForTable(TableView* table)
{
    return Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(680));
}

void InspirationLayer::scrollViewDidScroll(ScrollView* view)
{
    log("%f",view->getContentOffset().y);
}

TableViewCell* InspirationLayer::tableCellAtIndex(TableView* table, ssize_t idex)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    InspirationInfo lInsInfo;
    lInsInfo = DownLoadPage::getInstance()->m_vInspirationInfo.at(idex);
    
    ServerSprite* lSprite = ServerSprite::create(lInsInfo.imageUrl, lInsInfo.ID, lInsInfo.likeNumber);
    lSprite->display();
    
    lSprite->setPosition(Vec2(ScreenAdapterHelper::getValue(350), ScreenAdapterHelper::getValue(380)));
    lSprite->setTag(st_Cell_children_tag);
    cell->addChild(lSprite);
    
    return cell;
}

ssize_t InspirationLayer::numberOfCellsInTableView(TableView* table)
{
//    log("%ld",DownLoadPage::getInstance()->m_vInspirationInfo.size());
    return DownLoadPage::getInstance()->m_vInspirationInfo.size();
}

void InspirationLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void InspirationLayer::tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch)
{

}

void InspirationLayer::tableCellRemoveTexture(TableView* table, TableViewCell* cell)
{
    
    if(cell && cell->getChildren().size()>0)
    {
        ServerSprite* lSprite = dynamic_cast<ServerSprite*>(cell->getChildByTag(st_Cell_children_tag));
        if(lSprite!=NULL)
        {
            lSprite->removeTextureCache();
        }
        
        cell->removeAllChildrenWithCleanup(true);
    }
}

void InspirationLayer::tableRequestNextPage(TableView* table,ssize_t idx)
{
    DownLoadPage::getInstance()->downLoadInspirationInfo(idx);
}

ssize_t InspirationLayer::tableLastID(TableView* table)
{
    return DownLoadPage::getInstance()->m_vInspirationInfo.at(DownLoadPage::getInstance()->m_vInspirationInfo.size() - 1).ID;
}

void InspirationLayer::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    STSystemFunction sf;
    if (sf.checkNetworkAvailable())
    {
        SwallowListenerLayer* lSwallowLayer = SwallowListenerLayer::create(Color4B(0, 0, 0, 0));
        this->getParent()->addChild(lSwallowLayer,100);
        lSwallowLayer->setTouchEnable(true);
        m_pLoadingLayer = lSwallowLayer;
        
        Sprite* rotateSprite = Sprite::create("load_rotate_icon.png");
        //    rotateSprite->setPosition(Vec2(lSwallowLayer->getContentSize().width/2, lSwallowLayer->getContentSize().height/2));
        ScreenAdapterHelper::setPosition(rotateSprite, Vec2(384, 512));
        lSwallowLayer->addChild(rotateSprite, 10);
        rotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));
    }
}


void InspirationLayer::setScrollViewOffset()
{
//    DataManager::isInspirationOffset = true;
//    DataManager::inspirationOffset = m_pScrollView->getContentOffset().y;
}

void InspirationLayer::waitDownloadJsonSuc(Ref* ref)
{
    if (m_pLoadingLayer)
    {
        m_pLoadingLayer->removeFromParentAndCleanup(true);
        m_pLoadingLayer = NULL;
    }
    
//    m_iImageNumber = (int)DownLoadPage::getInstance()->m_vInspirationInfo.size();
//    this->initContentLayer();
    if (DownLoadPage::getInstance()->m_vInspirationInfo.size() > 0)
    {
        this->loadTableView();
    }

}

void InspirationLayer::updateContentSize(Ref* ref)
{
    m_pTableView->updateContentSize();
}

void InspirationLayer::waitDownloadJsonFai(Ref* ref)
{
    if (m_pLoadingLayer)
    {
        m_pLoadingLayer->removeFromParentAndCleanup(true);
        m_pLoadingLayer = NULL;
    }
    STAlertDialog* dialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["PostFaild"], DM_getInstance->getLanguageMes()["OK"], "");
    dialog->setLeftListener(this, callfuncN_selector(InspirationLayer::removeDialog));
//    dialog->ignoreAnchorPointForPosition(false);
//    dialog->setAnchorPoint(Vec2(0.5, 0.5));
//    dialog->setPosition(Vec2(this->getContentSize().width/2, ScreenAdapterHelper::getValue(512)/ScreenAdapterHelper::getRealDesignScale() + ScreenAdapterHelper::getValue(100)));
    this->getParent()->addChild(dialog,100);
//    this->addChild(dialog, 100);
}

void InspirationLayer::removeDialog(Node* node)
{
    STAlertDialog* dialog = dynamic_cast<STAlertDialog*>(node);
    if (dialog)
    {
        dialog->removeFromParentAndCleanup(true);
    }
    
    DataManager::m_bIsGalleryLayer = true;
    
    ChooseGalleryScene* scene = dynamic_cast<ChooseGalleryScene*>(this->getParent());
//    this->removeFromParentAndCleanup(true);
    scene->destoryInspirationLayer();
    scene->createGalleryLayer();


    
}