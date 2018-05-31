//
//  ChooseMapLayer.cpp
//  ColorGarden
//
//  Created by houhuang on 16/1/12.
//
//

#include "ChooseMapLayer.hpp"
#include "../helper/ScreenAdapterHelper.h"
#include "../game/DataManager.h"
#include "../helper/ImageDownload.h"
#include "../scene/GameScene.h"
#include "../commpoent/ImageBox.h"
#include "../define/ResoureMacro.h"

enum
{
    st_cell_imagebox_tag = 666,
};

ChooseMapLayer::ChooseMapLayer()
{
    m_pTableView = NULL;
    m_pDotSprite = NULL;
    m_pPageLabel = NULL;
}

ChooseMapLayer::~ChooseMapLayer()
{

}

bool ChooseMapLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) return false;
    
    Sprite* line = Sprite::create("line.png");
    ScreenAdapterHelper::setPosition(line, Vec2(384, 200));
    this->addChild(line);
    
    Sprite* dot = Sprite::create("arrows.png");
    dot->setAnchorPoint(Vec2(0.5, 0.11));
    line->addChild(dot);
    m_pDotSprite = dot;

    stringstream ss;
    ss<<"";
    ss<<DataManager::m_iChooseCurrentPage;
    ss<<"/";
    ss<<(int)(DataManager::getInstance()->_resourceInfos.size()+ImageDownload::getInstance()->_downloadFinishResourceInfo.size());
    
    Sprite* lSprite = Sprite::create("paint.png");
    ScreenAdapterHelper::setPosition(lSprite, Vec2(384, 230),false,true,false,false);
    this->addChild(lSprite);
    
//    Label* label = Label::createWithSystemFont(ss.str().c_str(), "KBStickToThePlan.ttf", ScreenAdapterHelper::getValue(40));
    Label* label = Label::createWithTTF(ss.str().c_str(), "fonts/KBStickToThePlan.ttf", ScreenAdapterHelper::getValue(40));
    label->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
    lSprite->addChild(label);
    m_pPageLabel = label;
    
    line->setVisible(false);
    dot->setVisible(false);
    
    STTableView* tableView = STTableView::create(this, Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(600)));
    tableView->setDirection(ScrollView::Direction::HORIZONTAL);
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableView->setDelegate(this);
    tableView->ignoreAnchorPointForPosition(false);
    tableView->setAnchorPoint(Vec2(0.5, 0.5));
    m_pTableView = tableView;
    
    ScreenAdapterHelper::setPosition(tableView, Vec2(384, 550));
    this->addChild(tableView);
//    tableView->setContentOffset(Vec2(tableView->maxContainerOffset().x, 0));
    
    
    if (DataManager::m_bTableViewIsOffset)
    {
        tableView->setContentOffset(Vec2(DataManager::m_fChooseTableViewOffset, tableView->getContentOffset().y));
    }else
    {
        tableView->setContentOffset(Vec2(tableView->maxContainerOffset().x, tableView->getContentOffset().y));
    }
    
    DataManager::m_bTableViewIsOffset = true;
    
    return true;
}

void ChooseMapLayer::scrollViewDidScroll(ScrollView* view)
{
    float scale = (m_pTableView->getContentOffset().x/m_pTableView->minContainerOffset().x);
    m_pDotSprite->setPosition(Vec2(scale * 600, m_pDotSprite->getPosition().y));
    
//    if (m_pTableView->getContentOffset().x > m_pTableView->maxContainerOffset().x)
//    {
//        m_pTableView->setContentOffset(Vec2(m_pTableView->minContainerOffset().x, m_pTableView->getContentOffset().y));
//    }
//    if (m_pTableView->getContentOffset().x < m_pTableView->minContainerOffset().x)
//    {
//        m_pTableView->setContentOffset(Vec2(m_pTableView->maxContainerOffset().x, m_pTableView->getContentOffset().y));
//    }
    
    DataManager::m_fChooseTableViewOffset = m_pTableView->getContentOffset().x;
    
    TableView* tableView = dynamic_cast<TableView*>(view);
    
    for (int i = 0; i < this->numberOfCellsInTableView(tableView); ++i)
    {
        TableViewCell* cell = dynamic_cast<TableViewCell*>(tableView->cellAtIndex(i));
        
        if (cell)
        {
            ImageBox* lImageBox = dynamic_cast<ImageBox*>(cell->getChildByTag(st_cell_imagebox_tag));
            
            float distance = abs(m_pTableView->getContentOffset().x + cell->getPosition().x);
//            log("%f",distance);
            if (distance < 350)
            {
                int id = lImageBox->getIndex();
//                log("current image box id = %d",id);
                this->updatePageLabel(id);
            }
        }
        
        
    }
}

