//
//  GallerySprite.h
//  IMG1001
//
//  Created by houhuang on 15/9/24.
//
//

#ifndef __IMG1001__GallerySprite__
#define __IMG1001__GallerySprite__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../data/DataManager.h"
#include "TargetSprite.h"
USING_NS_CC;
using namespace std;

class GallerySprite_New : public Node
{

public:
    static GallerySprite_New* create(GalleryInfo* glInfo);
    CREATE_FUNC(GallerySprite_New);
    
    GallerySprite_New();
    ~GallerySprite_New();
    bool init(GalleryInfo* glInfo);
    bool init();
public:
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
public:
    void addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow = false);
  
    void setTouchEnable(bool pBool);
    
    string getImageName();
    
    void showFinish(float dt);
    
public:
    st_property(int, _index, Index);
protected:
    Vec2    m_vFirstPoint;
    
    SEL_CallFuncN _selectedCallBack;
    Node*         _target;
    
    GalleryInfo*   m_gGalleryInfo;

};

#endif /* defined(__IMG1001__GallerySprite__) */
