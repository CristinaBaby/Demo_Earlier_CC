//
//  DeluxeLayer.cpp
//  TIME006
//
//  Created by Steven.Xc.Tian.
//
//

#include "DeluxeLayer.h"
#include "../AppConfig.h"
#include "../AppGlobal.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/EncryptDataHelper.h"
#include "../helpers/PurchaseHelper.h"
#include "../modules/STSystemFunction.h"
#include "../widgets/STButton.h"
#include "../widgets/STUILayer.h"
#include "../utilities/CSVParse.h"


USING_NS_CC;
USING_NS_CC_EXT;
using std::string;

enum
{
    tag_managed=100, tag_unmanaged,
};



DeluxeLayer* DeluxeLayer::create()
{
    DeluxeLayer *pRet = new DeluxeLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

DeluxeLayer* DeluxeLayer::createWithColor(ccColor4B color)
{
    DeluxeLayer *pRet = new DeluxeLayer();
    if (pRet && pRet->initWithColor(color))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

DeluxeLayer::DeluxeLayer()
{
    m_pIAB = new STInAppBilling();
    m_pIAB->setIABDelegate(PurchaseHelper::getInstance());
    TABLE_CELL_SIZE = CCSizeMake2(1460, 320);
}

DeluxeLayer::~DeluxeLayer()
{
    CC_SAFE_DELETE(m_pIAB);
}


bool DeluxeLayer::initWithColor(const ccColor4B& color)
{
    bool pRet = false;
    
    do
    {
        CC_BREAK_IF(! CoverLayer::initWithColor(color));
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sku/sku.plist", "sku/sku.pvr.ccz");
        
        
        STUILayer* pUILayer = STUILayer::create();
        if (!pUILayer) return false;
        this->addChild(pUILayer);
        
        // frame
        CCSprite* pFrame = CCSprite::createWithSpriteFrameName("ui05_box.png");
        if (!pFrame) return false;
        // in the center of screen
        pFrame->setPosition(ccp_center_screen);
        pUILayer->addChild(pFrame);
        
        CCPoint bgCenter = ccp_get_center(pFrame->getContentSize());
        
        // text
        CCSprite* pText = CCSprite::createWithSpriteFrameName("ui08_deluxe.png");
        if (!pText) return false;
        pText->setPosition(ccp_fixed_X(pFrame->getContentSize().width / 2, 1100));
        pFrame->addChild(pText);
        
        
        STButton *pClose = STButton::createWithSpriteFrameName(string("ui07_close.png"), string("ui07_close_h.png"),
                                                               this, callfuncN_selector(DeluxeLayer::onCloseClicked));
        if (!pClose) return false;
        pClose->setAnchorPoint(ccp(1, 1));
        float offset = get_offset(35);
        pClose->setPosition(ccp(pFrame->getContentSize().width-offset, pFrame->getContentSize().height-offset));
        pFrame->addChild(pClose);
        _mTouchPriority = pClose->getTouchPriority() - 1;
        pClose->setTouchPriority(_mTouchPriority);
        
        
        STButton *pRestore = STButton::createWithSpriteFrameName(string("ui08_btn_restore.png"), string("ui08_btn_restore_h.png"),
                                                                 this, callfuncN_selector(DeluxeLayer::onRestoreClicked));
        CC_BREAK_IF(!pRestore);
        pRestore->setPosition(ccp_fixed_X(pFrame->getContentSize().width / 2, 220));
        pRestore->setTouchPriority(_mTouchPriority);
        pFrame->addChild(pRestore);
        
        int _offset;
        if (AppGlobal::IS_2048)
        {
            _offset = 100;
        }else
        {
            _offset = 50;
        }
        
        m_pTable = CCTableView::create(this, CCSizeMake(TABLE_CELL_SIZE.width, get_offset(645)));
        CC_BREAK_IF(!m_pTable);
        m_pTable->ignoreAnchorPointForPosition(false);
        m_pTable->setAnchorPoint(ccp(.5f, 0));
        m_pTable->setPosition(ccp_fixed_X(bgCenter.x, 300));
#ifndef ONE_IAP
        m_pTable->setPosition(ccp_fixed_X(bgCenter.x - _offset, 300));
#endif
        pFrame->addChild(m_pTable);
        
        m_pTable->setDirection(kCCScrollViewDirectionVertical);
        m_pTable->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_pTable->setTouchPriority(_mTouchPriority);
#ifndef ONE_IAP
        
        m_pTable->setContentOffset(ccp(0,-m_pTable->getContentSize().height));
        m_pTable->setContentOffsetInDuration(ccp(0, 0), 0.4f);
        
        CCCallFunc* lFunc=CCCallFunc::create(this, callfunc_selector(DeluxeLayer::excuteAnimation));
        CCDelayTime* lDelayTime=CCDelayTime::create(0.5f);
        CCSequence* lAction=CCSequence::create(lDelayTime,lFunc,NULL);
        this->runAction(lAction);
        
        
        if (AppGlobal::IS_2048)
        {
            progressBar_bg = CCSprite::create("iap_bar/bar1@2x.png");
            m_pProgressPoint = CCSprite::create("iap_bar/bar2@2x.png");
        }else
        {
            progressBar_bg = CCSprite::create("iap_bar/bar1.png");
            m_pProgressPoint = CCSprite::create("iap_bar/bar2.png");
        }
        
        progressBar_bg->setAnchorPoint(ccp(0.5, 0));
        progressBar_bg->setPosition(ccp(pFrame->getContentSize().width * 0.9,pFrame->getContentSize().height * 0.28));
        pFrame->addChild(progressBar_bg);
        
        m_pProgressPoint->ignoreAnchorPointForPosition(false);
        m_pProgressPoint->setAnchorPoint(ccp(0.5, 0.5));
        m_pProgressPoint->setPosition(ccp(progressBar_bg->getContentSize().width / 2, progressBar_bg->getContentSize().height * 0.9));
        progressBar_bg->addChild(m_pProgressPoint);
        
        CCLOG("%f.....................",progressBar_bg->getContentSize().height);
#endif
        pRet = true;
    } while (0);
    
    return pRet;
}

void DeluxeLayer::excuteAnimation()
{
    m_pTable->setContentOffsetInDuration(ccp(0,-TABLE_CELL_SIZE.height), 0.2f);
}

void DeluxeLayer::onCloseClicked(CCNode* button)
{
    AudioHelper::getInstance()->playSound(sound_button);
    CoverLayer::keyBackClicked();
}


void DeluxeLayer::onBuyClicked(CCNode* button)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    CCString *sku = dynamic_cast<CCString*>(button->getUserObject());
    int tag = button->getTag();
    switch (tag)
    {
        case tag_managed:
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            // if already unlocked all levels, pop up the dialog to prevent purchase again.
            string iapID=sku->getCString();
            if(strcmp(sku->getCString(), st_iap_sku_unlock_all)==0)
            {
                // if already unlocked all levels, pop up the dialog to prevent purchase again.
                if (PurchaseHelper::isPurchased) {
                    const char *msg = AppGlobal::stAllStrings->valueForKey(strings_already_purchased)->getCString();
                    STSystemFunction* sf = new STSystemFunction();
                    sf->popAlertDialog(msg);
                    CC_SAFE_DELETE(sf);
                    
                    break;
                }
            }
            else if(strcmp(sku->getCString(), st_iap_sku_ad)==0)
            {
                // if already unlocked all levels, pop up the dialog to prevent purchase again.
                if (PurchaseHelper::isPurchased && PurchaseHelper::isAdPurchased) {
                    const char *msg = AppGlobal::stAllStrings->valueForKey(strings_already_purchased)->getCString();
                    STSystemFunction* sf = new STSystemFunction();
                    sf->popAlertDialog(msg);
                    CC_SAFE_DELETE(sf);
                    
                    break;
                }
            }
#endif
            
            CCLOG("IAP SKU: %s", sku->getCString());
#ifdef DEBUG_IAP
            PurchaseHelper::getInstance()->purchaseSuccessful(sku->getCString());
#else
            m_pIAB->purchase(sku->getCString());
#endif
            
            
        }
            break;
        case tag_unmanaged:
        {
            CCLOG("IAP SKU: %s", sku->getCString());
#ifdef DEBUG_IAP
            PurchaseHelper::getInstance()->purchaseSuccessful(sku->getCString());
#else
            m_pIAB->purchaseUnmanaged(sku->getCString());
#endif
            
        }
            break;
    }
}

