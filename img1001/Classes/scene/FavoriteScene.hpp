//
//  FavoriteScene.hpp
//  IMG1001
//
//  Created by houhuang on 15/12/2.
//
//

#ifndef FavoriteScene_hpp
#define FavoriteScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "../data/DataManager.h"
#include "../base/BaseScene.hpp"
#include "../define/Globalmacro.h"
#include "../compoment/UIButton.h"
#include "../layer/ColorScrollLayer/ColorScrollLayerMask.h"
USING_NS_CC;

using namespace std;

class FavoriteScene : public BaseScene
{
public:
    SCENE_FUNC(FavoriteScene);
    bool init();
    ~FavoriteScene();
    FavoriteScene();

    void onEnterTransitionDidFinish();
public:
    void updateScene();
    
public:
    void gotoScene(Node* node);
    void doAction(Ref* ref);
    
    void createFavoritelayer();
    void destoryFavoriteLayer();
    
    void createPostLayer();
    void destoryPostLayer();
protected:
    Label*      m_pTitleLabel;
    Label*      m_pEditLabel;
    
    Label*      m_pFavoriteBtnLabel;
    Label*      m_pPostBtnLabel;
    
    UIButton*   m_pBackUIbtn;
    
    Menu*       m_pMenu;

    bool        m_bIsEditButton;
    ColorScrollLayerMask* m_pBtnBG;
};
#endif /* FavoriteScene_hpp */
