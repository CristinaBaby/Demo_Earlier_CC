//
//  STMenuItemSprite.cpp
//  HIDD
//
//  Created by lidong on 15-4-9.
//
//

#include "STMenuItemSprite.h"

const unsigned int    kNormalTag = 0x1;
const unsigned int    kSelectedTag = 0x2;
const unsigned int    kDisableTag = 0x3;



STMenuItemSprite * STMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite)
{
    return STMenuItemSprite::create(normalSprite, selectedSprite, disabledSprite, (const ccMenuCallback&)nullptr);
}

STMenuItemSprite * STMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, const ccMenuCallback& callback)
{
    return STMenuItemSprite::create(normalSprite, selectedSprite, nullptr, callback);
}
STMenuItemSprite * STMenuItemSprite::create(Node *normalSprite, Node *selectedSprite, Node *disabledSprite, const ccMenuCallback& callback)
{
    STMenuItemSprite *ret = new STMenuItemSprite();
    ret->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
    ret->autorelease();
    return ret;
}


bool STMenuItemSprite::initWithNormalSprite(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback)
{
    MenuItem::initWithCallback(callback);
    
    float lNormalWidth=0;
    float lNormalHeight=0;
    if(normalSprite!=NULL)
    {
        lNormalWidth=normalSprite->getContentSize().width;
        lNormalHeight=normalSprite->getContentSize().height;
    }
    float lSelectedWidth=0;
    float lSelectedHeight=0;
    if(selectedSprite!=NULL)
    {
        lSelectedWidth=selectedSprite->getContentSize().width;
        lSelectedHeight=selectedSprite->getContentSize().height;
    }
    
    float lMaxWidth=MAX(lNormalWidth, lSelectedWidth);
    float lMaxHeight=MAX(lSelectedWidth, lSelectedHeight);
    
    this->setContentSize(Size(lMaxWidth, lMaxHeight));
    
    setNormalImage(normalSprite);
    setSelectedImage(selectedSprite);
    setDisabledImage(disabledSprite);
    

    
    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);
    
    return true;
}


void STMenuItemSprite::setNormalImage(Node* image)
{
    if (image != _normalImage)
    {
        if (image)
        {
            addChild(image, 0, kNormalTag);
            image->setAnchorPoint(Vec2(0.5f, 0.5f));
        }
        
        if (_normalImage)
        {
            removeChild(_normalImage, true);
        }
        
        _normalImage = image;
        this->setContentSize(_normalImage->getContentSize());
        image->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
        this->updateImagesVisibility();
    }
}

void STMenuItemSprite::setSelectedImage(Node* image)
{
    if (image != _normalImage)
    {
        if (image)
        {
            addChild(image, 0, kSelectedTag);
            image->setAnchorPoint(Vec2(0.5f, 0.5f));
        }
        
        if (_selectedImage)
        {
            removeChild(_selectedImage, true);
        }
        
        _selectedImage = image;
        _selectedImage->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
        this->updateImagesVisibility();
    }
}

void STMenuItemSprite::setDisabledImage(Node* image)
{
    if (image != _normalImage)
    {
        if (image)
        {
            addChild(image, 0, kDisableTag);
            image->setAnchorPoint(Vec2(0.5f, 0.5f));
        }
        
        if (_disabledImage)
        {
            removeChild(_disabledImage, true);
        }

        _disabledImage = image;
        if(_disabledImage)
        {
            _disabledImage->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
        }
        this->updateImagesVisibility();
    }
}
