//
//  MyArtworksSprite.hpp
//  IMG1001
//
//  Created by houhuang on 15/11/17.
//
//

#ifndef MyArtworksSprite_hpp
#define MyArtworksSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../data/DataManager.h"
#include "TargetSprite.h"
USING_NS_CC;
using namespace std;

class MyArtworksSprite : public Node
{

public:
    bool init(int imageNumber, const string& imageName);
//    CREATE_FUNC(MyArtworksSprite);
    static MyArtworksSprite* create(int imageNumber, const string& imageName);
    
    //my post sprite
    static MyArtworksSprite* create(const string& imageName);
    bool init(const string& imageName);
    void display_post();
    string getPostImageFullPath();
    
    MyArtworksSprite();
    ~MyArtworksSprite();
    
    void onEnterTransitionDidFinish();
    
    //多线程
    void display();
    void imageLoadedCallback(Ref* pSender);
    
    void hideThis();
    
    void LoadMapSprite(Ref* pSender);
    void doAction(Ref* ref);
    
    void setDeleteBtnVisible(bool pBool);
    
public:
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
public:
    void addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow = false);
    void removeListener();
    
public:
    string getImageName_fullPath();
    void setImageName(int index, const string& imageName);
    st_property(int, _index, Index);
    st_property(string, _postImageName, PostImageName);
    int getImageNumber();
    string getImageName();

protected:
    
    string      m_sImageName_post;
    string      m_sImageName;
    string      m_sGalleryName;
    int         m_iImageNumber;
    Vec2        m_vFirstPoint;
    
    Sprite*     m_pRotateSprite;
    Sprite*     m_pTextureSprite;
    Texture2D*  m_pTargetTexture;
    LayerColor* m_pLayerColor;
    
    Menu*       m_pMenu;

    SEL_CallFuncN _selectedCallBack;
    Node*         _target;
    
    bool        m_bIsHide;
    bool        m_bIsPost;
};

#endif /* MyArtworksSprite_hpp */
