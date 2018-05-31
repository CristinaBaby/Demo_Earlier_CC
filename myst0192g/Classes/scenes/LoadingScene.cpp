//
//  LoadingScene.cpp
//  LVUP004
//
//  Created by Steven.Xc.Tian on 13-11-18.
//
//

#include <string.h>
#include "LoadingScene.h"
#include "AppGlobal.h"
#include "HomeScene.h"
#include "../AppGlobal.h"
#include "../FileModifiers.h"
#include "../helpers/EncryptDataHelper.h"
#include "../helpers/PurchaseHelper.h"
#include "../utilities/CSVParse.h"
#include "../widgets/STUILayer.h"
#include "../utilities/STUtility.h"


USING_NS_CC;
using std::string;

enum {
    tag_progress_1 = 70, tag_progress_2,
};

CCScene* LoadingLayer::scene()
{
    CCScene* pScene = CCScene::create();
    if (pScene)
    {
        LoadingLayer* p_lLayer = LoadingLayer::create();
        if (p_lLayer)
        {
            pScene->addChild(p_lLayer);
        }
    }
    
    return pScene;
}

bool LoadingLayer::init()
{
    bool pRet = false;
    
    do
    {
        // load sprite sheet
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("load-home.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pop-window.plist");
        
        CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
        
        CCSprite *p_Bg = CCSprite::create("ui01_menu_bg.jpg");
        CC_BREAK_IF(!p_Bg);
        p_Bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        this->addChild(p_Bg);
        
        STUILayer* uiLayer = STUILayer::create();
        CC_BREAK_IF(!uiLayer);
        this->addChild(uiLayer);
        
        //
        CCSprite* progressBG = CCSprite::createWithSpriteFrameName("ui09_bar_bg.png");
        progressBG->setPosition(ccp_horizontal_center(690));
        progressBG->setAnchorPoint(ccp(.5, .5));
        uiLayer->addChild(progressBG);
        
        
        CCSprite* text = CCSprite::createWithSpriteFrameName("ui09_loading.png");
        text->setPosition(ccp_horizontal_center(600));
        uiLayer->addChild(text);
        
        m_pProgress = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("ui09_bar.png"));
        CC_BREAK_IF(!m_pProgress);
        
        m_pProgress->setType(kCCProgressTimerTypeBar);
        m_pProgress->setMidpoint(ccp(0, 1));
        m_pProgress->setBarChangeRate(ccp(1, 0));
        m_pProgress->setPosition(ccp(progressBG->getContentSize().width / 2, progressBG->getContentSize().height / 2));
        m_pProgress->setPercentage(5);
        progressBG->addChild(m_pProgress);
        
        loadMapInfos();
        
        pRet = true;
    } while (0);
    
    return pRet;
}

void LoadingLayer::onEnter()
{
    CCLayer::onEnter();
}

void LoadingLayer::onExit()
{
    CCLayer::onExit();
}

