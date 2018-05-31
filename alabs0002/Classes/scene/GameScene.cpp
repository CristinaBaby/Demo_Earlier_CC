//
//  GameScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "GameScene.h"
#include "UICpp.h"
#include "AppConfigs.h"
#include "UndoManager.h"
#include "PalletteManager.hpp"
#include "MakeMask.h"
#include "BaseMode.h"
#include "NoneMode.h"
#include "PaintMode.h"
#include "MoveMode.h"
#include "ZoomMode.h"
#include "WaitMode.h"
#include "ChapterManager.hpp"
#include "BlockLayer.h"
#include "ProgressManager.h"
#include "LockManager.h"
#include "MyArtDataManager.hpp"

#include "STSystemFunction.h"


GameScene::GameScene():_graph(xChapter->getCurrentChapter().getCurrentGraph()){
    
}

GameScene::~GameScene(){
    _scheduler->unscheduleAllForTarget(this);
}

bool GameScene::init(){
    
    if(Scene::init())
    {
        setName("GameScene");
        
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("game.json"));
        addChild(_root);
        _root->cocos2d::Node::visit();
        
        _undo = quickLayout("undo", _root, CC_CALLBACK_2(GameScene::onButton, this));
        _back = quickLayout("back_touch", _root, CC_CALLBACK_2(GameScene::onButton, this));
        _done = quickLayout("done_touch", _root, CC_CALLBACK_2(GameScene::onButton, this));
        _del = quickLayout("delete_touch", _root, CC_CALLBACK_2(GameScene::onButton, this));
        _undo->setVisible(false);
        _del->setVisible(false);
        
        _palletteName = (Text*)Helper::seekNodeByName(_root, "TXT_pallette");
        _blockLayer = BlockLayer::create();
        addChild(_blockLayer, 10);
        
        auto event_move_pages = EventListenerCustom::create("event_move_pages", CC_CALLBACK_1(GameScene::onPageMove, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(event_move_pages, this);
        
        auto event_color = EventListenerCustom::create("event_color", CC_CALLBACK_1(GameScene::onChooseColor, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(event_color, this);
        
        auto event_after_visit = EventListenerCustom::create("director_after_visit", CC_CALLBACK_1(GameScene::onAfterVisit, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(event_after_visit, this);
        
        createZoomLayer();
        createPallette();
        
        xAnalytics->sendScreenEvent("GameScene");
        
        return true;
    }

    return false;
}

void GameScene::onEnterTransitionDidFinish()
{
    if (UserDefault::getInstance()->getIntegerForKey(rateUsCount) >= 3)
    {
        UserDefault::getInstance()->setIntegerForKey(rateUsCount, 0);
        UserDefault::getInstance()->flush();
        
//        STSystemFunction sf;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        sf.rating("1040798228", "Your suggestion will help us improving our apps! Let us know what do you want!");
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        sf.rating();
//#endif
    }
    
    xProgress->start(CC_CALLBACK_1(GameScene::onEnterGame, this));
}

void GameScene::onAfterVisit(EventCustom* event)
{
    //修正坐标
    _eventDispatcher->removeCustomEventListeners("director_after_visit");
    auto offset = (_done->getPositionX() - _back->getPositionX()) / 3;
    _del->setPositionX(_back->getPositionX() + offset);
    _del->setVisible(true);
    _undo->setPositionX(_back->getPositionX() + 2 * offset);
    _undo->setVisible(true);
}

float GameScene::onEnterGame(int index)
{
    float interval = 0;
    
    switch (index) {
        case 0:
        {
            SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
            Director::getInstance()->getTextureCache()->removeUnusedTextures();
        }
            break;
        case 1: //检查并生成mask
        {
//            MakeMask mask;
//            mask.make(_graph.getPngName());
        }
            break;
        case 2:
        {
            createScribble();
        }
            break;
        case 3:
        {
            _noneMode = new NoneMode(this);
            _waitMode = new WaitMode(this);
            _paintMode = new PaintMode(this);
            _moveMode = new MoveMode(this);
            _zoomMode = new ZoomMode(this);
            switchMode(BaseMode::Mode::NONE);
            _zoomMode->setMaxScale(_fillNode->getScale());
            
            auto listener = EventListenerTouchAllAtOnce::create();
            listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
            listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
            listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
            listener->onTouchesCancelled = CC_CALLBACK_2(GameScene::onTouchesCancelled, this);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        }
            break;
        case 4:
        {
            _blockLayer->removeFromParent();
        }
            break;
        case 5:
        {
            xProgress->stop();
        }
            break;
        default:
            break;
    }
    
    return interval;
}

float GameScene::onExitGame(int index)
{
    float interval = 0;
    
    switch (index) {
        case 0:
        {
            _eventDispatcher->removeEventListenersForTarget(this);
            
            //截屏保存
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            utils::captureScreen(CC_CALLBACK_2(GameScene::onAfterCaptured, this), "screen.png");
            xProgress->pause();
#else
            auto rotateSprite = Sprite::create("other/load_rotate_icon.png");
            rotateSprite->setPosition(_rotatePos);
            addChild(rotateSprite);
            
            auto action = RotateBy::create(1.0f, 360.0f);
            rotateSprite->runAction(RepeatForever::create(action));
#endif
        }
            break;
        case 1:
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            //释放不需要的资源
            CC_SAFE_RETAIN(_fillNode);
            _root->removeFromParent();
#endif
        }
            break;
        case 2:
        {
            //清除内存
            SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
            Director::getInstance()->getTextureCache()->removeUnusedTextures();
            GUIReader::getInstance()->destroyInstance();
            FileUtils::getInstance()->purgeCachedEntries();
        }
            break;
        case 3:
        {
            _graph = xChapter->getCurrentChapter().getCurrentGraph();
            
            //保存 preview, 用于 GraphScene 预览
            auto pos = _fillNode->getPosition();
            
            _fillNode->setPosition(Vec2::ZERO);
            auto canvans = _fillNode->getCanvas()->newImage();
            canvans->autorelease();
            auto texture = new Texture2D();
            texture->initWithImage(canvans);
            texture->autorelease();
            auto image = Sprite::createWithTexture(texture);
            image->setScale(THUMB_WIDTH/_fillNode->getContentSize().width);
            image->setPosition(Vec2(THUMB_WIDTH/2, THUMB_WIDTH/2));
            
            auto renderTexture = RenderTexture::create(THUMB_WIDTH, THUMB_WIDTH, Texture2D::PixelFormat::RGBA8888);
            renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
            image->visit();
            renderTexture->end();
            Director::getInstance()->getRenderer()->render();
            
            auto previewImage = renderTexture->newImage();
            previewImage->autorelease();
            eraseLineFrame(previewImage->getData());
            previewImage->saveToFile(FileUtils::getInstance()->getWritablePath() + _graph.getPreviewName(), false);
            
            _fillNode->setPosition(pos);
        }
            break;
        case 4:
        {
            //保存 save
            auto count = xUndo->getRecordCount();
            xGraph->destorySaveImage();
            
            if (_forward) {
                auto pos = _fillNode->getPosition();
                _fillNode->setPosition(Vec2::ZERO);
                
                auto scale = _fillNode->getScale();
                _fillNode->setScale(1);
                
                auto renderTexture = RenderTexture::create(_fillNode->getContentSize().width, _fillNode->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
                renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
                _fillNode->visit();
                renderTexture->end();
                Director::getInstance()->getRenderer()->render();
                
                _fillNode->setScale(scale);
                
                auto saveImage = renderTexture->newImage();
                xGraph->setSaveImage(saveImage);
                
                MyArtData lMyart;
                lMyart.name = _graph.getPngName();
                lMyart.previewName = _graph.getPreviewName();
                
                Color3B color = _graph.bgColor;
                lMyart.bgColor_R = color.r;
                lMyart.bgColor_G = color.g;
                lMyart.bgColor_B = color.b;
                
                xMyArtData->addImageToVec(lMyart);
                
                
                
                _fillNode->setPosition(pos);
 
            }else
            {
                if (count > 0)
                {
                    MyArtData lMyart;
                    lMyart.name = _graph.getPngName();
                    lMyart.previewName = _graph.getPreviewName();
                    
                    Color3B color = _graph.bgColor;
                    lMyart.bgColor_R = color.r;
                    lMyart.bgColor_G = color.g;
                    lMyart.bgColor_B = color.b;
                    
                    xMyArtData->addImageToVec(lMyart);
                }
            }
        }
            break;
        case 5:
        {
            _fillNode->getCanvas()->saveToFile(_graph.getHistoryName());
            Director::getInstance()->getRenderer()->render();
        }
            break;
        case 6:
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            CC_SAFE_DELETE(_fillNode);
#endif
        }
            break;
        case 7:
        {
            //清除内存
            SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
            Director::getInstance()->getTextureCache()->removeUnusedTextures();
            GUIReader::getInstance()->destroyInstance();
            FileUtils::getInstance()->purgeCachedEntries();
        }
            break;
        case 8:
        {
            xProgress->stop();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
            _root->setClippingEnabled(true);
#endif
            if (_forward) {
                xScene->forward(SceneManager::Tag::SHARE);
            }
            else
            {
                xScene->back();
            }
        }
            break;
        default:
            break;
    }
    
    return interval;
}

void GameScene::applicationDidEnterBackground()
{
    //保存 preview, 用于 GraphScene 预览
    auto pos = _fillNode->getPosition();
    
    _fillNode->setPosition(Vec2::ZERO);
    auto canvans = _fillNode->getCanvas()->newImage();
    canvans->autorelease();
    auto texture = new Texture2D();
    texture->initWithImage(canvans);
    texture->autorelease();
    auto image = Sprite::createWithTexture(texture);
    image->setScale(THUMB_WIDTH/_fillNode->getContentSize().width);
    image->setPosition(Vec2(THUMB_WIDTH/2, THUMB_WIDTH/2));
    
    auto renderTexture = RenderTexture::create(THUMB_WIDTH, THUMB_WIDTH, Texture2D::PixelFormat::RGBA8888);
    renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
    image->visit();
    renderTexture->end();
    Director::getInstance()->getRenderer()->render();
    
    auto previewImage = renderTexture->newImage();
    previewImage->autorelease();
    eraseLineFrame(previewImage->getData());
    previewImage->saveToFile(FileUtils::getInstance()->getWritablePath() + _graph.getPreviewName(), false);
    
    _fillNode->setPosition(pos);
    
    _fillNode->getCanvas()->saveToFile(_graph.getHistoryName());
    Director::getInstance()->getRenderer()->render();
}

void GameScene::createZoomLayer()
{
    auto up = (Layout*)Helper::seekNodeByName(_root, "up_ui");
    up->setContentSize(Size(getContentSize().width, up->getContentSize().height));
    auto bottom = (Layout*)Helper::seekNodeByName(_root, "pallette");
    bottom->setContentSize(Size(getContentSize().width, bottom->getContentSize().height));
    
    auto height = up->getPosition().y - bottom->getPosition().y - bottom->getContentSize().height;
    _zoomLayer = LayerColor::create(Color4B::WHITE, getContentSize().width, height);
    _zoomLayer->ignoreAnchorPointForPosition(false);
    _zoomLayer->setPosition(Vec2(_zoomLayer->getContentSize() / 2) + Vec2(0, bottom->getPosition().y + bottom->getContentSize().height));
    _root->addChild(_zoomLayer, 4);
    
    _rotatePos = _zoomLayer->getPosition();
    _blockLayer->setLoadingPosition(_rotatePos);
}

void GameScene::createScribble()
{
    _fillNode = ColoringNode::create(_graph.getMaskName(), _graph.getPngName());
    _fillNode->setDefaultColor(Color4F::WHITE);
    _fillNode->setColoringMode(ColoringNode::ColoringMode::eFlat);
    
    if (!_graph.isNew) {
        auto path = FileUtils::getInstance()->getWritablePath();
        bool isExsit = FileUtils::getInstance()->isFileExist(path + _graph.getHistoryName());
        if (isExsit)
        {
            Sprite *history = Sprite::create(_graph.getHistoryName());
            history->setPosition(_fillNode->getContentSize()/2);
            
            _fillNode->getCanvas()->begin();
            history->visit();
            _fillNode->getCanvas()->end();
            quickFlurry("img_paint_" + _graph.name);
        }
    }

    _fillNode->setScale(quickAdaptScale(_fillNode, _zoomLayer));
    Size sizeOffset = _zoomLayer->getContentSize() - _fillNode->getBoundingBox().size;
    _fillNode->setPosition(sizeOffset/2);
    
    _zoomLayer->addChild(_fillNode);
    _zoomLayer->setColor(_graph.bgColor);
    
    xUndo->init(_fillNode, this);
    drawLineFrame();
}

//这个函数会卡住1秒钟
void GameScene::drawLineFrame()
{
    _fillNode->setStartColor(Color4B::BLACK);
    _fillNode->drawLineFrame();
    _fillNode->setStartColor(Color4B(xPallette->getCurrentColor()));
}

void GameScene::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    _currentMode->onTouchesBegan(touches, unused_event);
}

void GameScene::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    _currentMode->onTouchesMoved(touches, unused_event);
}

void GameScene::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    _currentMode->onTouchesEnded(touches, unused_event);
}

void GameScene::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    _currentMode->onTouchesCancelled(touches, unused_event);
}

