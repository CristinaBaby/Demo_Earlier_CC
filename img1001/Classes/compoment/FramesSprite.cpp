//
//  FramesSprite.cpp
//  ColorFree
//
//  Created by houhuang on 15/9/8.
//
//

#include "FramesSprite.h"
#include "../data/DataManager.h"
#include "../define/Globalmacro.h"
#include "../helper/ScreenAdapterHelper.h"

FramesSprite* FramesSprite::create(string framesName)
{
    FramesSprite* lSprite = new FramesSprite();
    if (lSprite && lSprite->init(framesName))
    {
        lSprite->autorelease();
        return lSprite;
    }
    
    CC_SAFE_DELETE(lSprite);
    return NULL;
}

FramesSprite* FramesSprite::create()
{
    FramesSprite* lSprite = new FramesSprite();
    if (lSprite && lSprite->init())
    {
        lSprite->autorelease();
        return lSprite;
    }
    
    CC_SAFE_DELETE(lSprite);
    return NULL;
}

bool FramesSprite::init()
{
    
    Color3B color = DM_getSaveBackColor;
    
    if (!LayerColor::initWithColor(Color4B(color.r, color.g, color.b, 255), ScreenAdapterHelper::getValue(1024), ScreenAdapterHelper::getValue(1024))) return false;
    
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    Sprite* lSprite = Sprite::create(this->getImageName_w());
    lSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(lSprite);
    
    
    return true;
}

bool FramesSprite::init(string framesName)
{
    Color3B color = DM_getSaveBackColor;
    
    if (!LayerColor::initWithColor(Color4B(color.r, color.g, color.b, 255), ScreenAdapterHelper::getValue(1024), ScreenAdapterHelper::getValue(1024))) return false;
    
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    Sprite* lSprite = Sprite::create(this->getImageName_w());
    lSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(lSprite);
    
    Sprite* frame = Sprite::create(framesName);
    frame->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(frame);
    frame->setScale(1.4);


    return true;
}

string FramesSprite::getImageName_w()
{
    stringstream ss;
    ss<<"";
    ss<<DataManager::getInstance()->m_pCurrentImage.imageName;
    ss<<DataManager::getInstance()->m_pCurrentImage.ID;
    ss<<"_white.png";
    
    string str = ss.str();
    string fullPath = FileUtils::getInstance()->getWritablePath() + str;
    
    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
        Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
        return fullPath;
    }
    
    return str;
    
}

