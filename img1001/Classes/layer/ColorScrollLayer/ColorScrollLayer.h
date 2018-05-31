//
//  ColorScrollLayer.h
//  ColorFree
//
//  Created by houhuang on 15/8/31.
//
//

#ifndef __ColorFree__ColorScrollLayer__
#define __ColorFree__ColorScrollLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "PageDot.h"
#include "../../compoment/ColorSprite.h"
#include "../../data/DataManager.h"
#include "../../modules/STInAppBilling.h"
#include "../../compoment/LayerButton.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ColorScrollLayer : public LayerColor
{
public:
    
    ColorScrollLayer();
    ~ColorScrollLayer();

    static ColorScrollLayer* create(Size size);
    virtual bool init(Size size);
    void onEnter();
public:
    void addListener();
    
    void initContentLayer();
    void initUILayer();
    
    void reponseColorSpriteTouch(Node* pSender);
    void reponseUnlockColorButton(Node* pNode);
    void doAction(Ref* pSender);
    
    void menuDelayEnable(float dt);
    
    void scrollMoveAction();
    
    void reponsePurchseSuccessfulNofiction(Ref* ref);
    
    
    void removeDialog(Node* pNode);
    void unlockColorPack(Node* pNode);
    
    void updataColorPackText();
    
    void resumeMenuEnable();
    void areadyMoveFinish(float dt);
    
    void deleteCurrentColor();
 
protected:
    int     m_iMaxPage;
    int     m_iCurrentPage;
    Vec2    m_vFirstPoint;
    Vec2    m_vOffset;
    Size    m_sViewSize;
    
    ScrollView* m_pScrollView;
    PageDot*    m_pPageDot;
    Layer*      m_pContentLayer;

    LayerColor*     m_pPreColorSprite;
    
    MenuItemSprite* m_pLeft;
    MenuItemSprite* m_pRight;
    MenuItemSprite* m_pHideScroll;
    Menu*           m_pMenu;
    Label*          m_pColorGalleryText;
    
    bool    m_bIsUpTowardsMove;
    bool    m_bIsMoveFinish;
    
    bool    m_bIsClickScroll;
    
    
    //颜色数据
    std::vector<ColorData>* m_vColorDatas;
    
    //颜色种类名称
    std::vector<std::string>* m_vColorGalleryText;
    
//    //最近使用的三种颜色
//    std::vector<ColorSprite*> m_pRensentlyColor;
    
    //vector下标
    int _index;
    
    /***color pack unlock***/
    LayerButton* m_pColorNeonLayer;
    LayerButton* m_pColorSeaLayer;
    LayerButton* m_pcolorUkiyoeLayer;
    
    
    std::vector<ColorSprite*>   m_vColorSpriteNeon;
    std::vector<ColorSprite*>   m_vColorSpriteSea;
    std::vector<ColorSprite*>   m_vColorSpriteUkiyoe;
    
    std::vector<ColorSprite*>   m_vColorSpriteAll;
    
    bool m_bIsInitColorSpriteVector;
    
    STInAppBilling*     _stInAppBilling;
    
    Vec2 _orginPos;
    bool _isFirstLoad;
    
    Sprite* m_pArrowSprite;
    
    bool m_bIsVerticalMove;
    
    Sprite* m_pColorBtn;
    bool m_bIsClickBtn;
    bool m_bIsUpTowards;
    bool m_bIsCurrentClick;
};

#endif /* defined(__ColorFree__ColorScrollLayer__) */
