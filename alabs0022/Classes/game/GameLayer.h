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
#include "FillNode.h"
#include "ColoringNode.h"
#include "BaseMode.h"

class NoneMode;
class PaintMode;
class MoveMode;
class ZoomMode;
class WaitMode;
class Palette;

USING_NS_CC;
class GameLayer : public Layer {
 
public:
    struct Paint
    {
        Vec2    pos;
        Vec2    previousPos;
    };
protected:
    ColoringNode *_fillNode;
    
    string _imageName;
    string _imageName_w;
    LayerColor* _colorLayer;
    
    Size        _imageSize = Size(1024, 1024);
    BaseMode    * _currentMode = nullptr;
    NoneMode    * _noneMode;
    WaitMode    * _waitMode;
    PaintMode   * _paintMode;
    ZoomMode    * _zoomMode;
    MoveMode    * _moveMode;
    Image       * _saveImage = nullptr;
    
    
public:
    bool init(int pIndex);
    ~GameLayer();
    static GameLayer* create(int pIndex);
    LayerColor* _zoomLayer;
    void reset();
    void saveImage();
    void deleteSaveImage();
    string getImageName_w();
    string getSaveImage_w_path();
    string getImageName();
    string getHistoryImageName();
    void changeBackColor();
    void changeBackColor(const Color3B &color);
    void changeBackColorDirect(const Color3B &color);
    void undo();
    void undoBackColor(Color4B pColor);
    void setTouchEnable(float dt);
    void enableFillNode(bool pBool);
    void setupScribble(bool reset);
    
    void onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void switchMode(BaseMode::Mode mode);
    void paintPrevious();
    void paint(Vec2 & current, Vec2 & previous);
    void stopPaint();
    
    void drawLineFrame(float pTime);
    Image * getSaveImage();
    Layer * getColouringLayer();
    void update(float dt);
    void loadFinished(float dt);
    
public: //状态机模式
    vector<Paint>   _paints;
    CC_SYNTHESIZE(int, _fingerCount, FingerCount);
    CC_SYNTHESIZE(Color3B, _backColor, BackColor);
    
public:
    st_property(int, _index, Index);
};

#endif /* defined(__ColorGarden__GameLayer__) */