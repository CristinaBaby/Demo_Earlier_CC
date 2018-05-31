//
//  EditLayer.cpp
//  ColorFree
//
//  Created by houhuang on 15/9/8.
//
//

#include "EditLayer.h"
#include "../data/DataManager.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../define/Globalmacro.h"
//#include "../compoment/CurrentImageSprite.h"
#include "../compoment/FramesSprite.h"
#include "FilterAndFrameLayer.h"
#include "renderer/CCTexture2D.h"
#include "../define/AppConfigs.h"
//#include "../IOS_Android_Include/AdsManager.h"
#include "../crossplatformapi/headers/ads/AdsManager.h"
//#include "../modules/AnalyticX.h"
#include "Signature/TextFieldNode.h"
#include "../helper/DeviceHelper.h"

enum
{
    st_editlayer_frame_tag = 100,
    st_editlayer_filters_tag,
    st_editlayer_textBtn,
    st_editlayer_filterBtn,
    st_editlayer_frameBtn,
    
    st_editlayer_table_tag = 111,
    st_editlayer_newIcon_tag,
    st_editlayer_label_tag,
};

enum
{
    eOverLay_Fliter_Tag=1,
   // eRouGuang_Filter_Tag,
   // eKelvin_Filter_Tag,
    //e1977_Filter_Tag,
    eNashville_Filter_Tag,
    eValencial_Filter_Tag,
    // eWalden_Filter_Tag,
    //eHefe_Filter_Tag=2,
    eOverLay2_Fliter_Tag,
    eOverLay3_Fliter_Tag
};

EditLayer::~EditLayer()
{
//    m_pMoreFilterSpr->release();
//    m_pCurrentSprite->release();
}

EditLayer* EditLayer::create(Sprite* pSprite)
{
    EditLayer* layer = new EditLayer();
    if (layer && layer->init(pSprite))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

EditLayer::EditLayer()
{
    m_iChooseFramesIndex = 0;
    m_iChooseFilterIndex = 0;
    
    m_bIsFilters = true;
    
    _currentIdx=-1;

    m_pCurrentSprite = NULL;
    
    m_pMoreFilterSpr = NULL;
    m_pLoadLayer = NULL;

    m_pCurrentFilterName = "";
    m_pCurrentFramesName = "";

    m_pSignture = NULL;
    m_pMenuItemText = NULL;
    
    m_vSignatureOrginVec = Vec2(0, 0);
    m_pLabelText = NULL;
    m_pLabelFilters = NULL;
    m_pLabelFrames = NULL;
    m_pLabelTitle = NULL;
    m_pFilelNode = NULL;
    m_pVirtualFrame = NULL;
    
    m_bCurrentIsKeyboard = true;
    
    m_vFileNodeOrignalVec = Vec2(0, 0);
    m_bFileNodeReadyMove = false;
}

bool EditLayer::init(Sprite* pSprite)
{
    if (!Layer::init()) return false;

    m_pLabelTitle = Label::createWithTTF("", st_font_AR, ScreenAdapterHelper::getValue(50));
    ScreenAdapterHelper::setPosition(m_pLabelTitle, Vec2(384, 50), KVBoarderTypeTop, KHBoarderTypeNone);
    m_pLabelTitle->setColor(Color3B::BLACK);
    if (DataManager::m_bIsFilters)
    {
        m_pLabelTitle->setString(DM_getInstance->getLanguageMes()["Filters"]);
    }else
    {
        m_pLabelTitle->setString(DM_getInstance->getLanguageMes()["Frames"]);
    }
    this->addChild(m_pLabelTitle);
    
    m_pCurrentSprite = Sprite::createWithTexture(pSprite->getTexture());

    m_pMoreFilterSpr = Sprite::createWithTexture(pSprite->getTexture());


    ScreenAdapterHelper::setPosition(m_pCurrentSprite, Vec2(384, 590));
    m_pCurrentSprite->setScale(0.6f);
    this->addChild(m_pCurrentSprite);


    m_pTableView = TableView::create(this, Size(ScreenAdapterHelper::getValue(650), ScreenAdapterHelper::getValue(160)));
    m_pTableView->setDirection(ScrollView::Direction::HORIZONTAL);
    m_pTableView->ignoreAnchorPointForPosition(false);
    m_pTableView->setAnchorPoint(Vec2(0.5, 0));
    m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    m_pTableView->setDelegate(this);
    m_pTableView->setTag(st_editlayer_table_tag);
    
    ScreenAdapterHelper::setPosition(m_pTableView, Vec2(384, 100), KVBoarderTypeBottom, KHBoarderTypeNone);
    this->addChild(m_pTableView);
    m_pTableView->reloadData();
    
    m_pTableView->setContentOffset(Vec2(m_pTableView->minContainerOffset().x, 0));
    m_pTableView->setContentOffsetInDuration(Vec2(0, 0), 0.01f);

    MenuItemImage* text = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(EditLayer::doAction, this));
    ScreenAdapterHelper::setPosition(text, Vec2(768/4 * 3, 50), KVBoarderTypeBottom, KHBoarderTypeNone);
    text->setTag(st_editlayer_textBtn);
    m_pMenuItemText = text;
    
    m_pLabelText = Label::createWithTTF(DM_getInstance->getLanguageMes()["Text"], st_font_AR, ScreenAdapterHelper::getValue(40));
    m_pLabelText->setPosition(Vec2(text->getContentSize().width/2, text->getContentSize().height/2));
    m_pLabelText->setAnchorPoint(Vec2(0.5, 0.5));
    m_pLabelText->setColor(Color3B::BLACK);
    text->addChild(m_pLabelText);
    
    MenuItemImage* filters = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(EditLayer::doAction, this));
    ScreenAdapterHelper::setPosition(filters, Vec2(768/4, 50), KVBoarderTypeBottom, KHBoarderTypeNone);
    filters->setTag(st_editlayer_filterBtn);
    
    m_pLabelFilters = Label::createWithTTF(DM_getInstance->getLanguageMes()["Filters"], st_font_AR, ScreenAdapterHelper::getValue(40));
    m_pLabelFilters->setPosition(Vec2(filters->getContentSize().width/2, filters->getContentSize().height/2));
    m_pLabelFilters->setAnchorPoint(Vec2(0.5, 0.5));
    m_pLabelFilters->setColor(Color3B(236, 64, 122));
    filters->addChild(m_pLabelFilters);
    
    MenuItemImage* frames = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(EditLayer::doAction, this));
    ScreenAdapterHelper::setPosition(frames, Vec2(768/4 * 2, 50), KVBoarderTypeBottom, KHBoarderTypeNone);
    frames->setTag(st_editlayer_frameBtn);
    
    m_pLabelFrames = Label::createWithTTF(DM_getInstance->getLanguageMes()["Frames"], st_font_AR, ScreenAdapterHelper::getValue(40));
    m_pLabelFrames->setPosition(Vec2(frames->getContentSize().width/2, frames->getContentSize().height/2));
    m_pLabelFrames->setAnchorPoint(Vec2(0.5, 0.5));
    m_pLabelFrames->setColor(Color3B::BLACK);
    frames->addChild(m_pLabelFrames);
    
    
    Menu* menu = Menu::create(filters, frames, text, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    Sprite* oriFrame = Sprite::create("virtualFrame.png");
    ScreenAdapterHelper::setPosition(oriFrame, Vec2(384, 590));
    oriFrame->setScale(0.6f);
    this->addChild(oriFrame, 100);
    m_pVirtualFrame = oriFrame;
    
    
    //add SignatureLayer
    SignatureLayer* lSignatureLayer = SignatureLayer::create();
    ScreenAdapterHelper::setPosition(lSignatureLayer, Vec2(384, 0), KVBoarderTypeBottom, KHBoarderTypeNone);
    this->addChild(lSignatureLayer, 101);
    m_pSignture = lSignatureLayer;
    
    m_vSignatureOrginVec = lSignatureLayer->getPosition();
    
    
    m_pFilelNode = TextFieldNode::create("signatureBG.png", "zoom.png", "delete.png");
    m_pFilelNode->setPosition(Vec2(oriFrame->getContentSize().width/2, oriFrame->getContentSize().height*0.7));
    m_pFilelNode->setScale(1.5f);
    oriFrame->addChild(m_pFilelNode);
    m_pFilelNode->setVisible(false);
    m_pFilelNode->setTouchEnable(false);
    
    
    return true;
}

