//
//  AdLoadingLayerDoll.h
//  AdsDemo
//
//  Created by zengwenbin on 15/8/31.
//
//

#ifndef __AdsDemo__AdLoadingLayerDoll__
#define __AdsDemo__AdLoadingLayerDoll__

#include "AdLoadingLayerBase.h"

class AdLoadingLayerDoll:public AdLoadingLayerBase{
    
    Sprite* _loadSprite;
    
public:
    virtual bool init();
    
    CREATE_FUNC(AdLoadingLayerDoll);
    
    void updateLayer(Ad_Interstitial_Type type);
    
};

#endif /* defined(__AdsDemo__AdLoadingLayerDoll__) */
