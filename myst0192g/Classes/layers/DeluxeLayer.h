//
//  DeluxeLayer.h
//  TIME006
//
//  Created by Steven.Xc.Tian.
//
//

#ifndef __TIME006__DeluxeLayer__
#define __TIME006__DeluxeLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../modules/STInAppBilling.h"
#include "../widgets/CoverLayer.h"

using namespace cocos2d;

class DeluxeLayer : public CoverLayer, public  cocos2d::extension::CCTableViewDataSource
{
    STInAppBilling  *m_pIAB;
    int _mTouchPriority;
    cocos2d::CCSize TABLE_CELL_SIZE;
    
public:
    static DeluxeLayer* create();
    static DeluxeLayer* createWithColor(cocos2d::ccColor4B color);
    
    DeluxeLayer();
    virtual ~DeluxeLayer();
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color);
    virtual void onExit();
    virtual void onEnter();
    
public:
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
private:
    
    void excuteAnimation();
    
    void onCloseClicked(cocos2d::CCNode*);
    void onBuyClicked(cocos2d::CCNode*);
    void onRestoreClicked(cocos2d::CCNode*);

public:
    void update(float dt);
    cocos2d::extension::CCTableView* m_pTable;
    
    cocos2d::CCSprite*   m_pProgressPoint;
    cocos2d::CCSprite* progressBar_bg;

};

#endif /* defined(__TIME006__DeluxeLayer__) */
