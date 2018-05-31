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
#include "../Coloring/ZoomLayer.h"
#include "../layer/ScrollColorLayer.h"
#include "../game/GameUILayer.h"
#include "../scene/GameScene.h"
#include "../helper/ImageDownload.h"
#include "../helper/DeviceHelper.h"

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
    
    _imageBox=NULL;
    _zoomLayer=NULL;
    
    this->setIndex(pIndex);
    
    Color3B lBackColor=DataManager::getInstance()->getSaveBackColor(pIndex);
    
    LayerColor* whiteColor=LayerColor::create(Color4B(lBackColor.r, lBackColor.g, lBackColor.b, 255));
    this->addChild(whiteColor);
    _colorLayer=whiteColor;

    float lScale = ScreenAdapterHelper::getRealDesignScale();
    if(lScale==1 || lScale==2)
    {
        zoomLayerScale=0.55f;
    }
    
    _fillNode=BoundaryFillNode::create(this->getImageName_w(),this->getImageName());
    ZoomLayer *lZoomLayer = ZoomLayer::create(_fillNode);
    lZoomLayer->setScale(zoomLayerScale * ScreenAdapterHelper::getRealDesignScale());
    this->addChild(lZoomLayer);
    if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Pad)
    {
        ScreenAdapterHelper::setPosition(lZoomLayer, Vec2(384, 645));
    }else
    {
        ScreenAdapterHelper::setPosition(lZoomLayer, Vec2(384, 645));
    }
    
    _zoomLayer=lZoomLayer;
    
    
    this->scheduleOnce(schedule_selector(GameLayer::setTouchEnable), 0.5f);
    
  
    
    
    return true;
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
            _fillNode->setStartColor(Color4B(lColorData.R, lColorData.G, lColorData.B, 255));
            return true;
        }
        else
        {
            return false;
        }
        
        
        
    };
    listener->onTouchMoved = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    listener->onTouchEnded = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
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

string GameLayer::getSaveImage_w_path()
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
    
   std::string fullpath = FileUtils::getInstance()->getWritablePath() + _imageName_w;
   return fullpath;
}
void GameLayer::reset()
{
    _fillNode->_dotColorDatas.clear();
    
    if(_fillNode!=NULL)
    {
       _fillNode->removeFromParentAndCleanup(true);
        _fillNode=NULL;
    }

    
    if(_zoomLayer!=NULL)
    {
        _zoomLayer->removeFromParentAndCleanup(true);
        _zoomLayer=NULL;
    }
    
    this->deleteSaveImage();
    
    ColorData lColorData;
    lColorData.R=255;
    lColorData.G=255;
    lColorData.B=255;
    lColorData.ID=this->getIndex();
    DataManager::getInstance()->saveBackColor(this->getIndex(), lColorData);
    
    _colorLayer->setColor(Color3B(255, 255, 255));
    
     _fillNode=BoundaryFillNode::create(this->getImageName_w(),this->getImageName());
    ZoomLayer *lZoomLayer = ZoomLayer::create(_fillNode);
    lZoomLayer->setScale(zoomLayerScale * ScreenAdapterHelper::getRealDesignScale());
    this->addChild(lZoomLayer);
    if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Pad)
    {
        ScreenAdapterHelper::setPosition(lZoomLayer, Vec2(384, 645));
    }else
    {
        ScreenAdapterHelper::setPosition(lZoomLayer, Vec2(384, 645));
    }
    _zoomLayer=lZoomLayer;
    

}

void GameLayer::enableFillNode(bool pBool)
{
    if(pBool)
    {
        _fillNode->stopFillColor=false;
    }
    else
    {
        _fillNode->stopFillColor=true;
    }
}

void GameLayer::saveImage()
{
    Image* lImage=new Image();
    const unsigned  char *lData = _fillNode->getImageData();
    long int lLength = _fillNode->getImageLength();
//    lImage->initWithImageData(lData, lLength);//这种不能直接使用
    lImage->initWithRawData(lData, lLength, _fillNode->getLineImage()->getWidth(), _fillNode->getLineImage()->getHeight(),4,true);
    
    
    
    lImage->saveToFile(this->getSaveImage_w_path(),false);

    Director::getInstance()->getTextureCache()->removeTextureForKey(this->getImageName_w());
    
    FileUtils::getInstance()->purgeCachedEntries();
    
    
    
    log("%s", this->getImageName_w().c_str());
//    Director::getInstance()->getTextureCache()->removeTextureForKey(this->getSaveImage_w_path());
    
    //    string lPath = FileUtils::getInstance()->getWritablePath() + this->getImageName_w();
//    lImage->saveToFile(lPath,false);
//    lImage->autorelease();

    
    
    //lImage->autorelease();
//    Sprite* lSprite=Sprite::create(this->getImageName_w()); //纹理已经缓存了 所以直接绘制它就可以了
//    lSprite->setPosition(lSprite->getContentSize()/2);
//    RenderTexture* lRenderText=RenderTexture::create(lSprite->getContentSize().width, lSprite->getContentSize().height);
//    lRenderText->clear(0, 0, 0, 0);
//    lRenderText->begin();
//    lSprite->visit();
//    lRenderText->end();
//    lRenderText->saveToFile(this->getImageName_w());
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
    ColorData lColorData=DataManager::getInstance()->selectedColorData;
    _colorLayer->setColor(Color3B(lColorData.R, lColorData.G, lColorData.B));
    
    DataManager::getInstance()->saveBackColor(this->getIndex(), lColorData);
    
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
    if(_fillNode->_dotColorDatas.size()<=0)
    {
        GameScene* lGameScene=(GameScene*)this->getParent();
        lGameScene->getGameUILayer()->undoEndPrompt();
    }
    else
    {
        _fillNode->undo();
    }
}

GameLayer::~GameLayer()
{
    Director::getInstance()->getTextureCache()->removeTextureForKey(this->getImageName());
}

