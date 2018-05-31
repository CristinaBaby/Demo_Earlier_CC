//
//  ScrollMapLayer_ScrollView.h
//  IMG1001
//
//  Created by houhuang on 15/9/21.
//
//

#ifndef __IMG1001__ScrollMapLayer_ScrollView__
#define __IMG1001__ScrollMapLayer_ScrollView__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../data/DataManager.h"
#include "../modules/STInAppBilling.h"
#include "../compoment/MapSprite_New.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ScrollMapLayer_ScrollView : public LayerColor
{
public:
    static ScrollMapLayer_ScrollView* create(int index);
    bool init(int index);
    
    ScrollMapLayer_ScrollView();
    ~ScrollMapLayer_ScrollView();
    
    void onEnterTransitionDidFinish();
    void onExit();
    void stopLoading();
public:
    void initContentLayer();
    
    void reponseMapSprite(Node* ref);
    void removeDialog(Node* pSender);
    void unlockMap(Node* pSender);
    
    //接受通知
    void waitNotification(Ref* ref);
    
    void waitDownLoadSucNotification(Ref* ref);
protected:
    ScrollView* m_pScrollView;
    int         m_iMapCellCount;
    string      m_sGalleryName;
    
    std::vector<GalleryInfo>* m_vGalleryInfo;
    std::vector<MapSprite_New*> m_vMapSprite;

    STInAppBilling* _stInAppBilling;
};

#endif /* defined(__IMG1001__ScrollMapLayer_ScrollView__) */
