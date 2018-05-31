//
//  GraphScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "UISlider.h"
#include "extensions/cocos-ext.h"

using namespace cocos2d::extension;

class  GraphScene : public Scene, public TableViewDataSource, public TableViewDelegate
{
public :
    virtual ~GraphScene();
    GraphScene();

    CREATE_FUNC(GraphScene);
    
    virtual bool init() override;
    void onButton(Ref* pSender, Widget::TouchEventType type);
    void onDialog(const string& name);
    void reloadData();
    void checkUpdate(float dt);
    void onSlider(Ref *pSender, Slider::EventType type);
    
    void waitDownloadFinish(Ref* ref);
public:
    void createTableView();
    virtual void tableCellTouched(TableView* table, TableViewCell* cell) override;
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) override;
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(TableView *table) override;
    virtual void scrollViewDidScroll(extension::ScrollView* view) override;
    
private:
    Layout * _root;
    ListView * _placeholder;
    TableView * _tableView;
    int _graphIndex = 0;
    Slider * _slider;
    Text *   _text;
};