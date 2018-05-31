//
//  LevelMapScene.cpp
//  TIME006
//
//  Created by Steven.Xc.Tian on 14-8-19.
//
//

#include "LevelMapScene.h"
#include "ChooseModeScene.h"
#include "HomeScene.h"
#include "../AppConfig.h"
#include "../AppGlobal.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/EncryptDataHelper.h"
#include "../helpers/PurchaseHelper.h"
#include "../layers/DeluxeLayer.h"
#include "../utilities/STUserDefault.h"
#include "../utilities/STUtility.h"
#include "../widgets/STAlertDialog.h"


USING_NS_CC;
USING_NS_CC_EXT;
USING_NS_ST;

using std::string;
using std::vector;
using std::list;


#define MAP_ROW_NUM               3
#define MAP_COL_NUM               3

enum
{
    z_cell_bg = 1, z_cell_map = 3, z_cell_cover = 5,
    tag_volatile_batchnode = 40, tag_deluxe_layer,
};


// ==================================================================================
/* LevelMapLayer Class */
// ==================================================================================

CCScene* LevelMapLayer::scene(const cocos2d::ccColor4B& color /*= cocos2d::ccc4(0,0,0,0)*/)
{
    CCScene* pScene = CCScene::create();
    if (pScene)
    {
        LevelMapLayer* p_lLayer = LevelMapLayer::create(color);
        if (p_lLayer)
        {
            pScene->addChild(p_lLayer);
        }
    }
    
    return pScene;
}

LevelMapLayer* LevelMapLayer::create(const ccColor4B& color)
{
    LevelMapLayer *pRet = new LevelMapLayer();
    if (pRet && pRet->initColor(color))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    
    return NULL;
}

LevelMapLayer::LevelMapLayer()
{
}

LevelMapLayer::~LevelMapLayer()
{
}

bool LevelMapLayer::initColor(const ccColor4B& color)
{
    bool bRet = false;
    
    do
    {
        CCSize winSize = GET_CCDIRCTOR->getWinSize();
        CC_BREAK_IF(! CCLayerColor::initWithColor(color, winSize.width, winSize.height));
        this->ignoreAnchorPointForPosition(false);
        this->setPosition(ccp_center_screen);
        
        // load sprite sheet
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("choose-map.plist");
        
        
        CCSprite *p_Bg = CCSprite::create("ui03_bg.jpg");
        CC_BREAK_IF(!p_Bg);
        p_Bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        this->addChild(p_Bg);
        
        
        STButton *pBackBtn = STButton::createWithSpriteFrameName(string("ui03_back.png"), string("ui03_back_h.png"), this,
                                                                 callfuncN_selector(LevelMapLayer::onBackClicked));
        CC_BREAK_IF(!pBackBtn);
        this->addChild(pBackBtn, 5);
        pBackBtn->setScale(AppGlobal::UI_SCALE);
        pBackBtn->setAnchorPoint(ccp(0, 1));
        CCRect vRect = VISIBLE_RECT;
        pBackBtn->setPosition(ccp((vRect.getMinX()+get_offset(20)),
                                  (vRect.getMaxY()-get_offset(10))));
        
        
        
        // init page controller size
        CCSize visibleSize = GET_CCDIRCTOR->getVisibleSize();
        mp_PageSize = CCSize(visibleSize.width * 0.9, visibleSize.height * 0.8);
        
        // page controller
        mp_PageController = STPageController::create(this, mp_PageSize);
        CC_BREAK_IF(!mp_PageController);
        mp_PageController->ignoreAnchorPointForPosition(false);
        mp_PageController->setPosition(ccp_get_center(p_Bg->getContentSize()));
        p_Bg->addChild(mp_PageController, 10);
        mp_PageController->setPageControllerListener(this);
        mp_PageController->setDirection(kCCScrollViewDirectionHorizontal);
        mp_PageController->reloadData();
        
        
        // set indicator of page controller
        mp_Indicator = STIndicator::create(getCapacity(), string("dot_2.png"), string("dot_1.png"));
        CC_BREAK_IF(!mp_Indicator);
        // add indicator
        mp_PageController->setIndicator(mp_Indicator);
        p_Bg->addChild(mp_Indicator, 10);
        
        const float layerWidth = this->getContentSize().width;
        const float margin = (layerWidth - mp_Indicator->getContentSize().width) / 2.f;
        mp_Indicator->setPosition(ccp(margin, VISIBLE_RECT.getMinY()+(get_offset(50))));
        
        bRet = true;
    } while (0);
    
    return bRet;
}

