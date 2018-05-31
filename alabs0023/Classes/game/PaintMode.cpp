//
//  PaintMode.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "PaintMode.h"
#include "GameLayer.h"
#include "UndoManager.h"


PaintMode::PaintMode(GameLayer * layer) : BaseMode(layer)
{
    
}

PaintMode::~PaintMode()
{
    
}

void PaintMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
//    auto current = _gameLayer->convertToWorldSpace(touches[0]->getLocation());
//    auto previous = _gameLayer->convertToWorldSpace(touches[0]->getPreviousLocation());
//
//    _gameLayer->paint(current, previous);
}

void PaintMode::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event)
{
    BaseMode::onTouchesEnded(touches, unused_event);
    
    auto current = _gameLayer->convertToWorldSpace(touches[0]->getLocation());
    //_gameLayer->paintStamp(current);
}

void PaintMode::stop()
{
    xUndo->painted();
    _gameLayer->stopPaint();
}