//
//  GameLayer.cpp
//  ColorGarden
//
//  Created by lidong on 15/8/3.
//
//

#include "GameLayer.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../helper/ControlHelper.h"
#include "ZoomLayer.h"
#include "../layer/ScrollColorLayer.h"
#include "../game/GameUILayer.h"
#include "../scene/GameScene.h"
#include "../helper/ImageDownload.h"
#include "../helper/DeviceHelper.h"
#include "DataManager.h"
#include "BaseMode.h"
#include "NoneMode.h"
#include "PaintMode.h"
#include "MoveMode.h"
#include "ZoomMode.h"
#include "WaitMode.h"
#include "UndoManager.h"
#include "ScribbleUtil.h"

static float zoomLayerScale=0.55f;

GameLayer* GameLayer::create(int pIndex)
{
    GameLayer *pRet = new GameLayer();
    if (pRet && pRet->init(pIndex))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}



bool GameLayer::init(int pIndex)
{
    if(!Layer::init()) return false;
    
    DataManager::getInstance()->isAlreadySelectedColor=false;
    
    _zoomLayer=NULL;
    
    this->setIndex(pIndex);
    
    _backColor=DataManager::getInstance()->getSaveBackColor(pIndex);
    
    _colorLayer=LayerColor::create(Color4B::WHITE);
    addChild(_colorLayer);

    float lScale = ScreenAdapterHelper::getRealDesignScale();
    if(lScale==1 || lScale==2)
    {
        zoomLayerScale=0.55f;
    }
    
    scheduleUpdate();
    
    return true;
}

void GameLayer::update(float dt)
{
    if (Director::getInstance()->getRunningScene() == getParent()->getParent()) {
        unscheduleUpdate();
        
        setupScribble(false);
        
        DataManager::getInstance()->selectedColorData.R = 239;
        DataManager::getInstance()->selectedColorData.G = 128;
        DataManager::getInstance()->selectedColorData.B = 49;
        
        //这些放到onEnter的下一帧去做
        _noneMode = new NoneMode(this);
        _waitMode = new WaitMode(this);
        _paintMode = new PaintMode(this);
        _moveMode = new MoveMode(this);
        _zoomMode = new ZoomMode(this);
        switchMode(BaseMode::Mode::NONE);
        
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
        listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
        listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
        listener->onTouchesCancelled = CC_CALLBACK_2(GameScene::onTouchesCancelled, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        scheduleOnce(schedule_selector(GameLayer::loadFinished), 0);
    }
}

void GameLayer::loadFinished(float dt)
{
    auto gameScene = (GameScene*)getParent();
    gameScene->unblock();
}

void GameLayer::setupScribble(bool reset)
{
    _fillNode = ColoringNode::create(getImageName_w(), getImageName());
    _fillNode->setDefaultColor(Color4F::WHITE);
    _fillNode->setColoringMode(ColoringNode::ColoringMode::eFlat);
    
    bool isExsit = FileUtils::getInstance()->isFileExist(getHistoryImageName());
    if (isExsit && !reset)
    {
        Sprite *history = Sprite::create(getHistoryImageName());
        if (history) {
            history->setPosition(_fillNode->getContentSize()/2);
            
            _fillNode->getCanvas()->begin();
            history->visit();
            _fillNode->getCanvas()->end();
        }
    }
    
    _zoomLayer = LayerColor::create(Color4B(0, 0, 0, 0), _imageSize.width, _imageSize.height);
    _zoomLayer->ignoreAnchorPointForPosition(false);
    _zoomLayer->setScale(zoomLayerScale * ScreenAdapterHelper::getRealDesignScale());
    ScreenAdapterHelper::setPosition(_zoomLayer, Vec2(384, 645));
    
    _zoomLayer->addChild(_fillNode);
    addChild(_zoomLayer);
    
    xUndo->init(_fillNode, this);
    
    drawLineFrame(0.f);
}

void GameLayer::drawLineFrame(float pTime){
    Color4F color(0, 0, 0, 1.0);
    _fillNode->setStartColor(Color4B(color));
    _fillNode->drawLineFrame();
    
    scheduleOnce(schedule_selector(GameLayer::setTouchEnable), 0.5f);
    changeBackColor(_backColor);
}

void GameLayer::setTouchEnable(float dt)
{
    //响应点击事件：根据点击位置，够造相应着色参数给着色器。
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [this](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->bool{
        
        GameScene* lGameScene=(GameScene*)this->getParent();
        lGameScene->getGameUILayer()->removeTutorial();
        
        if(DataManager::getInstance()->isAlreadySelectedColor)
        {
            ColorData lColorData=DataManager::getInstance()->selectedColorData;
            auto color = Color4B(lColorData.R, lColorData.G, lColorData.B, 255);
            _fillNode->setStartColor(color);
            return true;
        }
        else
        {
            return false;
        }
    };
    
    listener->onTouchMoved = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    listener->onTouchEnded = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}


void GameLayer::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
        _currentMode->onTouchesBegan(touches, unused_event);
}

void GameLayer::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    
        _currentMode->onTouchesMoved(touches, unused_event);
}

