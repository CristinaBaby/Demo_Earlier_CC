//
//  GameScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "UIPageViewEx.h"
#include "ColoringNode.h"
#include "BaseMode.h"
#include "ColorScrollLayer.h"

class Graph;
class NoneMode;
class PaintMode;
class MoveMode;
class ZoomMode;
class WaitMode;
class BlockLayer;

class  GameScene : public Scene
{
public:
    struct Paint
    {
        Vec2    pos;
        Vec2    previousPos;
    };
    
public :
    GameScene();
    virtual ~GameScene();
    
    CREATE_FUNC(GameScene);
    
    virtual bool init() override;
    void onButton(Ref* pSender, Widget::TouchEventType type);
    void onDialog(const string& name);
    void createScribble();
    void createZoomLayer();
    void drawLineFrame();
    float onEnterGame(int index);
    float onExitGame(int index);
    
    void onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void switchMode(BaseMode::Mode mode);
    void paintPrevious();
    void paint(Vec2 & current, Vec2 & previous);
    void stopPaint();
    Layer * getZoomLayer();
    void undoBackgroundColor(const Color3B & color);
    void onAfterCaptured(bool succeed, const string & filename);
    void onPageMove(EventCustom* event);
    void onChooseColor(EventCustom* event);
    void onAfterVisit(EventCustom* event);
    virtual void onEnterTransitionDidFinish() override;
    void applicationDidEnterBackground();
    
    void changePalletteName();
private:
    void createPallette();
    void changeBackgroundColor();
    void eraseLineFrame(unsigned char * imageData);
    
public: //状态机模式
    vector<Paint>   _paints;
    CC_SYNTHESIZE(int, _fingerCount, FingerCount);

    
private:
    Layout *        _root;
    PageViewEx *    _pallettes;
    ColoringNode *  _fillNode;
    Text *          _palletteName;
    LayerColor *    _zoomLayer;
    BaseMode    * _currentMode = nullptr;
    NoneMode    * _noneMode;
    WaitMode    * _waitMode;
    PaintMode   * _paintMode;
    ZoomMode    * _zoomMode;
    MoveMode    * _moveMode;
    BlockLayer  * _blockLayer;
    
    Graph &     _graph;
    bool            _blockTouch = false;
    bool            _forward = true;
    Vec2            _rotatePos;
    Layout *        _undo;
    Layout *        _back;
    Layout *        _del;
    Layout *        _done;
    
    ColorScrollLayer*   _palletteLayer;
};