//
//  ChooseMapLayer.hpp
//  ColorGarden
//
//  Created by houhuang on 16/1/12.
//
//

#ifndef ChooseMapLayer_hpp
#define ChooseMapLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../commpoent/STTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ChooseMapLayer : public LayerColor, public TableViewDataSource,public STTableViewDelegate
{
public:
    CREATE_FUNC(ChooseMapLayer);
    
    ChooseMapLayer();
    ~ChooseMapLayer();
    bool init();
public:
    void tableCellRemoveTexture(TableView* table, TableViewCell* cell);

public:
    Size cellSizeForTable(TableView* table);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idex);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
    void tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch);

public:
    void scrollViewDidScroll(ScrollView* view);
    void scrollViewDidZoom(ScrollView* view);
    
public:
    void updatePageLabel(int id);

    void removeDialog(Node* pSender);
    void startDownload(cocos2d::Node *pSender);

protected:
    STTableView*    m_pTableView;
    Sprite*         m_pDotSprite;
    Label*          m_pPageLabel;
};

#endif /* ChooseMapLayer_hpp */
