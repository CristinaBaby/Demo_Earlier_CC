//
//  ControlHelper.cpp
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#include "ControlHelper.h"
#include "DeviceHelper.h"
using namespace std;

Sprite* ControlHelper::AddSprite(const char *pFileName, cocos2d::Node *pParentNode,  int pZorder,  int pTag)
{
    Sprite* lSprite=Sprite::create(pFileName);
    pParentNode->addChild(lSprite, pZorder, pTag);
    return lSprite;
}

Sprite* ControlHelper::addSpriteWithFrame(const char *pFrameName, cocos2d::Node *pParentNode,  int pZorder,  int pTag)
{
    SpriteFrame* lFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pFrameName);
    if(lFrame)
    {
        Sprite* lSpr = Sprite::createWithSpriteFrame(lFrame);
        pParentNode->addChild(lSpr, pZorder, pTag);
        return lSpr;
    }
    return NULL;
}

cocos2d::MenuItemSprite* ControlHelper::menuItemSprite(const char* pName)
{
    string str1(pName);
    string str2(pName);
    Sprite* lNormalSprite=Sprite::create(str1.c_str());
    Sprite* lSelectSprite=Sprite::create(str2.c_str());
    lSelectSprite->setColor(Color3B::GRAY);
    MenuItemSprite* lMenuItem= MenuItemSprite::create(lNormalSprite, lSelectSprite);
    return lMenuItem;
}
cocos2d::MenuItemSprite* ControlHelper::menuItemSprite(const char* pName,const char* pSelectName)
{
    string str1(pName);
    string str2(pSelectName);
    Sprite* lNormalSprite=Sprite::create(str1.c_str());
    Sprite* lSelectSprite=Sprite::create(str2.c_str());
    MenuItemSprite* lMenuItem= MenuItemSprite::create(lNormalSprite, lSelectSprite);
    return lMenuItem;
}
cocos2d::MenuItemSprite* ControlHelper::menuItemSpriteWithFrameName(const char* pName,const char* pSelectName)
{
    string str1(pName);
    string str2(pSelectName);
    Sprite* lNormalSprite=Sprite::createWithSpriteFrameName(str1.c_str());
    Sprite* lSelectSprite=Sprite::createWithSpriteFrameName(str2.c_str());
    MenuItemSprite* lMenuItem= MenuItemSprite::create(lNormalSprite, lSelectSprite);
    return lMenuItem;
}

STMenuItemSprite* ControlHelper::menuItemSpriteWithFrameName_NoSameSize(const char* pName,const char* pSelectName)
{
    string str1(pName);
    string str2(pSelectName);
    Sprite* lNormalSprite=Sprite::createWithSpriteFrameName(str1.c_str());
    Sprite* lSelectSprite=Sprite::createWithSpriteFrameName(str2.c_str());
    STMenuItemSprite* lMenuItem= STMenuItemSprite::create(lNormalSprite, lSelectSprite);
    return lMenuItem;
}

cocos2d::MenuItemSprite* ControlHelper::getMenuItem(const char *pImageName, int pTag,const cocos2d::ccMenuCallback& callback)
{
    MenuItemSprite* lMenuItem=ControlHelper::menuItemSprite(pImageName);
    lMenuItem->setTag(pTag);
    lMenuItem->setCallback(callback);
    return lMenuItem;
}
cocos2d::MenuItemSprite* ControlHelper::getMenuItem(const char* pImageName,const char* pSelectName,int pTag,const cocos2d::ccMenuCallback& callback)
{
    MenuItemSprite* lMenuItem=ControlHelper::menuItemSprite(pImageName,pSelectName);
    lMenuItem->setCallback(callback);
    lMenuItem->setTag(pTag);
    return lMenuItem;
}

cocos2d::MenuItemSprite* ControlHelper::getMenuItemWithFrameName(const char* pImageName,const char* pSelectName,int pTag,const cocos2d::ccMenuCallback& callback)
{
    MenuItemSprite* lMenuItem=ControlHelper::menuItemSpriteWithFrameName(pImageName, pSelectName);
    lMenuItem->setTag(pTag);
    lMenuItem->setCallback(callback);
    return lMenuItem;
}

STMenuItemSprite* ControlHelper::getMenuItemWithFrameName_NoSameSize(const char* pImageName,const char* pSelectName,int pTag,const cocos2d::ccMenuCallback& callback)
{
    STMenuItemSprite* lMenuItem=ControlHelper::menuItemSpriteWithFrameName_NoSameSize(pImageName, pSelectName);
    lMenuItem->setTag(pTag);
    lMenuItem->setCallback(callback);
    return lMenuItem;
}

cocos2d::Label* ControlHelper::getLabel(const char *font, const char *text,int fontsize,const Size& dimensions, TextHAlignment hAlignment , TextVAlignment vAlignment)
{
    if(DeviceHelper::getInstance()->getIs2048Device())
    {
        fontsize=fontsize*2;
    }
    Label* label=Label::createWithTTF(text,font, fontsize,dimensions,hAlignment,vAlignment);
    return label;
}
