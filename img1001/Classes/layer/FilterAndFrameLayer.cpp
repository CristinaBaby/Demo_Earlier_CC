//
//  FilterAndFrameLayer.cpp
//  ColorFree
//
//  Created by houhuang on 15/9/9.
//
//

#include "FilterAndFrameLayer.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../define/Globalmacro.h"
#include "../data/DataManager.h"
#include "../compoment/UIButton.h"
#include "../ad3_2/AdCross/AdLoadingLayerDoll.h"
#include "../define/AppConfigs.h"
#include "../scene/EditScene.h"
#include "../layer/LoadLayer.h"
#include "../modules/STSystemFunction.h"
//#include "../IOS_Android_Include/RewardedAds.h"
#include "../crossplatformapi/headers/ads/RewardedAds.h"
#include "../helper/MusicHelper.h"
enum
{
    st_FFLayer_filter_menu  =  50,
    st_FFLayer_frame_menu,
    
    st_FFLayer_purchase_frames1 = 60,
    st_FFLayer_purchase_frames2,
    st_FFLayer_purchase_frames3,
    st_FFLayer_purchase_filters1,
    st_FFLayer_purchase_filters2,
    
    st_FFLayer_UIButton_back = 70,
};

FilterAndFrameLayer::FilterAndFrameLayer()
{
    m_pTableView        = NULL;
    m_pLabelFilters     = NULL;
    m_pLabelFrames      = NULL;
    
//    m_pMoreFilterSpr    = NULL;
    m_pLabel            = NULL;
    m_pLoadLayer        = NULL;
    
    m_bIsPreloadedFinish   = true;
    m_bIsUpdateDataSuc     = true;
    m_bIsShowRewardSuc     = false;
    
    m_iTableOffset      = 0.0f;
    m_iCurrentFramesMenuTag = -1;
    
    m_bIsFilters = DataManager::m_bIsFilters;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(FilterAndFrameLayer::waitMessage), st_filterAndFrames_notifiction, NULL);
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(FilterAndFrameLayer::waitMessageRewardSu), st_filterAndFrames_LoadRewardSu, NULL);
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(FilterAndFrameLayer::waitMessageDownLoadSuc), st_filterAndFrames_downLoadSuc, NULL);
}

FilterAndFrameLayer::~FilterAndFrameLayer()
{
//    m_pMoreFilterSpr->release();
    NotificationCenter::getInstance()->removeObserver(this, st_filterAndFrames_notifiction);
     NotificationCenter::getInstance()->removeObserver(this, st_filterAndFrames_LoadRewardSu);
    
    NotificationCenter::getInstance()->removeObserver(this, st_filterAndFrames_downLoadSuc);
}

FilterAndFrameLayer* FilterAndFrameLayer::create(Sprite* pSprite)
{
    FilterAndFrameLayer* layer = new FilterAndFrameLayer();
    if (layer && layer->init(pSprite))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

void FilterAndFrameLayer::waitMessageDownLoadSuc(Ref* ref)
{
    if (m_pTableView)
    {
        m_pTableView->reloadData();
    }
}

bool FilterAndFrameLayer::init(Sprite* pSprite)
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;
    
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 1));
    