void GameLayer::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    
        _currentMode->onTouchesEnded(touches, unused_event);
}

void GameLayer::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
        _currentMode->onTouchesCancelled(touches, unused_event);
}

void GameLayer::switchMode(BaseMode::Mode mode)
{
    if (_currentMode)
    {
        _currentMode->stop();
    }
    
    switch (mode) {
        case BaseMode::Mode::NONE:
        {
            _currentMode = _noneMode;
        }
            break;
        case BaseMode::Mode::PAINT:
        {
            _currentMode = _paintMode;
        }
            break;
        case BaseMode::Mode::WAIT:
        {
            _currentMode = _waitMode;
        }
            break;
        case BaseMode::Mode::ZOOM:
        {
            _currentMode = _zoomMode;
        }
            break;
        case BaseMode::Mode::MOVE:
        {
            _currentMode = _moveMode;
        }
            break;
        default:
            break;
    }

    _currentMode->start();
}

void GameLayer::paintPrevious()
{
    DataManager::m_bIspainted = true;
    for (int i = 0; i < _paints.size(); i ++) {
        Paint paint = _paints.at(i);
        if(!_fillNode->insideLineframe(paint.pos))
        {
            xUndo->record(Color3B(_backColor));
            changeBackColor();
        }
        else if(_fillNode->willColoring(paint.pos))
        {
            xUndo->record(paint.pos);
            _fillNode->coloring(paint.pos);
        }
    }
    
    _paints.clear();
}

void GameLayer::paint(Vec2 & current, Vec2 & previous)
{
    if(!_fillNode->insideLineframe(current))
    {
        xUndo->record(Color3B(_backColor));
        changeBackColor();
    }
    else if(_fillNode->willColoring(current))
    {
        xUndo->record(current);
        _fillNode->coloring(current);
    }
}

void GameLayer::stopPaint()
{
    _fillNode->resetPatternColor();
    _paints.clear();
}

Layer * GameLayer::getColouringLayer()
{
    return _zoomLayer;
}

string GameLayer::getImageName()
{
    if(_imageName.empty())
    {
        string filename;
        if(this->getIndex()<=DataManager::getInstance()->_resourceInfos.size())
        {
            ResourceInfo* lInfo=&DataManager::getInstance()->_resourceInfos.at(this->getIndex()-1);
            filename=lInfo->image_name;
        }
        else
        {
            ResourceInfo* lInfo=&ImageDownload::getInstance()->_downloadFinishResourceInfo.at(this->getIndex()-DataManager::getInstance()->_resourceInfos.size()-1);
            filename=lInfo->image_name;
        }
        
        _imageName=filename;
    }
    return _imageName;
}

string GameLayer::getImageName_w()
{
    if(_imageName_w.empty())
    {
        string filename;
        if(this->getIndex()<=DataManager::getInstance()->_resourceInfos.size())
        {
            ResourceInfo* lInfo=&DataManager::getInstance()->_resourceInfos.at(this->getIndex()-1);
            filename=lInfo->image_name_w;
        }
        else
        {
            ResourceInfo* lInfo=&ImageDownload::getInstance()->_downloadFinishResourceInfo.at(this->getIndex()-DataManager::getInstance()->_resourceInfos.size()-1);
            filename=lInfo->image_name_w;
        }
        
        _imageName_w=filename;
    }

    return _imageName_w;
    
}

string GameLayer::getHistoryImageName()
        {
    string name = getImageName();
    name = name.substr(0, name.find_last_of(".")) + "_history.png";
    return name;
    }
    
string GameLayer::getSaveImage_w_path()
{
    string name = getImageName();
    name = FileUtils::getInstance()->getWritablePath() + name.substr(0, name.find_last_of(".")) + "_save.png";
    return name;
}
void GameLayer::reset()
{
    deleteSaveImage();
    _fillNode->reset();
    
    {
        auto color = Color3B::WHITE;
        //将值写入背景颜色, selectedColorData 不变
        ColorData lColorData;
        lColorData.R = color.r;
        lColorData.G = color.g;
        lColorData.B = color.b;
    
        DataManager::getInstance()->saveBackColor(getIndex(), lColorData);
        _backColor = Color3B(lColorData.R, lColorData.G, lColorData.B);
    
        //整个背景颜色
        _colorLayer->setColor(color);
    }

    xUndo->reset();
}

void GameLayer::enableFillNode(bool pBool)
{
    if(pBool)
    {
        
    }
}

