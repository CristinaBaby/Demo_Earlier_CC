//
//  ScrollColorLayer.cpp
//  ColorGarden
//
//  Created by lidong on 15/8/4.
//
//

#include "ScrollColorLayer.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../helper/ControlHelper.h"
#include "../commpoent/ColorSprite.h"
#include "../game/DataManager.h"
#include "../define/ResoureMacro.h"
#include "../game/GameUILayer.h"
#include "../helper/DeviceHelper.h"
#define st_scroll_number 36
#define st_move_distance 100
#define st_dot_distance 10

USING_NS_CC_EXT;

enum
{
  eColorSpriteTag=10,
  eNewIconSpriteTag = 11,
};

bool ScrollColorLayer::init()
{
    if(!Layer::init()) return false;
    
    m_bIsFirstCreateLayer = true;
    
    _isShowingTutorial=DataManager::getInstance()->isShowingTutorial();
    
    if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Pad)
    {
        _colorDatas = DataManager::getInstance()->readCsvData("ColorRGB_pad.csv");
    }else
    {
        _colorDatas = DataManager::getInstance()->readCsvData("ColorRGB_phone.csv");
    }
    
    int colorHeight = ScreenAdapterHelper::getValue(270);
    int tableHeightOff = 0.0f;
    if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Phone)
    {
        colorHeight = ScreenAdapterHelper::getValue(340);
        tableHeightOff = 70.0f;
    }
    
    LayerColor* layerColor=LayerColor::create(Color4B(49, 49, 49, 255), ScreenAdapterHelper::getRealDesignSize().width, colorHeight);
    this->addChild(layerColor);
    
    Sprite* lResSprite=ColorSprite::create("color01.png");
    _targetSize=lResSprite->getContentSize();
    
    float lWidth=768;
    float lDistance=lWidth*1.0f/this->getPageNum();
    _distance=lDistance;

    TableView *tableView=TableView::create(this, Size(ScreenAdapterHelper::getValue(768), lResSprite->getContentSize().height*1.4 + ScreenAdapterHelper::getValue(tableHeightOff)));
    tableView->setDirection(ScrollView::Direction::HORIZONTAL);
    ScreenAdapterHelper::setPosition(tableView, Vec2(0, 130),false,true,false,false);
    tableView->setAnchorPoint(Vec2(0, 0));
    tableView->setVerticalFillOrder(cocos2d::extension::TableView::VerticalFillOrder::TOP_DOWN);
    tableView->setDelegate(this);
    this->addChild(tableView);
    tableView->reloadData();
    m_pTableView = tableView;
    
    tableView->setContentOffset(Vec2(1000, 0));
    tableView->setContentOffsetInDuration(Vec2(-ScreenAdapterHelper::getValue(lDistance*3+lDistance/2),0),1.0f);
    
    
    return true;
}

int ScrollColorLayer::getPageNum()
{
    return 6;
}


void ScrollColorLayer::reponseTouchEvent(ColorSprite* pColorSprite)
{
    if(pColorSprite->getIsSelected())
    {
        DataManager::getInstance()->isAlreadySelectedColor=true;
        DataManager::getInstance()->selectedColorData=pColorSprite->getColorData();
    }
}


#pragma mark - TableView

 Size ScrollColorLayer::cellSizeForTable(TableView *table) {
     
     if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Pad)
     {
         return  Size(ScreenAdapterHelper::getValue(_distance),_targetSize.height*1.4);
     }else
     {
         return  Size(ScreenAdapterHelper::getValue(_distance),_targetSize.height*1.4*2);
     }
    
};

