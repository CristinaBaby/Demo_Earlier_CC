//
//  BaseMode.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "BaseMode.h"
#include "UndoManager.h"
#include "GameScene.h"

BaseMode::BaseMode(GameScene * layer)
{
    _gameScene = layer;
    _zoomLayer = layer->getZoomLayer();
    _original = _zoomLayer->getBoundingBox();
    
    _gameScene->setFingerCount(0);
}

BaseMode::~BaseMode()
{
    
}

void BaseMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    _gameScene->setFingerCount(_gameScene->getFingerCount() + touches.size());
}

void BaseMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    
}

void BaseMode::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event)
{
    int fingerCount = _gameScene->getFingerCount() - touches.size();

    if(fingerCount <= 0)
    {
        fingerCount = 0;
        if(!_gameScene->_paints.empty())
            xUndo->painted();
        _gameScene->paintPrevious();
        _gameScene->switchMode(Mode::NONE);
    }
    
    _gameScene->setFingerCount(fingerCount);
}

void BaseMode::onTouchesCancelled(const vector<Touch*>& touches, Event *unused_event)
{
    int fingerCount = _gameScene->getFingerCount() - touches.size();
    
    if(fingerCount <= 0)
    {
        fingerCount = 0;
        if(!_gameScene->_paints.empty())
            xUndo->painted();
        _gameScene->paintPrevious();
        _gameScene->switchMode(Mode::NONE);
    }
    
    _gameScene->setFingerCount(fingerCount);
}

void BaseMode::start()
{
    
}

void BaseMode::stop()
{
    
}

void BaseMode::checkBoundingBox()
{
    Rect current = _zoomLayer->getBoundingBox();
    Vec2 offset = Vec2::ZERO;
        
    if (current.getMinX() > _original.getMinX()) {
        offset.x =  _original.getMinX() - current.getMinX();
    }
    else if (current.getMaxX() < _original.getMaxX()) {
        offset.x =  _original.getMaxX() - current.getMaxX();
    }
    
    if (current.getMinY() > _original.getMinY()) {
        offset.y =  _original.getMinY() - current.getMinY();
    }
    else if (current.getMaxY() < _original.getMaxY()) {
        offset.y =  _original.getMaxY() - current.getMaxY();
    }
    
    _zoomLayer->setPosition(_zoomLayer->getPosition() + offset);
}