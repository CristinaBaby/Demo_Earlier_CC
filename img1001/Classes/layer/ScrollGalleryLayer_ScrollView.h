//
//  ScrollGalleryLayer_ScrollView.h
//  IMG1001
//
//  Created by houhuang on 15/9/24.
//
//

#ifndef __IMG1001__ScrollGalleryLayer_ScrollView__
#define __IMG1001__ScrollGalleryLayer_ScrollView__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../define/Globalmacro.h"
#include "../data/DataManager.h"
#include "../modules/STInAppBilling.h"
#include "../compoment/GallerySprite.h"
USING_NS_CC;
USING_NS_CC_EXT;

class ScrollGalleryLayer_ScrollView : public Layer
{
public:
    
    CREATE_FUNC(ScrollGalleryLayer_ScrollView);
    bool init();
    ScrollGalleryLayer_ScrollView();
    ~ScrollGalleryLayer_ScrollView();
    void onEnterTransitionDidFinish();
    
    void onExit();
public:
    void initContentLayer();
    void reponseMapSprite(Node* ref);
    void reponseTouchButton(Ref* ref);
    
    void resumeSpriteTouch(float dt);
    
protected:
    ScrollView* m_pScrollView;

    std::vector<GalleryInfo>* m_vGalleryInfo;
    int     m_iGalleryCount;

    STInAppBilling* _stInAppBilling;
    std::vector<GallerySprite_New*> _galleryArrays;
    
    float   m_fOffset;
};

#endif /* defined(__IMG1001__ScrollGalleryLayer_ScrollView__) */