void GameScene::switchMode(BaseMode::Mode mode)
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

void GameScene::paintPrevious()
{
    for (int i = 0; i < _paints.size(); i ++) {
        Paint paint = _paints.at(i);
        if(!_fillNode->insideLineframe(paint.pos))
        {
            xUndo->record(_graph.bgColor);
            changeBackgroundColor();
        }
        else if(_fillNode->willColoring(paint.pos))
        {
            xUndo->record(paint.pos);
            _fillNode->coloring(paint.pos);
        }
    }
    
    _paints.clear();
}

void GameScene::paint(Vec2 & current, Vec2 & previous)
{
    if(!_fillNode->insideLineframe(current))
    {
        xUndo->record(_graph.bgColor);
        changeBackgroundColor();
    }
    else if(_fillNode->willColoring(current))
    {
        xUndo->record(current);
        _fillNode->coloring(current);
    }
}

void GameScene::changeBackgroundColor()
{
    _graph.setBGColor(xPallette->getCurrentColor());
    _fillNode->coloringNodeSpace(Vec2::ZERO + Vec2(1, 1));
    _zoomLayer->setColor(xPallette->getCurrentColor());
}

void GameScene::undoBackgroundColor(const Color3B & color)
{
    _graph.setBGColor(color);
    
    _fillNode->setStartColor(Color4B(color));
    _fillNode->coloringNodeSpace(Vec2::ZERO + Vec2(1, 1));
    
    _zoomLayer->setColor(color);
}

