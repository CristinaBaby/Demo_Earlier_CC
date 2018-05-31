//
//  MyArtworksSprite.cpp
//  IMG1001
//
//  Created by houhuang on 15/11/17.
//
//

#include "MyArtworksSprite.hpp"
#include "../helper/ScreenAdapterHelper.h"
#include "../helper/DeviceHelper.h"

MyArtworksSprite::~MyArtworksSprite()
{
    NotificationCenter::getInstance()->removeObserver(this, st_datamanager_dynamicLoadTexture);
    this->removeListener();
//    NotificationCenter::getInstance()->removeObserver(this, st_MyArtworks_deleteBtn_ShowOrHide);
}

MyArtworksSprite::MyArtworksSprite()
{
    m_sImageName_post = "";
    m_sImageName = "";
    m_pRotateSprite = NULL;
    m_pTextureSprite = NULL;
    m_pTargetTexture = NULL;
    m_vFirstPoint = Vec2(0, 0);
    m_bIsHide = true;
    m_pLayerColor = NULL;
    m_bIsPost = false;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MyArtworksSprite::LoadMapSprite), st_datamanager_dynamicLoadTexture, NULL);

//    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MyArtworksSprite::LoadMapSprite), st_MyArtworks_deleteBtn_ShowOrHide, NULL);
}

MyArtworksSprite* MyArtworksSprite::create(int imageNumber, const string& imageName)
{
    MyArtworksSprite* lSprite = new MyArtworksSprite();
    if (lSprite && lSprite->init(imageNumber, imageName))
    {
        lSprite->autorelease();
        return lSprite;
    }
    
    CC_SAFE_DELETE(lSprite);
    return NULL;
}

bool MyArtworksSprite::init(int imageNumber, const string& imageName)
{
    if (!Node::init()) return false;
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    m_sGalleryName = imageName;
    m_iImageNumber = imageNumber;
    
    Sprite* border = Sprite::create("map_frame.png");
    this->addChild(border, -1);
    this->setContentSize(Size(border->getContentSize().width, border->getContentSize().height));
    border->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    
    
    Color3B color = DataManager::getInstance()->getSaveBackColor(m_sGalleryName, m_iImageNumber);
    if (color.r == 0 && color.g == 0 && color.b == 0)
    {
        color = Color3B::WHITE;
        ColorData lColorData;
        lColorData.R = color.r;
        lColorData.G = color.g;
        lColorData.B = color.b;
        
        DataManager::getInstance()->saveBackColor(m_sGalleryName, m_iImageNumber, lColorData);
    }
    
    LayerColor* lLayerColor = LayerColor::create(Color4B(color.r, color.g, color.b, 255));
    //        lLayerColor->setOpacity(0);
    lLayerColor->setContentSize(this->getContentSize());
    lLayerColor->setScale(0.85f);
    this->addChild(lLayerColor);
    
    return true;
}



void MyArtworksSprite::onEnterTransitionDidFinish()
{
    Node::onEnterTransitionDidFinish();
    
//    this->display();
}

//void MyArtworksSprite::doAction(Ref* ref)
//{
//    
//    DataManager::getInstance()->deleteArtworksImage(m_sGalleryName, m_iImageNumber);
//    this->removeFromParentAndCleanup(true);
//    NotificationCenter::getInstance()->postNotification(st_MyArtworks_deleteImage);
//}


bool MyArtworksSprite::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    m_vFirstPoint = lPos;
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    if (m_pTextureSprite!=NULL && this->boundingBox().containsPoint(lPos))
    {
        return true;
    }
    
    return false;
}

void MyArtworksSprite::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void MyArtworksSprite::onTouchEnded(Touch *touch, Event *unused_event)
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
            (_target->*_selectedCallBack)(this);
        }
    }
}

