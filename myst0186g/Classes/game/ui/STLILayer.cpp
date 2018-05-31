//
//  STLILayer.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-8.
//
//

#include "STLILayer.h"
#include "STGameUILabelTTF.h"
#include "STPageItemGameLabel.h"
#include "STPageItemGameImage.h"
#include "../../widgets/STButtonMenu.h"
#include "../../utilities/STUtility.h"
#include "../../helpers/AudioHelper.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespaceST::STUtility;

enum
{
    tag_button_next = 19, tag_button_previous,
};

#define LABEL_ROW_NUM               2
#define LABEL_COL_NUM               3
#define IMAGE_ROW_NUM               1
#define IMAGE_COL_NUM               5

#define SCROLL_VIEW_SIZE            CCSizeMake2(1180, 170)

// when occur skill bonus event, set label to this color.
#define TextSpecialColor            ccc3(255, 252, 172)

STLILayer* STLILayer::create(GameScene* gameScene)
{
    STLILayer* pRet = new STLILayer(gameScene);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

STLILayer * STLILayer::create(const ccColor4B& color, GameScene* gameScene)
{
    CCSize s = GET_CCDIRCTOR->getWinSize();
    
    STLILayer * pLayer = new STLILayer(gameScene);
    if(pLayer && pLayer->initWithColor(color, s.width, s.height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

bool STLILayer::initWithColor(const ccColor4B& color, GLfloat width, GLfloat height)
{
    bool bRet = false;
    
    do
    {
        // call super class method
        CC_BREAK_IF(! CCLayerColor::initWithColor(color, width, height));
        
        // set size and position
        this->setContentSize(CCSizeMake2(1390, 170));
        this->setPosition(ccp2(330, 0));
        
        // previous button
        pBtnPrevious = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui04_previous.png"),
                                                CCSprite::createWithSpriteFrameName("ui04_previous_h.png"),
                                                this, menu_selector(STLILayer::onPageClicked));
        CC_BREAK_IF(!pBtnPrevious);
        pBtnPrevious->setTag(tag_button_previous);
        pBtnPrevious->setAnchorPoint(ccp(0, 0.5));
        pBtnPrevious->setPosition(ccp2(0, (170 / 2)));
        // when init on the first layer, so set left button invisible
        pBtnPrevious->setVisible(false);
        
        // next button
        pBtnNext = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ui04_next.png"),
                                            CCSprite::createWithSpriteFrameName("ui04_next_h.png"),
                                            this, menu_selector(STLILayer::onPageClicked));
        CC_BREAK_IF(!pBtnNext);
        pBtnNext->setTag(tag_button_next);
        pBtnNext->setAnchorPoint(ccp(1, 0.5));
        pBtnNext->setPosition(ccp2(1390, (170 / 2)));
        
        //
        STButtonMenu* pMenu = STButtonMenu::create(pBtnPrevious, pBtnNext, NULL);
        CC_BREAK_IF(!pMenu);
        this->addChild(pMenu);
        
        
        // init page controller.
        mp_PageController = STPageController::create(this, SCROLL_VIEW_SIZE);
        CC_BREAK_IF(!mp_PageController);
        mp_PageController->setPosition(ccp(((this->getContentSize().width-SCROLL_VIEW_SIZE.width) / 2), 0));
        this->addChild(mp_PageController, 10);
        mp_PageController->setPageControllerListener(this);
        mp_PageController->setDirection(kCCScrollViewDirectionHorizontal);
        mp_PageController->reloadData();
        
        
        bRet = true;
    } while (0);
    
    return bRet;
}

STPage* STLILayer::getANewPageAtIndex(unsigned index)
{
    STPage* pPage = NULL;
    
    if (mpGameScene->mSearchMode == Name)
    {
        pPage = STPageItemGameLabel::create(mpGameScene->stAllHiddObjectsTexts, mpGameScene->getHiddObjs(), getPageSize(),
                                            LABEL_ROW_NUM, LABEL_COL_NUM, index);
    }
    else
    {
        // "Physical" and "Silhouette" uses same page.
        pPage = STPageItemGameImage::create(mp_a_SpriteFrameArray, getPageSize(), IMAGE_ROW_NUM, IMAGE_COL_NUM, index);
    }
    
    return pPage;
}

CCSize STLILayer::getPageSize()
{
    return SCROLL_VIEW_SIZE;
}

unsigned STLILayer::getCapacity()
{
    int page = 0;
    
    switch (mpGameScene->mSearchMode)
    {
        case Name:
            page = ceilf((float)(mpGameScene->getHiddObjs()->count()) / (LABEL_ROW_NUM * LABEL_COL_NUM));
            break;
            
        default: // "Physical" and "Silhouette" uses same layout
            page = ceilf((float)(mpGameScene->getHiddObjs()->count()) / (IMAGE_ROW_NUM * IMAGE_COL_NUM));
            
            break;
    }
    
    return page;
}

void STLILayer::onPageClicked(CCObject * btn)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    const int tag = (dynamic_cast<CCNode*>(btn))->getTag();
    
    if (tag_button_previous == tag)
    {
        mp_PageController->turnPage(turnLeftPage);
    }
    else if(tag_button_next == tag)
    {
        mp_PageController->turnPage(turnRightPage);
    }
}

void STLILayer::onTurnPage(unsigned focusPageIndex)
{
    pBtnPrevious->setVisible(true);
    pBtnNext->setVisible(true);
    
    if (focusPageIndex == 0)
    {
        pBtnPrevious->setVisible(false);
    }
    if (focusPageIndex == (this->getCapacity() - 1))
    {
        pBtnNext->setVisible(false);
    }
}

void STLILayer::refresh(int objIdx)
{
    // this means current page is the last one.
    if (mp_PageController->getFocusPageIndex() + 1 >= this->getCapacity())
    {
        pBtnNext->setVisible(false);
    }
    
    if (mp_a_SpriteFrameArray && objIdx > -1)
    {
        mp_a_SpriteFrameArray->fastRemoveObjectAtIndex(objIdx);
    }
    
    CCPoint pre = mp_PageController->getContentOffset();
    mp_PageController->reloadData();
    mp_PageController->setContentOffset(pre);
    
    if (autoTurnPage())
    {
        mp_PageController->turnPage(turnLeftPage);
    }
}

void STLILayer::turnToFirstPage()
{
    mp_PageController->turnToPage(0);
}

unsigned STLILayer::getRandomIndexInPage()
{
    unsigned pageIdx = mp_PageController->getFocusPageIndex();
    unsigned pageCapacity;
    switch (mpGameScene->mSearchMode)
    {
        case Name:
            pageCapacity = LABEL_ROW_NUM * LABEL_COL_NUM;
            break;
        default:
            pageCapacity = IMAGE_COL_NUM * IMAGE_ROW_NUM;
            break;
            
    }
    
    const unsigned baseIdx = pageIdx * pageCapacity;
    
    unsigned limited = mpGameScene->getHiddObjs()->count();
    if (pageCapacity < limited)
    {
        limited = pageCapacity;
    }
    
    int offset = STUtility::getRandom(limited);
    
    unsigned index = baseIdx + offset;
    
    
    return index;
}

bool STLILayer::autoTurnPage()
{
    const unsigned pageIdx = mp_PageController->getFocusPageIndex();
    unsigned pageCapacity;
    switch (mpGameScene->mSearchMode)
    {
        case Name:
            pageCapacity = LABEL_ROW_NUM * LABEL_COL_NUM;
            break;
        default:
            pageCapacity = IMAGE_COL_NUM * IMAGE_ROW_NUM;
            break;
            
    }
    
    const unsigned baseIdx = pageIdx * pageCapacity;
    
    const unsigned limited = mpGameScene->getHiddObjs()->count();
    
    return (baseIdx >= limited);
}

STLILayer::STLILayer(GameScene* gameScene) : mp_PageController(NULL)
{
    mpGameScene = gameScene;
    
    if (mpGameScene->mSearchMode == Name)
    {
        mp_a_SpriteFrameArray = NULL;
        return;
    }
    
    mp_a_SpriteFrameArray = new CCArray();
    
    bool isPictureMode = (mpGameScene->mSearchMode == Picture) ? true : false;
    
    // packed texture path
    std::string tipsObjsPlist = mpGameScene->stLevelResRoot;
    if (isPictureMode)
    {
        tipsObjsPlist.append(st_file_seperator).append(mpGameScene->stLevelId)
        .append(st_map_hidd_objs_pic_suffix);
        
    }
    else
    {
        tipsObjsPlist.append(st_file_seperator).append(mpGameScene->stLevelId)
        .append(st_map_hidd_objs_silhouette_pic_suffix);
    }
    
    // add sprite sheet via .plist file
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tipsObjsPlist.c_str());
    
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(mpGameScene->getHiddObjs(), pObj)
    {
        CCString *pID = dynamic_cast<CCString*>(pObj);
        CC_BREAK_IF(!pID);
        
        std::string picName = pID->m_sString;
        if (isPictureMode)
            picName.append(st_map_tips_objs_pic_suffix);
        else
            picName.append(st_map_tips_objs_silhouette_pic_suffix);
        
        
        CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(picName.c_str());
        CC_BREAK_IF(!pFrame);
        
        mp_a_SpriteFrameArray->addObject(pFrame);
    }
}

STLILayer::~STLILayer()
{
    CC_SAFE_DELETE(mp_a_SpriteFrameArray);
}