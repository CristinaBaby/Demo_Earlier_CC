//
//  GallerySprite.cpp
//  IMG1001
//
//  Created by houhuang on 15/9/24.
//
//

#include "GallerySprite.h"
#include "../helper/ScreenAdapterHelper.h"
#include "MapSprite_New.h"

GallerySprite_New* GallerySprite_New::create(GalleryInfo* glInfo)
{
    GallerySprite_New* lNode = new GallerySprite_New();
    if (lNode && lNode->init(glInfo))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;
}

GallerySprite_New::GallerySprite_New()
{
    m_gGalleryInfo = NULL;
}

GallerySprite_New::~GallerySprite_New()
{
    this->setTouchEnable(false);
}

bool GallerySprite_New::init(GalleryInfo* glInfo)
{
    if (!Node::init()) return false;
    
    m_gGalleryInfo = glInfo;
    _index = m_gGalleryInfo->ID;
    
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    Sprite* border = Sprite::create("frame2.png");
    this->addChild(border, -1);
    this->setContentSize(border->getContentSize());
    border->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    border->setOpacity(0);
    border->runAction(FadeIn::create(0.5f));

    Sprite* lGallery = Sprite::create(m_gGalleryInfo->image);
    lGallery->setPosition(Vec2(this->getContentSize().width/2 + ScreenAdapterHelper::getValue(6), this->getContentSize().height/2 + ScreenAdapterHelper::getValue(29)));
    this->addChild(lGallery);
    lGallery->setOpacity(0);
    lGallery->runAction(FadeIn::create(0.5f));
    
    
    
    return true;
}

bool GallerySprite_New::init()
{
    if (!Node::init()) return false;
    
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    Sprite* border = Sprite::create("frame2.png");
    this->addChild(border, -1);
    this->setContentSize(border->getContentSize());
    border->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    border->setOpacity(0);
    border->runAction(FadeIn::create(0.5f));
    
    
    Color3B lColor = DataManager::getInstance()->getSaveBackColor(DM_getInstance->m_vMyArtWorksImageName.at(DM_getInstance->m_vMyArtWorksImageName.size()-1), DM_getInstance->m_vMyArtWorksImageNumber.at(DM_getInstance->m_vMyArtWorksImageNumber.size()-1));
    if (lColor.r == 0 && lColor.g == 0 && lColor.b == 0)
    {
        lColor = Color3B::WHITE;
        ColorData lColorData;
        lColorData.R = lColor.r;
        lColorData.G = lColor.g;
        lColorData.B = lColor.b;
        DataManager::getInstance()->saveBackColor(DM_getInstance->m_vMyArtWorksImageName.at(DM_getInstance->m_vMyArtWorksImageName.size()-1), DM_getInstance->m_vMyArtWorksImageNumber.at(DM_getInstance->m_vMyArtWorksImageNumber.size()-1), lColorData);
    }
    
    LayerColor* lLayerColor = LayerColor::create(Color4B(lColor.r, lColor.g, lColor.b, 255));
    lLayerColor->setOpacity(0);
    lLayerColor->ignoreAnchorPointForPosition(false);
    lLayerColor->setAnchorPoint(Vec2(0.5, 0.5));
    lLayerColor->setPosition(Vec2(this->getContentSize().width/2 + ScreenAdapterHelper::getValue(6), this->getContentSize().height/2 + ScreenAdapterHelper::getValue(29)));
    lLayerColor->setContentSize(Size(ScreenAdapterHelper::getValue(455), ScreenAdapterHelper::getValue(455)));
    this->addChild(lLayerColor);
    lLayerColor->runAction(FadeIn::create(0.3f));
    
//    this->scheduleOnce(schedule_selector(GallerySprite_New::showFinish), 0.5f);
    
    Sprite* lGallery = Sprite::create(this->getImageName());
    lGallery->setPosition(Vec2(lLayerColor->getContentSize().width/2, lLayerColor->getContentSize().height/2));
    lGallery->setScale(1/2.251);
    lLayerColor->addChild(lGallery);
    lGallery->setOpacity(0);
    
    lGallery->runAction(FadeIn::create(0.5f));
    
    return true;
}

void GallerySprite_New::showFinish(float dt)
{
    this->setTouchEnable(true);
}

string GallerySprite_New::getImageName()
{
    stringstream ss;
    ss<<"";
    ss<<DM_getInstance->m_vMyArtWorksImageName.at(DM_getInstance->m_vMyArtWorksImageName.size()-1);
    ss<<DM_getInstance->m_vMyArtWorksImageNumber.at(DM_getInstance->m_vMyArtWorksImageNumber.size()-1);;
    ss<<"_white.png";
    
    string fileName = ss.str();
    string m_sImageName = fileName;
    
    string fullPath = FileUtils::getInstance()->getWritablePath() + fileName;
    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
        Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
        m_sImageName = fullPath;
    }
    
    return m_sImageName;
}

void GallerySprite_New::addListener(SEL_CallFuncN callBackFun, Node* target, bool isSwallow /* = false */)
{
    _selectedCallBack = callBackFun;
    _target = target;
    
}

void GallerySprite_New::setTouchEnable(bool pBool)
{
   if(pBool)
   {
       auto dispatcher = Director::getInstance()->getEventDispatcher();
       auto listener = EventListenerTouchOneByOne::create();
       listener->onTouchBegan = CC_CALLBACK_2(GallerySprite_New::onTouchBegan, this);
       listener->onTouchMoved = CC_CALLBACK_2(GallerySprite_New::onTouchMoved, this);
       listener->onTouchEnded = CC_CALLBACK_2(GallerySprite_New::onTouchEnded, this);
       listener->onTouchCancelled = CC_CALLBACK_2(GallerySprite_New::onTouchCancelled, this);
       listener->setSwallowTouches(false);
       dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
   }
   else{
      auto dispatcher = Director::getInstance()->getEventDispatcher();
       dispatcher->removeEventListenersForTarget(this);
   }
}

bool GallerySprite_New::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    m_vFirstPoint = lPos;
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    
    if (this->boundingBox().containsPoint(lPos))
    {
        return true;
    }
    
    return false;
}

void GallerySprite_New::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}

void GallerySprite_New::onTouchEnded(Touch *touch, Event *unused_event)
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

void GallerySprite_New::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}