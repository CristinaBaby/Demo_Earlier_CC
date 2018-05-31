//
//  GuideScene.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-26.
//
//

#ifndef __LVUP002__GuideScene__
#define __LVUP002__GuideScene__

#include <cocos2d.h>
#include "../widgets/STUILayer.h"

NS_CC_BEGIN

class GuideScene : public CCScene
{
    STUILayer* pUILayer;
    SEL_SCHEDULE selector;
//    cocos2d::CCArray *pAllSprites;
public:
    
    CREATE_FUNC(GuideScene);
    
    GuideScene();
    virtual ~GuideScene();
    
    virtual bool init();
    
private:
    void onSkipClicked(CCObject* obj = NULL);
    /**
     * play the next picture of guide
     */
    void nextFrame(float delta = 0.f);
};

NS_CC_END

#endif /* defined(__LVUP002__GuideScene__) */
