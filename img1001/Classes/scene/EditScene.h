//
//  EditScene.h
//  ColorFree
//
//  Created by houhuang on 15/9/7.
//
//

#ifndef __ColorFree__EditScene__
#define __ColorFree__EditScene__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../layer/EditLayer.h"
#include "../compoment/UIButton.h"
#include "../base/BaseScene.hpp"
USING_NS_CC;

class EditScene : public BaseScene
{
public:
//    SCENE_FUNC(EditScene);
    static Scene* scene(Sprite* pSprite);
    bool init(Sprite* pSprite);
    
    ~EditScene();
    EditScene();

    void onEnter();
public:
    void initMenu();
    void doAction(Node* pNode);

    void resumeShareButtonEnable(float dt);
    
    EditLayer* getEditLayer();

protected:
    
    UIButton*   m_pShareBtn;
    UIButton*   m_pBackBtn;
    
};

#endif /* defined(__ColorFree__EditScene__) */
