//
//  UndoManager.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__UndoManager__
#define __ColorBook__UndoManager__

#include "UIHeader.h"
#include <deque>

#define xUndo  (UndoManager::getInstance())

class ColoringNode;
class GameLayer;

class UndoManager : public Node
{
    struct DotColorData
    {
        int     type;       //0:中间图案, 1:周围背景, 2:touch end
        Vec2    dot;
        Color4B color;
    };
    
public:
    static UndoManager* getInstance();
    void destroyInstance();
    
    void init(ColoringNode * fillSVGNode, GameLayer *gameLayer);
    bool undo();
    void painted();
    void reset();
    void record(const Vec2 &pos);
    void record(const Color3B &color);
    
protected:
    UndoManager();

protected:    
    bool             _painted = false;
    GameLayer *     _gameLayer;
    ColoringNode *  _fillNode;
    
    deque<DotColorData> _dotColorDatas;
    Color4B          _oldColor;
};

#endif /* defined(__ColorBook__UndoManager__) */
