//
//  ChooseMapScene.h
//  HH002
//
//  Created by houhuang on 15/8/27.
//
//

#ifndef __HH002__ChooseMapScene__
#define __HH002__ChooseMapScene__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../layer/LoadLayer.h"
#include "../compoment/UIButton.h"
#include "../base/BaseScene.hpp"
USING_NS_CC;

//using namespace cocos2d;
class ScrollMapLayer_ScrollView;
class ChooseMapScene : public BaseScene
{
    
protected:
    ScrollMapLayer_ScrollView* _scrollLayer;
public:
    
    static Scene* scene(int index, const std::string& galleryName);
    bool init(int index, const std::string& galleryName);
    
    ChooseMapScene();
public:
    st_property(int, _index, Index);
public:
    void doAction(Node* node);
    void resumeButtonEnable(float dt);
    
    void reponseButton(Ref* ref);
    
public:
    void updateScene();
    
protected:
    LoadLayer*  m_pLoadLayer;
    UIButton*   m_pBackUIbtn;
};


#endif /* defined(__HH002__ChooseMapScene__) */