void EditLayer::updateImageFilter()
{
    
    
}

void EditLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    if (m_pTableView)
    {
        m_pTableView->reloadData();
    }
    
    if (m_iChooseFilterIndex != 0 && m_pCurrentSprite)
    {
        this->changeSprite(m_pCurrentSprite, m_iChooseFilterIndex);
    }
}

std::string EditLayer::getImageName_w()
{
    if(m_sCurrentImageName.empty())
    {
        
        stringstream ss;
        ss<<"";
        ss<<DataManager::getInstance()->m_pCurrentImage.imageName;
        ss<<DataManager::getInstance()->m_pCurrentImage.ID;
        ss<<"_white.png";
    
        m_sCurrentImageName = ss.str();
    
        string fullPath = FileUtils::getInstance()->getWritablePath() + m_sCurrentImageName;
        if (FileUtils::getInstance()->isFileExist(fullPath))
        {
            Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
        
            m_sCurrentImageName = fullPath;
        }
    }
    return m_sCurrentImageName;
}

Sprite* EditLayer::getShareSprite()
{
    Sprite* lSprite = Sprite::createWithTexture(m_pCurrentSprite->getTexture());
    this->changeSprite(lSprite, m_iChooseFilterIndex);
    
    FramesInfo frameInfo = DataManager::getInstance()->m_vFramesDatasBase.at(m_iChooseFramesIndex);
    Sprite* lFrames = Sprite::create(frameInfo.imageName);
    
    lFrames->addChild(lSprite,-1);
    lSprite->setPosition(lFrames->getContentSize().width/2, lFrames->getContentSize().height/2);
    lSprite->setScale(frameInfo.childScale * 0.75);
    
//    lFrames->setScale(0.8f);

//    Node* lParentNode=m_pFilelNode->getParent();
//    
//    m_pFilelNode->retain();
//    m_pFilelNode->removeFromParent();
//    
//    lSprite->addChild(m_pFilelNode, 10);
//    m_pFilelNode->setScale(1/(frameInfo.childScale * 0.75));
    Node* lParentNode = m_pVirtualFrame->getParent();
    m_pVirtualFrame->retain();
    m_pVirtualFrame->removeFromParent();
    lFrames->addChild(m_pVirtualFrame);
    m_pVirtualFrame->setPosition(Vec2(lFrames->getContentSize().width/2, lFrames->getContentSize().height/2));
    m_pVirtualFrame->setScale(0.75f);
    
    
    Vec2 lOrginPos = lFrames->getPosition();
    
    RenderTexture* lRenderTexture =RenderTexture::create(lFrames->getContentSize().width, lFrames->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    
    lRenderTexture->setClearColor(Color4F(1, 1, 1, 1));
    
    lRenderTexture->begin();
    lFrames->setPosition(Vec2(lFrames->getContentSize().width/2, lFrames->getContentSize().height/2));
    lFrames->visit();

    lRenderTexture->end();
   
    lFrames->setPosition(lOrginPos);


    Sprite* lResSprite=lRenderTexture->getSprite();
    lResSprite->getTexture()->setAntiAliasTexParameters();
    
    
//    m_pFilelNode->removeFromParent();
//    lParentNode->addChild(m_pFilelNode);
//    
//    m_pFilelNode->release();
    
    m_pVirtualFrame->removeFromParent();
    lParentNode->addChild(m_pVirtualFrame, 100);
    m_pVirtualFrame->release();
    ScreenAdapterHelper::setPosition(m_pVirtualFrame, Vec2(384, 590));
    m_pVirtualFrame->setScale(0.6f);
    
    lResSprite->retain();
    lResSprite->removeFromParent();
    return lResSprite;
}

Size EditLayer::cellSizeForTable(TableView* table)
{
    return Size(ScreenAdapterHelper::getValue(120), ScreenAdapterHelper::getValue(155));
}

void EditLayer::doAction(Ref* ref)
{
    MenuItemImage* lMenuItemImage = (MenuItemImage*)ref;
    switch (lMenuItemImage->getTag())
    {
        case st_editlayer_textBtn:
        {
            DataManager::m_bIsClickText = true;
            lMenuItemImage->setEnabled(false);
            
            TextFieldNode::m_bIsOpenKeyboard = true;
            m_pFilelNode->showWhiteBG();
            
            m_pLabelText->setColor(Color3B(236, 64, 122));
            m_pLabelFilters->setColor(Color3B::BLACK);
            m_pLabelFrames->setColor(Color3B::BLACK);
            
            if (m_pFilelNode)
            {
                m_pFilelNode->setVisible(true);
                m_pFilelNode->setTouchEnable(true);
//                m_pFilelNode->setWhiteBGVisible(true);
            }
            
            //不同设备系统键盘高度不同
//            m_pSignture->runAction(EaseExponentialInOut::create(MoveTo::create(0.5f, (m_vSignatureOrginVec + Vec2(0, ScreenAdapterHelper::getRealDesignScale() * ScreenAdapterHelper::getValue(505))))));
            this->showSignatureLayer();
            
            if (m_bCurrentIsKeyboard)
            {
                m_pFilelNode->openIME();
            }

            this->scheduleOnce(schedule_selector(EditLayer::delayeResumeBtn), 0.35f);
        }
            break;
            
        case st_editlayer_filterBtn:
        {
            m_pLabelTitle->setString(DM_getInstance->getLanguageMes()["Filters"]);
            m_pLabelFilters->setColor(Color3B(236, 64, 122));
            m_pLabelFrames->setColor(Color3B::BLACK);
            m_pLabelText->setColor(Color3B::BLACK);
            
            DataManager::m_bIsFilters = true;
        
            this->chooseFilterOrFrames(true);
        }
            break;
            
        case st_editlayer_frameBtn:
        {
            m_pLabelTitle->setString(DM_getInstance->getLanguageMes()["Frames"]);
            m_pLabelFilters->setColor(Color3B::BLACK);
            m_pLabelFrames->setColor(Color3B(236, 64, 122));
            m_pLabelText->setColor(Color3B::BLACK);
            
            DataManager::m_bIsFilters = false;
            
            this->chooseFilterOrFrames(false);
        }
            break;
            
        default:
            break;
    }
}

void EditLayer::resetFieldNode()
{
    m_bCurrentIsKeyboard = true;
    m_pFilelNode->setPosition(Vec2(m_pVirtualFrame->getContentSize().width/2, m_pVirtualFrame->getContentSize().height/2));
    m_pFilelNode->setScale(1.5f);
    
    this->closeKeyboard();
    m_pSignture->hideThisLayer();
    this->scheduleOnce(schedule_selector(EditLayer::rebuildSinature), 0.35f);
}

void EditLayer::rebuildSinature(float dt)
{
    if (m_pSignture)
    {
        m_pSignture->removeFromParentAndCleanup(true);
        m_pSignture = NULL;
        
        m_pSignture = SignatureLayer::create();
        ScreenAdapterHelper::setPosition(m_pSignture, Vec2(384, 0), KVBoarderTypeBottom, KHBoarderTypeNone);
        this->addChild(m_pSignture, 101);
    }
}

void EditLayer::hideSignatureLayer()
{
    if (m_bFileNodeReadyMove)
    {
        m_bFileNodeReadyMove = false;
        m_pFilelNode->setPosition(m_vFileNodeOrignalVec);
    }
    
    this->closeKeyboard();
    m_pSignture->hideThisLayer();
    m_pFilelNode->hideWhiteBG();
    TextFieldNode::m_bIsOpenKeyboard = false;
}

void EditLayer::showSignatureLayer()
{
    switch (DeviceHelper::getInstance()->getCurrentDeviceType())
    {
        case st_DeviceType_iPhone4:
        {
//            log("file node positionY = %f",m_pFilelNode->getPosition().y);
            if (m_pFilelNode->getPosition().y < ScreenAdapterHelper::getValue(400))
            {
                m_bFileNodeReadyMove = true;
                m_vFileNodeOrignalVec = Vec2(m_pFilelNode->getPosition().x, m_pFilelNode->getPosition().y);
                m_pFilelNode->setPosition(Vec2(m_pVirtualFrame->getContentSize().width/2, m_pVirtualFrame->getContentSize().height/2));
            }
            m_pSignture->runAction(EaseExponentialInOut::create(MoveTo::create(0.5f, (m_vSignatureOrginVec + Vec2(0, ScreenAdapterHelper::getRealDesignScale() * ScreenAdapterHelper::getValue(532))))));
        }
            break;
            
        case st_DeviceType_iPhonePlus:
        {
//            log("file node positionY = %f",m_pFilelNode->getPosition().y);
            if (m_pFilelNode->getPosition().y < ScreenAdapterHelper::getValue(250))
            {
                m_bFileNodeReadyMove = true;
                m_vFileNodeOrignalVec = Vec2(m_pFilelNode->getPosition().x, m_pFilelNode->getPosition().y);
                m_pFilelNode->setPosition(Vec2(m_pVirtualFrame->getContentSize().width/2, m_pVirtualFrame->getContentSize().height/2));
            }
            m_pSignture->runAction(EaseExponentialInOut::create(MoveTo::create(0.5f, (m_vSignatureOrginVec + Vec2(0, ScreenAdapterHelper::getRealDesignScale() * ScreenAdapterHelper::getValue(452))))));
        }
            break;
            
        case st_DeviceType_iPhoneOther:
        {
//            log("file node positionY = %f",m_pFilelNode->getPosition().y);
            if (m_pFilelNode->getPosition().y < ScreenAdapterHelper::getValue(250))
            {
                m_bFileNodeReadyMove = true;
                m_vFileNodeOrignalVec = Vec2(m_pFilelNode->getPosition().x, m_pFilelNode->getPosition().y);
                m_pFilelNode->setPosition(Vec2(m_pVirtualFrame->getContentSize().width/2, m_pVirtualFrame->getContentSize().height/2));
            }
            m_pSignture->runAction(EaseExponentialInOut::create(MoveTo::create(0.35f, (m_vSignatureOrginVec + Vec2(0, ScreenAdapterHelper::getRealDesignScale() * ScreenAdapterHelper::getValue(450))))));
        }
            break;
            
        case st_DeviceType_iPad:
        {
//            log("file node positionY = %f",m_pFilelNode->getPosition().y);
            if (m_pFilelNode->getPosition().y < ScreenAdapterHelper::getRealDesignScale() * ScreenAdapterHelper::getValue(150))
            {
                m_bFileNodeReadyMove = true;
                m_vFileNodeOrignalVec = Vec2(m_pFilelNode->getPosition().x, m_pFilelNode->getPosition().y);
                m_pFilelNode->setPosition(Vec2(m_pVirtualFrame->getContentSize().width/2, m_pVirtualFrame->getContentSize().height/2));
            }
            m_pSignture->runAction(EaseExponentialInOut::create(MoveTo::create(0.25f, (m_vSignatureOrginVec + Vec2(0, ScreenAdapterHelper::getRealDesignScale() * ScreenAdapterHelper::getValue(385))))));
        }
            break;
            
        case st_DeviceType_Android_iphone:
        {
            m_pSignture->runAction(EaseExponentialInOut::create(MoveTo::create(0.25f, (m_vSignatureOrginVec + Vec2(0, ScreenAdapterHelper::getRealDesignScale() * ScreenAdapterHelper::getValue(530))))));
        }
            break;
            
        case st_DeviceType_Android_ipad:
        {
            m_pSignture->runAction(EaseExponentialInOut::create(MoveTo::create(0.25f, (m_vSignatureOrginVec + Vec2(0, ScreenAdapterHelper::getRealDesignScale() * ScreenAdapterHelper::getValue(530))))));
        }
            break;
            
        default:
        {
            m_pSignture->runAction(EaseExponentialInOut::create(MoveTo::create(0.5f, (m_vSignatureOrginVec + Vec2(0, ScreenAdapterHelper::getRealDesignScale() * ScreenAdapterHelper::getValue(345))))));
        }
            break;
    }
}

void EditLayer::updateTextColor(Color3B& color)
{
    m_pFilelNode->updateTextColor(color);
}

void EditLayer::updateTextFont(const string& fontName)
{
    m_pFilelNode->updareTextFont(fontName);
}

void EditLayer::setFileNodeBGVisible(bool pBool)
{
    m_pFilelNode->setWhiteBGVisible(pBool);
}

bool EditLayer::getCurrentIsKeyboard()
{
    return m_bCurrentIsKeyboard;
}

void EditLayer::updateCurrentIsKeyboard(bool pBool)
{
    m_bCurrentIsKeyboard = pBool;
}

void EditLayer::delayeResumeBtn(float dt)
{
    m_pMenuItemText->setEnabled(true);
}

void EditLayer::closeKeyboard()
{
    
    m_pFilelNode->closeIME();
}

void EditLayer::openKeyboard()
{
    m_pFilelNode->openIME();
}

TableViewCell* EditLayer::tableCellAtIndex(TableView* table, ssize_t idex)
{
#ifdef REMOVE_REWARD_ADS
    idex ++;
#endif
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
 
  // log("cell is idex = %d", idex);
    cell->removeAllChildrenWithCleanup(true);
//    if(cell->getChildren().size()==0)
    {
        if (m_bIsFilters)
        {
            this->updataFilters(idex, cell);
        }
        else
        {
            this->updataFrames(idex, cell);
        }
        cell->setTag((int)idex);
        
    }
//    else
//    {
//        Sprite* lResSprite=(Sprite*)cell->getChildren().at(0);
//        if(lResSprite->getTag()!=idex)
//        {
//            cell->removeAllChildrenWithCleanup(true);
//            this->updataFilters(idex, cell);
//        }
//    }
    
    
    

    return cell;
}

void EditLayer::updataTableView()
{
    m_pTableView->reloadData();
}

ssize_t EditLayer::numberOfCellsInTableView(TableView* table)
{
    int count = 0;
    if (m_bIsFilters)
    {
        count = (int)DataManager::getInstance()->m_vFilterDatasBase.size() + 1;
    }
    else
    {
        count = (int)DataManager::getInstance()->m_vFramesDatasBase.size() + 1;
    }
    
#ifdef REMOVE_REWARD_ADS
    
    count--;
#endif
    
    return  count;
}

void EditLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
   
    if (m_bIsFilters)
    {
        if (cell->getTag() == 0)
        {
            FilterAndFrameLayer* lFFLayer = FilterAndFrameLayer::create(m_pMoreFilterSpr);
            ScreenAdapterHelper::setPosition(lFFLayer, Vec2(384, 100), KVBoarderTypeBottom, KHBoarderTypeNone);
            this->getParent()->addChild(lFFLayer,10);
            
        }
        else
        {
            for (int i=0; i<this->numberOfCellsInTableView(table); i++) {
                TableViewCell* lCell = dynamic_cast<TableViewCell*>(table->cellAtIndex(i));
                if(lCell)
                {
                    Sprite* lResSprite=dynamic_cast<Sprite*>(lCell->getChildren().at(0));
                    if(lResSprite)
                    {
                        if(lResSprite->getChildren().size()>0)
                        {
                            Sprite* lSprite=dynamic_cast<Sprite*>(lResSprite->getChildren().at(0));
                            if(lSprite)
                            {
                                lSprite->setVisible(false);
                            }
                        }
                    }
                }
            }
            

            if (cell->getChildByTag(st_editlayer_filters_tag))
            {
                Sprite* lSprite = (Sprite*)cell->getChildByTag(st_editlayer_filters_tag);
                
                lSprite->getChildren().at(0)->setVisible(true);
                _currentIdx=(int)cell->getIdx();
                
#ifdef REMOVE_REWARD_ADS
                _currentIdx++;
#endif
            }

            FilterInfo* lFilterInfo = &DataManager::getInstance()->m_vFilterDatasBase.at(cell->getTag() - 1);
            
            m_pCurrentFilterName = lFilterInfo->filterName;
            
            this->changeSprite(m_pCurrentSprite, lFilterInfo->changeIndex);
            m_iChooseFilterIndex = lFilterInfo->changeIndex;
            
            if (cell->getChildByTag(st_editlayer_newIcon_tag))
            {
                cell->getChildByTag(st_editlayer_newIcon_tag)->removeFromParentAndCleanup(true);
                lFilterInfo->isNotUsed = false;
                DataManager::getInstance()->m_vFilterDatasBase.at(cell->getTag() - 1).isNotUsed = false;
                
#ifdef REMOVE_REWARD_ADS
                UserDefault::getInstance()->setBoolForKey(lFilterInfo->userdefault.c_str(), true);
#endif
            }
            
            //添加选中状态
            for (int i = 0; i < this->numberOfCellsInTableView(table); ++i)
            {
                TableViewCell* lCell = dynamic_cast<TableViewCell*>(table->cellAtIndex(i));
                if (lCell)
                {
                    Label* label = dynamic_cast<Label*>(lCell->getChildByTag(st_editlayer_label_tag));
                    if (label)
                    {
                        label->setColor(Color3B::BLACK);
                    }
                }
            }
            
            for (int i = 0 ; i < DataManager::getInstance()->m_vFilterDatasBase.size(); ++i)
            {
                FilterInfo* lFilterInfo;
                lFilterInfo = &DataManager::getInstance()->m_vFilterDatasBase.at(i);
                lFilterInfo->labelIsBlack = true;
            }
            
            //            for (auto lFrameInfo : DataManager::getInstance()->m_vFramesDatasBase)
            //            {
            //                lFrameInfo.labelIsBlack = true;
            //            }
            
            Label* label = dynamic_cast<Label*>(cell->getChildByTag(st_editlayer_label_tag));
            if (label)
            {
                label->setColor(Color3B(236, 64, 122));
                lFilterInfo->labelIsBlack = false;
            }
        }
    }
    else
    {
        m_pTableView->setTouchEnabled(false);
        this->scheduleOnce(schedule_selector(EditLayer::delayeTableViewTouch), 0.2f);
        
        if (cell->getTag() == 0)
        {
            FilterAndFrameLayer* lFFLayer = FilterAndFrameLayer::create(m_pMoreFilterSpr);
            ScreenAdapterHelper::setPosition(lFFLayer, Vec2(384, 100), KVBoarderTypeBottom, KHBoarderTypeNone);
            this->getParent()->addChild(lFFLayer,10);
        }
        else
        {
            Sprite* lSprite = (Sprite*)this->getChildByTag(st_editlayer_frame_tag);
            m_pCurrentSprite->setScale(0.6f);
            
            if (lSprite)
            {
                lSprite->removeFromParentAndCleanup(true);
            }
            
            FramesInfo* frameInfo = &DataManager::getInstance()->m_vFramesDatasBase.at(cell->getTag()-1);
            
            if (cell->getChildByTag(st_editlayer_newIcon_tag))
            {
                cell->getChildByTag(st_editlayer_newIcon_tag)->removeFromParentAndCleanup(true);
                
                frameInfo->isNotUsed=false;
                
                for (int i = 0; i < DataManager::getInstance()->m_vFramesDatas.size(); ++i)
                {
                    FramesInfo* lFrames = &DataManager::getInstance()->m_vFramesDatas.at(i);
                    if (lFrames->frameName == frameInfo->frameName)
                    {
                        lFrames->isNotUsed = false;
                    }
                }
                
#ifdef REMOVE_REWARD_ADS
                UserDefault::getInstance()->setBoolForKey(frameInfo->userdefault.c_str(), true);
#endif
//                DataManager::getInstance()->m_vFramesDatas.at(cell->getTag() - 1).isNotUsed = false;
            }
            
            
            m_pCurrentSprite->setScale(m_pCurrentSprite->getScale() * frameInfo->childScale);
            
            Sprite* framesSprite = Sprite::create(frameInfo->imageName);
            ScreenAdapterHelper::setPosition(framesSprite, Vec2(384, 590));
            framesSprite->setTag(st_editlayer_frame_tag);
            framesSprite->setScale(0.8f);
            this->addChild(framesSprite);
            
            if (m_pCurrentFramesName != "")
            {
                Director::getInstance()->getTextureCache()->removeTextureForKey(m_pCurrentFramesName);
            }
            m_pCurrentFramesName = frameInfo->imageName;
            
            m_iChooseFramesIndex = cell->getTag() - 1;
            
            
            //添加选中状态
            for (int i = 0; i < this->numberOfCellsInTableView(table); ++i)
            {
                TableViewCell* lCell = dynamic_cast<TableViewCell*>(table->cellAtIndex(i));
                if (lCell)
                {
                    Label* label = dynamic_cast<Label*>(lCell->getChildByTag(st_editlayer_label_tag));
                    if (label)
                    {
                        label->setColor(Color3B::BLACK);
                    }
                }
            }
            
            for (int i = 0 ; i < DataManager::getInstance()->m_vFramesDatasBase.size(); ++i)
            {
                FramesInfo* lFrameInfo;
                lFrameInfo = &DataManager::getInstance()->m_vFramesDatasBase.at(i);
                lFrameInfo->labelIsBlack = true;
            }
            
//            for (auto lFrameInfo : DataManager::getInstance()->m_vFramesDatasBase)
//            {
//                lFrameInfo.labelIsBlack = true;
//            }
            
            Label* label = dynamic_cast<Label*>(cell->getChildByTag(st_editlayer_label_tag));
            if (label)
            {
                label->setColor(Color3B(236, 64, 122));
                frameInfo->labelIsBlack = false;
            }
        }
        
    }
    
}