//    m_pMoreFilterSpr = Sprite::createWithTexture(pSprite->getTexture());
//    m_pMoreFilterSpr->retain();
    
    Sprite* topLine = Sprite::create("seperator-2.png");
    ScreenAdapterHelper::setPosition(topLine, Vec2(384, 100),KVBoarderTypeTop,KHBoarderTypeNone);
    this->addChild(topLine);
    
    MenuItemImage* filters = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(FilterAndFrameLayer::reponseMenu, this));
    ScreenAdapterHelper::setPosition(filters, Vec2(768/3, 50), KVBoarderTypeTop, KHBoarderTypeNone);
    filters->setTag(st_FFLayer_filter_menu);
    
    m_pLabelFilters = Label::createWithTTF(DM_getInstance->getLanguageMes()["Filters"], st_font_AR, ScreenAdapterHelper::getValue(40));
    m_pLabelFilters->setPosition(Vec2(filters->getContentSize().width/2, filters->getContentSize().height/2));
    m_pLabelFilters->setAnchorPoint(Vec2(0.5, 0.5));
    filters->addChild(m_pLabelFilters);
    
    MenuItemImage* frames = MenuItemImage::create("button.png", "button.png", CC_CALLBACK_1(FilterAndFrameLayer::reponseMenu, this));
    ScreenAdapterHelper::setPosition(frames, Vec2(768/3 * 2, 50), KVBoarderTypeTop, KHBoarderTypeNone);
    frames->setTag(st_FFLayer_frame_menu);
    
    m_pLabelFrames = Label::createWithTTF(DM_getInstance->getLanguageMes()["Frames"], st_font_AR, ScreenAdapterHelper::getValue(40));
    m_pLabelFrames->setPosition(Vec2(frames->getContentSize().width/2, frames->getContentSize().height/2));
    m_pLabelFrames->setAnchorPoint(Vec2(0.5, 0.5));
    frames->addChild(m_pLabelFrames);
    
    if (DataManager::m_bIsFilters)
    {
        m_pLabelFilters->setColor(Color3B(236, 64, 122));
        m_pLabelFrames->setColor(Color3B::BLACK);
    }else
    {
        m_pLabelFilters->setColor(Color3B::BLACK);
        m_pLabelFrames->setColor(Color3B(236, 64, 122));
    }
    
    Menu* menu = Menu::create(filters, frames, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    UIButton* back = UIButton::create("btn_down.png");
    back->initListener(this, callfuncN_selector(FilterAndFrameLayer::doAction));
    ScreenAdapterHelper::setPosition(back, Vec2(80, 50), KVBoarderTypeTop, KHBoarderTypeLeft);
    back->setTag(st_FFLayer_UIButton_back);
    this->addChild(back);
    
    this->addListener();
    
    
    m_pTableView = TableView::create(this, Size(ScreenAdapterHelper::getValue(650), ScreenAdapterHelper::getValue(924 * ScreenAdapterHelper::getRealDesignScale())));
    m_pTableView->ignoreAnchorPointForPosition(false);
    m_pTableView->setAnchorPoint(Vec2(0.5, 1.0));
    m_pTableView->setDirection(ScrollView::Direction::VERTICAL);
    m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    
    ScreenAdapterHelper::setPosition(m_pTableView, Vec2(384, 100), KVBoarderTypeTop, KHBoarderTypeNone);
    
    this->addChild(m_pTableView);
    
    m_pTableView->setDelegate(this);
    m_pTableView->reloadData();
    
    MoveBy* moveTo = MoveBy::create(0.5f, Vec2(0, ScreenAdapterHelper::getRealDesignSize().height - ScreenAdapterHelper::getValue(100)));
    CallFunc* lCallFun = CallFunc::create(CC_CALLBACK_0(FilterAndFrameLayer::reponseCreateLayer, this));
    this->runAction(Sequence::create(EaseExponentialOut::create(moveTo), lCallFun, NULL));

    
    LoadLayer* loadLayer = LoadLayer::create();
    this->addChild(loadLayer, 100);
    m_pLoadLayer = loadLayer;
    m_pLoadLayer->setVisible(false);
    m_pLoadLayer->setTouchEnabled(false);
    
    return true;
}

Size FilterAndFrameLayer::cellSizeForTable(TableView* table)
{
    Size size;
    if (m_bIsFilters)
    {
        size = Size(ScreenAdapterHelper::getValue(650), ScreenAdapterHelper::getValue(420));
    }
    else
    {
        size = Size(ScreenAdapterHelper::getValue(650), ScreenAdapterHelper::getValue(450));
    }
    
#ifdef REMOVE_REWARD_ADS
    size = Size(ScreenAdapterHelper::getValue(650), ScreenAdapterHelper::getValue(300));
#endif
    
    return size;
}