void GameScene::stopPaint()
{
    _fillNode->resetPatternColor();
    _paints.clear();
}

Layer * GameScene::getZoomLayer()
{
    return _zoomLayer;
}

void GameScene::onDialog(const string& name)
{
    if(name == "reset"){
        xUndo->reset();
        _graph.setBGColor(Color3B::WHITE);
        _zoomLayer->setColor(Color3B::WHITE);
        _fillNode->reset();
        drawLineFrame();
        
        quickFlurry("btn_reset");
    }
    else if (name == "watch_ad"){
        xLock->unlock(xDialog->getUserString(), LockManager::Type::PALLETTE);
        quickFlurry("watchAds_" + xDialog->getUserString());
    }
}

void GameScene::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED || _blockTouch) return;
    
    string name = ((Node*)pSender)->getName();
    
    if (name == "done_touch") {
        _blockTouch = true;
        xProgress->start(CC_CALLBACK_1(GameScene::onExitGame, this));
    }
    else if(name == "back_touch")
    {
        _blockTouch = true;
        _forward = false;
        xProgress->start(CC_CALLBACK_1(GameScene::onExitGame, this));
    }
    else if(name == "undo")
    {
        _blockTouch = true;
        
        if (!xUndo->undo()) {
            xDialog->show("undo", "ok", nullptr);
        }
        else
        {
            quickFlurry("btn_undo");
        }
        
        runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create([&](){
            _blockTouch = false;
        }), nullptr));
    }
    else if(name == "delete_touch")
    {
        xDialog->show("reset", "cancel", "reset", CC_CALLBACK_1(GameScene::onDialog, this));
    }
}

