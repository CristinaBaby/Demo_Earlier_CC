//
//  InspirationLayer.hpp
//  IMG1001
//
//  Created by houhuang on 15/12/10.
//
//

#ifndef InspirationLayer_hpp
#define InspirationLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../data/DataManager.h"
#include "../compoment/ServerSprite.hpp"
#include "../base/SwallowListenerLayer.hpp"
#include "../compoment/STTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class InspirationLayer : public LayerColor, public STTableViewDelegate, public TableViewDataSource
{
public:
    CREATE_FUNC(InspirationLayer);
    bool init();
    
    InspirationLayer();
    ~InspirationLayer();
    
public:

    void reponseMapSprite(Node* ref);
    
    void onEnterTransitionDidFinish();
    void onExit();
    
    
    void setScrollViewOffset();
    
    void downloadJsonInfo();
    
    void waitDownloadJsonSuc(Ref* ref);
    void waitDownloadJsonFai(Ref* ref);
    
    //load new page
    void updateContentSize(Ref* ref);
    
    void removeDialog(Node* node);
    
    void loadTableView();
    
public:
    Size cellSizeForTable(TableView* table);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idex);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
    void tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch);
    void tableCellRemoveTexture(TableView* table, TableViewCell* cell);
    void tableRequestNextPage(TableView* table,ssize_t idx);
    ssize_t tableLastID(TableView* table);
    
    void scrollViewDidScroll(ScrollView* view);
protected:
    int         m_iImageNumber;
    string      m_sImageName;
    
    STTableView*  m_pTableView;
    ScrollView* m_pScrollView;
    
    bool        m_bIsEdit;
    bool        m_bIsNetwork;
    bool        m_bIsShowLoading;
    
    SwallowListenerLayer* m_pLoadingLayer;
    
    Vector<ServerSprite*>   m_vServerSprite;
    
    float       m_fTime;
};

#endif /* InspirationLayer_hpp */
