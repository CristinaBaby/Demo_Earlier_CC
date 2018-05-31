//
//  MapSprite.cpp
//  HH002
//
//  Created by houhuang on 15/8/27.
//
//

#include "MapSprite.h"
#include "../data/DataManager.h"

MapSprite* MapSprite::create(int index, const string& imageName)
{
    MapSprite* lNode = new MapSprite();
    if (lNode && lNode->init(index, imageName))
    {
        lNode->autorelease();
        return lNode;
    }
    CC_SAFE_DELETE(lNode);
    return NULL;
}

bool MapSprite::init(int index, const string& imageName)
{
    if (!Node::init())
    {
        return false;
    }
    
    _index = index;
    
    this->setImageName(index, imageName);
    
    Color3B lColor = DataManager::getInstance()->getSaveBackColor(imageName, index);
    LayerColor* lLayerColor = LayerColor::create(Color4B(lColor.r, lColor.g, lColor.b, 255));
    this->addChild(lLayerColor);
    
    Sprite* lSprite = Sprite::create(this->getIamgeName());
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setContentSize(lSprite->getContentSize());
    lLayerColor->setContentSize(this->getContentSize());
    
    lSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(lSprite);

    return true;
}

string MapSprite::getIamgeName()
{
    return m_sMapSpriteName;
}

void MapSprite::setImageName(int index, const string& imageName)
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