TableViewCell* FilterAndFrameLayer::tableCellAtIndex(TableView* table, ssize_t idex)
{
    TableViewCell* cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    cell->removeAllChildrenWithCleanup(true);
    
    if (m_bIsFilters)
    {
        
        MenuItemImage* lMenuItemImage = MenuItemImage::create("btn_buy.png", "downloaded.png", CC_CALLBACK_1(FilterAndFrameLayer::reponseFramesMenu, this));
        lMenuItemImage->ignoreAnchorPointForPosition(false);
        lMenuItemImage->setAnchorPoint(Vec2(0.5, 0.5));
        lMenuItemImage->setPosition(Vec2(ScreenAdapterHelper::getValue(325), ScreenAdapterHelper::getValue(100)));
        
        Label* menu_label = Label::createWithTTF("Watch an Ad to Unlock", st_font_PTS, ScreenAdapterHelper::getValue(30));
        menu_label->setPosition(Vec2(lMenuItemImage->getContentSize().width/2, lMenuItemImage->getContentSize().height/2));
        menu_label->setColor(Color3B::WHITE);
        lMenuItemImage->addChild(menu_label,5);
        
        for (int i = 0; i < 3; i++)
        {
            FilterInfo lFilterInfo = DataManager::getInstance()->m_vFilterDatas.at(i + (idex * 3) + 5);
            //            Sprite* lSprite = Sprite::createWithTexture(m_pMoreFilterSpr->getTexture());
            Sprite* lSprite = Sprite::create("filter_icon.png");
            
#ifdef REMOVE_REWARD_ADS
            lSprite->setPosition(Vec2((ScreenAdapterHelper::getValue(650) * (0.5 + i) / 3), ScreenAdapterHelper::getValue(190)));
#else
            lSprite->setPosition(Vec2((ScreenAdapterHelper::getValue(650) * (0.5 + i) / 3), ScreenAdapterHelper::getValue(300)));
#endif
            lSprite->setScale(0.85f);
            cell->addChild(lSprite);
            EditLayer* lEditLayer = dynamic_cast<EditLayer*>(this->getParent());
            lEditLayer->changeSprite(lSprite, lFilterInfo.changeIndex, true);
            
            
//            FilterInfo lFilterInfo = DataManager::getInstance()->m_vFilterDatas.at(i + (idex * 3) + 5);
            Label* label = Label::createWithTTF(lFilterInfo.filterName, st_font_PTS, ScreenAdapterHelper::getValue(25));
            label->setPosition(Vec2(lSprite->getPosition().x, lSprite->getPosition().y - lSprite->getBoundingBox().size.height / 2 - ScreenAdapterHelper::getValue(20)));
            label->setColor(Color3B::BLACK);
            cell->addChild(label);
            
            lMenuItemImage->setTag(lFilterInfo.tag);
            
            if (UserDefault::getInstance()->getBoolForKey(lFilterInfo.userdefault.c_str()))
            {
                lMenuItemImage->setEnabled(false);
                lMenuItemImage->setNormalImage(Sprite::create("downloaded.png"));
                menu_label->setString("Unlocked");
                menu_label->setScale(1.2);
            }
        }


        Menu* menu = Menu::create(lMenuItemImage, NULL);
        menu->setPosition(Vec2::ZERO);
        
#ifndef REMOVE_REWARD_ADS
        cell->addChild(menu);
#endif
        
    }
    else
    {

        MenuItemImage* lItemImage = MenuItemImage::create("btn_buy.png", "downloaded.png", CC_CALLBACK_1(FilterAndFrameLayer::reponseFramesMenu, this));
        lItemImage->setPosition(Vec2(ScreenAdapterHelper::getValue(650/2), ScreenAdapterHelper::getValue(130)));
        
        Label* menu_label = Label::createWithTTF("Watch an Ad to Unlock", st_font_PTS, ScreenAdapterHelper::getValue(30));
        menu_label->setPosition(Vec2(lItemImage->getContentSize().width/2, lItemImage->getContentSize().height/2));
        menu_label->setColor(Color3B::WHITE);
        lItemImage->addChild(menu_label,5);
        
        for (int j = 0; j < 3; j++)
        {
            FramesInfo framesInfo = DataManager::getInstance()->m_vFramesDatas.at(j + (idex * 3) + 7);
            //                Sprite* lSprite = Sprite::create(framesInfo.imageName);
            Sprite* lSprite = Sprite::createWithSpriteFrameName(framesInfo.imageName);
            
#ifdef REMOVE_REWARD_ADS
            lSprite->setPosition(Vec2((ScreenAdapterHelper::getValue(650) * (0.5 + j) / 3), ScreenAdapterHelper::getValue(190)));
#else
            lSprite->setPosition(Vec2((ScreenAdapterHelper::getValue(650) * (0.5 + j) / 3), ScreenAdapterHelper::getValue(300)));
#endif
            
            lSprite->setScale(2.0f);
        
            lItemImage->setTag(framesInfo.tag);
            
            Sprite* lSprite_bg = Sprite::create("filter_icon.png");
            lSprite_bg->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
            lSprite->addChild(lSprite_bg, -1);
            lSprite_bg->setScale(framesInfo.childScale * 0.41f);
            
            Label* label = Label::createWithTTF(framesInfo.frameName.c_str(), st_font_PTS, ScreenAdapterHelper::getValue(25));
            label->setPosition(Vec2(lSprite->getPosition().x, lSprite->getPosition().y - lSprite->getBoundingBox().size.height / 2 - ScreenAdapterHelper::getValue(20)));
            label->setColor(Color3B::BLACK);
            
            cell->addChild(lSprite);
            cell->addChild(label);
            
            if (UserDefault::getInstance()->getBoolForKey(framesInfo.userdefault.c_str()))
            {
                lItemImage->setEnabled(false);
                lItemImage->setNormalImage(Sprite::create("downloaded.png"));
                menu_label->setString("Unlocked");
                menu_label->setScale(1.2f);
            }
        }
        
        Menu* menu = Menu::create(lItemImage, NULL);
        menu->setPosition(Vec2::ZERO);

#ifndef REMOVE_REWARD_ADS
        cell->addChild(menu);
#endif
        
    }

    return cell;
}

