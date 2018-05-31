//
//  GameLayer.cpp
//  ColorFree
//
//  Created by houhuang on 15/8/31.
//
//

#include "GameLayer.h"

#include "../helper/ScreenAdapterHelper.h"
#include "../data/DataManager.h"
#include "../scene/GameScene.h"
#include "GameUILayer.h"
#include "../modules/STSystemFunction.h"

#include <stdio.h>
#include <vector>
#include <thread>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#endif

static float zoomLayerScale = 0.5f;

GameLayer* GameLayer::create()
{
    GameLayer* layer = new GameLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

GameLayer::~GameLayer()
{
    Director::getInstance()->getTextureCache()->removeTextureForKey(this->getImageName());
    
}

bool GameLayer::init()
{
    if (!Layer::init()) return false;

    m_iImageCount = DataManager::getInstance()->m_pCurrentImage.ID;
    m_sImageGallery = DataManager::getInstance()->m_pCurrentImage.imageName;

//    Sprite* lRotateSprite = Sprite::create("load_rotate_icon.png");
//    ScreenAdapterHelper::setPosition(lRotateSprite, Vec2(384, 580));
//    this->addChild(lRotateSprite, 100);
//    lRotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));
//    m_pRotateSprite = lRotateSprite;
    
    this->display();
    
    return true;
}

void GameLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    
}

void GameLayer::display()
{
//    if (m_pRotateSprite)
//    {
//        m_pRotateSprite->removeFromParentAndCleanup(true);
//        m_pRotateSprite = NULL;
//    }
    
    Color3B color = DataManager::getInstance()->getSaveBackColor(m_sImageGallery, m_iImageCount);
    
    //background
    m_pLayerColor = LayerColor::create(Color4B(color.r, color.g, color.b, 255));
    this->addChild(m_pLayerColor);
    
    
    m_pFileNode = BoundaryFillNode::create(this->getImageName_w(), this->getImageName());
    ZoomLayer* lZoomLayer = ZoomLayer::create(m_pFileNode);
    lZoomLayer->setScale(zoomLayerScale * ScreenAdapterHelper::getRealDesignScale());
    this->addChild(lZoomLayer);
    ScreenAdapterHelper::setPosition(lZoomLayer, Vec2(384, 670));
    m_pZoomLayer = lZoomLayer;
    
    this->schedule(schedule_selector(GameLayer::setTouchEnable), 0.35f);
}

void GameLayer::reset()
{
    DM_getInstance->m_bIsAddMyAtrworks = false;
    DataManager::m_bIsImagereset = true;
    
    m_pFileNode->_dotColorDatas.clear();

    if (m_pFileNode)
    {
        m_pFileNode->removeFromParentAndCleanup(true);
        m_pFileNode = nullptr;
    }
    
    if (m_pZoomLayer)
    {
        m_pZoomLayer->removeFromParentAndCleanup(true);
        m_pZoomLayer = nullptr;
    }
    
    this->deleteSaveImage();
    
    ColorData lColorData;
    lColorData.R = 255;
    lColorData.G = 255;
    lColorData.B = 255;
    lColorData.ID = m_iImageCount;
    DataManager::getInstance()->saveBackColor(m_sImageGallery, m_iImageCount, lColorData);
    m_pLayerColor->setColor(Color3B(255, 255, 255));

    m_pFileNode = BoundaryFillNode::create(this->getImageName_w(), this->getImageName());
    ZoomLayer* lZoomLayer = ZoomLayer::create(m_pFileNode);
    lZoomLayer->setScale(zoomLayerScale * ScreenAdapterHelper::getRealDesignScale());
    ScreenAdapterHelper::setPosition(lZoomLayer, Vec2(384, 670));
    m_pZoomLayer = lZoomLayer;
    this->addChild(lZoomLayer);
    
}

