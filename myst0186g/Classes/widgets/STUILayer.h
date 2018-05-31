//
//  STUILayer.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-24.
//
//

#ifndef __LVUP002__STUILayer__
#define __LVUP002__STUILayer__

#include "cocos2d.h"

class STUILayer : public cocos2d::CCLayerColor
{
public:
    static STUILayer* create();
    
    /** creates a CCLayer with color, width and height in Points */
    static STUILayer * create(const cocos2d::ccColor4B& color, GLfloat width, GLfloat height);
    /** creates a CCLayer with color. Width and height are the window size. */
    static STUILayer * create(const cocos2d::ccColor4B& color);
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color, GLfloat width, GLfloat height);
    
    virtual ~STUILayer(){}
};

#endif /* defined(__LVUP002__STUILayer__) */