void LevelMapLayer::onBackClicked(CCNode*)
{
    GET_CCDIRCTOR->replaceScene(HomeLayer::scene());
}

STPage* LevelMapLayer::getANewPageAtIndex(unsigned index)
{
    return LevelMapPage::create(getPageSize());
}

CCSize LevelMapLayer::getPageSize()
{
    return mp_PageSize;
}

unsigned LevelMapLayer::getCapacity()
{
    return ceilf(AppGlobal::stAllLevelsInformation->count() / (float)(MAP_ROW_NUM * MAP_COL_NUM));
}

void LevelMapLayer::onPageClicked(CCNode* view, unsigned index)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    LevelMapItem* item = (LevelMapItem*)view->getUserObject();
    if (item)
    {

        if (item->isFree)
        {
    
            if (item->isAvailable)
            {
                CCScene* pChooseScene = ChooseModeLayer::scene(index);
                if (pChooseScene)
                {
                    GET_CCDIRCTOR->replaceScene(pChooseScene);
                }
            }
            else
            {
                std::string message = AppGlobal::stAllStrings->valueForKey(strings_not_play_previous)->m_sString;
                std::string btnText = AppGlobal::stAllStrings->valueForKey(strings_button_ok)->m_sString;
                
                STAlertDialog::show(this, message, btnText);
            }
            
        }
        else
        {
            DeluxeLayer *pDeluxe = DeluxeLayer::create();
            PurchaseHelper::getInstance()->setDelegate(this);
            this->addChild(pDeluxe, INT_MAX);
            pDeluxe->setTag(tag_deluxe_layer);
        }
        
    }
}

void LevelMapLayer::purchaseSuccessful(const char* pid)
{
    if (strcmp(pid, st_iap_sku_unlock_all) == 0 || strcmp(pid, st_iap_sku_ad) == 0)
    {
        for (int i = 0; i < getCapacity(); i++)
        {
            mp_PageController->reloadData();
        }
    }
}

void LevelMapLayer::purchaseFailed(const char* pid, int errorCode)
{
}

void LevelMapLayer::restoreSuccessful(const char* pid)
{
    purchaseSuccessful(pid);
}

void LevelMapLayer::restoreFailed(const char* pid, int errorCode)
{
}

void LevelMapLayer::onExit()
{
    PurchaseHelper::getInstance()->setDelegate(NULL);
    CCLayerColor::onExit();
}



// ==================================================================================
/* LevelMapPage Class */
// ==================================================================================

