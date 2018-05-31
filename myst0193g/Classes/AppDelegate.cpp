#include "AppDelegate.h"
#include "FileModifiers.h"
#include "AppConfig.h"
#include "AppGlobal.h"
#include "SimpleAudioEngine.h"
#include "utilities/STResManager.h"
#include "scenes/LoadingScene.h"
#include "scenes/GameScene.h"
#include "modules/AnalyticX.h"
#include "helpers/AdHelper.h"
//#include "CheckResource_New.h"
#include "helpers/AdHelper.h"


// ------------------ screen size ------------------
/*
 // vertical
 #define DESIGN_WIDTH            768
 #define DESIGN_HEIGHT           1024
 #define DESIGN_WIDTH_HD         1536
 #define DESIGN_HEIGHT_HD        2048
 */

// horizontal
#define DESIGN_WIDTH            1024
#define DESIGN_HEIGHT           768
#define DESIGN_WIDTH_HD         2048
#define DESIGN_HEIGHT_HD        1536
// -------------------------------------------------


USING_NS_CC;
USING_NS_ST;
using CocosDenshion::SimpleAudioEngine;

AppDelegate::AppDelegate() {
    isFirstRunOnAndroid = true;
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);
    
   // checkResource(15);
    
    //
    float dw = 0, dh = 0;
    
    switch (CCApplication::sharedApplication()->getTargetPlatform())
    {
        case kTargetIphone:
        {
            //            CCLOG("This is a iPhone.");
            dw = DESIGN_WIDTH;
            dh = DESIGN_HEIGHT;
            
            AppGlobal::IS_2048 = false;
        }
            
            break;
        case kTargetIpad:
        {
            CCSize screenSize = pEGLView->getFrameSize();
            if(screenSize.width > 2000 || screenSize.height > 2000)
            {
                //                CCLOG("This is a retina iPad.");
                dw = DESIGN_WIDTH_HD;
                dh = DESIGN_HEIGHT_HD;
                
                AppGlobal::IS_2048 = true;
            }
            else
            {
                //                CCLOG("This is a normal iPad.");
                dw = DESIGN_WIDTH;
                dh = DESIGN_HEIGHT;
                
                AppGlobal::IS_2048 = false;
            }
        }
            
            break;
        case kTargetAndroid:
            //            CCLOG("This is an android device.");
            dw = DESIGN_WIDTH;
            dh = DESIGN_HEIGHT;
            
            AppGlobal::IS_2048 = false;
            
            break;
        default:
            dw = DESIGN_WIDTH;
            dh = DESIGN_HEIGHT;
            
            AppGlobal::IS_2048 = false;
            
            break;
    }
    
    //
    pEGLView->setDesignResolutionSize(dw, dh, kResolutionNoBorder);
    
    // set ui search path
    CCFileUtils::sharedFileUtils()->setSearchPaths(STResManager::getInstance()->getFileSearchPath());
    
    //
    CCSize vsize = CCDirector::sharedDirector()->getVisibleSize();
    float scaleX = vsize.width / dw;
    float scaleY = vsize.height/ dh;
    AppGlobal::UI_SCALE = MIN(scaleX, scaleY);
    
    // create a scene. it's an autorelease object
    CCScene *pScene = LoadingLayer::scene();

    // run
    pDirector->runWithScene(pScene);
    
    
    // start flurry session.
    AnalyticX::flurryStartSession(st_flurry_key);
    
    AdHelper::preloadInterstitial();
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    if (scene->getTag() == tag_scene_game)
    {
        GameScene* gs = dynamic_cast<GameScene*>(scene);
        gs->pauseGame();
    }
    
#endif
    
    
}

// this function will be called when the app is active again
// but on Android OS, this will be called every time, even run app on the first time.
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //
//    AdHelper::requestInterstitialAds();
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    if (scene->getTag() == tag_scene_game)
    {
        GameScene* gs = dynamic_cast<GameScene*>(scene);
        gs->pauseGame();
    }
    
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    // on Android OS, this will be called every time, even run app on the first time.
    // so we must forbid request iterstitial ads when game started.
//    if (isFirstRunOnAndroid)
//        isFirstRunOnAndroid = false;
//    else
    {
         AdHelper::requestInterstitialAds_Application();
        
        
#ifndef DEBUG_REMOVE_ADS
       
#endif
    }
    
#endif
    
    
}
