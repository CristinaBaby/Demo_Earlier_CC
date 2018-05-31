//
//  SignatureLayer.hpp
//  IMG1001
//
//  Created by houhuang on 15/10/27.
//
//

#ifndef SignatureLayer_hpp
#define SignatureLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../define/Globalmacro.h"
#include "../../compoment/ColorSprite.h"
USING_NS_CC;
USING_NS_CC_EXT;

class SignatureLayer : public LayerColor, public TableViewDelegate, public TableViewDataSource
{
public:
//    CREATE_FUNC(SignatureLayer);
    
    static SignatureLayer* create();
    
    bool init();

    SignatureLayer();
    ~SignatureLayer();
    
    void onEnterTransitionDidFinish();
public:
    void addListener();
    void removeListener();
    
public:
    void createFontTableView();
    void createColorLayer();
    
    void doAction(Ref* ref);
    void initMenuState();
    
    void delayeResumeBtn(float dt);
    
    void reponseTextColor(Node* pNode);
    void hideThisLayer();
    
public:
    Size cellSizeForTable(TableView* table);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idex);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
protected:
    MenuItemSprite* m_pBtnKeyboard;
    MenuItemSprite* m_pBtnFont;
    MenuItemSprite* m_pBtnColor;
    MenuItemSprite* m_pBtnDone;
    
    Vec2    m_vThisOrginVec;
    
    LayerColor* m_pLayerColor;
    TableView*  m_pTableView;

    std::vector<ColorSprite*>    m_vTextColor;
    
    LayerColor* m_pPreColorSprite;
    
    Color3B m_cCurrentColor;
    
    std::vector<Label*> m_vFontTableView;
    
    bool    m_bIsCreateThis;
    Label*  m_pFontPre;

};

#endif /* SignatureLayer_hpp */
