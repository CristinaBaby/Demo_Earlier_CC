//
//  FilterLayer.h
//  TestShader
//
//  Created by lidong on 15/9/8.
//
//

#ifndef __TestShader__FilterLayer__
#define __TestShader__FilterLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Filter.h"
USING_NS_CC;
USING_NS_CC_EXT;
class FilterLayer : public Layer, public TableViewDelegate, public TableViewDataSource{
    Sprite* _targetSprite;
    std::vector<std::string> _names;

public:
    bool init();
    CREATE_FUNC(FilterLayer);
public:
    void changeSprite(Sprite* pSprite,const GLchar* pVsh,const GLchar* pFsh, Texture2D* pTexture=NULL,GLuint pIndex=0,Texture2D* pTexture2=NULL,GLuint pIndex2=0);
    void changeSprite(Sprite*pSprite,int idx);
 public:
     Size cellSizeForTable(TableView *table);
     TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
     ssize_t numberOfCellsInTableView(TableView *table);
     void tableCellTouched(TableView* table, TableViewCell* cell);
};


#endif /* defined(__TestShader__FilterLayer__) */
