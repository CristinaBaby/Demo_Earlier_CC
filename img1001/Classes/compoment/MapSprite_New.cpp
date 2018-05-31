//
//  MapSprite_New.cpp
//  IMG1001
//
//  Created by houhuang on 15/9/21.
//
//

#include "MapSprite_New.h"
#include "../data/DataManager.h"
#include "../helper/ScreenAdapterHelper.h"
#include "TargetSprite.h"
#include "../layer/ScrollMapLayer_ScrollView.h"
using namespace std;

bool MapSprite_New::init()
{
    if(!Node::init()) return false;
    
    _isStopLoading=false;
    _textureSprite=NULL;
    m_sMapSpriteName.clear();
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    Sprite* border = Sprite::create("map_frame.png");

    this->addChild(border, -1);
    this->setContentSize(border->getContentSize());
    border->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    
    Sprite* lRotateSprite = Sprite::create("load_rotate_icon.png");
    lRotateSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(lRotateSprite, 100);
    lRotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 250)));
    _rotateSprite=lRotateSprite;
    
    return true;
}

MapSprite_New::MapSprite_New()
{
    m_sMapSpriteName    = "";
    m_vFirstPoint       = Vec2(0, 0);
    
    _selectedCallBack   = NULL;
    _target             = NULL;
    _rotateSprite       = NULL;
    _textureSprite      = NULL;

    _targetTexture=NULL;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MapSprite_New::LoadMapSprite), st_datamanager_dynamicLoadTexture, NULL);
}

MapSprite_New::~MapSprite_New()
{
   auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
    
    NotificationCenter::getInstance()->removeObserver(this, st_datamanager_dynamicLoadTexture);
}


void MapSprite_New::display()
{
    if(m_sMapSpriteName.empty())
    {
        this->setImageName(_galleryInfo->ID, _galleryInfo->galleryname);
        
        Color3B lColor = DataManager::getInstance()->getSaveBackColor(_galleryInfo->galleryname, _galleryInfo->ID);
        if (lColor.r == 0 && lColor.g == 0 && lColor.b == 0)
        {
            lColor = Color3B::WHITE;
            ColorData lColorData;
            lColorData.R = lColor.r;
            lColorData.G = lColor.g;
            lColorData.B = lColor.b;
            DataManager::getInstance()->saveBackColor(_galleryInfo->galleryname, _galleryInfo->ID, lColorData);
        }
        
        LayerColor* lLayerColor = LayerColor::create(Color4B(lColor.r, lColor.g, lColor.b, 255));
        lLayerColor->setOpacity(0);
        lLayerColor->setContentSize(this->getContentSize());
        lLayerColor->setScale(0.85f);
        this->addChild(lLayerColor);
        lLayerColor->runAction(FadeIn::create(0.3f));
        
        Texture2D* lTexture2D=Director::getInstance()->getTextureCache()->getTextureForKey(this->getIamgeName());
        if(lTexture2D!=NULL)
        {
            this->imageLoadedCallback(lTexture2D);
            _targetTexture=lTexture2D;
        }
        else
        {
            DataManager::getInstance()->m_vNeedLoadImage.push_back(this->getIamgeName());
            DataManager::getInstance()->startLoadImage();
//            DataManager::getInstance()->dynamicLoadTexture(this->getIamgeName());
            
           //Director::getInstance()->getTextureCache()->addImageAsync(this->getIamgeName(), CC_CALLBACK_1(MapSprite_New::imageLoadedCallback, this));
        }
        
    }
}

void MapSprite_New::imageLoadedCallback(Ref* pSender)
{
    Texture2D* lTexture=(Texture2D*)pSender;
    Sprite* lSprite=Sprite::createWithTexture(lTexture);
    lSprite->setScale(0.5f);

    //将Texture绘制成小图显示在界面上，减少内存
    RenderTexture* lRenderTexture = RenderTexture::create(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2, Texture2D::PixelFormat::RGBA8888);
    //lRenderTexture->setClearColor(Color4F(1, 1, 1, 1));
    
    lRenderTexture->begin();
    lSprite->setPosition(Vec2(lSprite->getContentSize().width/4, lSprite->getContentSize().width/4));
    lSprite->visit();
    lRenderTexture->end();
    
    Sprite* lRenderSprite = Sprite::createWithTexture(lRenderTexture->getSprite()->getTexture());
    lRenderSprite->getTexture()->setAntiAliasTexParameters();
    
    
    Director::getInstance()->getTextureCache()->removeTexture(lTexture);
    
    if(lRenderSprite!=NULL)
    {
        if(!DataManager::stopLoadThread)
        {
            lRenderSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
            //            lRenderSprite->setScale(2.0f);
            lRenderSprite->setOpacity(0);
            lRenderSprite->setFlippedY(true);
            this->addChild(lRenderSprite);
            lRenderSprite->runAction(FadeIn::create(0.3f));

            _textureSprite=lRenderSprite;
            
            if(_rotateSprite!=NULL)
            {
                _rotateSprite->removeFromParentAndCleanup(true);
                _rotateSprite=NULL;
            }
            
        }
    }
}

void MapSprite_New::LoadMapSprite(Ref* pSender)
{
    if(_targetTexture==NULL)
    {
       Texture2D* lTexture2D=Director::getInstance()->getTextureCache()->getTextureForKey(this->getIamgeName());
       if(lTexture2D!=NULL)
       {
           this->imageLoadedCallback(lTexture2D);
       }
    }
}


void MapSprite_New::addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow /* = false */)
{
    _selectedCallBack = callBackFun;
    _target = target;
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MapSprite_New::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MapSprite_New::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MapSprite_New::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(MapSprite_New::onTouchCancelled, this);
    listener->setSwallowTouches(isSwallow);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

bool MapSprite_New::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    m_vFirstPoint = lPos;
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    if (_textureSprite!=NULL && this->boundingBox().containsPoint(lPos))
    {
        return true;
    }
    
    return false;
}

void MapSprite_New::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}

void MapSprite_New::onTouchEnded(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    
    
    //    lPos = Director::getInstance()->convertToGL(lPos);
    //    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    float distance = sqrt(pow(lPos.x - m_vFirstPoint.x, 2) + pow(lPos.y - m_vFirstPoint.y, 2));
    if (distance < ScreenAdapterHelper::getValue(25))
    {
        //        this->selected(true);
        if (_target && _selectedCallBack)
        {

//            auto dispatcher = Director::getInstance()->getEventDispatcher();
//            dispatcher->removeEventListenersForTarget(this);
            
            if (DataManager::stopLoadThread == false)
            {
                ScrollMapLayer_ScrollView* lParentLayer=dynamic_cast<ScrollMapLayer_ScrollView*>(this->getParent()->getParent()->getParent());
                if(lParentLayer!=NULL)
                {
                    lParentLayer->stopLoading();
                }
                
                (_target->*_selectedCallBack)(this);
            }
            
            
        }
    }
}

void MapSprite_New::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}


string MapSprite_New::getIamgeName()
{
    return m_sMapSpriteName;
}

void MapSprite_New::setImageName(int index, const string& imageName)
{
    stringstream ss;
    ss<<"";
    ss<<imageName;
    ss<<index;
    ss<<"_white.png";
    
    string fileName = ss.str();
    
    m_sMapSpriteName = fileName;
    
    std::string fullPath = FileUtils::getInstance()->getWritablePath() + fileName;
    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
        Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
        
        m_sMapSpriteName = fullPath;
    }
    
    
}