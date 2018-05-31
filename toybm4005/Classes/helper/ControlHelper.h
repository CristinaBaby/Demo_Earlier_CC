//
//  ControlHelper.h
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#ifndef __HIDD__ControlHelper__
#define __HIDD__ControlHelper__

#include <stdio.h>
#include "../commpoent/STMenuItemSprite.h"
USING_NS_CC;

class ControlHelper {
    
    
public:
    static cocos2d::Sprite* AddSprite(const char* pFileName,cocos2d::Node* pParentNode, int pZorder=0, int pTag=0);
    static cocos2d::Sprite* addSpriteWithFrame(const char* pFrameName,cocos2d::Node* pParentNode, int pZorder=0, int pTag=0);
    static cocos2d::MenuItemSprite* menuItemSprite(const char* pName);
    static cocos2d::MenuItemSprite* menuItemSprite(const char* pName,const char* pSelectName);
    static cocos2d::MenuItemSprite* menuItemSpriteWithFrameName(const char* pName,const char* pSelectName);
    
    static cocos2d::MenuItemSprite* getMenuItem(const char* pImageName,int pTag,const cocos2d::ccMenuCallback& callback);
    static cocos2d::MenuItemSprite* getMenuItem(const char* pImageName,const char* pSelectName,int pTag,const cocos2d::ccMenuCallback& callback);
    static cocos2d::MenuItemSprite* getMenuItemWithFrameName(const char* pImageName,const char* pSelectName,int pTag,const cocos2d::ccMenuCallback& callback);
    
    
    static STMenuItemSprite* menuItemSpriteWithFrameName_NoSameSize(const char* pName,const char* pSelectName);
    static STMenuItemSprite* getMenuItemWithFrameName_NoSameSize(const char* pImageName,const char* pSelectName,int pTag,const cocos2d::ccMenuCallback& callback);
    
    static cocos2d::Sprite* capture(cocos2d::Node *node);
    static cocos2d::Label* getLabel(const char* font,const char* text,int fontsize,const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT,TextVAlignment vAlignment = TextVAlignment::TOP);
};


#endif /* defined(__HIDD__ControlHelper__) */
