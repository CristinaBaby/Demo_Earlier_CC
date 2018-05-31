//
//  InspirationScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "extensions/cocos-ext.h"
#include "STTableView.h"
#include "ServerSprite.hpp"
#include "SwallowListenerLayer.hpp"

using namespace cocos2d::extension;

class  InspirationScene : public Scene ,public STTableViewDelegate ,public TableViewDataSource
{
public:
    virtual ~InspirationScene();
    InspirationScene();
    
    CREATE_FUNC(InspirationScene);
    
    virtual bool init();
    void onButton(Ref* pSender, Widget::TouchEventType type);
    
public:
    
    void onDialog(const string & name);
    
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
    Size tableCellSizeForIndex(TableView *table, ssize_t idx);
//    Size cellSizeForTable(TableView* table);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idex);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
    void tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch);
    void tableCellRemoveTexture(TableView* table, TableViewCell* cell);
    void tableRequestNextPage(TableView* table,ssize_t idx);
    ssize_t tableLastID(TableView* table);
    
//    void scrollViewDidScroll(ScrollView* view);
protected:
    int         m_iImageNumber;
    string      m_sImageName;
    
    STTableView*  m_pTableView;
    cocos2d::extension::ScrollView* m_pScrollView;
    
    bool        m_bIsEdit;
    bool        m_bIsNetwork;
    bool        m_bIsShowLoading;
    
    SwallowListenerLayer* m_pLoadingLayer;
    
    Vector<ServerSprite*>   m_vServerSprite;
    
    float       m_fTime;
    Layout*     _placeholder;
    Layout*     _root;
};