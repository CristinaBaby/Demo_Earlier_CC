//
//  MyArtworksScene.hpp
//  IMG1001
//
//  Created by houhuang on 15/11/13.
//
//

#ifndef MyArtworksScene_hpp
#define MyArtworksScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../layer/LoadLayer.h"
#include "../compoment/UIButton.h"
#include "../base/BaseScene.hpp"
USING_NS_CC;


class MyArtworksScene : public BaseScene
{
    

public:
    
    SCENE_FUNC(MyArtworksScene);
    bool init();
    
    void onEnterTransitionDidFinish();
    MyArtworksScene();

public:
    void doAction(Node* node);
    void reponseLabel(Ref* ref);
public:
    UIButton* m_pBackUIbtn;
  
    Label*      m_pEditLabel;
    bool        m_bIsEditButton;
};

#endif /* MyArtworksScene_hpp */
