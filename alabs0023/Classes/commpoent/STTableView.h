//
//  STTableView.hpp
//  ColorGarden
//
//  Created by lidong on 15/12/29.
//
//

#ifndef STTableView_hpp
#define STTableView_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
USING_NS_CC_EXT;


class STTableViewDelegate : public TableViewDelegate {
    
    
public:
    virtual void tableCellTouched(TableView* table, TableViewCell* cell,Touch* pTouch){};
    virtual void tableCellRemoveTexture(TableView* table, TableViewCell* cell){}
    virtual void tableRequestNextPage(TableView* table,ssize_t idx){}
    virtual ssize_t tableLastID(TableView* table){ return 0;}
    
    virtual void tableUpdateFunc(TableView* table,TableViewCell* cell){}
    
};

class STTableView : public TableView {
    
protected:
    bool _isUpdateShow;
    
public:
    static STTableView* create(TableViewDataSource* dataSource, Size size);
    static STTableView* create(TableViewDataSource* dataSource, Size size, Node *container);

    void initSetting();
    
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
    
    // Overrides
    virtual void scrollViewDidScroll(ScrollView* view) override;
    
    void updateContentSize();
    
    void setIsUpdateShow(bool pBool);
    
    void update(float dt);
    
};


#endif /* STTableView_hpp */