void DeluxeLayer::onRestoreClicked(CCNode* button)
{
    AudioHelper::getInstance()->playSound(sound_button);
    m_pIAB->restore();
}

void DeluxeLayer::onExit()
{
    CoverLayer::onExit();
}

void DeluxeLayer::onEnter()
{
    CoverLayer::onEnter();
    this->scheduleUpdate();
}
// ==============================================================



CCSize DeluxeLayer:: tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return cellSizeForTable(table);
};

CCSize DeluxeLayer:: cellSizeForTable(CCTableView *table) {
    return TABLE_CELL_SIZE;
};

CCTableViewCell* DeluxeLayer:: tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
    }
    else
    {
        cell->removeAllChildren();
    }
    
    CCDictionary *_info = dynamic_cast<CCDictionary*>( AppGlobal::stSKUInformation->objectAtIndex(idx) );
    
    CCString *_bgname = dynamic_cast<CCString*>( _info->objectForKey(st_sku_bgname) );
    bool _isStore = dynamic_cast<CCBool*>( _info->objectForKey(st_sku_store) )->getValue();
    
    CCString *_sku = NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _sku = dynamic_cast<CCString*>( _info->objectForKey(st_sku_ios) );
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _sku = dynamic_cast<CCString*>( _info->objectForKey(st_sku_android) );
#endif
    
    
    CCSprite *cellbg = CCSprite::createWithSpriteFrameName(_bgname->getCString());
    
    cellbg->setAnchorPoint(ccp(0.5f, 1));
    cellbg->setPosition(ccp(TABLE_CELL_SIZE.width/2, TABLE_CELL_SIZE.height));
