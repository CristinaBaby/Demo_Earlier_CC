//
//  InspirationScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "InspirationScene.h"
#include "UICpp.h"
#include "MyArtSprite.hpp"
#include "MyArtDataManager.hpp"
#include "ChapterManager.hpp"
#include "STSystemFunction.h"
#include "DownLoadPage.h"
#include "ThreadManager.hpp"
#include "STSystemFunction.h"

enum
{
    st_MyArtworks_deleteBtn = 15,
    st_Cell_children_tag,
};

InspirationScene::~InspirationScene(){

    DownLoadPage::getInstance()->m_mNeedDownloadImage.clear();
    xThread->m_vNeedLoadImage.clear();
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    DownLoadPage::getInstance()->m_bIsFinishDownloadImage = false;
    DownLoadPage::getInstance()->_isRequestingAsk = false;
    //    DataManager::getInstance()->m_vNeedLoadImage.clear();
    
    NotificationCenter::getInstance()->removeObserver(this, st_inspirationLayer_downLoadJson_fai);
    NotificationCenter::getInstance()->removeObserver(this, st_inspirationLayer_firstDownLoadJson_suc);
    NotificationCenter::getInstance()->removeObserver(this, st_inspirationLayer_downLoadJson_suc);
}

InspirationScene::InspirationScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();

    m_iImageNumber = 0;
    
    m_pTableView = NULL;
    m_bIsEdit = true;
    m_bIsNetwork = false;
    m_pLoadingLayer = NULL;
    m_bIsShowLoading = true;
    m_fTime = 1.0f;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(InspirationScene::waitDownloadJsonSuc), st_inspirationLayer_firstDownLoadJson_suc, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(InspirationScene::waitDownloadJsonFai), st_inspirationLayer_downLoadJson_fai, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(InspirationScene::updateContentSize), st_inspirationLayer_downLoadJson_suc, NULL);
}

bool InspirationScene::init(){
    
    if(Scene::init())
    {
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("inspiration.json"));
        addChild(_root);
        _root->cocos2d::Node::visit();
        
        _placeholder = (Layout*)Helper::seekNodeByName(_root, "listview");
        
        quickLayoutColor("back", _root, CC_CALLBACK_2(InspirationScene::onButton, this));
        
        Director::getInstance()->getTextureCache()->addImage("cocos/my_arts/paper.png");
        
//        loadTableView();
    
        
        xMyArtData->m_bIsChapterEnterMyart = false;
        
        STSystemFunction sf;
        if (!sf.checkNetworkAvailable())
        {
            xDialog->show("network error", "ok", "", CC_CALLBACK_1(InspirationScene::onDialog, this));
        }else
        {
            DownLoadPage::getInstance()->firstDownLoadInspirationInfo();
        }
        
        xAnalytics->sendScreenEvent("InspirationScene");
        
        return true;
    }

    return false;
}

void InspirationScene::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    string name = ((Node*)pSender)->getName();
    
    if (name == "back") {
        
        if (xScene->getLastSceneTag() == SceneManager::Tag::SHARE) {
            auto graph = xChapter->getCurrentChapter().getCurrentGraph();
            auto path = FileUtils::getInstance()->getWritablePath() + graph.getPreviewName().c_str();
            if (!FileUtils::getInstance()->isFileExist(path)) {
                xScene->back(SceneManager::Tag::CHAPTER);
                return;
            }
        }
        
        xScene->back();
    }
}



void InspirationScene::onExit()
{
    Scene::onExit();
    xThread->m_vNeedLoadImage.clear();
    xThread->m_bIsload = false;
    
//    DataManager::getInstance()->m_vNeedLoadImage.clear();
//    DataManager::getInstance()->m_bIsLoadFinish = false;
}

void InspirationScene::loadTableView()
{
//    float lScale = ScreenAdapterHelper::getRealDesignScale();
//    float lY = 0.0f;
//    if(lScale==1 || lScale==2)
//    {
//        lY = ScreenAdapterHelper::getValue(860) * ScreenAdapterHelper::getRealDesignScale();
//    }else
//    {
//        lY = ScreenAdapterHelper::getValue(885) * ScreenAdapterHelper::getRealDesignScale();
//    }
//    _placeholder->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
//    _placeholder->setBackGroundColor(cocos2d::Color3B::BLUE);
//    _placeholder->setContentSize(getContentSize());
//    _placeholder->setPositionX(0);
//    quickLogPosition(_placeholder);
//    return;
    
    auto up_ui = Helper::seekNodeByName(_root, "up_ui");
    STTableView *table = STTableView::create(this, Size(getContentSize().width, up_ui->getPositionY()));
    
    table->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    table->setDelegate(this);
    table->reloadData();
    _placeholder->getParent()->addChild(table);
    m_pTableView = table;
    _placeholder->removeFromParent();
}


