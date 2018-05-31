//
//  BaseScene.hpp
//  IMG1001
//
//  Created by houhuang on 15/10/19.
//
//

#ifndef BaseScene_hpp
#define BaseScene_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class BaseScene : public LayerColor
{
public:
    CREATE_FUNC(BaseScene);
    bool init();
    
    BaseScene();
    ~BaseScene();
    
public:
    void waitMessage(Ref* ref);
    void waitMessageDownLoadSuc(Ref* ref);
    
    void downLoadResource(Node* pNode);
    void removeDialog(Node* pNode);
    
    void removeLabel(float dt);
public:
    virtual void updateScene(){}
    
public:
    Sprite*     m_pLabelSprite;
};

#endif /* BaseScene_hpp */
