//
//  ZoomMode.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "ZoomMode.h"
#include "GameLayer.h"

ZoomMode::ZoomMode(GameLayer * layer) : BaseMode(layer)
{
    _originalScale = _gameLayer->getColouringLayer()->getScale();
}

ZoomMode::~ZoomMode()
{
    stop();
}

void ZoomMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    BaseMode::onTouchesBegan(touches, unused_event);
    
//    if(_canChange && _gameLayer->getFingerCount() > 2)
//    {
//        _gameLayer->switchMode(Mode::MOVE);
//    }
}

void ZoomMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    if(touches.size() < 2) return;
    
//    if (!checkIsMoveMode(touches)) {
    if(true) {
        resetPictureAnchorPoint(touches);
        
        auto distance1 = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());
        auto distance2 = touches[0]->getLocation().distance(touches[1]->getLocation());
        auto gradient = distance2 / distance1;
        auto offset = 1 - gradient;
        
        if (std::abs(offset) < 0.02) {
            return;
        }
        
        float scale = _colouringLayer->getScale();
        scale *= gradient;
        
        if (scale > 5) {
            scale = 5;
        }
        else if (scale < _originalScale)
        {
            scale = _originalScale;
        }
        
        _colouringLayer->setScale(scale);
        
        if (gradient < 1) {
            checkBoundingBox();
        }
        
        if (_canChange && isOutOfRange(touches)) {
            _canChange = false;
        }
    }
    else
    {
        //直接在zoom里面move
        Vec2 offset = touches[0]->getLocation() - touches[0]->getPreviousLocation();
        _colouringLayer->setPosition(_colouringLayer->getPosition() + offset);
        checkBoundingBox();
    }
}

void ZoomMode::start()
{
    //0.5秒内如果没有收到第3根手指, 就不再检查到move 模式
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ZoomMode::timeout), this, 0, 0, 0.25, false);
    _canChange = true;
    _gameLayer->_paints.clear();
}

void ZoomMode::stop()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(ZoomMode::timeout), this);
}

void ZoomMode::timeout(float dt)
{
    _canChange = false;
}

bool ZoomMode::checkIsMoveMode(const vector<Touch*>& touches)
{
    //如果2只手指的方向几乎相同, 就判定为move mode
    Vec2 offset0 = touches[0]->getLocation() - touches[0]->getStartLocation();
    Vec2 offset1 = touches[1]->getLocation() - touches[1]->getStartLocation();
    
    if(offset0.distance(offset1) < 50)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ZoomMode::resetPictureAnchorPoint(const std::vector<Touch *> &touches)
{
    if(touches.size() < 2) return;
    
    auto touch0 = touches[0]->getLocation();
    auto touch1 = touches[1]->getLocation();
    auto center = convertToWorldSpace((touch0 + touch1) / 2);
    center = _colouringLayer->convertToNodeSpace(center);
    
    //重新设置锚点
    Vec2 newAnchor;
    newAnchor.x = center.x / _colouringLayer->getContentSize().width;
    newAnchor.y = center.y / _colouringLayer->getContentSize().height;
    
    //动了锚点后, 重新设置坐标
    Vec2 offset;
    Vec2 oldAnchor = _colouringLayer->getAnchorPoint();
    offset.x = (newAnchor.x - oldAnchor.x) * _colouringLayer->getContentSize().width * _colouringLayer->getScale();
    offset.y = (newAnchor.y - oldAnchor.y) * _colouringLayer->getContentSize().height * _colouringLayer->getScale();
    
    _colouringLayer->setAnchorPoint(newAnchor);
    _colouringLayer->setPosition(_colouringLayer->getPosition() + offset);
}

bool ZoomMode::isOutOfRange(const vector<Touch*>& touches)
{
    for (auto touch : touches) {
        Vec2 current = _gameLayer->convertToWorldSpace(touch->getLocation());
        Vec2 start = _gameLayer->convertToWorldSpace(touch->getStartLocation());
        
        Vec2 distance = current - start;
        
        if (std::abs(distance.x) + std::abs(distance.y) > 40)
        {
            return true;
        }
    }
    
    return false;
}