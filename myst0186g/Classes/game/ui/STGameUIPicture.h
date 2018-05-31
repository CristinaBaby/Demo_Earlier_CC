//
//  STGameUIPicture.h
//  LVUP004
//
//  Created by Steven.Xc.Tian on 13-11-18.
//
//

#ifndef __LVUP004__STGameUIPicture__
#define __LVUP004__STGameUIPicture__

#include "../../widgets/STToggleButton.h"

class STGameUIPicture : public STToggleButton
{
    cocos2d::CCSprite *m_pPicture;
    
public:
    static STGameUIPicture* create(std::string bgNormalName, std::string bgHighlightName);
    
    STGameUIPicture(std::string normalName, std::string highlightName);
    virtual ~STGameUIPicture();
    
    // in this case, we don't receive any touch events.
    virtual void setOnClickedListener(CCObject *target, cocos2d::SEL_CallFuncN selector){}
    
    void refreshPicture(cocos2d::CCSpriteFrame *pNewFrame);
};

#endif /* defined(__LVUP004__STGameUIPicture__) */
