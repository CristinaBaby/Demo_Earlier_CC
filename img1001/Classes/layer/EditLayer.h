//
//  EditLayer.h
//  ColorFree
//
//  Created by houhuang on 15/9/8.
//
//

#ifndef __ColorFree__EditLayer__
#define __ColorFree__EditLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../compoment/CurrentImageSprite.h"
#include "../filters/Filter.h"
#include "../data/DataManager.h"
#include "../define/Globalmacro.h"
#include "LoadLayer.h"
#include "Signature/SignatureLayer.hpp"
#include "Signature/TextFieldNode.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class EditLayer : public Layer, public TableViewDelegate, public TableViewDataSource
{
public:
    static EditLayer* create(Sprite* pSprite);
//    CREATE_FUNC(EditLayer);
    bool init(Sprite* pSprite);

    EditLayer();
    ~EditLayer();
    
    void onEnterTransitionDidFinish();
public:
    std::string getImageName_w();
    
    void updataCurrentSprite();
    void chooseFilterOrFrames(bool pBool);
    
    void delayeTableViewTouch(float dt);
public:
    Size cellSizeForTable(TableView* table);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idex);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
    //Filters
    void changeSprite(Sprite* pSprite,const GLchar* pVsh,const GLchar* pFsh, Texture2D* pTexture=NULL,GLuint pIndex=0,Texture2D* pTexture2=NULL,GLuint pIndex2=0);
    void changeSprite(Sprite*pSprite,int idx,bool isIcon = false);
    void changeSprite(Sprite* pSprite,const GLchar* pVsh,const GLchar* pFsh,Texture2D* pBackgroundTexture,Texture2D* pNoiseTexture,Texture2D* pBatchTexture);
    
    void updataFilters(ssize_t idex, TableViewCell* cell);
    void updataFrames(ssize_t idex, TableViewCell* cell);
    
    void updataTableView();
    
    Sprite* getShareSprite();
    void sendFlurryDatas();
    
    void doAction(Ref* ref);
    void delayeResumeBtn(float dt);
    
    void showSignatureLayer();
    void hideSignatureLayer();
    void closeKeyboard();
    void openKeyboard();
    void updateCurrentIsKeyboard(bool pBool);
    bool getCurrentIsKeyboard();
    
    void setFileNodeBGVisible(bool pBool);
    
    void updateTextColor(Color3B& color);
    void updateTextFont(const string& fontName);
    
    void resetFieldNode();
    void rebuildSinature(float dt);
    
    void updateImageFilter();
protected:
    string m_sCurrentImageName;
    bool   m_bIsFilters;
    
    TableView* m_pTableView;
    
    int         m_iChooseFilterIndex;
    int         m_iChooseFramesIndex;
    
protected:
    st_property(int, _currentIdx, CurrentIdx);
    
public:
    Sprite* m_pCurrentSprite;
    Texture2D* _texture;
    
    Sprite* m_pMoreFilterSpr;
    LoadLayer*  m_pLoadLayer;

    //flurry datas
    string  m_pCurrentFilterName;
    string  m_pCurrentFramesName;
    
    //signature
    SignatureLayer* m_pSignture;
    Vec2    m_vSignatureOrginVec;
    MenuItemImage*  m_pMenuItemText;
    
    Label*  m_pLabelText;
    Label*  m_pLabelFilters;
    Label*  m_pLabelFrames;
    Label*  m_pLabelTitle;
    
    TextFieldNode*  m_pFilelNode;
    
    bool    m_bCurrentIsKeyboard;
    Sprite* m_pVirtualFrame;
    
    bool    m_bFileNodeReadyMove;
    Vec2    m_vFileNodeOrignalVec;
};

#endif /* defined(__ColorFree__EditLayer__) */