void LoadingLayer::onLoadFinish()
{
    PurchaseHelper::isPurchased   = EncryptDataHelper::getPurchaseFlag(key_iap_purchased_flag);
    PurchaseHelper::isAdPurchased = EncryptDataHelper::getPurchaseFlag(key_iap_ad_purchased_flag);
    
    CCScene *pScene = HomeLayer::scene();
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void LoadingLayer::load()
{
    string stringsPath = st_strings_root;
    stringsPath.append(st_file_seperator);
    stringsPath.append(st_strings_file);
    
    //    CCLOG("texts path is %s", textsPath.c_str());
    
    // below is strings
    AppGlobal::stAllStrings = new CCDictionary();
    
    CSVParse *csv = CSVParse::create(stringsPath.c_str());
    if (!csv)
    {
        CCLOGERROR("Can't load CSV file: %s", stringsPath.c_str());
        return;
    }
    
    unsigned rown = csv->getRows();
    
    // exclude the first row which is the column's name
    for (int r = 1; r < rown; r++)
    {
        const char* key = csv->getDatas(r, st_disname_id);
        CCString* value = CCString::create(csv->getDatas(r, st_disname_Name));
        // add names to dictionary
        AppGlobal::stAllStrings->setObject(value, key);
    }
    
    
    
    
    CCAction* action = CCSequence::create(
                                          CCProgressTo::create(.5, 85),
                                          CCDelayTime::create(0.3),
                                          CCCallFunc::create(this, callfunc_selector(LoadingLayer::loadSKU)),
                                          NULL);
    m_pProgress->runAction(action);
}

void LoadingLayer::loadMapInfos()
{
    // play loading animation, and delay until this step over
    CCAction* action = CCSequence::create(CCProgressTo::create(0.1, 50), CCDelayTime::create(100), NULL);
    m_pProgress->runAction(action);
    
    
    /* load Description.csv file */
    string descFilePath = st_maps_root;
    descFilePath.append(st_file_seperator).append(st_maps_descfile_name);
    
    // must call release() when game exit.
    AppGlobal::stAllLevelsInformation = CCArray::create();
    AppGlobal::stAllLevelsInformation->retain();
    
    CSVParse* desc_csv = CSVParse::create(descFilePath.c_str());
    if (!desc_csv)
    {
        CCLOGERROR("Can't load CSV file: %s", descFilePath.c_str());
        return;
    }
    
    const unsigned rown = desc_csv->getRows();
    // exclude the first row which is the column's name
    for (int r = 1; r < rown; r++)
    {
        CCDictionary* eachLevel = CCDictionary::create();
        
        CCString* freeFlag = CCString::create(desc_csv->getDatas(r, st_level_free));
        eachLevel->setObject(freeFlag, st_level_free);
        
        CCString* location = CCString::create(desc_csv->getDatas(r, st_level_location));
        eachLevel->setObject(location, st_level_location);
        
        // folder name
        CCString* folder_id = CCString::create(desc_csv->getDatas(r, st_level_folder_id));
        eachLevel->setObject(folder_id, st_level_folder_id);
        
        // resources name
        CCString* res_id = CCString::create(desc_csv->getDatas(r, st_level_res_id));
        eachLevel->setObject(res_id, st_level_res_id);
        
        CCString* background = CCString::create(desc_csv->getDatas(r, st_level_background));
        eachLevel->setObject(background, st_level_background);
        
        CCString* thumb = CCString::create(desc_csv->getDatas(r, st_level_bg_thumb));
        eachLevel->setObject(thumb, st_level_bg_thumb);
        
        AppGlobal::stAllLevelsInformation->addObject(eachLevel);
    }
    
    CC_SAFE_RELEASE(desc_csv);
    
    /* load MapsConfig.csv file */
    AppGlobal::stMapsConfig = CCDictionary::create();
    AppGlobal::stMapsConfig->retain();
    
    string mapsConfigPath = st_maps_root;
    mapsConfigPath.append(st_file_seperator).append(st_maps_config_file);
    CSVParse* mc_csv = CSVParse::create(mapsConfigPath.c_str());
    if (!mc_csv)
    {
        CCLOGERROR("Can't load MapsConfig file: %s", mapsConfigPath.c_str());
        return;
    }
    
    CCString* version = CCString::create(mc_csv->getDatas(1, st_maps_config_version));
    AppGlobal::stMapsConfig->setObject(version, st_maps_config_version);
    
    CCString* capacity = CCString::create(mc_csv->getDatas(1, st_maps_config_capacity));
    AppGlobal::stMapsConfig->setObject(capacity, st_maps_config_capacity);
    
    CCString* free = CCString::create(mc_csv->getDatas(1, st_maps_config_free));
    AppGlobal::stMapsConfig->setObject(free, st_maps_config_free);
    
    CC_SAFE_RELEASE(mc_csv);
    
    m_pProgress->stopAction(action);
    
    CCAction* goon = CCSequence::create(
                                        CCProgressTo::create(.5, 50),
                                        CCCallFunc::create(this, callfunc_selector(LoadingLayer::load)),
                                        NULL);
    m_pProgress->runAction(goon);
}

void LoadingLayer:: loadSKU()
{
    CSVParse* csv = CSVParse::create("sku/sku.csv");
    if (!csv)
    {
        CCLOGERROR("Can't load sku.csv");
        return;
    }
    
    AppGlobal::stSKUInformation = CCArray::create();
    CC_SAFE_RETAIN(AppGlobal::stSKUInformation);
    
    unsigned rown = csv->getRows();
    // exclude the first row which is the column's name
    for (int r = 1; r < rown; r++)
    {
        CCDictionary *info = CCDictionary::create();
        
        bool isStroe = namespaceST::STUtility::parseBoolean(csv->getDatas(r, st_sku_store));
        info->setObject(CCBool::create(isStroe), st_sku_store);
        
        CCString* sku_ios = CCString::create(csv->getDatas(r, st_sku_ios));
        info->setObject(sku_ios, st_sku_ios);
        
        CCString* sku_android = CCString::create(csv->getDatas(r, st_sku_android));
        info->setObject(sku_android, st_sku_android);
        
        CCString* background = CCString::create(csv->getDatas(r, st_sku_bgname));
        info->setObject(background, st_sku_bgname);
        
        
        AppGlobal::stSKUInformation->addObject(info);
    }
    
    CC_SAFE_RELEASE(csv);
    
    
    
    
    CCAction* action = CCSequence::create(
                                          CCProgressTo::create(.5, 100),
                                          CCDelayTime::create(0.3),
                                          CCCallFunc::create(this, callfunc_selector(LoadingLayer::onLoadFinish)),
                                          NULL);
    m_pProgress->runAction(action);
    
}

LoadingLayer::~LoadingLayer()
{
}