void MyArtworksSprite::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void MyArtworksSprite::addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow /* = false */)
{
    _target = target;
    _selectedCallBack = callBackFun;
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(isSwallow);
    
    listener->onTouchBegan = CC_CALLBACK_2(MyArtworksSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyArtworksSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyArtworksSprite::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(MyArtworksSprite::onTouchCancelled, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void MyArtworksSprite::removeListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

void MyArtworksSprite::setDeleteBtnVisible(bool pBool)
{
    m_pMenu->setVisible(pBool);
}

void MyArtworksSprite::display()
{
    if (!m_bIsHide)
    {
        return;
    }
    
    m_bIsHide = false;
    
    Sprite* rotateSprite = Sprite::create("load_rotate_icon.png");
    rotateSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(rotateSprite, 10);
    rotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));
    m_pRotateSprite = rotateSprite;
    
//    if (m_sImageName.empty())
    {
        
        this->setImageName(m_iImageNumber, m_sGalleryName);
        
        stringstream ss2;
        ss2<<"";
        ss2<<m_sGalleryName;
        ss2<<m_iImageNumber;
        ss2<<"_small";
        ss2<<"_white.png";
        
        
        Texture2D* lTexture2D = Director::getInstance()->getTextureCache()->getTextureForKey(this->getImageName_fullPath());

        DataManager::getInstance()->m_vNeedLoadImage.push_back(this->getImageName_fullPath());
        DataManager::getInstance()->startLoadImage();
//        DataManager::getInstance()->dynamicLoadTexture(this->getImageName_fullPath());
    }
}


void MyArtworksSprite::hideThis()
{
    if (m_bIsHide)
    {
        return;
    }
    
    m_bIsHide = true;
    if (m_pTextureSprite)
    {
        
        m_pTextureSprite->removeFromParentAndCleanup(true);
        m_pTextureSprite = NULL;
        
        Director::getInstance()->getTextureCache()->removeTextureForKey(this->getImageName_fullPath());
    }
    
    if (m_pRotateSprite)
    {
        m_pRotateSprite->removeFromParentAndCleanup(true);
        m_pRotateSprite = NULL;
    }
    
//    if (m_pLayerColor)
//    {
//        m_pLayerColor->removeFromParentAndCleanup(true);
//        m_pLayerColor = NULL;
//    }
}

void MyArtworksSprite::imageLoadedCallback(Ref* pSender)
{
    Texture2D* lTexture = (Texture2D*)pSender;
    
    Sprite* lRenderSprite = Sprite::createWithTexture(lTexture);
    
     Director::getInstance()->getTextureCache()->removeTexture(lTexture);
    
    if (lRenderSprite != NULL)
    {
        lRenderSprite->getTexture()->setAntiAliasTexParameters();
        lRenderSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
        lRenderSprite->setOpacity(0);
//        lRenderSprite->setFlippedY(true);
        lRenderSprite->runAction(FadeIn::create(0.3f));
        this->addChild(lRenderSprite);
        
        if (DeviceHelper::getInstance()->getIs2048Device() && m_bIsPost)
        {
            lRenderSprite->setScale(2.0f);
        }
        
        m_pTextureSprite = lRenderSprite;
        
        if (m_pRotateSprite)
        {
            m_pRotateSprite->removeFromParentAndCleanup(true);
            m_pRotateSprite = NULL;
        }
    }
}

void MyArtworksSprite::LoadMapSprite(Ref* pSender)
{
    if (m_pTargetTexture == NULL)
    {
        Texture2D* lTexture;
        if (m_bIsPost)
        {
            lTexture = Director::getInstance()->getTextureCache()->getTextureForKey(this->getPostImageFullPath());
        }else
        {
            lTexture = Director::getInstance()->getTextureCache()->getTextureForKey(this->getImageName_fullPath());
        }
        
        if (lTexture != NULL)
        {
            this->imageLoadedCallback(lTexture);
        }
    }
}

string MyArtworksSprite::getImageName_fullPath()
{
    return m_sImageName;
}

int MyArtworksSprite::getImageNumber()
{
    return m_iImageNumber;
}

string MyArtworksSprite::getImageName()
{
    return m_sGalleryName;
}

void MyArtworksSprite::setImageName(int index, const string& imageName)
{
    stringstream ss;
    ss<<"";
    ss<<imageName;
    ss<<index;
    ss<<"_small";
    ss<<"_white.png";
    
    string fileName = ss.str();
    m_sImageName = fileName;
    
    string fullPath = FileUtils::getInstance()->getWritablePath() + fileName;
    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
      //  Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
        m_sImageName = fullPath;
    }
      
}

#pragma mark My Posts

MyArtworksSprite* MyArtworksSprite::create(const string& imageName)
{
    MyArtworksSprite* lSprite = new MyArtworksSprite();
    if (lSprite && lSprite->init(imageName))
    {
        lSprite->autorelease();
        return lSprite;
    }
    
    CC_SAFE_DELETE(lSprite);
    return NULL;
}

bool MyArtworksSprite::init(const string& imageName)
{
    if (!Node::init()) return false;
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    m_bIsPost = true;
    
    m_sImageName_post = imageName;
    _postImageName = imageName;
    
    Sprite* border = Sprite::create("map_frame.png");
    this->addChild(border, -1);
    this->setContentSize(Size(border->getContentSize().width, border->getContentSize().height));
    border->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    
    return true;
}

void MyArtworksSprite::display_post()
{
    if (!m_bIsHide)
    {
        return;
    }
    
    m_bIsHide = false;
    
    Sprite* rotateSprite = Sprite::create("load_rotate_icon.png");
    rotateSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(rotateSprite, 10);
    rotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));
    m_pRotateSprite = rotateSprite;
    
    DataManager::getInstance()->m_vNeedLoadImage.push_back(this->getPostImageFullPath());
    DataManager::getInstance()->startLoadImage();
//    DM_getInstance->dynamicLoadTexture(this->getPostImageFullPath());
    
}

string MyArtworksSprite::getPostImageFullPath()
{
    string fullPath = FileUtils::getInstance()->getWritablePath() + m_sImageName_post;
    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
        //  Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
        m_sImageName_post = fullPath;
    }
    
    return m_sImageName_post;
}