//
//  ZoomMode.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "ZoomMode.h"
#include "GameScene.h"

ZoomMode::ZoomMode(GameScene * layer) : BaseMode(layer)
{
}

ZoomMode::~ZoomMode()
{
    stop();
}

void ZoomMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    BaseMode::onTouchesBegan(touches, unused_event);
    
//    if(_canChange && _gameScene->getFingerCount() > 2)
//    {
//        _gameScene->switchMode(Mode::MOVE);
//    }
}

void ZoomMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    if(touches.size() < 2) return;
    
    if (!checkIsMoveMode(touches)) {
        resetPictureAnchorPoint(touches);
        
        auto distance1 = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());
        auto distance2 = touches[0]->getLocation().distance(touches[1]->getLocation());
        auto gradient = distance2 / distance1;
        auto offset = 1 - gradient;
        
        if (std::abs(offset) < 0.02) {
            return;
        }
        
        float scale = _zoomLayer->getScale();
        scale *= gradient;
        
        if (scale > _maxScale) {
            scale = _maxScale;
        }
        else if (scale < 1)
        {
            scale = 1;
        }
        
        _zoomLayer->setScale(scale);
        
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
        _zoomLayer->setPosition(_zoomLayer->getPosition() + offset);
        checkBoundingBox();
    }
}

void ZoomMode::start()
{
    //0.5秒内如果没有收到第3根手指, 就不再检查到move 模式
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ZoomMode::timeout), this, 0, 0, 0.25, false);
    _canChange = true;
    _gameScene->_paints.clear();
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
    center = _zoomLayer->convertToNodeSpace(center);
    
    //重新设置锚点
    Vec2 newAnchor;
    newAnchor.x = center.x / _zoomLayer->getContentSize().width;
    newAnchor.y = center.y / _zoomLayer->getContentSize().height;
    
    //动了锚点后, 重新设置坐标
    Vec2 offset;
    Vec2 oldAnchor = _zoomLayer->getAnchorPoint();
    offset.x = (newAnchor.x - oldAnchor.x) * _zoomLayer->getContentSize().width * _zoomLayer->getScale();
    offset.y = (newAnchor.y - oldAnchor.y) * _zoomLayer->getContentSize().height * _zoomLayer->getScale();
    
    _zoomLayer->setAnchorPoint(newAnchor);
    _zoomLayer->setPosition(_zoomLayer->getPosition() + offset);
}

bool ZoomMode::isOutOfRange(const vector<Touch*>& touches)
{
    for (auto touch : touches) {
        Vec2 current = _gameScene->convertToWorldSpace(touch->getLocation());
        Vec2 start = _gameScene->convertToWorldSpace(touch->getStartLocation());
        
        Vec2 distance = current - start;
        
        if (std::abs(distance.x) + std::abs(distance.y) > 40)
        {
            return true;
        }
    }
    
    return false;
}

void ZoomMode::setMaxScale(float scale)
{
    _maxScale = (1.f / scale) * 3.f;
}