void EditLayer::delayeTableViewTouch(float dt)
{
    m_pTableView->setTouchEnabled(true);
}

void EditLayer::sendFlurryDatas()
{

    if (m_pCurrentFramesName != "Frame999.png")
    {
        stringstream ss;
        ss<<"";
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ss<<"Redact_";
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ss<<"Edit_";
        
#endif
        ss<<m_pCurrentFramesName;
        
        for (int i = 0; i < DM_FlurryDatas.size(); ++i)
        {
            FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
            if (strcmp(lFlurryInfo->message.c_str(), ss.str().c_str()) == 0)
            {
                lFlurryInfo->number++;
            }
        }
    }
    
    if (m_pCurrentFilterName != "Original")
    {
        stringstream ss;
        ss<<"";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ss<<"Redact_Filter";
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ss<<"Edit_Filter";
        
#endif
        ss<<m_pCurrentFilterName;
        
        for (int i = 0; i < DM_FlurryDatas.size(); ++i)
        {
            FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
            if (strcmp(lFlurryInfo->message.c_str(), ss.str().c_str()) == 0)
            {
                lFlurryInfo->number++;
            }
        }
    }
    
}

void EditLayer::updataFilters(ssize_t idex, TableViewCell* cell)
{
    
    if (idex == 0)
    {
        Sprite* bg = Sprite::create("more.png");
        bg->setTag((int)idex);
        bg->setPosition(Vec2(ScreenAdapterHelper::getValue(60), ScreenAdapterHelper::getValue(80)));
        //            bg->setTag(idex);
        cell->addChild(bg);
    }
    else
    {
        
        FilterInfo lFilterInfo = DataManager::getInstance()->m_vFilterDatasBase.at(idex - 1);
        
//        Sprite* lSprite = Sprite::createWithTexture(m_pCurrentSprite->getTexture());
        Sprite* lSprite = Sprite::create("filter_icon.png");
        
        lSprite->setTag((int)idex);
        lSprite->setPosition(Vec2(ScreenAdapterHelper::getValue(60), ScreenAdapterHelper::getValue(80)));
        lSprite->setScale(0.5f);
        cell->addChild(lSprite);
        lSprite->setTag(st_editlayer_filters_tag);
        
        
#ifdef REMOVE_REWARD_ADS
        if (lFilterInfo.isNotUsed && UserDefault::getInstance()->getBoolForKey(lFilterInfo.userdefault.c_str(), false) == false)
        {
            Sprite* new_icon = Sprite::create("new.png");
            new_icon->setPosition(Vec2(ScreenAdapterHelper::getValue(10), ScreenAdapterHelper::getValue(130)));
            new_icon->setTag(st_editlayer_newIcon_tag);
            cell->addChild(new_icon);
        }
#else
        if (lFilterInfo.isNotUsed)
        {
            Sprite* new_icon = Sprite::create("new.png");
            new_icon->setPosition(Vec2(ScreenAdapterHelper::getValue(10), ScreenAdapterHelper::getValue(130)));
            new_icon->setTag(st_editlayer_newIcon_tag);
            cell->addChild(new_icon);
        }
#endif
        
        this->changeSprite(lSprite, lFilterInfo.changeIndex, true);

        Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()[lFilterInfo.filterName.c_str()], st_font_PTS, ScreenAdapterHelper::getValue(25));
        label->setTag(st_editlayer_label_tag);
//        label->setColor(Color3B::BLACK);
        label->setPosition(Vec2(ScreenAdapterHelper::getValue(60), ScreenAdapterHelper::getValue(10)));
        cell->addChild(label);
 
        Sprite* lResSprite=Sprite::create("clicked_filter.png");
        lResSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
        lResSprite->setScale(2.0f);
        lSprite->addChild(lResSprite);
        lResSprite->setVisible(false);
        
        if(_currentIdx!=-1 && _currentIdx==idex)
        {
            lResSprite->setVisible(true);
        }

        if (lFilterInfo.labelIsBlack)
        {
            label->setColor(Color3B::BLACK);
        }else
        {
            label->setColor(Color3B(236, 64, 122));
            lResSprite->setVisible(true);
        }
        
    }
}

