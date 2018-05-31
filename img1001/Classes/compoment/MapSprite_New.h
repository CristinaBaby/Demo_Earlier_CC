//
//  MapSprite_New.h
//  IMG1001
//
//  Created by houhuang on 15/9/21.
//
//

#ifndef __IMG1001__MapSprite_New__
#define __IMG1001__MapSprite_New__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../data/DataManager.h"
#include "TargetSprite.h"
USING_NS_CC;
using namespace std;

class MapSprite_New : public Node
{
  
    
    Texture2D* _targetTexture;
    
public:
    bool init();
    CREATE_FUNC(MapSprite_New);
    ~MapSprite_New();
    MapSprite_New();
public:
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
public:
    void addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow = false);
    void selected(bool isColorBoard);
    void setEnableTouch(bool pBool);
 
    //多线程
    void display();
    void imageLoadedCallback(Ref* pSender);
    
    void LoadMapSprite(Ref* pSender);
    
public:
    string getIamgeName();
    void setImageName(int index, const string& imageName);
    st_property(int, _index, Index);
    
    st_property(GalleryInfo*, _galleryInfo, GalleryInfo);
    st_property(bool, _isStopLoading, IsStopLoading);
protected:
    string m_sMapSpriteName;
    Vec2    m_vFirstPoint;
    
    SEL_CallFuncN _selectedCallBack;
    Node*         _target;
    Sprite*       _rotateSprite;
public:
    Sprite* _textureSprite;

};

#endif /* defined(__IMG1001__MapSprite_New__) */
