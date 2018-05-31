//
//  MapSmallSprite.cpp
//  IMG1001
//
//  Created by houhuang on 15/11/12.
//
//

#include "MapSmallSprite.hpp"
#include "../define/Globalmacro.h"
#include "../helper/ScreenAdapterHelper.h"

bool MapSmallSprite::init()
{
    if(!Node::init()) return false;

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
    m_pRotateSprite=lRotateSprite;
    
    return true;
}

MapSmallSprite::MapSmallSprite()
{
    m_sMapSpriteName    = "";
    m_vFirstPoint       = Vec2(0, 0);
    m_pRotateSprite     = NULL;
    
    _selectedCallBack   = NULL;
    _target             = NULL;
    m_pTextureSpite     = NULL;
    m_pTexture2D        = NULL;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MapSmallSprite::LoadMapSprite), st_datamanager_dynamicLoadTexture, NULL);
}

MapSmallSprite::~MapSmallSprite()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
    
    NotificationCenter::getInstance()->removeObserver(this, st_datamanager_dynamicLoadTexture);
}


void MapSmallSprite::display()
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
            m_pTexture2D=lTexture2D;
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

void MapSmallSprite::imageLoadedCallback(Ref* pSender)
{
    Texture2D* lTexture=(Texture2D*)pSender;
    Sprite* lSprite=Sprite::createWithTexture(lTexture);
    if(lSprite!=NULL)
    {
        if(!DataManager::stopLoadThread)
        {
            lSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
            lSprite->setScale(0.5f);
            lSprite->setOpacity(0);
            this->addChild(lSprite);
            lSprite->runAction(FadeIn::create(0.3f));
            
            m_pTextureSpite=lSprite;
            
            if(m_pRotateSprite!=NULL)
            {
                m_pRotateSprite->removeFromParentAndCleanup(true);
                m_pRotateSprite=NULL;
            }
            
        }
    }
}

void MapSmallSprite::LoadMapSprite(Ref* pSender)
{
    if(m_pTexture2D==NULL)
    {
        Texture2D* lTexture2D=Director::getInstance()->getTextureCache()->getTextureForKey(this->getIamgeName());
        if(lTexture2D!=NULL)
        {
            this->imageLoadedCallback(lTexture2D);
        }
    }
}


void MapSmallSprite::addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow /* = false */)
{
    _selectedCallBack = callBackFun;
    _target = target;
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MapSmallSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MapSmallSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MapSmallSprite::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(MapSmallSprite::onTouchCancelled, this);
    listener->setSwallowTouches(isSwallow);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
}

bool MapSmallSprite::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    m_vFirstPoint = lPos;
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    if (m_pTextureSpite!=NULL && this->boundingBox().containsPoint(lPos))
    {
        return true;
    }
    
    return false;
}

void MapSmallSprite::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}

void MapSmallSprite::onTouchEnded(Touch *touch, Event *unused_event)
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
                (_target->*_selectedCallBack)(this);
            }
            
            
        }
    }
}

void MapSmallSprite::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}


string MapSmallSprite::getIamgeName()
{
    return m_sMapSpriteName;
}

void MapSmallSprite::setImageName(int index, const string& imageName)
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