void EditLayer::updataFrames(ssize_t idex, TableViewCell* cell)
{
    if (idex == 0)
    {
        Sprite* bg = Sprite::create("more.png");
        bg->setPosition(Vec2(ScreenAdapterHelper::getValue(60), ScreenAdapterHelper::getValue(80)));
        //            bg->setTag(idex);
        cell->addChild(bg);
    }else if (idex == 1)
    {
        Sprite* bg = Sprite::create("Frame999_icon.png");
        bg->setPosition(Vec2(ScreenAdapterHelper::getValue(60), ScreenAdapterHelper::getValue(80)));
        //            bg->setTag(idex);
        Sprite* lSprite = Sprite::create("filter_icon.png");
        lSprite->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2));
        bg->addChild(lSprite);
        lSprite->setScale(0.48f);
        
        Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["Original"], st_font_PTS, ScreenAdapterHelper::getValue(25));
        label->setPosition(Vec2(ScreenAdapterHelper::getValue(60), ScreenAdapterHelper::getValue(12)));
        label->setTag(st_editlayer_label_tag);
        cell->addChild(label);
        cell->addChild(bg);
        
        FramesInfo framesInfo = DataManager::getInstance()->m_vFramesDatasBase.at(idex-1);
        if (framesInfo.labelIsBlack)
        {
            label->setColor(Color3B::BLACK);
        }else
        {
            label->setColor(Color3B(236, 64, 122));
        }
    }
    else
    {
        FramesInfo framesInfo = DataManager::getInstance()->m_vFramesDatasBase.at(idex-1);
        
//        Sprite* lSprite = Sprite::create(framesInfo.imageName);
        Sprite* lSprite = Sprite::createWithSpriteFrameName(framesInfo.imageName);
        lSprite->setPosition(Vec2(ScreenAdapterHelper::getValue(60), ScreenAdapterHelper::getValue(80)));
        lSprite->setScale(1.2f);
//        lSprite->setScale(0.12f);
        cell->addChild(lSprite);
        
        Sprite* lSprite_bg = Sprite::create("filter_icon.png");
        lSprite_bg->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
        lSprite->addChild(lSprite_bg,-1);
        lSprite_bg->setScale(framesInfo.childScale * 0.4f);
      
#ifdef REMOVE_REWARD_ADS
        if (framesInfo.isNotUsed && UserDefault::getInstance()->getBoolForKey(framesInfo.userdefault.c_str(), false) == false)
        {
            Sprite* new_icon = Sprite::create("new.png");
            new_icon->setPosition(Vec2(ScreenAdapterHelper::getValue(10), ScreenAdapterHelper::getValue(130)));
            new_icon->setTag(st_editlayer_newIcon_tag);
            cell->addChild(new_icon);
        }
#else
        if (framesInfo.isNotUsed)
        {
            Sprite* new_icon = Sprite::create("new.png");
            new_icon->setPosition(Vec2(ScreenAdapterHelper::getValue(10), ScreenAdapterHelper::getValue(130)));
            new_icon->setTag(st_editlayer_newIcon_tag);
            cell->addChild(new_icon);
        }
#endif
        
        Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()[framesInfo.frameName.c_str()], st_font_PTS, ScreenAdapterHelper::getValue(25));
        label->setPosition(Vec2(ScreenAdapterHelper::getValue(60), ScreenAdapterHelper::getValue(12)));
