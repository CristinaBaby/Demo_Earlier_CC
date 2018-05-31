//
//  MoveMode.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "MoveMode.h"
#include "GameLayer.h"

MoveMode::MoveMode(GameLayer * layer) : BaseMode(layer)
{
}

void MoveMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    Vec2 offset = touches[0]->getLocation() - touches[0]->getPreviousLocation();
    _colouringLayer->setPosition(_colouringLayer->getPosition() + offset);
    checkBoundingBox();
}

void MoveMode::start()
{
    _gameLayer->_paints.clear();
}