TableViewCell* ScrollColorLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{

    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new TableViewCell();
        cell->autorelease();
    }

    cell->removeAllChildrenWithCleanup(true);

    if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Pad)
    {
        ColorSprite* lSprite=ColorSprite::create("color01.png");
        lSprite->setDelegate(this);
        lSprite->setPosition(Vec2(ScreenAdapterHelper::getValue(_distance/2),lSprite->getContentSize().height/2+lSprite->getContentSize().height/4));
        ColorData lColorData=_colorDatas.at(idx);
        lSprite->setTag(eColorSpriteTag);
        lSprite->setTag(6);
        lSprite->addListener(callfuncN_selector(ScrollColorLayer::reponseColorSpriteTouchEvent), this);
        
        lSprite->setColor(Color3B(lColorData.R, lColorData.G, lColorData.B));
        lSprite->setColorData(lColorData);
        lSprite->setClickSpite(false);
        
        if (lColorData.R == 109 && lColorData.G == 201 && lColorData.B == 243 && m_bIsFirstCreateLayer)
        {
            m_bIsFirstCreateLayer = false;
            lSprite->responseTouchEvent(Vec2(0, 0));
        }
        
        stringstream ss;
        ss<<"";
        ss<<lColorData.R;
        ss<<lColorData.G;
        ss<<lColorData.B;
        
        if (!UserDefault::getInstance()->getBoolForKey(ss.str().c_str()) && lColorData.isNewColor == true)
        {
            Sprite* newIcon = Sprite::create("point.png");
            newIcon->setAnchorPoint(Vec2(1.05, 1.05));
            newIcon->setPosition(Vec2(lSprite->getContentSize().width, lSprite->getContentSize().height));
            newIcon->setTag(eNewIconSpriteTag);
            lSprite->addChild(newIcon);
        }
        
        
        //    LayerColor* layerColor=LayerColor::create(Color4B(lColorData.R, lColorData.G, lColorData.G, 255));
        //    layerColor->setContentSize(Size(ScreenAdapterHelper::getValue(_distance),_targetSize.height*2));
        //    cell->addChild(layerColor);
        
        cell->addChild(lSprite);
        
        if(DataManager::getInstance()->isAlreadySelectedColor)
        {
            ColorData lTargetColorData=DataManager::getInstance()->selectedColorData;
            if(lColorData.R==lTargetColorData.R && lColorData.G==lTargetColorData.G && lColorData.B==lTargetColorData.B)
            {
                lSprite->responseTouchEvent(Vec2(0, 0));
            }
        }
    }else
    {
        ColorSprite* lSprite=ColorSprite::create("color01.png");
        lSprite->setDelegate(this);
        lSprite->setPosition(Vec2(ScreenAdapterHelper::getValue(_distance/2),lSprite->getContentSize().height/2+lSprite->getContentSize().height/4+ _targetSize.height*1.4 + ScreenAdapterHelper::getValue(5)));
        ColorData lColorData=_colorDatas.at(2 * idx);
        lSprite->setTag(6);
        lSprite->addListener(callfuncN_selector(ScrollColorLayer::reponseColorSpriteTouchEvent), this);
        
        lSprite->setColor(Color3B(lColorData.R, lColorData.G, lColorData.B));
        lSprite->setColorData(lColorData);
        lSprite->setClickSpite(false);
        
        if (lColorData.R == 109 && lColorData.G == 201 && lColorData.B == 243 && m_bIsFirstCreateLayer)
        {
            m_bIsFirstCreateLayer = false;
            lSprite->responseTouchEvent(Vec2(0, 0));
        }
        
        ColorSprite* lSprite2=ColorSprite::create("color01.png");
        lSprite2->setDelegate(this);
        lSprite2->setPosition(Vec2(ScreenAdapterHelper::getValue(_distance/2),lSprite->getContentSize().height/2+lSprite->getContentSize().height/2 + ScreenAdapterHelper::getValue(20)));
        ColorData lColorData2=_colorDatas.at(2 * idx + 1);
        lSprite2->setTag(7);
        
        lSprite2->setColor(Color3B(lColorData2.R, lColorData2.G, lColorData2.B));
        lSprite2->setColorData(lColorData2);
        lSprite2->addListener(callfuncN_selector(ScrollColorLayer::reponseColorSpriteTouchEvent), this);
 
        lSprite2->setClickSpite(false);
        
        stringstream ss;
        ss<<"";
        ss<<lColorData.R;
        ss<<lColorData.G;
        ss<<lColorData.B;
        
        if (!UserDefault::getInstance()->getBoolForKey(ss.str().c_str()) && lColorData.isNewColor == true)
        {
            Sprite* newIcon = Sprite::create("point.png");
            newIcon->setAnchorPoint(Vec2(1.05, 1.05));
            newIcon->setPosition(Vec2(lSprite->getContentSize().width, lSprite->getContentSize().height));
            newIcon->setTag(eNewIconSpriteTag);
            lSprite->addChild(newIcon);
        }
        
        stringstream ss1;
        ss1<<"";
        ss1<<lColorData2.R;
        ss1<<lColorData2.G;
        ss1<<lColorData2.B;
        
        if (!UserDefault::getInstance()->getBoolForKey(ss1.str().c_str()) && lColorData2.isNewColor == true)
        {
            Sprite* newIcon = Sprite::create("point.png");
            newIcon->setAnchorPoint(Vec2(1.05, 1.05));
            newIcon->setPosition(Vec2(lSprite->getContentSize().width, lSprite->getContentSize().height));
            newIcon->setTag(eNewIconSpriteTag);
            lSprite2->addChild(newIcon);
        }
        
        //    LayerColor* layerColor=LayerColor::create(Color4B(lColorData.R, lColorData.G, lColorData.G, 255));
        //    layerColor->setContentSize(Size(ScreenAdapterHelper::getValue(_distance),_targetSize.height*2));
        //    cell->addChild(layerColor);
        
        cell->addChild(lSprite);
        cell->addChild(lSprite2);
        
        if(DataManager::getInstance()->isAlreadySelectedColor)
        {
            ColorData lTargetColorData=DataManager::getInstance()->selectedColorData;
            if(lColorData.R==lTargetColorData.R && lColorData.G==lTargetColorData.G && lColorData.B==lTargetColorData.B)
            {
                lSprite->responseTouchEvent(Vec2(0, 0));
            }
            
            if(lColorData2.R==lTargetColorData.R && lColorData2.G==lTargetColorData.G && lColorData2.B==lTargetColorData.B)
            {
                lSprite2->responseTouchEvent(Vec2(0, 0));
            }
        }
    }
    
    return cell;
}

