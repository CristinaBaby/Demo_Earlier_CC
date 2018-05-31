//
//  LevelMapScene.h
//  TIME006
//
//  Created by Steven.Xc.Tian on 14-8-19.
//
//

#ifndef __TIME006__LevelMapScene__
#define __TIME006__LevelMapScene__

#include "cocos2d.h"
#include "../modules/STIABDelegate.h"
#include "../widgets/STPageController.h"


class LevelMapItem : public ItemCell
{
public:
    LevelMapItem(unsigned idx, bool available, bool free)
    : ItemCell(idx), isAvailable(available), isFree(free)
    {
    }
    virtual ~LevelMapItem(){};
    
    bool        isFree;
    bool        isAvailable;
};


class LevelMapPage : public STPage
{
    std::vector<cocos2d::CCSize>    p_AllPositions;
    std::string                     mapRootPath;
    cocos2d::CCSize                 m_CellSize;
    cocos2d::CCSpriteFrame          *mp_CellBg, *mp_CellLock, *mp_CellStarGray, *mp_CellStarYellow;
    cocos2d::CCSpriteBatchNode      *mp_BgPageLayer, *mp_CoverPageLayer;
    bool                            mbIsCreated;
    
public:
    /**
     *  @param data. just pass BootMoreGames::mp_a_AllGames
     */
    static LevelMapPage* create(cocos2d::CCSize size, const cocos2d::ccColor4B& color = cocos2d::ccc4(0,0,0,0));
    LevelMapPage();
    virtual ~LevelMapPage();
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color, cocos2d::CCSize size);
    virtual void refreshPage(unsigned idx);
    virtual CCNode* onClickedPage(cocos2d::CCTouch *pTouch);
    
    virtual void reset();
};



class LevelMapLayer : public cocos2d::CCLayerColor, public STTableDelegate, public STPageControllerListener,
public STIABDelegate
{
    STPageController    *mp_PageController;
    STIndicator         *mp_Indicator;
    cocos2d::CCSize     mp_PageSize;
    
    void onBackClicked(CCNode*);
    
public:
    static cocos2d::CCScene* scene(const cocos2d::ccColor4B& color = cocos2d::ccc4(0,0,0,0));
    
    static LevelMapLayer* create(const cocos2d::ccColor4B& color);
    LevelMapLayer();
    virtual ~LevelMapLayer();
    
    virtual bool initColor(const cocos2d::ccColor4B& color);
    void onExit();
    
public:
    virtual STPage* getANewPageAtIndex(unsigned index);

    virtual cocos2d::CCSize getPageSize();
    
    virtual unsigned getCapacity();
    
    virtual void onPageClicked(cocos2d::CCNode* view, unsigned index);
    
    virtual void onTurnPage(unsigned focusPageIndex){};
    
public:
    void purchaseSuccessful(const char* pid);
    void purchaseFailed(const char* pid, int errorCode);
    void restoreSuccessful(const char* pid);
    void restoreFailed(const char* pid, int errorCode);
};



#endif /* defined(__TIME006__LevelMapScene__) */
