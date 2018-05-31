//
//  ScrollColorLayer.h
//  ColorGarden
//
//  Created by lidong on 15/8/4.
//
//

#ifndef __ColorGarden__ScrollColorLayer__
#define __ColorGarden__ScrollColorLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../game/DataManager.h"
#include "../commpoent/ColorSprite.h"
USING_NS_CC;
USING_NS_CC_EXT;

class ScrollColorLayer : public Layer ,public ColorSpriteDelegate , public TableViewDelegate,public TableViewDataSource{
    
protected:

    std::vector<ColorData> _colorDatas;
    
    float _distance;
    
    TableView* m_pTableView;
    ScrollView* _scrollView;
    bool _isTouchEnable;
    Vec2 _startPoint;
    Vec2 _orginPoint;
    
    bool _isMayExcuteTouchMove;
    bool _isTouchMoving;
    int _currentPage;
    vector<ColorSprite*> _colorSprites;
    vector<Sprite*> _dots;
    Vec2 _firstOrginPoint;
    
    bool _isShowingTutorial;
    
    Size _targetSize;
    
    bool m_bIsFirstCreateLayer;
    
public:
    ColorData colorData;
    float _orginY;
public:
    virtual bool init();
    CREATE_FUNC(ScrollColorLayer);
    int getPageNum();

    void reponseTouchEvent(ColorSprite* pColorSprite);
    
    void reponseColorSpriteTouchEvent(Node* node);
public:
    Size cellSizeForTable(TableView *table);
    TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView *table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
public:
    st_property(int, _index, Index);
};


#endif /* defined(__ColorGarden__ScrollColorLayer__) */
