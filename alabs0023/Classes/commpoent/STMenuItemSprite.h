//
//  STMenuItemSprite.h
//  HIDD
//
//  Created by lidong on 15-4-9.
//
//

#ifndef __HIDD__STMenuItemSprite__
#define __HIDD__STMenuItemSprite__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class STMenuItemSprite : public MenuItemSprite {
    
    
public:
     static STMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite = nullptr);
     static STMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, const ccMenuCallback& callback);
    /** creates a menu item with a normal,selected  and disabled image with target/selector */
    static STMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback);
public:
     bool initWithNormalSprite(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback);
public:
    /** Gets the image used when the item is not selected */
    inline Node* getNormalImage() const { return _normalImage; };
    
    /** Sets the image used when the item is not selected */
    void setNormalImage(Node* image);
    
    /** Gets the image used when the item is selected */
    inline Node* getSelectedImage() const { return _selectedImage; };
    
    /** Sets the image used when the item is selected */
    void setSelectedImage(Node* image);
    
    /** Gets the image used when the item is disabled */
    inline Node* getDisabledImage() const { return _disabledImage; };
    
    /** Sets the image used when the item is disabled */
    void setDisabledImage(Node* image);
    
};


#endif /* defined(__HIDD__STMenuItemSprite__) */