void GameScene::createPallette()
{ 
    //替换 pallette
    auto pallettes = (Layout*)Helper::seekNodeByName(_root, "pallette");
//    pallettes->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
//    pallettes->setBackGroundColor(cocos2d::Color3B::BLUE);
    
    //return;
    ColorScrollLayer* layer = ColorScrollLayer::create(Size(0, 0));
    _palletteLayer = layer;
    float scale = V::getRealDesignSize().width/768.f;
    layer->setScale(scale);
    layer->setPosition(Vec2(pallettes->getContentSize()/2) - Vec2(0, 75 * scale));
    pallettes->addChild(layer);
}

void GameScene::onPageMove(EventCustom* event)
{
    float percent = _pallettes->getPercent();
    _palletteName->setOpacity(255 - 255 * percent);
}

void GameScene::onChooseColor(EventCustom* event)
{
    ColorIndex * ci = (ColorIndex*)event->getUserData();
    
    //检查是否解锁
    auto & data = xPallette->getData();
    if (data.at(ci->page).isLock &&
        xLock->isLocked(data.at(ci->page).name, LockManager::Type::PALLETTE))
    {
        xDialog->show("watch_ad_pallette", "cancel", "watch_ad", CC_CALLBACK_1(GameScene::onDialog, this));
        xDialog->setUserString(xPallette->getData().at(ci->page).name);
    }
    else
    {
        auto color = xPallette->getColor(ci->page, ci->index);
        _fillNode->setStartColor(Color4B(color));
    }
    quickFlurry("pallete_" + xPallette->getData().at(ci->page).name);
}

