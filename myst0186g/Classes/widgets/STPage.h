//
//  STPage.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-9.
//
//

#ifndef __LVUP002__STPage__
#define __LVUP002__STPage__

#include <cocos2d.h>
#include "cocos-ext.h"


class ItemCell : public cocos2d::CCObject
{
protected:
    const unsigned mIdx;
    
public:
    ItemCell(unsigned idx)
    : mIdx(idx)
    {
        this->autorelease();
    }
    virtual ~ItemCell(){};
    
    const unsigned getItemCellIndex(){ return mIdx; }
};



class STPage : public cocos2d::extension::CCTableViewCell
{
protected:
    cocos2d::CCLayerColor* pBaseLayer;
    
public:
    STPage();
    virtual ~STPage();
    
    static STPage * create();
    /** creates a CCLayer with color. Width and height are the window size. */
    static STPage * create(const cocos2d::ccColor4B& color);
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color, GLfloat width, GLfloat height);
    
public:
    /**
     *  when reuse this page item, we need change its content.
     *
     *  @param  idx the page number
     */
    virtual void refreshPage(unsigned idx){};
    
    virtual void reset(){};
    
    virtual CCNode* onClickedPage(cocos2d::CCTouch *pTouch){ return NULL; };
};

#endif /* defined(__LVUP002__STPage__) */