ssize_t FilterAndFrameLayer::numberOfCellsInTableView(TableView* table)
{
    int count;
    if (m_bIsFilters)
    {
        count = (int)(DataManager::getInstance()->m_vFilterDatas.size() - 5) / 3;
    }
    else
    {
        count = (int)( DataManager::getInstance()->m_vFramesDatas.size() - 7 ) / 3;
    }
    return count;
}

void FilterAndFrameLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

void FilterAndFrameLayer::reponseFramesMenu(Ref* ref)
{
    m_iTableOffset = m_pTableView->getContentOffset().y;
    
    STSystemFunction sf;
    if(!sf.checkNetworkAvailable())
    {
        sf.popAlertDialog("No internet connection! Need internet to load the ads, watch the ads to unlock!");
        return;
    }
    
    
    MenuItemImage* lMenuItemImage = (MenuItemImage*)ref;
    m_iCurrentFramesMenuTag = lMenuItemImage->getTag();
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    cocos2d::NotificationCenter::getInstance()->postNotification(st_filterAndFrames_LoadRewardSu);
#ifdef DEBUG_IAP
    this->onAdsCollapsed( "", 1, false);
#else
    if (RewardedAds::getInstance()->isPreloaded())
    {
        MusicHelper::getInstance()->pauseMusic();
        RewardedAds::getInstance()->show();
    }else
    {
        if (m_bIsPreloadedFinish == true)
        {
            m_bIsPreloadedFinish = false;
            RewardedAds::getInstance()->preload();
        }
        
    }
#endif
    cocos2d::NotificationCenter::getInstance()->postNotification(st_filterAndFrames_LoadRewardSu);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    if (RewardedAds::getInstance()->isPreloaded())
    {
        cocos2d::NotificationCenter::getInstance()->postNotification(st_filterAndFrames_LoadRewardSu);
        RewardedAds::getInstance()->show();
    }else
    {

        RewardedAds::getInstance()->preload();
        sf.popAlertDialog("Failed to load the ads, please try again later.");
        
    }
#endif
    


}

void FilterAndFrameLayer::reponseMenu(Ref* ref)
{

    MenuItemImage* lItmeImage = (MenuItemImage*)ref;
    
    switch (lItmeImage->getTag())
    {
        case st_FFLayer_filter_menu:
        {
            m_bIsFilters = true;
            m_pTableView->reloadData();
            
            m_pLabelFilters->setColor(Color3B(236, 64, 122));
            m_pLabelFrames->setColor(Color3B::BLACK);
        }
            break;
            
        case st_FFLayer_frame_menu:
        {
            m_bIsFilters = false;
            m_pTableView->reloadData();
            
            m_pLabelFilters->setColor(Color3B::BLACK);
            m_pLabelFrames->setColor(Color3B(236, 64, 122));
        }
            break;
            
        default:
            break;
    }
    
//    if(lItmeImage->getTag() >= st_FFLayer_purchase_frames1)
//    {
//
//        cocos2d::NotificationCenter::getInstance()->postNotification(st_filterAndFrames_LoadRewardSu);
//    }
}

