//
//  SignatureLayer.cpp
//  IMG1001
//
//  Created by houhuang on 15/10/27.
//
//

#include "SignatureLayer.hpp"
#include "../../helper/ScreenAdapterHelper.h"
#include "../EditLayer.h"
#include "../../data/DataManager.h"
#include "../EditLayer.h"
#include "../../helper/DeviceHelper.h"
enum
{
    st_signature_btn_keyboard = 10,
    st_signature_btn_font,
    st_signature_btn_color,
    st_signature_btn_done,
};

SignatureLayer* SignatureLayer::create()
{
    SignatureLayer* layer = new SignatureLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

SignatureLayer::~SignatureLayer()
{
    this->removeListener();
}

SignatureLayer::SignatureLayer()
{
    m_pBtnColor = NULL;
    m_pBtnDone = NULL;
    m_pBtnFont = NULL;
    m_pBtnKeyboard = NULL;
    
    m_vThisOrginVec = Vec2(0, 0);
    
    m_pTableView = NULL;
    m_pLayerColor = NULL;
    m_pPreColorSprite = NULL;
    m_cCurrentColor = Color3B(0, 0, 0);
    
    m_vFontTableView.clear();
    m_bIsCreateThis = true;
    m_pFontPre = NULL;
}

bool SignatureLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 225))) return false;
    
    
    this->setContentSize(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(700)));
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 1));
    
    LayerColor* lLayerColor = LayerColor::create(Color4B(255, 255, 255, 25), ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(80));
    lLayerColor->ignoreAnchorPointForPosition(false);
    lLayerColor->setAnchorPoint(Vec2(0.5, 1));
    lLayerColor->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    this->addChild(lLayerColor);
    

    MenuItemSprite* keyboard = MenuItemSprite::create(Sprite::create("keyboard_active.png"), NULL, CC_CALLBACK_1(SignatureLayer::doAction, this));
    keyboard->setPosition(Vec2(lLayerColor->getContentSize().width * 0.1, lLayerColor->getContentSize().height/2));
    keyboard->setTag(st_signature_btn_keyboard);
    keyboard->setScale(1.5f);
    m_pBtnKeyboard = keyboard;
    
    MenuItemSprite* font = MenuItemSprite::create(Sprite::create("font_normal.png"), NULL, CC_CALLBACK_1(SignatureLayer::doAction, this));
    font->setPosition(Vec2(lLayerColor->getContentSize().width * 0.25, lLayerColor->getContentSize().height/2));
    font->setTag(st_signature_btn_font);
    font->setScale(1.5f);
    m_pBtnFont = font;
    
    MenuItemSprite* color = MenuItemSprite::create(Sprite::create("color_normal.png"), NULL, CC_CALLBACK_1(SignatureLayer::doAction, this));
    color->setPosition(Vec2(lLayerColor->getContentSize().width * 0.4, lLayerColor->getContentSize().height/2));
    color->setTag(st_signature_btn_color);
    color->setScale(1.5f);
    m_pBtnColor = color;
    
    MenuItemSprite* done = MenuItemSprite::create(Sprite::create("done_normal.png"), Sprite::create("done_active.png"), CC_CALLBACK_1(SignatureLayer::doAction, this));
    done->setPosition(Vec2(lLayerColor->getContentSize().width * 0.9, lLayerColor->getContentSize().height/2));
    done->setTag(st_signature_btn_done);
    done->setScale(1.5f);
    m_pBtnDone = done;

    Menu* lMenu = Menu::create(keyboard, font, color, done, NULL);
    lMenu->setPosition(Vec2::ZERO);
    lLayerColor->addChild(lMenu);
    
    this->addListener();
    
    this->createFontTableView();
    this->createColorLayer();
    return true;
}

void SignatureLayer::createFontTableView()
{
    float tableHight = 0.0;
    
//    log("%d,%d",st_DeviceType_iPhoneOther,DeviceHelper::getInstance()->getCurrentDeviceType());
    
    switch (DeviceHelper::getInstance()->getCurrentDeviceType())
    {
        case st_DeviceType_iPhone4:
        {
            tableHight = ScreenAdapterHelper::getValue(455);
        }
            break;
        case st_DeviceType_iPhoneOther:
        {
            tableHight = ScreenAdapterHelper::getValue(385);
        }
            break;
        case st_DeviceType_iPhonePlus:
        {
            tableHight = ScreenAdapterHelper::getValue(390);
        }
            break;
        case st_DeviceType_iPad:
        {
            tableHight = ScreenAdapterHelper::getValue(300);
        }
            break;
            
        case st_DeviceType_Android_iphone:
        {
            tableHight = ScreenAdapterHelper::getValue(450);
        }
            break;
            
        case st_DeviceType_Android_ipad:
        {
            tableHight = ScreenAdapterHelper::getValue(450);
        }
            break;
            
        default:
            break;
    }
    
    TableView* table = TableView::create(this, Size(ScreenAdapterHelper::getValue(768), tableHight* ScreenAdapterHelper::getRealDesignScale()) );
    table->ignoreAnchorPointForPosition(false);
    table->setAnchorPoint(Vec2(0.5, 1));
    table->setDirection(ScrollView::Direction::VERTICAL);
    table->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    
    table->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height * 0.885));
    this->addChild(table);
    table->setDelegate(this);
    table->reloadData();
    m_pTableView = table;
    
    table->setVisible(false);
}

