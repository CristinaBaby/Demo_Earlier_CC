//
//  CurrentImageSprite.cpp
//  ColorFree
//
//  Created by houhuang on 15/9/8.
//
//

#include "CurrentImageSprite.h"
#include "../data/DataManager.h"
#include "../define/Globalmacro.h"

bool CurrentImageSprite::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255), 1024, 1024)) return false;
    
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    Sprite* lSprite = Sprite::create(this->getImageName_w());
    lSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(lSprite);
    
    return true;
}

std::string CurrentImageSprite::getImageName_w()
{
    stringstream ss;
    ss<<DataManager::getInstance()->m_pCurrentImage.imageName;
    ss<<DataManager::getInstance()->m_pCurrentImage.ID;
    ss<<"_white.png";
    
    string imageName = ss.str();
    
    string fullPath = FileUtils::getInstance()->getWritablePath() + imageName;
    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
        Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
        return fullPath;
    }
    
    return imageName;
}