void GameScene::onAfterCaptured(bool succeed, const string & filename)
{
    if (succeed) {
        auto screen = Sprite::create(filename);
        screen->setPosition(getContentSize()/2);
        screen->setScale(V::getRealDesignScale());
        addChild(screen);
        
        //add animation
        auto rotateSprite = Sprite::create("other/load_rotate_icon.png");
        rotateSprite->setPosition(_rotatePos);
        addChild(rotateSprite);
        
        auto action = RotateBy::create(1.0f, 360.0f);
        rotateSprite->runAction(RepeatForever::create(action));
        
        xProgress->resume();
    }
}

void GameScene::eraseLineFrame(unsigned char * imageData)
{
    return;
    for (int y = 0; y < THUMB_WIDTH; y++) {
        
        for (int x = 0; x < THUMB_WIDTH; x++) {
            
            unsigned int *data = (unsigned int *)(imageData);
            data += (y * THUMB_WIDTH + x);
            
            Color4B color;
            color.r = *data & 0xFF;
            color.g = (*data >> 8) & 0xFF;
            color.b = (*data >> 16) & 0xFF;
            color.a = (*data >> 24) & 0xFF;

            //去掉 line frame
            if (color.r < 100 && color.a != 0 &&
                color.r == color.g && color.g == color.b) {
                *data = (color.r | (color.g << 8) | (color.b << 16) | (0 << 24));
            }
        }
    }
}

void GameScene::changePalletteName()
{
    if (_palletteLayer)
    {
        _palletteLayer->waitUnlockSuc();
    }
    
}