LevelMapPage* LevelMapPage::create(CCSize size, const ccColor4B& color /*= cocos2d::ccc4(0,0,0,0)*/)
{
    LevelMapPage* pRet = new LevelMapPage();
    if (pRet && pRet->initWithColor(color, size))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

LevelMapPage::LevelMapPage()
: mp_CellBg(NULL), mp_CellLock(NULL), mp_CellStarGray(NULL), mp_CellStarYellow(NULL),
mbIsCreated(false)
{
    p_AllPositions = vector<CCSize>();
}

LevelMapPage::~LevelMapPage()
{
    CC_SAFE_RELEASE(mp_CellBg);
    CC_SAFE_RELEASE(mp_CellLock);
    CC_SAFE_RELEASE(mp_CellStarGray);
    CC_SAFE_RELEASE(mp_CellStarYellow);
}

bool LevelMapPage::initWithColor(const cocos2d::ccColor4B& color, cocos2d::CCSize size)
{
    bool bRet = false;
    
    do
    {
        // super class method
        CC_BREAK_IF(! STPage::initWithColor(color, size.width, size.height));
        
        mp_BgPageLayer = CCSpriteBatchNode::create("choose-map.pvr.ccz");
        pBaseLayer->addChild(mp_BgPageLayer, z_cell_bg);
        
        mp_CoverPageLayer = CCSpriteBatchNode::create("choose-map.pvr.ccz");
        pBaseLayer->addChild(mp_CoverPageLayer, z_cell_cover);
        
        // retain sprite frame for reuse
        mp_CellBg = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ui03_frame.png");
        mp_CellBg->retain();
        
        mp_CellLock = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ui03_lock.png");
        mp_CellLock->retain();
        
        mp_CellStarGray = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ui03_star_u.png");
        mp_CellStarGray->retain();
        
        mp_CellStarYellow = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ui03_star.png");
        mp_CellStarYellow->retain();
        
        
        m_CellSize = CCSizeMake(size.width / MAP_COL_NUM, size.height / MAP_ROW_NUM);
        
        for (int i = MAP_ROW_NUM; i > 0; i--)
        {
            for (int j = 0; j < MAP_COL_NUM; j++)
            {
                CCSize position = ccp(m_CellSize.width * j + m_CellSize.width * 0.5f,
                                      m_CellSize.height * i - m_CellSize.height * 0.5f);
                
                p_AllPositions.push_back(position);
            }
        }
        
        bRet = true;
    } while (0);
    
    return bRet;
}

void LevelMapPage:: reset()
{
    mbIsCreated = false;
    mp_CoverPageLayer->removeAllChildren();
    mp_BgPageLayer->removeAllChildren();
    
    CCNode *node = pBaseLayer->getChildByTag(tag_volatile_batchnode);
    while (node) {
        node->removeFromParent();
        node = pBaseLayer->getChildByTag(tag_volatile_batchnode);
    }
}

void LevelMapPage::refreshPage(unsigned idx)
{
    if (mbIsCreated) return;
    
    mbIsCreated = true;
    const unsigned baseIdx = idx * MAP_ROW_NUM * MAP_COL_NUM;
    unsigned offset = 0;
    
    float deltay = get_offset(6);
    for (vector<int>::size_type i = 0; i != p_AllPositions.size(); i++)
    {
        const unsigned dataIdx = baseIdx + offset;
        offset++;
        
        if (dataIdx >= AppGlobal::stAllLevelsInformation->count())
            break;
        
        // cell background
        CCSprite *cellbg = CCSprite::createWithSpriteFrame(mp_CellBg);
        CC_BREAK_IF (!cellbg);
        cellbg->setPosition(p_AllPositions[i]);
        cellbg->setScale(AppGlobal::UI_SCALE);
        mp_BgPageLayer->addChild(cellbg);
        
        // add level thumbnail
        mapRootPath = st_maps_root;
        mapRootPath.append(st_file_seperator);
        // maps/
        
        CCDictionary* levelInfo  = dynamic_cast<CCDictionary*>(AppGlobal::stAllLevelsInformation->objectAtIndex(dataIdx));
        
        // identifer
        const CCString* folderID = levelInfo->valueForKey(st_level_folder_id);
        mapRootPath.append(folderID->m_sString).append(st_file_seperator);
        
        // thumbnail path
        const CCString* thumb = levelInfo->valueForKey(st_level_bg_thumb);
        string thumbPath = mapRootPath;
        thumbPath.append(thumb->m_sString);
        
        CCSprite *picture = CCSprite::create(thumbPath.c_str());
        CC_BREAK_IF(!picture);
        picture->setPosition(p_AllPositions[i]+ccp(1.5, deltay - 3));
        picture->setScale(AppGlobal::UI_SCALE);
        picture->setTag(tag_volatile_batchnode);
        pBaseLayer->addChild(picture, z_cell_map);
        
        
        // add stars and locks
        bool isAvailable = false;
        // is free?
        const CCString* free = levelInfo->valueForKey(st_level_free);
        bool isFree = STUtility::parseBoolean(free->m_sString);
        
#ifdef DEBUG_ALL_LEVELS
        isFree = true;
#endif

        if (!isFree && !PurchaseHelper::isPurchased)
        {
            isFree = false;
            
            CCLayerColor *colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 120),
                                                            picture->getContentSize().width,
                                                            picture->getContentSize().height);
            picture->addChild(colorLayer);
            colorLayer->ignoreAnchorPointForPosition(false);
            colorLayer->setAnchorPoint(CCPointZero);
            
            
            CCSprite *lock = CCSprite::createWithSpriteFrame(mp_CellLock);
            CC_BREAK_IF(!lock);
            lock->setTag(tag_volatile_batchnode);
            lock->setScale(AppGlobal::UI_SCALE);
            mp_CoverPageLayer->addChild(lock);
            lock->setPosition(p_AllPositions[i]);
        }
        else
        {
            isFree = true;
            
            const int levelComplete = STLoadIntegerFromUD(key_complete_flag, 0);
            bool _available = (dataIdx > levelComplete);
            
#ifdef DEBUG_ALL_LEVELS
            _available = false;
#endif
            
            if (_available)
            {
                isAvailable = false;
                
                CCLayerColor *colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 120),
                                                           picture->getContentSize().width,
                                                           picture->getContentSize().height);
                picture->addChild(colorLayer);
                colorLayer->ignoreAnchorPointForPosition(false);
                colorLayer->setAnchorPoint(CCPointZero);
            }
            else
            {
                isAvailable = true;
                
                string kFinishPicture = "";
                string kFinishSilhouette = "";
                string kFinishWord = "";
                
                kFinishPicture.append(key_level_picture).append(folderID->m_sString);
                kFinishSilhouette.append(key_level_silhouette).append(folderID->m_sString);
                kFinishWord.append(key_level_word).append(folderID->m_sString);
                
                unsigned starNum = 0;
                bool bPict = STLoadBooleanFromUD(kFinishPicture.c_str(), false);
                bool bSilh = STLoadBooleanFromUD(kFinishSilhouette.c_str(), false);
                bool bWord = STLoadBooleanFromUD(kFinishWord.c_str(), false);
                
                // 查看已完成几个模式，获得了几个颗星
                if (bPict) starNum++;
                if (bSilh) starNum++;
                if (bWord) starNum++;
                CCLOG("star number %d", starNum);
                
                float starBaseLine = cellbg->boundingBox().origin.y + cellbg->boundingBox().size.height * 0.15;
                for (int in = 0; in < 3; in++)
                {
                    CCSprite *star = NULL;
                    if (in < starNum)
                        star = CCSprite::createWithSpriteFrame(mp_CellStarYellow);
                    else
                        star = CCSprite::createWithSpriteFrame(mp_CellStarGray);
                    
                    CC_BREAK_IF(!star);
                    star->setAnchorPoint(ccp(.5, 0));
                    star->setTag(tag_volatile_batchnode);
                    star->setScale(AppGlobal::UI_SCALE);
                    mp_CoverPageLayer->addChild(star);
                    float px = p_AllPositions[i].width + star->boundingBox().size.width * (in - 1);
                    star->setPosition(ccp(px, starBaseLine));
                }
            }
            
        }
        
        // set item cell for touch event
        LevelMapItem *cell = new LevelMapItem(dataIdx, isAvailable, isFree);
        cellbg->setUserObject(cell);
    }
}

CCNode* LevelMapPage::onClickedPage(cocos2d::CCTouch *pTouch)
{
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(mp_BgPageLayer->getChildren(), pObj)
    {
        CCNode* child = dynamic_cast<CCNode*>(pObj);
        if (!child && !child->isVisible())
        {
            continue;
        }
        
        CCRect bb = child->boundingBox();
        bb.origin = child->getParent()->convertToWorldSpace(bb.origin);
        
        if (bb.containsPoint(pTouch->getLocation()))
        {
            return child;
        }
    }
    
    return NULL;
}

