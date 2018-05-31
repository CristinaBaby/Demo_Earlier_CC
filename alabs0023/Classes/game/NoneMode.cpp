//
//  NoneMode.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "NoneMode.h"
#include "GameLayer.h"

NoneMode::NoneMode(GameLayer * layer) : BaseMode(layer)
{
}

void NoneMode::start()
{
    _gameLayer->stopPaint();
}

void NoneMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    BaseMode::onTouchesBegan(touches, unused_event);
    
    //如果收到一个began消息, 立刻进入wait mode
    if(_gameLayer->getFingerCount() == 1)
    {
        //_gameLayer->changeColor();
        
        //收集paint
        _gameLayer->_paints.clear();
        
        GameLayer::Paint paint;
        paint.pos = _gameLayer->convertToWorldSpace(touches[0]->getLocation());
        
        if(!checkIsOutside(paint.pos))
        {
            _gameLayer->_paints.push_back(paint);
            _gameLayer->switchMode(Mode::WAIT);
        }
    }
    else if (_gameLayer->getFingerCount() == 2)
    {
        _gameLayer->switchMode(Mode::ZOOM);
    }
    else
    {
        _gameLayer->switchMode(Mode::ZOOM);
    }
}