void FilterAndFrameLayer::waitMessage(Ref* ref)
{
    
    if (m_bIsUpdateDataSuc)
    {
        m_bIsUpdateDataSuc = false;
        DataManager::getInstance()->updataFramesDataBase(true);
        DataManager::getInstance()->updataFilterDataBase(true);
        
        //解决有时广告加载完成后不会启动动画
        Director::getInstance()->startAnimation();
        
        this->scheduleOnce(schedule_selector(FilterAndFrameLayer::delayeUpdateTableView), 0.2f);
        this->scheduleOnce(schedule_selector(FilterAndFrameLayer::delayeDestoryRotateLayer), 2.0f);
    }
}

void FilterAndFrameLayer::delayeUpdateTableView(float dt)
{
    m_pTableView->reloadData();
}

void FilterAndFrameLayer::delayeDestoryRotateLayer(float dt)
{
    m_bIsShowRewardSuc = false;
    m_bIsUpdateDataSuc = true;
    if (MusicHelper::m_bIsPlayBackgroudMusic)
    {
        MusicHelper::getInstance()->resumeMusic();
    }
//    m_pTableView->setContentOffset(Vec2(0, m_iTableOffset));
    if (m_pLoadLayer != NULL)
    {
//        m_pLoadLayer->removeFromParentAndCleanup(true);
//        m_pLoadLayer=NULL;
        m_pLoadLayer->setVisible(false);
        m_pLoadLayer->setTouchEnabled(false);
        
    }
}

void FilterAndFrameLayer::waitMessageRewardSu(Ref *ref)
{
    if(m_pLoadLayer!=NULL)
    {
        m_pLoadLayer->setVisible(true);
        m_pLoadLayer->setTouchEnabled(true);
        return;
    }
    
    LoadLayer* loadLayer = LoadLayer::create();
    this->addChild(loadLayer, 100);
    m_pLoadLayer = loadLayer;
    m_pLoadLayer->setVisible(true);
    m_pLoadLayer->setTouchEnabled(true);
    
    
}

void FilterAndFrameLayer::onEnter()
{
    Layer::onEnter();
    this->initAdsDelegate();
}

void FilterAndFrameLayer::onExit()
{
    Layer::onExit();
    this->releaseAdsDelegate();
}

void FilterAndFrameLayer::releaseAdsDelegate()
{
    RewardedAds::getInstance()->onRewardedLoaded = nullptr;
    RewardedAds::getInstance()->onRewardedFailed = nullptr;
    RewardedAds::getInstance()->onRewardedClicked = nullptr;
    RewardedAds::getInstance()->onRewardedExpanded = nullptr;
    RewardedAds::getInstance()->onRewardedCollapsed = nullptr;

 
}

void FilterAndFrameLayer::initAdsDelegate()
{
    RewardedAds::getInstance()->onRewardedLoaded = std::bind(&FilterAndFrameLayer::onAdsLoaded, this);
    RewardedAds::getInstance()->onRewardedFailed = std::bind(&FilterAndFrameLayer::onAdsLoadFailed, this, std::placeholders::_1);
    RewardedAds::getInstance()->onRewardedClicked = std::bind(&FilterAndFrameLayer::onAdsClicked, this);
    RewardedAds::getInstance()->onRewardedExpanded = std::bind(&FilterAndFrameLayer::onAdsExpanded, this);
//    RewardedAds::getInstance()->onRewardedCollapsed = std::bind(&FilterAndFrameLayer::onAdsCollapsed, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    RewardedAds::getInstance()->onRewarded = std::bind(&FilterAndFrameLayer::onAdsCollapsed, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);


}

void FilterAndFrameLayer::onAdsLoaded()
{
//    log("Ads show succeeful(finish)");
   
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return;
#endif
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
        this->scheduleOnce(schedule_selector(FilterAndFrameLayer::delayeDestoryRotateLayer), 2.0f);
    });

}