void SignatureLayer::createColorLayer()
{
    LayerColor* lLayerColor = LayerColor::create(Color4B(0, 0, 0, 0));
    lLayerColor->setContentSize(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getRealDesignScale() * this->getContentSize().height * 0.4));
    lLayerColor->ignoreAnchorPointForPosition(false);
    lLayerColor->setAnchorPoint(Vec2(0.5, 1));
    lLayerColor->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height * 0.925));
    lLayerColor->setVisible(false);
    m_pLayerColor = lLayerColor;
    
    this->addChild(lLayerColor);
    
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            if ((i * 7 + j >= DataManager::getInstance()->m_vSignatureColorInfo.size()))
            {
                return;
            }
            
            ColorData lColorData;;
            lColorData = DataManager::getInstance()->m_vSignatureColorInfo.at(i * 7 + j);
            
            Color3B color = Color3B(lColorData.R, lColorData.G, lColorData.B);
            
            ColorSprite* lSprite;
            if (color.r == 0 && color.g == 0 && color.b == 0)
            {
                lSprite = ColorSprite::create("sqare_color.png", "signature_color_black.png");
            }else
            {
                lSprite = ColorSprite::create("sqare_color.png", "signature_color.png");
            }
            
            
            
            if (DeviceHelper::getInstance()->getCurrentDeviceType() == st_DeviceType_iPad)
            {
                lSprite->setPosition(Vec2(ScreenAdapterHelper::getValue(13.33) * j + ScreenAdapterHelper::getValue(83.5) * j + lSprite->getContentSize().width/2 + ScreenAdapterHelper::getValue(50), lLayerColor->getContentSize().height - ( (lLayerColor->getContentSize().height - ScreenAdapterHelper::getValue(80) )/2 * i + ScreenAdapterHelper::getValue(55) + lSprite->getContentSize().height/2 )));
            }else
            {
                lSprite->setPosition(Vec2(ScreenAdapterHelper::getValue(13.33) * j + ScreenAdapterHelper::getValue(83.5) * j + lSprite->getContentSize().width/2 + ScreenAdapterHelper::getValue(50), lLayerColor->getContentSize().height - ( (lLayerColor->getContentSize().height - ScreenAdapterHelper::getValue(100) )/2 * i + ScreenAdapterHelper::getValue(55) + lSprite->getContentSize().height/2 )));
                
            }
            
            lSprite->setScale(1.1f);
            lSprite->addListener(callfuncN_selector(SignatureLayer::reponseTextColor), this);
            lSprite->setColor(color);
            lSprite->setColorData(lColorData);
            m_vTextColor.push_back(lSprite);
            
            lLayerColor->addChild(lSprite);
            
            if (color.r == 255 && color.g == 195 && color.b == 0)
            {
                m_cCurrentColor = color;
                m_pPreColorSprite = LayerColor::create(Color4B(255, 255, 255, 255), ScreenAdapterHelper::getValue(40), ScreenAdapterHelper::getValue(40));
                m_pPreColorSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
                m_pPreColorSprite->ignoreAnchorPointForPosition(false);
                m_pPreColorSprite->setAnchorPoint(Vec2(0.5, 0.5));
                m_pPreColorSprite->setOpacity(0);
                lSprite->addChild(m_pPreColorSprite);
                m_pPreColorSprite->runAction(FadeIn::create(0.5f));
            }
        }
    }
}

