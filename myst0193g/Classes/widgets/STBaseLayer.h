//
//  STBaseLayer.h
//  CHUPS003
//
//  Created by Steven.Xc.Tian on 13-10-31.
//
//

#ifndef __CHUPS003__STBaseLayer__
#define __CHUPS003__STBaseLayer__

#include <cocos2d.h>
#include "STBatchNodeLayer.h"
#include "STUILayer.h"

class STBaseLayer : public cocos2d::CCLayer
{
protected:
    STBatchNodeLayer        *mp_ScaledBatchLayer;
    STUILayer               *mp_ScaledUILayer;
    
    
public:
    STBaseLayer();
    virtual ~STBaseLayer();
    
    
    CREATE_FUNC(STBaseLayer);
};

#endif /* defined(__CHUPS003__STBaseLayer__) */
