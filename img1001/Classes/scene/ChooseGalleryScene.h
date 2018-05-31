//
//  ChooseGalleryScene.h
//  HH002
//
//  Created by houhuang on 15/8/26.
//
//

#ifndef __HH002__ChooseGalleryScene__
#define __HH002__ChooseGalleryScene__

#include <stdio.h>
#include "cocos2d.h"
#include "../data/DataManager.h"
#include "../base/BaseScene.hpp"
#include "../layer/ScrollGalleryLayer_ScrollView.h"
#include "../compoment/UIButton.h"
#include "../layer/InspirationLayer.hpp"
USING_NS_CC;

using namespace std;

class ChooseGalleryScene : public BaseScene
{
public:
    static Scene* scene();
    bool init();
    ~ChooseGalleryScene();
    ChooseGalleryScene();
    
    void loadingAds(float dt);
    
    void onEnterTransitionDidFinish();
public:
    void updateScene();
    
public:
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void quit(Node* pNode);
    void removeDialog(Node* pNode);

    void reponseButton(Ref* ref);
    
    void doAction(Ref* ref);
    
    void createGalleryLayer();
    void destoryGalleryLayer();
    
    void createInspirationLayer();
    void destoryInspirationLayer();
    
    void resumeMenu(float dt);
    void reponseFavBtn(Node* node);
public:
    ScrollGalleryLayer_ScrollView*  m_pScrollViewLayer;
    InspirationLayer*   m_pInspirationLayer;
    
    Label* m_pTitleLabel;
    Label* m_pGalleryBtnLabel;
    Label* m_pInspirationBtnLabel;
    
    Menu*  m_pMenu;
    Menu*  m_pMusicMenu;
    UIButton*  m_pFavMenu;
    
};


#endif /* defined(__HH002__ChooseGalleryScene__) */