void ChooseMapLayer::updatePageLabel(int id)
{
    if (DataManager::m_iChooseCurrentPage != id)
    {
        DataManager::m_iChooseCurrentPage = id;
        
        stringstream ss;
        ss<<"";
        ss<<DataManager::m_iChooseCurrentPage;
        ss<<"/";
        ss<<(int)(DataManager::getInstance()->_resourceInfos.size()+ImageDownload::getInstance()->_downloadFinishResourceInfo.size());
        
        m_pPageLabel->setString(ss.str().c_str());
    }
}

void ChooseMapLayer::scrollViewDidZoom(ScrollView* view)
{

}

void ChooseMapLayer::tableCellRemoveTexture(TableView* table, TableViewCell* cell)
{
    ImageBox* lImageBox = dynamic_cast<ImageBox*>(cell->getChildByTag(st_cell_imagebox_tag));
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(lImageBox->getImageName());
}


Size ChooseMapLayer::cellSizeForTable(TableView* table)
{
    return Size(ScreenAdapterHelper::getValue(600), ScreenAdapterHelper::getValue(600));
}

TableViewCell* ChooseMapLayer::tableCellAtIndex(TableView* table, ssize_t idex)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    Sprite* bg = Sprite::create("paper_2.png");
    bg->setPosition(Vec2(ScreenAdapterHelper::getValue(300), ScreenAdapterHelper::getValue(300)));
    cell->addChild(bg);
    
    ImageBox* lImageBox=ImageBox::create();
    lImageBox->setIndex((int)idex+1);
    lImageBox->setScale(0.45f);
    lImageBox->setAnchorPoint(Vec2(0.5, 0.5));
    lImageBox->setTag(st_cell_imagebox_tag);
    lImageBox->setPosition(Vec2(ScreenAdapterHelper::getValue(295), ScreenAdapterHelper::getValue(305)));
    cell->addChild(lImageBox);
    
    lImageBox->display();
    lImageBox->setIsShowingNew(false);
    
    if (idex >= 10 && !UserDefault::getInstance()->getBoolForKey(lImageBox->getImageName().c_str()))
    {
        lImageBox->setIsShowingNew(true);
    }
    
    return cell;
}

ssize_t ChooseMapLayer::numberOfCellsInTableView(TableView* table)
{
    return (int)(DataManager::getInstance()->_resourceInfos.size()+ImageDownload::getInstance()->_downloadFinishResourceInfo.size());
}

void ChooseMapLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
//    ImageBox* lImageBox = dynamic_cast<ImageBox*>(cell->getChildByTag(st_cell_imagebox_tag));
//    int index = lImageBox->getIndex();
//    
//    Scene* scene = GameScene::createScene(index);
//    Director::getInstance()->replaceScene(scene);
}

void ChooseMapLayer::tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch)
{
    ImageBox* lImageBox = dynamic_cast<ImageBox*>(cell->getChildByTag(st_cell_imagebox_tag));
    int index = lImageBox->getIndex();
    
    UserDefault::getInstance()->setBoolForKey(lImageBox->getImageName().c_str(), true);
    
    Scene* scene = GameScene::createScene(index);
    Director::getInstance()->replaceScene(scene);
}