void GameLayer::saveImage()
{
    auto pos = _zoomLayer->getPosition();
    auto anchor = _zoomLayer->getAnchorPoint();
    auto scale = _zoomLayer->getScale();
    
    _zoomLayer->setPosition(Vec2(512, 512));
    _zoomLayer->setAnchorPoint(Vec2(0.5, 0.5));
    _zoomLayer->setScale(1);
    
    auto renderTexture = RenderTexture::create(_imageSize.width, _imageSize.height, Texture2D::PixelFormat::RGBA8888);
    renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
    _zoomLayer->visit();
    renderTexture->end();
    Director::getInstance()->getRenderer()->render();
    
    _zoomLayer->setPosition(pos);
    _zoomLayer->setAnchorPoint(anchor);
    _zoomLayer->setScale(scale);
    
    CC_SAFE_DELETE(_saveImage);
    _saveImage = renderTexture->newImage();
    _saveImage->saveToFile(getSaveImage_w_path(),false); //这张图, 做纯粹的预览图
    _fillNode->getCanvas()->saveToFile(getHistoryImageName());
    Director::getInstance()->getRenderer()->render();
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(getSaveImage_w_path());
    Director::getInstance()->getTextureCache()->removeTextureForKey(getHistoryImageName());
    
    FileUtils::getInstance()->purgeCachedEntries();
}

Image * GameLayer::getSaveImage()
{
    log("%p", _saveImage);
    return _saveImage;
}

void GameLayer::deleteSaveImage()
{
    std::string fullpath = FileUtils::getInstance()->getWritablePath() + _imageName_w;
    if(FileUtils::getInstance()->isFileExist(fullpath))
    {
        remove(fullpath.c_str());
    }
    
     Director::getInstance()->getTextureCache()->removeTextureForKey(this->getImageName_w());
    
    FileUtils::getInstance()->purgeCachedEntries();
    
    _imageName_w.clear();
    
}

void GameLayer::changeBackColor()
{
    //将值写入背景颜色
    ColorData lColorData = DataManager::getInstance()->selectedColorData;
    DataManager::getInstance()->saveBackColor(getIndex(), lColorData);
    
    _backColor = Color3B(lColorData.R, lColorData.G, lColorData.B);
    
    //整个背景颜色
    _colorLayer->setColor(Color3B(lColorData.R, lColorData.G, lColorData.B));
    
    //周围一圈的颜色
    Vec2 pos = _fillNode->getParent()->convertToWorldSpace(Vec2::ZERO);
    _fillNode->coloring(pos + Vec2(1, 1));
}

void GameLayer::changeBackColor(const Color3B &color)
{
    //将值写入背景颜色, selectedColorData 不变
    ColorData lColorData;
    lColorData.R = color.r;
    lColorData.G = color.g;
    lColorData.B = color.b;

    DataManager::getInstance()->saveBackColor(getIndex(), lColorData);
    _backColor = Color3B(lColorData.R, lColorData.G, lColorData.B);
    
    //整个背景颜色
    _colorLayer->setColor(Color3B(lColorData.R, lColorData.G, lColorData.B));
    
    //周围一圈的颜色
    Vec2 pos = _fillNode->getParent()->convertToWorldSpace(Vec2::ZERO);
    
    _fillNode->setStartColor(Color4B(color));
    _fillNode->coloring(pos + Vec2(1, 1));
    
    auto data = DataManager::getInstance()->selectedColorData;
    _fillNode->setStartColor(Color4B(Color3B(data.R, data.G, data.B)));
}

void GameLayer::changeBackColorDirect(const Color3B &color)
{
    //将值写入背景颜色, selectedColorData 不变
    ColorData lColorData;
    lColorData.R = color.r;
    lColorData.G = color.g;
    lColorData.B = color.b;
    
    DataManager::getInstance()->saveBackColorDirect(getIndex(), lColorData);
    _backColor = Color3B(lColorData.R, lColorData.G, lColorData.B);
    
    //整个背景颜色
    _colorLayer->setColor(Color3B(lColorData.R, lColorData.G, lColorData.B));
    
    //周围一圈的颜色
    Vec2 pos = _fillNode->getParent()->convertToWorldSpace(Vec2::ZERO);
    _fillNode->coloring(pos + Vec2(1, 1));
}

void GameLayer::undoBackColor(cocos2d::Color4B pColor)
{
    _colorLayer->setColor(Color3B(pColor.r, pColor.g, pColor.b));
    ColorData lColorData;
    lColorData.R=pColor.r;
    lColorData.G=pColor.g;
    lColorData.B=pColor.b;
    lColorData.ID=this->getIndex();
    DataManager::getInstance()->saveBackColor(this->getIndex(), lColorData);
}

void GameLayer::undo()
{
    if (!xUndo->undo()) {
        GameScene* lGameScene=(GameScene*)this->getParent();
        lGameScene->getGameUILayer()->undoEndPrompt();
    }
}

GameLayer::~GameLayer()
{
    CC_SAFE_DELETE(_saveImage);
    xUndo->destroyInstance();
    Director::getInstance()->getTextureCache()->removeTextureForKey(this->getImageName());
}

