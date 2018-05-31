//
//  GameLayer.h
//  ColorGarden
//
//  Created by lidong on 15/8/3.
//
//

#ifndef __ColorGarden__GameLayer__
#define __ColorGarden__GameLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/GlobalMacro.h"
#include "../commpoent/ImageBox.h"
#include "../Coloring/BoundaryFillNode.h"
#include "../Coloring/ZoomLayer.h"
USING_NS_CC;
class GameLayer : public Layer {
 
    
protected:
    ImageBox* _imageBox;
    BoundaryFillNode *_fillNode;
    
    string _imageName;
    string _imageName_w;
    LayerColor* _colorLayer;
public:
    bool init(int pIndex);
    ~GameLayer();
    static GameLayer* create(int pIndex);
    ZoomLayer* _zoomLayer;
    void reset();
    void saveImage();
    void deleteSaveImage();
    string getImageName_w();
    string getSaveImage_w_path();
    string getImageName();
    void changeBackColor();
    void undo();
    void undoBackColor(Color4B pColor);
    void setTouchEnable(float dt);
    void enableFillNode(bool pBool);
    
public:
    st_property(int, _index, Index);
};

#endif /* defined(__ColorGarden__GameLayer__) */
