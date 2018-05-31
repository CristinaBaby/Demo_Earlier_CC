//
//  FavoriteSprite.hpp
//  IMG1001
//
//  Created by houhuang on 15/12/10.
//
//

#ifndef FavoriteSprite_hpp
#define FavoriteSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
#include "../data/DataManager.h"
#include "TargetSprite.h"
USING_NS_CC;
using namespace std;

class FavoriteSprite : public Node
{
public:
    ~FavoriteSprite();
    FavoriteSprite();
    
    static FavoriteSprite* create(int imageName, int likeNum);
    bool init(int imageName, int likeNum);
    
    //post layer
    static FavoriteSprite* create(string imageName, int imageID);
    bool init(string imageName, int imageID);
public:
    //多线程
    void display();
    void hideThis();

    void setImageFullPath();
  
    st_property(int, _index, Index);
    st_property(string, _postImageName, PostImageName);
    
    string getImageFullPath();
    string getPostSpriteImage();
    int getImageName();
    
    void imageLoadedCallback(Ref* pSender);
    void LoadMapSprite(Ref* pSender);
    void getLikeNumberSuc(Ref* pSender);
protected:
    Sprite*     m_pRotateSprite;
    Sprite*     m_pTextureSprite;
    int         m_iLikeNumber;
    int         m_iImageName;
    bool        m_bIsHide;
    bool        m_bIsPost;
    bool        m_bIsGetLike;
    bool        m_bIsHeart;
    
    string      m_sImageFullPath;
    Texture2D*  m_pTargetTexture;
    
    Label*      m_pLikeLabel;
    Sprite*     m_pHeartSpr;
    
    bool        m_bIsPostLayer;
};

#endif /* FavoriteSprite_hpp */
