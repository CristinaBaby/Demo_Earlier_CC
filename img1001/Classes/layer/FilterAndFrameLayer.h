//
//  FilterAndFrameLayer.h
//  ColorFree
//
//  Created by houhuang on 15/9/9.
//
//

#ifndef __ColorFree__FilterAndFrameLayer__
#define __ColorFree__FilterAndFrameLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LoadLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;

class FilterAndFrameLayer : public LayerColor, public TableViewDelegate, public TableViewDataSource
{
public:
//    CREATE_FUNC(FilterAndFrameLayer);
    static FilterAndFrameLayer* create(Sprite* pSprite);
    bool init(Sprite* pSprite);
    ~FilterAndFrameLayer();
    FilterAndFrameLayer();
    
    void onEnter();
    void onExit();
public:
    Size cellSizeForTable(TableView* table);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idex);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
public:
    void reponseMenu(Ref* ref);
    void doAction(Node* pNode);
    
    void reponseFramesMenu(Ref* ref);
    
    void reponseDestoryLayer();
    void reponseCreateLayer();
    
    //屏蔽点击事件
    void addListener();
    
    void waitMessage(Ref* ref);
    void waitMessageRewardSu(Ref* ref);
    void waitMessageDownLoadSuc(Ref* ref);
    
    void delayeDestoryRotateLayer(float dt);
    void delayeUpdateTableView(float dt);
    
    void delayeLoadRotateLayer(float dt);
public:
    void initAdsDelegate();
    void releaseAdsDelegate();
    void onAdsLoaded(void);
    void onAdsLoadFailed(std::string error);
    void onAdsClicked(void);
    void onAdsExpanded(void);
    void onAdsCollapsed(std::string unitId, int amount, bool isSkipped);

protected:
    TableView*  m_pTableView;
    Label*  m_pLabelFilters;
    Label*  m_pLabelFrames;
    bool    m_bIsFilters;
    
    Sprite* m_pMoreFilterSpr;
    
    Label* m_pLabel;
    LoadLayer*  m_pLoadLayer;

    bool    m_bIsPreloadedFinish;
    bool    m_bIsUpdateDataSuc;
    bool    m_bIsShowRewardSuc;
    
    float   m_iTableOffset;
    
    int     m_iCurrentFramesMenuTag;
};

#endif /* defined(__ColorFree__FilterAndFrameLayer__) */