void SignatureLayer::reponseTextColor(Node* pNode)
{
    ColorSprite* lColorSprite = (ColorSprite*)pNode;
    
    if (m_pPreColorSprite)
    {
        m_pPreColorSprite->removeFromParentAndCleanup(true);
        m_pPreColorSprite = NULL;
    }
    
    if (lColorSprite->getColorData().R > 250 && lColorSprite->getColorData().G > 250 && lColorSprite->getColorData().B > 250)
    {
        m_pPreColorSprite = LayerColor::create(Color4B(0, 0, 0, 255), ScreenAdapterHelper::getValue(40), ScreenAdapterHelper::getValue(40));
    }else
    {
        m_pPreColorSprite = LayerColor::create(Color4B(255, 255, 255, 255), ScreenAdapterHelper::getValue(40), ScreenAdapterHelper::getValue(40));
    }
    
    
    m_pPreColorSprite->setPosition(Vec2(lColorSprite->getContentSize().width/2, lColorSprite->getContentSize().height/2));
    m_pPreColorSprite->ignoreAnchorPointForPosition(false);
    m_pPreColorSprite->setAnchorPoint(Vec2(0.5, 0.5));
    m_pPreColorSprite->setOpacity(0);
    lColorSprite->addChild(m_pPreColorSprite);
    
    m_pPreColorSprite->runAction(FadeIn::create(0.5f));
    
    ColorData lColorData = lColorSprite->getColorData();
    Color3B color = Color3B(lColorData.R, lColorData.G, lColorData.B);
    EditLayer* lEditLayer = dynamic_cast<EditLayer*>(this->getParent());
    lEditLayer->updateTextColor(color);
    m_cCurrentColor = color;
}

void SignatureLayer::doAction(Ref* ref)
{
    MenuItemSprite* lMenuItem = (MenuItemSprite*)ref;
    EditLayer* lEditLayer = dynamic_cast<EditLayer*>(this->getParent());
    
    switch (lMenuItem->getTag())
    {
        case st_signature_btn_keyboard:
        {
            this->initMenuState();
            lMenuItem->setNormalImage(Sprite::create("keyboard_active.png"));
            
            lEditLayer->openKeyboard();
            lEditLayer->updateCurrentIsKeyboard(true);
        }
            break;
            
        case st_signature_btn_font:
        {
            DataManager::m_bIsClickTextFont = true;
            this->initMenuState();
            lMenuItem->setNormalImage(Sprite::create("font_active.png"));
            
            m_pTableView->setVisible(true);
            lEditLayer->updateCurrentIsKeyboard(false);
        }
            break;
            
        case st_signature_btn_color:
        {
            DataManager::m_bIsClickTextColor = true;
            this->initMenuState();
            lMenuItem->setNormalImage(Sprite::create("color_active.png"));
            
            m_pLayerColor->setVisible(true);
            lEditLayer->updateCurrentIsKeyboard(false);
        
            //color not choose
            for (int i = 0; i < m_vTextColor.size(); ++i)
            {
                ColorSprite* lColorSprite = m_vTextColor.at(i);
                lColorSprite->setEnableTouch(true);
            }
        }
            break;
            
        case st_signature_btn_done:
        {
            DataManager::m_bIsOpenKeyboard = false;
            
            lMenuItem->setEnabled(false);
            
            lEditLayer->closeKeyboard();
            lEditLayer->hideSignatureLayer();
//            lEditLayer->setFileNodeBGVisible(false);
            
            this->runAction(EaseExponentialInOut::create(MoveTo::create(0.35, m_vThisOrginVec)));
            this->scheduleOnce(schedule_selector(SignatureLayer::delayeResumeBtn), 0.5f);
        }
            break;
            
        default:
            break;
    }
}

void SignatureLayer::hideThisLayer()
{
    DataManager::m_bIsOpenKeyboard = false;

    this->runAction(EaseExponentialInOut::create(MoveTo::create(0.35, m_vThisOrginVec)));
}

void SignatureLayer::delayeResumeBtn(float dt)
{
    m_pBtnDone->setEnabled(true);
}

void SignatureLayer::initMenuState()
{
    m_pBtnKeyboard->setNormalImage(Sprite::create("keyboard_normal.png"));
    m_pBtnFont->setNormalImage(Sprite::create("font_normal.png"));
    m_pBtnColor->setNormalImage(Sprite::create("color_normal.png"));
    
    EditLayer* lEditLayer = dynamic_cast<EditLayer*>(this->getParent());
    lEditLayer->closeKeyboard();
    
    m_pTableView->setVisible(false);
    m_pLayerColor->setVisible(false);
    
    //color not choose
    for (int i = 0; i < m_vTextColor.size(); ++i)
    {
        ColorSprite* lColorSprite = m_vTextColor.at(i);
        lColorSprite->setEnableTouch(false);
    }
}

void SignatureLayer::onEnterTransitionDidFinish()
{
    m_vThisOrginVec = this->getPosition();
}

void SignatureLayer::addListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)
    {
//        Vec2 lPoint = touch->getLocation();
        Vec2 lPoint = this->getParent()->convertTouchToNodeSpace(touch);
        if (this->getBoundingBox().containsPoint(lPoint))
        {
            
            return true;
        }
        
        return false;
    };
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SignatureLayer::removeListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