void GameLayer::saveImage()
{
    Image* lImage = new Image();
    const unsigned char* lData = m_pFileNode->getImageData();
    long int lLength = m_pFileNode->getImageLength();
    
    lImage->initWithRawData(lData, lLength, m_pFileNode->getLineImage()->getWidth(), m_pFileNode->getLineImage()->getHeight(), 8);
    
    
    string lPath = FileUtils::getInstance()->getWritablePath() + this->getImageName_w();
    lImage->saveToFile(lPath, false);
    lImage->autorelease();
    
    
    Sprite* lSprite = Sprite::create(lPath);
    lSprite->setScale(0.5f);
    
    RenderTexture* lRenderTexture = RenderTexture::create(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2, Texture2D::PixelFormat::RGBA8888);
    lRenderTexture->begin();
    lSprite->setPosition(Vec2(lSprite->getContentSize().width/4, lSprite->getContentSize().height/4));
    lSprite->visit();
    lRenderTexture->end();
    Director::getInstance()->getRenderer()->render();
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(lPath);
    
    stringstream ss;
    ss<<"";
    ss<<DM_getInstance->m_pCurrentImage.imageName;
    ss<<DM_getInstance->m_pCurrentImage.ID;
    ss<<"_small";
    ss<<"_white.png";

    
    string smallImagePath = FileUtils::getInstance()->getWritablePath() + ss.str();
    lRenderTexture->newImage()->saveToFile(smallImagePath, false);
    lRenderTexture->newImage()->autorelease();

    Director::getInstance()->getTextureCache()->removeTextureForKey(smallImagePath);
}

bool GameLayer::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, nullptr);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#endif
    
}

Sprite* GameLayer::getSpriteDrawScene()
{
    Color3B color = DM_getSaveBackColor;
    LayerColor* layerColor = LayerColor::create(Color4B(color.r, color.g, color.b, 255), ScreenAdapterHelper::getValue(1024), ScreenAdapterHelper::getValue(1024));
    layerColor->setAnchorPoint(Vec2(0.5, 0.5));

//    std::string realPath;
    std::string sullPath = FileUtils::getInstance()->getWritablePath() + m_sImageName_w;
    
//    if (FileUtils::getInstance()->isFileExist(sullPath))
//    {
//        realPath = sullPath;
//    }else
//    {
//        realPath = m_sImageName_w;
//    }
//    
    
    Sprite* lSprite = Sprite::create(sullPath);
    lSprite->setPosition(Vec2(layerColor->getContentSize().width/2, layerColor->getContentSize().height/2));
    lSprite->addChild(layerColor, -1);
    
    m_pEditSprite = lSprite;
    
    lSprite->setFlippedY(false);
    
    Vec2 lOrginPos = m_pEditSprite->getPosition();
    RenderTexture* lRenderTexture = RenderTexture::create(m_pEditSprite->getContentSize().width, m_pEditSprite->getContentSize().height);
    lRenderTexture->begin();
    m_pEditSprite->setPosition(Vec2(m_pEditSprite->getContentSize().width/2, m_pEditSprite->getContentSize().height/2));
    m_pEditSprite->visit();
    lRenderTexture->end();
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(sullPath);
    
    m_pEditSprite->setPosition(lOrginPos);
    m_pEditSprite->setFlippedY(true);
    
    
    Sprite* lResSprite=lRenderTexture->getSprite();
    lResSprite->getTexture()->setAntiAliasTexParameters();

    
    lResSprite->retain();
    lResSprite->removeFromParent();
    lResSprite->setFlippedY(false);
    return lResSprite;
    
}

