#include "AppDelegate.h"
#include "HelloWorldScene.h"


#include "Scene/LoadingScene.h"
#include "helper/ResourceHelper.h"
#include "helper/ScreenAdapterHelper.h"

#include "define/GlobalMacro.h"
#include "helper/AudioHelper.h"
#include "helper/DeviceHelper.h"


#include "SimpleAudioEngine.h"
#include "crossplatformapi/headers/analytics/Analytics.h"
#include "scene/GameScene.h"
#include "game/GameLayer.h"
#include "HelloWorldScene.h"
#include "scene/ChooseLevelScene.h"
#include "game/DataManager.h"
#include "AdsManager.h"
#include "helper/ImageDownload.h"
#include "ColorAdsManager.h"
#include "PatchManager.h"
#include "AdsLoadingLayer.h"
#include "AdsLoadingManager.h"
#include "ColorAdsManager.h"

USING_NS_CC;
using namespace CocosDenshion;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}


#define IS_ALREADY_DELETE_FILE "IS_ALREADY_DELETE_FILE"


bool AppDelegate::applicationDidFinishLaunching() {
    
    
     DataManager::getInstance()->initSetting();
    
     //AudioHelper::setup(1.0f, 1.0f);
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
 
    // turn on display FPS
    director->setDisplayStats(false);
    log("%s",FileUtils::getInstance()->getWritablePath().c_str());

//    AdsManager::getInstance()->setDebugMode(true);
    AdsManager::getInstance()->preloadAllAds();
    AdsManager::getInstance()->showAds(kTypeBannerAds);
    AdsManager::getInstance()->setVisiable(kTypeBannerAds, false);
    
    srand((unsigned int)time(0));
    DataManager::getInstance()->initCurrentBGColor();
    
    //设置资源目录结构
    ResourceHelper::getInstance()->setupResourceDir();
    DeviceHelper::getInstance()->setCurrentDeviceType();

//    bool isAlreadyDeleteFile=UserDefault::getInstance()->getBoolForKey(IS_ALREADY_DELETE_FILE);
//    if(isAlreadyDeleteFile==false)
//    {
//        Size lSize=Director::getInstance()->getWinSize();
//        if(lSize.width==1536 && lSize.height==2048)
//        {
//            for (int i=0; i<10; i++) {
//                stringstream ss;
//                ss<<"map";
//                ss<<i+1;
//                ss<<"_white.png";
//                
//                string filename = ss.str();
//                std::string fullpath = FileUtils::getInstance()->getWritablePath() + filename;
//                if(FileUtils::getInstance()->isFileExist(fullpath))
//                {
//                    remove(fullpath.c_str());
//                }
//                
//                ColorData lColorData;
//                lColorData.R=255;
//                lColorData.G=255;
//                lColorData.B=255;
//                lColorData.ID=i+1;
//                
//                DataManager::getInstance()->saveBackColor(i+1, lColorData);
//            }
//            
//        }
//        
//        UserDefault::getInstance()->setBoolForKey(IS_ALREADY_DELETE_FILE, true);
//        UserDefault::getInstance()->destroyInstance();
//    }

    xPatch->patchApp(0, nullptr);
    ImageDownload::getInstance()->excuteFunc(CANCEL_DOWNLOAD_STATE);
    ImageDownload::getInstance()->setCurrentState(STRAT_CHECK_VERSION_STATE);
    
    
    //设置屏幕尺寸匹配 对于屏幕适配的坐标都要通过ScreenAdapterHelper设置了
    Size lDesignSize(Vec2(st_design_screen_width, st_design_screen_height));
//    if(DeviceHelper::getInstance()->getIs2048Device())
//    {
//        lDesignSize=lDesignSize*2;
//    }
    ScreenAdapterHelper::setupScreenAdapter(director->getOpenGLView()->getFrameSize(), lDesignSize);
    director->getOpenGLView()->setDesignResolutionSize(ScreenAdapterHelper::getRealDesignSize().width,ScreenAdapterHelper::getRealDesignSize().height, ResolutionPolicy::NO_BORDER);
    
    Director::getInstance()->setProjection(Director::Projection::_2D);
    xAdLoading->init(AdsLoadingLayer::create(false));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    //场景切换进去LoadingScene
    Director::getInstance()->runWithScene(ChooseLevelScene::scene(1));
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    //场景切换进去LoadingScene
    Director::getInstance()->runWithScene(LoadingScene::scene());
    
#endif
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    
    
    
    Director::getInstance()->stopAnimation();
    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    
    Scene *scene =Director::getInstance()->getRunningScene();
    if(scene!=NULL &&  scene->getChildrenCount()>0)
    {
        Vector<Node*> nodes=scene->getChildren();
        Vector<Node*>::iterator iter=nodes.begin();
        GameScene* lGameScene=dynamic_cast<GameScene*>(*iter);
        if(lGameScene!=NULL)
        {
            GameLayer* lGameLayer=lGameScene->getGameLayer();
            lGameLayer->saveImage();
        }
    }
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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
    

   

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

  //  AdHelper::requestInterstitialAds_Application();
    
#endif
    
    xColorAds->setShowed(false);
}