//        label->setColor(Color3B::BLACK);
        label->setTag(st_editlayer_label_tag);
        cell->addChild(label);
        
        if (framesInfo.labelIsBlack)
        {
            label->setColor(Color3B::BLACK);
        }else
        {
            label->setColor(Color3B(236, 64, 122));
        }
        
    }
}

void EditLayer::updataCurrentSprite()
{

}

void EditLayer::chooseFilterOrFrames(bool pBool)
{
    m_bIsFilters = pBool;
    m_pTableView->reloadData();
}


void EditLayer::changeSprite(Sprite*pSprite,int idx, bool isIcon /*= false */)
{
    Sprite* lSprite=pSprite;
//    lSprite->removeAllChildrenWithCleanup(true);
    switch (idx) {
        case 0:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_ORGIN);
        }
            break;
        case 1:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_LOMO);
        }
            break;
        case 2:
        {
            if (isIcon)
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1_icon.png");
                Sprite* lNoiseSprite=Sprite::create("pencil_disp2_icon.jpg");
                Sprite* lBatchSprite=Sprite::create("watercolor1_icon.jpg");
                this->changeSprite(lSprite,str_VSH,strFSH_GaussianBlur,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }else
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1.png");
                Sprite* lNoiseSprite=Sprite::create("pencil_disp2.jpg");
                Sprite* lBatchSprite=Sprite::create("watercolor1.jpg");
                this->changeSprite(lSprite,str_VSH,strFSH_GaussianBlur,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }
   
        }
            break;
        case 3:
        {
            if (isIcon)
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1_icon.png");
                Sprite* lNoiseSprite=Sprite::create("pencil_disp2_icon.jpg");
                Sprite* lBatchSprite=Sprite::create("penciltest_icon.jpg");
                this->changeSprite(lSprite,str_VSH,strFSH_Pencil,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }else
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1.png");
                Sprite* lNoiseSprite=Sprite::create("pencil_disp2.jpg");
                Sprite* lBatchSprite=Sprite::create("penciltest.jpg");
                this->changeSprite(lSprite,str_VSH,strFSH_Pencil,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }
            
        }
            break;
        case 4:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_MENGHUAN);
        }
            break;
            
            
            //1
        case 5:
        {
            
            if (isIcon)
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1_icon.png");
                Sprite* lNoiseSprite=Sprite::create("tietu_d_icon.png");
                Sprite* lBatchSprite=Sprite::create("noise_icon.png");
                this->changeSprite(lSprite,str_VSH,strFSH_NewOverLay,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }else
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1.png");
                Sprite* lNoiseSprite=Sprite::create("tietu_d.png");
                Sprite* lBatchSprite=Sprite::create("noise.png");
                this->changeSprite(lSprite,str_VSH,strFSH_NewOverLay,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }
            
            
            
            
        }
            break;
            
        case 6:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_JIUHONG);
        }
            break;
        case 7:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_QINGNING);
        }
            break;
            
            
            //2
        case 8:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_GETE);
        }
            break;
        case 9:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_LANGMAN);
        }
            break;
        case 10:
        {
            this->changeSprite(lSprite, str_VSH, strFSH_DANYA);
        }
            break;
            
        case 11: //Crayon
        {
            if (isIcon)
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1_icon.png");
                Sprite* lNoiseSprite=Sprite::create("pencil_disp2_icon.jpg");
                Sprite* lBatchSprite=Sprite::create("crayon3_icon.jpg");
                this->changeSprite(lSprite,str_VSH,strFSH_Crayon,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }else
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1.png");
                Sprite* lNoiseSprite=Sprite::create("pencil_disp2.jpg");
                Sprite* lBatchSprite=Sprite::create("crayon3.jpg");
                this->changeSprite(lSprite,str_VSH,strFSH_Crayon,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }
        }
            break;

            
        case 12: //Water Color
        {
            if (isIcon)
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1_icon.png");
                Sprite* lNoiseSprite=Sprite::create("pencil_disp2_icon.jpg");
                Sprite* lBatchSprite=Sprite::create("watercolor1_icon.jpg");
                this->changeSprite(lSprite,str_VSH,strFSH_WaterColor,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }else
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1.png");
                Sprite* lNoiseSprite=Sprite::create("pencil_disp2.jpg");
                Sprite* lBatchSprite=Sprite::create("watercolor1.jpg");
                this->changeSprite(lSprite,str_VSH,strFSH_WaterColor,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }
            
        }
            break;

            break;
        case 13: //Shiny
        {
            if (isIcon)
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1_icon.png");
                Sprite* lNoiseSprite=Sprite::create("tietu_d_icon.png");
                Sprite* lBatchSprite=Sprite::create("tietu_b_gai_icon.jpg");
                this->changeSprite(lSprite,str_VSH,strFSH_Shiny,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }else
            {
                Sprite* lBackgroundSprite=Sprite::create("paper1.png");
                Sprite* lNoiseSprite=Sprite::create("tietu_d.png");
                Sprite* lBatchSprite=Sprite::create("tietu_b_gai.jpg");
                this->changeSprite(lSprite,str_VSH,strFSH_Shiny,lBackgroundSprite->getTexture(), lNoiseSprite->getTexture(), lBatchSprite->getTexture());
            }
        }
            
        default:
            break;
    }
    
}