void FilterAndFrameLayer::delayeLoadRotateLayer(float dt)
{
    if (m_bIsShowRewardSuc == false)
    {
        NotificationCenter::getInstance()->postNotification(st_filterAndFrames_notifiction);
    }

}

void FilterAndFrameLayer::onAdsLoadFailed(std::string error)
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return;
#endif
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
        STSystemFunction sf;
        sf.popAlertDialog("Failed to load the ads, please try again later.");
        
        NotificationCenter::getInstance()->postNotification(st_filterAndFrames_notifiction);

        m_bIsPreloadedFinish = true;
    });

//    log("Ads Test Demo ====================>onRewardedAdsLoadFailed: [%s]" ,error.c_str());
}

void FilterAndFrameLayer::onAdsClicked()
{
//    log("Ads Test Demo ====================>onRewardedAdsClicked");
}

void FilterAndFrameLayer::onAdsExpanded()
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
       
    });
    
    m_bIsShowRewardSuc = true;
    
//    log("Ads show succeeful(begin)");

//    Director::getInstance()->pause();
//    log("--pause...............................................");
    
//    CallFunc* lCallFunc = CallFunc::create(CC_CALLBACK_0(FilterAndFrameLayer::delayeLoadRotateLayer, this));
    
    //延迟。 解决UIKit和Cocos2d 绘制冲突
//    this->runAction(Sequence::create(DelayTime::create(0.5f), lCallFunc, NULL));
    
}



void FilterAndFrameLayer::onAdsCollapsed( std::string unitId, int amount, bool isSkipped )
{
    
//
    if (amount > 0)
    {
        for (int i = 0; i < DataManager::getInstance()->m_vFramesDatas.size(); ++i)
        {
            FramesInfo* lFramesInfo = &DataManager::getInstance()->m_vFramesDatas.at(i);
            if (lFramesInfo->tag == m_iCurrentFramesMenuTag)
            {
                UserDefault::getInstance()->setBoolForKey(lFramesInfo->userdefault.c_str(), true);
            }
        }
        
        for (int i = 0; i < DataManager::getInstance()->m_vFilterDatas.size(); ++i)
        {
            FilterInfo* lFramesInfo = &DataManager::getInstance()->m_vFilterDatas.at(i);
            if (lFramesInfo->tag == m_iCurrentFramesMenuTag)
            {
                UserDefault::getInstance()->setBoolForKey(lFramesInfo->userdefault.c_str(), true);
            }
        }
    }

    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
        
        NotificationCenter::getInstance()->postNotification(st_filterAndFrames_notifiction);

    });
    
//    log("close ads or ads finish");
//    Director::getInstance()->resume();
      // m_pTableView->reloadData();

}



void FilterAndFrameLayer::doAction(Node* pNode)
{
    UIButton* lUIButton = (UIButton*)pNode;
    switch (lUIButton->getTag())
    {
        case st_FFLayer_UIButton_back:
        {
#ifndef DEBUG_ADS
            AdsManager::getInstance()->removeAds(kTypeBannerAds);
#endif
            
            MoveBy* moveTo = MoveBy::create(0.5f, Vec2(0, -ScreenAdapterHelper::getRealDesignSize().height + ScreenAdapterHelper::getValue(100)));
            CallFunc* cllFunc = CallFunc::create(CC_CALLBACK_0(FilterAndFrameLayer::reponseDestoryLayer, this));
            this->runAction(Sequence::create(EaseExponentialIn::create(moveTo), cllFunc, NULL));
            EditScene* lEditScene = (EditScene*)this->getParent();
            lEditScene->getEditLayer()->updataTableView();
            
        }
            break;
            
        default:
            break;
    }
}

void FilterAndFrameLayer::addListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event)
    {
        return true;
    };
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void FilterAndFrameLayer::reponseCreateLayer()
{
#ifndef DEBUG_ADS
    AdsManager::getInstance()->showAds(kTypeBannerAds);
#endif
}

void FilterAndFrameLayer::reponseDestoryLayer()
{
#ifndef DEBUG_ADS
    AdsManager::getInstance()->removeAds(kTypeBannerAds);
#endif
    
    this->removeFromParentAndCleanup(true);
}