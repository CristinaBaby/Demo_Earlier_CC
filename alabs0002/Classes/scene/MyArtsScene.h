//
//  MyArtsScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "extensions/cocos-ext.h"
#include "STTableView.h"
#include "MyArtSprite.hpp"

using namespace cocos2d::extension;

class  MyArtsScene : public Scene ,public STTableViewDelegate ,public TableViewDataSource
{
public:
    virtual ~MyArtsScene();
    MyArtsScene();
    
    CREATE_FUNC(MyArtsScene);
    
    virtual bool init();
    void onButton(Ref* pSender, Widget::TouchEventType type);
    
    void reponseDeleteMyArt(Ref* ref);
public:
    void createTableView();
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    virtual void tableCellRemoveTexture(TableView* table, TableViewCell* cell);
    
public:
    void reponseSprite(Node* node);
    
    void onDialog(const string& name);
    void enterNextScene();
private:
    STTableView* _tableView;
    Layout* _placeholder;
    Layout* _root;
    
    int _offset;
    MyArtSprite* m_pMyartSprite;
};