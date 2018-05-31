//
//  ColorAdsManager.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__ColorAdsManager__
#define __ColorBook__ColorAdsManager__

#include "UIHeader.h"

#define xColorAds  (ColorAdsManager::getInstance())

class ColorAdsManager : public Node
{
public:
    static ColorAdsManager* getInstance();
    void destroyInstance();
    
    void share();
    void share(bool forSave);
    void back(Scene* popupAdsScene);
    void update(float dt);
    void loadFinish();
    
protected:
    ColorAdsManager();
    
protected:
    CC_SYNTHESIZE(bool, _showed, Showed);
    CC_SYNTHESIZE(bool, _shared, Shared);
    
    Scene* _popupAdsScene = nullptr;
};

#endif /* defined(__ColorBook__ColorAdsManager__) */
