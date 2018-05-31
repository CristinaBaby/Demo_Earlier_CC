//
//  LoadingScene.h
//  LVUP004
//  Created by Steven.Xc.Tian on 13-11-18.
//
//
//

#ifndef __LVUP004__LoadingScene__
#define __LVUP004__LoadingScene__

#include <cocos2d.h>

class LoadingLayer : public cocos2d::CCLayer
{
    cocos2d::CCProgressTimer* m_pProgress;
public:
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(LoadingLayer);
    
    virtual ~LoadingLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
private:
    // call this when loading scene is finish all tasks.
    void onLoadFinish();
    // load resources step by step.
    void load();
    //
    void loadMapInfos();
    
    void loadSKU();
};

#endif /* defined(__LVUP004__LoadingScene__) */