void EditLayer::changeSprite(Sprite* pSprite,const GLchar* pVsh,const GLchar* pFsh, Texture2D* pTexture,GLuint pIndex,Texture2D* pTexture2,GLuint pIndex2)
{
    GLProgram* p= GLProgram::createWithByteArrays(pVsh, pFsh);
    if(pTexture!=NULL)
    {
        GL::bindTexture2D(0);
        
        GLuint lTexture1 =  p->getUniformLocation("CC_Texture1");
        // GLuint lTexture1_id = Director::getInstance()->getTextureCache()->addImage(pTextureName)->getName();
        GLuint lTexture1_id = pTexture->getName();
        GL::bindTexture2DN(pIndex, lTexture1_id);
        p->setUniformLocationWith1i(lTexture1, pIndex);
        
     
        
    }
    if(pTexture2!=NULL)
    {
        GL::bindTexture2D(0);
        
        GLuint lTexture1 =  p->getUniformLocation("CC_Texture2");
        // GLuint lTexture1_id = Director::getInstance()->getTextureCache()->addImage(pTextureName)->getName();
        GLuint lTexture1_id = pTexture->getName();
        GL::bindTexture2DN(pIndex2, lTexture1_id);
        p->setUniformLocationWith1i(lTexture1, pIndex2);
        
        
        
    }
    //GLProgram* p= GLProgram::createWithFilenames("gray.vsh", "test3.fsh");
    auto glProgramState = GLProgramState::getOrCreateWithGLProgram( p );
    pSprite->setGLProgramState( glProgramState );
    
}


void EditLayer::changeSprite(Sprite* pSprite,const GLchar* pVsh,const GLchar* pFsh,Texture2D* pBackgroundTexture,Texture2D* pNoiseTexture,Texture2D* pBatchTexture)
{
    GLProgram* p= GLProgram::createWithByteArrays(pVsh, pFsh);
    
    
    auto glProgramState = GLProgramState::getOrCreateWithGLProgram( p );
    glProgramState->setUniformTexture("background", pBackgroundTexture->getName());
    glProgramState->setUniformTexture("noise", pNoiseTexture->getName());
    glProgramState->setUniformTexture("hatch", pBatchTexture->getName());
    glProgramState->setUniformTexture("tex", pSprite->getTexture()->getName());
    
    pSprite->setGLProgramState( glProgramState );
}