void GameLayer::saveEditImage()
{
    Color3B color = DM_getSaveBackColor;
    LayerColor* layerColor = LayerColor::create(Color4B(color.r, color.g, color.b, 255), 1024, 1024);
    layerColor->setAnchorPoint(Vec2(0.5, 0.5));
    
    Sprite* lSprite = Sprite::create(this->getImageName_w());
    lSprite->setPosition(Vec2(layerColor->getContentSize().width/2, layerColor->getContentSize().height/2));
    lSprite->addChild(layerColor, -1);
    
    m_pEditSprite = lSprite;
    
    
    Vec2 lOrginPos = m_pEditSprite->getPosition();
    RenderTexture* lRenderTexture = RenderTexture::create(m_pEditSprite->getContentSize().width, m_pEditSprite->getContentSize().height);
    lRenderTexture->begin();
    m_pEditSprite->setPosition(Vec2(m_pEditSprite->getContentSize().width/2, m_pEditSprite->getContentSize().height/2));
    m_pEditSprite->visit();
    lRenderTexture->end();
    
    Director::getInstance()->getRenderer()->render();

    Image* lImage = lRenderTexture->newImage();
    
    string lPath;
    
#if (CC_TARGET_PLATFROM == CC_PLATFROM_IOS)
    lPath = FileUtils::getInstance()->getWritablePath() + "edit.png";
#elif (CC_TARGET_PLATFROM == CC_PLATFORM_ANDROID)
    STSystemFunction* sf;
    lPath = sf.getSdCardPath()+"edit.png";
#endif
    
    m_sImagePath = lPath;
    lImage->saveToFile(lPath, false);
    lImage->autorelease();
    
    m_pEditSprite->setPosition(lOrginPos);
    Director::getInstance()->getTextureCache()->removeTextureForKey(lPath);
    
}

void GameLayer::deleteSaveImage()
{
    
    std::string fullPath = FileUtils::getInstance()->getWritablePath() + m_sImageName_w;

    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
        remove(fullPath.c_str());
        Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
    }
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(m_sImageName_w);
    m_sImageName_w.clear();

}

void GameLayer::undo()
{
    if (m_pFileNode->_dotColorDatas.size() <= 0)
    {
        this->enableFillNode(false);
        GameScene* lGameScene = dynamic_cast<GameScene*>(this->getParent());
        lGameScene->getGameUILayer()->undoEndPrompt();
    }else
    {
        DataManager::m_bIsImageundo;
        DataManager::m_iCurrentCancelNumber++;
        m_pFileNode->undo();
    }
}

void GameLayer::setTouchEnable(float dt)
{
    this->enableFillNode(true);
}

void GameLayer::enableFillNode(bool pBool)
{
    m_pFileNode->stopFillColor = pBool;
}

std::string GameLayer::getImageName_w()
{
    if (m_sImageName_w.empty())
    {
        stringstream ss;
        ss<<"";
        ss<<m_sImageGallery;
        ss<<m_iImageCount;
        ss<<"_white.png";
        m_sImageName_w = ss.str();
        
        std::string sullPath = FileUtils::getInstance()->getWritablePath() + m_sImageName_w;
        if (FileUtils::getInstance()->isFileExist(sullPath))
        {
            return sullPath;
        }
    }
    
    return m_sImageName_w;
}

std::string GameLayer::getImageName()
{
    if (m_sImageName.empty())
    {
        stringstream ss;
        ss<<"";
        ss<<m_sImageGallery;
        ss<<m_iImageCount;
        ss<<".png";
        m_sImageName = ss.str();
        
        std::string fullPath = FileUtils::getInstance()->getWritablePath() + m_sImageName;
        if (FileUtils::getInstance()->isFileExist(fullPath))
        {
            return fullPath;
        }
    }

    return m_sImageName;
}

void GameLayer::undoBackColor(Color4B pColor)
{
    m_pLayerColor->setColor(Color3B(pColor.r, pColor.g, pColor.b));
    
    ColorData lColorData;
    lColorData.R = pColor.r;
    lColorData.G = pColor.g;
    lColorData.B = pColor.b;
    lColorData.ID = m_iImageCount;
    DataManager::getInstance()->saveBackColor(m_sImageGallery, m_iImageCount, lColorData);
    
}

void GameLayer::changeBackColor()
{
    ColorData lColorData = DataManager::getInstance()->m_cSelectedColor;
    m_pLayerColor->setColor(Color3B(lColorData.R, lColorData.G, lColorData.B));
    DataManager::getInstance()->saveBackColor(m_sImageGallery, m_iImageCount, lColorData);
}