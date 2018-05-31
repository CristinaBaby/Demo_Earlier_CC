//
//  GameUILayer.h
//  ColorFree
//
//  Created by houhuang on 15/8/28.
//
//

#ifndef __ColorFree__GameUILayer__
#define __ColorFree__GameUILayer__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../compoment/ColorSprite.h"
#include "../layer/ColorScrollLayer/ColorScrollLayerMask.h"
#include "../layer/ColorScrollLayer/ColorScrollLayer.h"
#include "../compoment/UIButton.h"
USING_NS_CC;

class GameUILayer : public Layer
{
public:
    ~GameUILayer();
    GameUILayer();
    bool init();
    static GameUILayer* create();
    
public:
    void initUI();
    void doAction(Node* pSender);
    
public:
    st_property(int, _index, Index);
    void removeTutorial();
    void removeDialog(Node* ref);
    void resetGame(Node* ref);
    void reset(float dt);
    void gotoEditScene();
    void undoEndPrompt();
  
    void updateResentlyColor();
    void reponseTouchResentlyColor(Node* pSender);
    
    void reponseMaskAnimation(bool pBool);
    
    void enableFillNode();
    
    void waitDownLoadSucNotification(Ref* ref);
    
    void returnPreScene(Node* ref);
protected:
    LayerColor* m_pTutorialLayer;
    
    //最近使用的三种颜色
    std::vector<ColorSprite*> m_pRensentlyColor;
    
    ColorScrollLayerMask* m_pBottomMask;

    Vec2    m_vBottomOrginalPos;
    
    ColorScrollLayer*   m_pColorScrollLayer;
    
    UIButton*   m_pBackUIB;
    UIButton*   m_pEditUIB;
    
    bool m_bIsShowDialog;
    bool m_bIsReset;

};

#endif /* defined(__ColorFree__GameUILayer__) */
