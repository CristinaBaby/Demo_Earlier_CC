//
//  MapSmallSprite.hpp
//  IMG1001
//
//  Created by houhuang on 15/11/12.
//
//

#ifndef MapSmallSprite_hpp
#define MapSmallSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "../data/DataManager.h"
#include "../define/Globalmacro.h"
USING_NS_CC;
using namespace std;

class MapSmallSprite : public Node
{
public:
    bool init();
    CREATE_FUNC(MapSmallSprite);
    MapSmallSprite();
    ~MapSmallSprite();
    
public:
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
public:
    void addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow = false);
    void setEnableTouch(bool pBool);
    
    //多线程
    void display();
    void imageLoadedCallback(Ref* pSender);
    
    void LoadMapSprite(Ref* pSender);
    
public:
    string getIamgeName();
    void setImageName(int index, const string& imageName);

    st_property(GalleryInfo*, _galleryInfo, GalleryInfo);
protected:
    string      m_sMapSpriteName;
    Vec2        m_vFirstPoint;
    Sprite*     m_pRotateSprite;
    Sprite*     m_pTextureSpite;
    Texture2D*  m_pTexture2D;
    
    
    SEL_CallFuncN _selectedCallBack;
    Node*         _target;
};

#endif /* MapSmallSprite_hpp */