//Size InspirationScene::cellSizeForTable(TableView* table)
//{
//    if (this->numberOfCellsInTableView(table))
//    {
//        <#statements#>
//    }
//    
//    return Size(700, 680);
//}

Size InspirationScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    if ((this->numberOfCellsInTableView(table) - 1) == idx)
    {
        return Size(table->getContentSize().width, 150);
    }else
    {
        return Size(table->getContentSize().width, 680);
    }
}


TableViewCell* InspirationScene::tableCellAtIndex(TableView* table, ssize_t idex)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    if ((this->numberOfCellsInTableView(table) - 1) == idex)
    {
        return cell;
    }
    
    InspirationInfo lInsInfo;
    lInsInfo = DownLoadPage::getInstance()->m_vInspirationInfo.at(idex);
    
    ServerSprite* lSprite = ServerSprite::create(lInsInfo.imageUrl, lInsInfo.ID, lInsInfo.likeNumber);
    lSprite->display();
    
    lSprite->setPosition(Vec2(table->getContentSize().width/2, 380));
    lSprite->setTag(st_Cell_children_tag);
    cell->addChild(lSprite);
    
    return cell;
}

ssize_t InspirationScene::numberOfCellsInTableView(TableView* table)
{
    //    log("%ld",DownLoadPage::getInstance()->m_vInspirationInfo.size());
    return DownLoadPage::getInstance()->m_vInspirationInfo.size() + 1;
}

void InspirationScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

void InspirationScene::tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch)
{
    
}

void InspirationScene::tableCellRemoveTexture(TableView* table, TableViewCell* cell)
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

void InspirationScene::tableRequestNextPage(TableView* table,ssize_t idx)
{
    DownLoadPage::getInstance()->downLoadInspirationInfo(idx);
}

ssize_t InspirationScene::tableLastID(TableView* table)
{
    return DownLoadPage::getInstance()->m_vInspirationInfo.at(DownLoadPage::getInstance()->m_vInspirationInfo.size() - 1).ID;
}

void InspirationScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    STSystemFunction sf;
    if (sf.checkNetworkAvailable())
    {
        SwallowListenerLayer* lSwallowLayer = SwallowListenerLayer::create(Color4B(0, 0, 0, 0));
        lSwallowLayer->setPosition(this->getPosition()/2);
        this->addChild(lSwallowLayer,100);
        lSwallowLayer->setTouchEnable(true);
        m_pLoadingLayer = lSwallowLayer;
        
        Sprite* rotateSprite = Sprite::create("cocos/other/load_rotate_icon.png");
        //    rotateSprite->setPosition(Vec2(lSwallowLayer->getContentSize().width/2, lSwallowLayer->getContentSize().height/2));
        rotateSprite->setPosition(lSwallowLayer->getContentSize()/2);
        lSwallowLayer->addChild(rotateSprite, 10);
        rotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));
    }
}


void InspirationScene::setScrollViewOffset()
{
    //    DataManager::isInspirationOffset = true;
    //    DataManager::inspirationOffset = m_pScrollView->getContentOffset().y;
}

void InspirationScene::waitDownloadJsonSuc(Ref* ref)
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

void InspirationScene::updateContentSize(Ref* ref)
{
    m_pTableView->updateContentSize();
}

void InspirationScene::waitDownloadJsonFai(Ref* ref)
{
    if (m_pLoadingLayer)
    {
        m_pLoadingLayer->removeFromParentAndCleanup(true);
        m_pLoadingLayer = NULL;
    }
    xDialog->show("post fail", "Ok", "", CC_CALLBACK_1(InspirationScene::onDialog, this));
    
//    STAlertDialog* dialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["PostFaild"], DM_getInstance->getLanguageMes()["OK"], "");
//    dialog->setLeftListener(this, callfuncN_selector(InspirationScene::removeDialog));
//    this->getParent()->addChild(dialog,100);

}



void InspirationScene::onDialog(const string & name)
{
    if (name == "OK") {
        
    }
}
