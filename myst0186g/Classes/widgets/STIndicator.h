//
//  STIndicator.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-9.
//
//

#ifndef __LVUP002__STIndicator__
#define __LVUP002__STIndicator__

#include <cocos2d.h>

class STIndicator   : public cocos2d::CCLayerColor
{
protected:
    cocos2d::CCSpriteFrame *mp_NormalFrame;
    cocos2d::CCSpriteFrame *mp_SelectedFrame;
    
    cocos2d::CCArray       *mp_a_Dots;
    unsigned               mPreFocusPosition;
    
public:
    static STIndicator* create(const unsigned capacity, cocos2d::CCSpriteFrame *onFrame, cocos2d::CCSpriteFrame *offFrame);
    static STIndicator* create(const unsigned capacity, cocos2d::CCTexture2D *onTexture2D, cocos2d::CCTexture2D *offTexture2D);
    static STIndicator* create(const unsigned capacity, std::string onSpriteFrameName, std::string offSpriteFrameName);
    static STIndicator* create(const unsigned capacity, const char *onFileName, const char *offFileName);
    
    
    STIndicator(cocos2d::CCSpriteFrame *onFrame, cocos2d::CCSpriteFrame *offFrame);
    STIndicator(cocos2d::CCTexture2D *onTexture2D, cocos2d::CCTexture2D *offTexture2D);
    STIndicator(std::string onSpriteFrameName, std::string offSpriteFrameName);
    STIndicator(const char *onFileName, const char *offFileName);
    
    virtual ~STIndicator();
    
    virtual bool initWithCapacity(const unsigned capacity);
    
    virtual void onTurnPage(unsigned curPageIndex);
};

#endif /* defined(__LVUP002__STIndicator__) */
