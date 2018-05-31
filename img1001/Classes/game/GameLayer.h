//
//  GameLayer.h
//  ColorFree
//
//  Created by houhuang on 15/8/31.
//
//

#ifndef __ColorFree__GameLayer__
#define __ColorFree__GameLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "../Coloring/BoundaryFillNode.h"
#include "../Coloring/ZoomLayer.h"
USING_NS_CC;

class GameLayer : public Layer
{
public:
    static GameLayer* create();
    bool init();
    
    void onEnterTransitionDidFinish();
    
    ~GameLayer();
public:
    void reset();
    void saveImage();
    void saveEditImage();
    void deleteSaveImage();
    void undo();
    
    void setTouchEnable(float dt);
    void enableFillNode(bool pBool);
    
    std::string getImageName();
    std::string getImageName_w();
    
    void undoBackColor(Color4B pColor);
    void changeBackColor();
    
    Sprite* getSpriteDrawScene();
    
    bool createDirectory(const char *path);
    
    void display();
    void waitMessageAddImage(Ref* ref);
protected:
    BoundaryFillNode*   m_pFileNode;
    ZoomLayer*          m_pZoomLayer;
    LayerColor*         m_pLayerColor;
    
    std::string         m_sImageName;
    std::string         m_sImageName_w;
    
    int         m_iImageCount;
    std::string m_sImageGallery;
    
    Sprite*     m_pRotateSprite;
    Sprite*     m_pEditSprite;
    string      m_sImagePath;
    
};

#endif /* defined(__ColorFree__GameLayer__) */
