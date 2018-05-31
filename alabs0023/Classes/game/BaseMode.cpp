//
//  BaseMode.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "BaseMode.h"
#include "GameLayer.h"
#include "UndoManager.h"
#include "ScreenAdapterHelper.h"
#include "DeviceHelper.h"

BaseMode::BaseMode(GameLayer * layer)
{
    _gameLayer = layer;
    _colouringLayer = layer->getColouringLayer();
    _original = _colouringLayer->getBoundingBox();
    
    _gameLayer->setFingerCount(0);
}

BaseMode::~BaseMode()
{
    
}

void BaseMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    _gameLayer->setFingerCount(_gameLayer->getFingerCount() + touches.size());
}

void BaseMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    
}

void BaseMode::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event)
{
    int fingerCount = _gameLayer->getFingerCount() - touches.size();

    if(fingerCount <= 0)
    {
        fingerCount = 0;
        if(!_gameLayer->_paints.empty())
            xUndo->painted();
        _gameLayer->paintPrevious();
        _gameLayer->switchMode(Mode::NONE);
    }
    
    _gameLayer->setFingerCount(fingerCount);
}

void BaseMode::onTouchesCancelled(const vector<Touch*>& touches, Event *unused_event)
{
    int fingerCount = _gameLayer->getFingerCount() - touches.size();
    
    if(fingerCount <= 0)
    {
        fingerCount = 0;
        if(!_gameLayer->_paints.empty())
            xUndo->painted();
        _gameLayer->paintPrevious();
        _gameLayer->switchMode(Mode::NONE);
    }
    
    _gameLayer->setFingerCount(fingerCount);
}

void BaseMode::start()
{
    
}

void BaseMode::stop()
{
    
}

void BaseMode::checkBoundingBox()
{
    Rect current = _colouringLayer->getBoundingBox();
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
    
    _colouringLayer->setPosition(_colouringLayer->getPosition() + offset);
}

bool BaseMode::checkIsOutside(const Vec2 &pos)
{
    float border = ScreenAdapterHelper::getValue(350);
    if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Phone)
    {
        border = ScreenAdapterHelper::getValue(380);
    }
    
    if(pos.y<border || pos.y>ScreenAdapterHelper::getRealDesignSize().height-ScreenAdapterHelper::getValue(90))
    {
        return true;
    }
    else
    {
        return false;
    }
}