ssize_t ScrollColorLayer::numberOfCellsInTableView(TableView *table)
{
//    return st_scroll_number;
    if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Phone)
    {
        return (ssize_t)_colorDatas.size()/2;
    }else
    {
        return (ssize_t)_colorDatas.size();
    }
    return (ssize_t)_colorDatas.size();
}

void ScrollColorLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
//    for (int i=0; i<_colorDatas.size(); i++) {
//        if(i!=cell->getIdx())
//        {
//            TableViewCell* lCell=table->cellAtIndex(i);
//            if(lCell)
//            {
//                ColorSprite* lSprite=(ColorSprite*)lCell->getChildByTag(eColorSpriteTag);
//                lSprite->selected(false);
//            }
//        }
//    }
//    
//    ColorSprite* lSprite=(ColorSprite*)cell->getChildByTag(eColorSpriteTag);
//    lSprite->responseTouchEvent(Vec2(0, 0));
//    
//    ColorData lColorData = lSprite->getColorData();
//    Sprite* lNewColorSpr = (Sprite*)lSprite->getChildByTag(eNewIconSpriteTag);
//    
//    stringstream ss;
//    ss<<"";
//    ss<<lColorData.R;
//    ss<<lColorData.G;
//    ss<<lColorData.B;
//    
//    if (lNewColorSpr)
//    {
//        lNewColorSpr->removeFromParentAndCleanup(true);
//        UserDefault::getInstance()->setBoolForKey(ss.str().c_str(), true);
//    }
}

void ScrollColorLayer::reponseColorSpriteTouchEvent(Node* node)
{
    for (int i = 0 ; i < this->numberOfCellsInTableView(m_pTableView); ++i)
    {
        TableViewCell* cell = dynamic_cast<TableViewCell*>(m_pTableView->cellAtIndex(i));
        if (cell)
        {
            ColorSprite* lSprite1 = dynamic_cast<ColorSprite*>(cell->getChildByTag(6));
            ColorSprite* lSprite2 = dynamic_cast<ColorSprite*>(cell->getChildByTag(7));
            
            lSprite1->selected(false);
            if (lSprite2)
            {
                lSprite2->selected(false);
            }
        }
    }

    ColorSprite* lSprite=(ColorSprite*)node;
    lSprite->responseTouchEvent(Vec2(0, 0));
    
    ColorData lColorData = lSprite->getColorData();
    Sprite* lNewColorSpr = (Sprite*)lSprite->getChildByTag(eNewIconSpriteTag);
    
    stringstream ss;
    ss<<"";
    ss<<lColorData.R;
    ss<<lColorData.G;
    ss<<lColorData.B;
    
    if (lNewColorSpr)
    {
        lNewColorSpr->removeFromParentAndCleanup(true);
        UserDefault::getInstance()->setBoolForKey(ss.str().c_str(), true);
    }
}