#ifdef ONE_IAP
    cellbg->setPosition(ccp(TABLE_CELL_SIZE.width/2, TABLE_CELL_SIZE.height / 2));
#endif
    cell->addChild(cellbg);
    
    CCSize bgSize = cellbg->getContentSize();
    float btnOffset = get_offset(24);
    
    STButton *buy = STButton::createWithSpriteFrameName(string("ui08_btn_buy.png"), string("ui08_btn_buy_h.png"),
                                                        this, callfuncN_selector(DeluxeLayer::onBuyClicked));
    buy->setAnchorPoint(ccp(0.7, 0.5));
    buy->setPosition(ccp(bgSize.width, bgSize.height / 2+btnOffset));
    buy->setTouchPriority(_mTouchPriority);
    cellbg->addChild(buy);
    
    buy->setTag( (_isStore ? tag_managed : tag_unmanaged) ); //
    buy->setUserObject(_sku); // iap
    
    
    return cell;
    
}


unsigned int DeluxeLayer:: numberOfCellsInTableView(CCTableView *table)
{
    return AppGlobal::stSKUInformation->count();
}

void DeluxeLayer::update(float dt)
{
//    CCLOG("(%f,%f)",m_pTable->getContentOffset().x,m_pTable->getContentOffset().y);
#ifndef ONE_IAP
    float detal = m_pTable->getContentOffset().y;
    
    float change,tex;
    
    if (AppGlobal::IS_2048)
    {
        change = -315;
    }else
    {
        change = -158;
    }
    
    tex = detal / change;
    
    if (tex < 0.125 )
    {
        tex = 0.125;
//        CCLOG("...............%f",detal/change);
        
    }
    if ( tex > 0.875)
    {
        tex = 0.875;
    }
    m_pProgressPoint->setPosition(ccp(progressBar_bg->getContentSize().width / 2 , progressBar_bg->getContentSize().height * tex));
#endif
}


