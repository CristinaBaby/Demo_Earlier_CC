#include "AppDelegate.h"
#include "HelloWorldScene.h"


#include "hidd/Scene/LoadingScene.h"
#include "hidd/helper/ResourceHelper.h"
#include "hidd/helper/ScreenAdapterHelper.h"

#include "define/GlobalMacro.h"
#include "hidd/helper/AudioHelper.h"
#include "hidd/helper/DeviceHelper.h"
#include "hidd/helper/AdHelper.h"

#include "SimpleAudioEngine.h"
#include "hidd/scene/GameScene.h"
#include "hidd/modules/AnalyticX.h"
#include "hidd/data/DataManager.h"
#include "hidd/modules/AdLoadingLayerBase.h"
#include "hidd/layer/AdsLoadingLayer.h"
USING_NS_CC;
using namespace CocosDenshion;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}




bool AppDelegate::applicationDidFinishLaunching() {
    
     AudioHelper::setup(1.0f, 1.0f);
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
 
    // turn on display FPS
    director->setDisplayStats(false);


    //设置资源目录结构
    ResourceHelper::getInstance()->setupResourceDir();

    
    //设置屏幕尺寸匹配 对于屏幕适配的坐标都要通过ScreenAdapterHelper设置了
    Size lDesignSize(Vec2(st_design_screen_width, st_design_screen_height));
    if(DeviceHelper::getInstance()->getIs2048Device())
    {
        lDesignSize=lDesignSize*2;
    }
    ScreenAdapterHelper::setupScreenAdapter(director->getOpenGLView()->getFrameSize(), lDesignSize);
    director->getOpenGLView()->setDesignResolutionSize(ScreenAdapterHelper::getRealDesignSize().width,ScreenAdapterHelper::getRealDesignSize().height, ResolutionPolicy::NO_BORDER);
    
    //场景切换进去LoadingScene
    Director::getInstance()->runWithScene(LoadingScene::scene());

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS
    // start flurry session.
    AnalyticX::flurryStartSession(st_flurry_key);
//#endif

    AdHelper::preloadInterstitial();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

     DataManager::getInstance()->save();
    
    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
//    Scene *scene =Director::getInstance()->getRunningScene();
//    if(scene->getChildrenCount()>0)
//    {
//        Vector<Node*> nodes=scene->getChildren();
//        Vector<Node*>::iterator iter=nodes.begin();
//        GameScene* lGameScene=dynamic_cast<GameScene*>(*iter);
//        if(lGameScene!=NULL)
//        {
//            lGameScene->pauseGame();
//        }
//    }
    
      NotificationCenter::getInstance()->postNotification(st_game_scene_pause_game_notification);
    
#endif
    
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
 #define st_isFirstRunOnAndroid "isFirstRunOnAndroid"
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Scene *scene =Director::getInstance()->getRunningScene();
    if(scene->getChildrenCount()>0)
    {
        Vector<Node*> nodes=scene->getChildren();
        Vector<Node*>::iterator iter=nodes.begin();
        GameScene* lGameScene=dynamic_cast<GameScene*>(*iter);
        if(lGameScene!=NULL)
        {
            lGameScene->pauseGame();
        }
    }
    

   

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    AdHelper::requestInterstitialAds_Application();
    
#endif
    
}