# pragma mark Font TableView
Size SignatureLayer::cellSizeForTable(TableView* table)
{
    return Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(80));
}

TableViewCell* SignatureLayer::tableCellAtIndex(TableView* table, ssize_t idex)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    
    cell->removeAllChildrenWithCleanup(true);
//    if (cell->getChildren().size() == 0)
    {
        FontTypeInfo lFontTypeInfo = DataManager::getInstance()->m_vFontTypeInfo.at(idex);
        
        Label* label = Label::createWithTTF(lFontTypeInfo.fontName.c_str(), lFontTypeInfo.fontTypeFileName.c_str(), ScreenAdapterHelper::getValue(60));
        label->ignoreAnchorPointForPosition(false);
        label->setAnchorPoint(Vec2(0.5, 0.5));
        label->setPosition(Vec2(ScreenAdapterHelper::getValue(384), ScreenAdapterHelper::getValue(30)));
        label->setTag(lFontTypeInfo.ID);
        cell->addChild(label);
        
        if (lFontTypeInfo.fontIsWhite)
        {
            label->setColor(Color3B::WHITE);
        }else
        {
            label->setColor(Color3B(236, 64, 122));
            m_pFontPre = label;
            m_pFontPre->retain();
        }
        
//        for (int i = 0; i < m_vFontTableView.size(); ++i)
//        {
//            if (m_vFontTableView.at(i)->getTag() == lFontTypeInfo.ID)
//            {
//                return cell;
//            }
//        }
//        
//        
//        m_vFontTableView.push_back(label);
 
    }
//    else
//    {
//        Label* label = dynamic_cast<Label*>(cell->getChildren().at(0));
//        if (label->getTag() != idex)
//        {
//            cell->removeAllChildrenWithCleanup(true);
//            FontTypeInfo lFontTypeInfo = DataManager::getInstance()->m_vFontTypeInfo.at(idex);
//            
//            Label* label = Label::createWithTTF(lFontTypeInfo.fontName.c_str(), lFontTypeInfo.fontTypeFileName.c_str(), ScreenAdapterHelper::getValue(60));
//            label->ignoreAnchorPointForPosition(false);
//            label->setAnchorPoint(Vec2(0.5, 0.5));
//            label->setPosition(Vec2(ScreenAdapterHelper::getValue(384), ScreenAdapterHelper::getValue(30)));
//            label->setTag(idex);
//            cell->addChild(label);
//            
//            if (idex == 0)
//            {
//                label->setColor(Color3B(236, 64, 122));
//            }
//            
////            m_vFontTableView.push_back(label);
//        }
//    }

    cell->setTag(idex);
    return cell;
}

ssize_t SignatureLayer::numberOfCellsInTableView(TableView* table)
{
    return DataManager::getInstance()->m_vFontTypeInfo.size();
}

void SignatureLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    FontTypeInfo* lFontTypeInfo = &DataManager::getInstance()->m_vFontTypeInfo.at(cell->getIdx());
    EditLayer* lEditLayer = dynamic_cast<EditLayer*>(this->getParent());
    lEditLayer->updateTextFont(lFontTypeInfo->fontTypeFileName.c_str());
 
//    for (int i = 0; i < m_vFontTableView.size(); ++i)
//    {
//        Label* label = m_vFontTableView.at(i);
//        label->setColor(Color3B::WHITE);
//        label->setScale(2.0f);
////        m_vFontTableView.at(i)->release();
//    
//    }
    
    if (m_pFontPre)
    {
        m_pFontPre->setColor(Color3B::WHITE);
        m_pFontPre->release();
        m_pFontPre = NULL;
    }
    
//    for (int i = 0; i < m_pTableView->getChildren().size(); ++i)
//    {
//        TableViewCell* lCell = dynamic_cast<TableViewCell*>(m_pTableView->getChildByTag(i));
//        Label*  label = dynamic_cast<Label*>(lCell->getChildren().at(0));
//        label->setColor(Color3B::WHITE);
//    }
    
    for (int i = 0; i < DataManager::getInstance()->m_vFontTypeInfo.size(); ++i)
    {
        FontTypeInfo* lFontTypeInfo = &DataManager::getInstance()->m_vFontTypeInfo.at(i);
        lFontTypeInfo->fontIsWhite = true;
    }
    
    Label* lLabel = dynamic_cast<Label*>(cell->getChildren().at(0));
    lLabel->setColor(Color3B(236, 64, 122));
    
    lFontTypeInfo->fontIsWhite = false;
    m_pFontPre = lLabel;
    m_pFontPre->retain();
    
}




