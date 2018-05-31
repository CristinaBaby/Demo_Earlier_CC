//
//  UndoManager.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "UndoManager.h"
#include "FillNode.h"
#include "GameLayer.h"
#include "DataManager.h"


static UndoManager *_sharedUndo = nullptr;

UndoManager* UndoManager::getInstance()
{
    if (! _sharedUndo)
    {
        _sharedUndo = new UndoManager();
    }
    
    return _sharedUndo;
}

void UndoManager::destroyInstance()
{
    xScheduler->unscheduleAllForTarget(this);
    CC_SAFE_DELETE(_sharedUndo);
}

UndoManager::UndoManager()
{
}

void UndoManager::init(ColoringNode * fillSVGNode, GameLayer *gameLayer)
{
    _fillNode = fillSVGNode;
    _gameLayer = gameLayer;
}

bool UndoManager::undo()
{
    if(!_dotColorDatas.empty())
    {
        //移除最上面的一个touch ended
        if (_dotColorDatas.back().type == 2) {
            _dotColorDatas.pop_back();
        }
        
        bool hasSetColor = false;
        
        while (!_dotColorDatas.empty()) {
            
            _fillNode->resetPatternColor();
            
            auto data = _dotColorDatas.back();
            if (data.type == 2)
                break;
            
            if (data.type == 0)
            {
                if (!hasSetColor) {
                    hasSetColor = true;
                    _oldColor = data.color;
                    _fillNode->setStartColor(_oldColor);
                }
                else if(_oldColor != data.color)
                {
                    _oldColor = data.color;
                    _fillNode->setStartColor(_oldColor);
                }
                
                _fillNode->coloringNodeSpace(data.dot);
            }
            else
            {
                if (!hasSetColor) {
                    hasSetColor = true;
                    _oldColor = data.color;
                    _fillNode->setStartColor(_oldColor);
                    _gameLayer->changeBackColorDirect(Color3B(_oldColor));
                }
                else if(_oldColor != data.color)
                {
                    _oldColor = data.color;
                    _fillNode->setStartColor(_oldColor);
                    _gameLayer->changeBackColorDirect(Color3B(_oldColor));
                }
            }
            
            _dotColorDatas.pop_back();
        }
        
        auto original = DataManager::getInstance()->selectedColorData;
        _fillNode->setStartColor(Color4B(original.R, original.G, original.B, 255));
        
        UserDefault::getInstance()->flush();
        
        return true;
    }
    
    return false;
}

void UndoManager::painted()
{
    _painted = true;
    
    DotColorData data;
    data.type = 2;
    
    _dotColorDatas.push_back(data);
}

void UndoManager::record(const Color3B &color)
{
    DotColorData data;
    data.color = Color4B(color);
    data.type = 1;
        
    _dotColorDatas.push_back(data);
}

void UndoManager::record(const Vec2 &pos)
{
    //转换为NodeSpace
    auto nodePos = _fillNode->convertToNodeSpace(pos);
    auto color = _fillNode->getCanvas()->readPixel(_fillNode->getCanvas(), nodePos.x, nodePos.y);

    if( color.r == color.g && color.g == color.b && color.r < 255) return;
    
    DotColorData data;
    data.dot = nodePos;
    data.color = color;
    data.type = 0;
    
    _dotColorDatas.push_back(data);
}

void UndoManager::reset()
{
    _dotColorDatas.clear();
}
