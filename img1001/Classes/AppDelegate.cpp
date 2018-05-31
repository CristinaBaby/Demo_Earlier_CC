#include "AppDelegate.h"
#include "define/Globalmacro.h"
#include "helper/ScreenAdapterHelper.h"
#include "helper/DeviceHelper.h"
#include "helper/ResourceHelper.h"
#include "scene/LoadingScene.h"
#include "scene/ChooseGalleryScene.h"
#include "crossplatformapi/headers/ads/AdsManager.h"
#include "define/AppConfigs.h"
//#include "modules/AnalyticX.h"
//#include "modules/Analytics.h"
#include "crossplatformapi/headers/analytics/Analytics.h"
#include "scene/GameScene.h"
#include "game/GameLayer.h"
#include "helper/MusicHelper.h"
#include "utilities/Common.h"

#include "ad3_2/AdCross/AdLoadingLayerDoll.h"
#include "ad3_2/AdCross/AdLoadingLayerBase.h"
#include "DownLoad/DownLoadPage.h"

USING_NS_CC;

#define IsAlreadyLoadGame_Color "IsAlreadyLoadGame_Color"

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
    
    bool isAlredyLoadGame=UserDefault::getInstance()->getBoolForKey(IsAlreadyLoadGame_Color);
    if(isAlredyLoadGame==false)
    {
        Common::saveInitLoadGameTime();
        UserDefault::getInstance()->setBoolForKey(IsAlreadyLoadGame_Color, true);
        UserDefault::getInstance()->destroyInstance();
    }
    
//    DownLoadPage::getInstance()->reset(NULL);
    
//    DownLoadPage::reset(NULL);
    FileUtils::getInstance()->getWritablePath();
    
    CCLOG("cache path = %s",FileUtils::getInstance()->getWritablePath().c_str());
    // turn on display FPS
//    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);


#ifndef DEBUG_ADS
//    AdsManager::getInstance()->preloadAllAds();

    BannerAds::getInstance()->setAutoShow(false);
    BannerAds::getInstance()->preload();
    AdsManager::getInstance()->preloadAds(kTypeInterstitialAds);
    
#endif
 
#ifndef REMOVE_REWARD_ADS
    AdsManager::getInstance()->preloadAds(kTypeRewardedAds);
#endif
    ResourceHelper::getInstance()->setUpEnviroment();
    ResourceHelper::getInstance()->setupResourceDir();
    
    
//    DataManager::getInstance()->initSettingBackColor();
    
    Size lDesignSize = Size(st_design_screen_width, st_design_screen_height);
    if (DeviceHelper::getInstance()->getIs2048Device())
    {
        lDesignSize = lDesignSize*2;
    }
    ScreenAdapterHelper::setupScreenAdapter(director->getOpenGLView()->getFrameSize(), lDesignSize);
    
    director->getOpenGLView()->setDesignResolutionSize(ScreenAdapterHelper::getRealDesignSize().width, ScreenAdapterHelper::getRealDesignSize().height, ResolutionPolicy::NO_BORDER);
    
    DeviceHelper::getInstance()->initCurrentDeviceType();
    
    log("%d",DeviceHelper::getInstance()->getCurrentDeviceType());

    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    Scene* scene = ChooseGalleryScene::scene();
    //    // run
    //    director->runWithScene(scene);
    //
    //#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Scene* scene = LoadingScene::scene();
    director->runWithScene(scene);
    
    //#endif

   // AnalyticX::flurryStartSession(flurry_key);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    
    //防止Android黑屏问题
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    
    Director::getInstance()->pause();
    Director::getInstance()->stopAnimation();
    UserDefault::getInstance()->setBoolForKey(st_IsPlayBackgrioundMusic, MusicHelper::m_bIsPlayBackgroudMusic);
//    UserDefault::getInstance()->destroyInstance();
    
    DM_getInstance->saveMyArtworksDatas();
//    DM_getInstance->saveMyPostsDatas();
//    DM_getInstance->saveFavoriteDatas();
//    DownLoadPage::getInstance()->saveInspirationDatas();
    
    static bool lIsFirstLoading=false;
    if(!lIsFirstLoading)
    {
        lIsFirstLoading=true;
    }
    else
    {
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
        
        for (int i = 0; i < DataManager::getInstance()->m_vFlurryDatas.size(); ++i)
        {
            FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
            if (lFlurryInfo->number > 0)
            {
                stringstream ss;
                ss<<"";
                ss<<lFlurryInfo->message;
                
                
//                CCString* value=CCString::createWithFormat("%s",lFlurryInfo->message.c_str());
//                CCDictionary* lDictionary = CCDictionary::create();
//                lDictionary->setObject(value, ss.str().c_str());

                Analytics::getInstance()->sendEvent(ss.str().c_str(), ss.str().c_str());
//                AnalyticX::flurryLogEventWithParameters(ss.str().c_str(), lDictionary);
                lFlurryInfo->number = 0;

            }
        }
        
        if (MusicHelper::m_bIsPlayBackgroudMusic)
        {
            Analytics::getInstance()->sendEvent("Music_play", "Music_play");
        }else
        {
            Analytics::getInstance()->sendEvent("Music_stop", "Music_stop");
        }

        if (DataManager::m_bIsImagereset)
        {
            DataManager::m_bIsImagereset = false;
            Analytics::getInstance()->sendEvent("Painting_reset", "Painting_reset");
        }
        if (DataManager::m_bIsImageundo)
        {
            DataManager::m_bIsImageundo = false;
            Analytics::getInstance()->sendEvent("Painting_undo", "Painting_undo");
        }
        if (DataManager::m_bIsClickText)
        {
            DataManager::m_bIsClickText = false;
            Analytics::getInstance()->sendEvent("Edit_text", "Edit_text");
        }
        if (DataManager::m_bIsClickTextFont)
        {
            DataManager::m_bIsClickTextFont = false;
            Analytics::getInstance()->sendEvent("Edit_textformat", "Edit_textformat");
        }
        if (DataManager::m_bIsClickTextColor)
        {
            DataManager::m_bIsClickTextColor = false;
            Analytics::getInstance()->sendEvent("Edit_textcolor", "Edit_textcolor");
        }
        if (DataManager::m_bIsClickGameSceneBack)
        {
            DataManager::m_bIsClickGameSceneBack = false;
            Analytics::getInstance()->sendEvent("Painting_back", "Painting_back");
        }
        if (DataManager::m_bIsClickGameSceneDone)
        {
            DataManager::m_bIsClickGameSceneDone = false;
            Analytics::getInstance()->sendEvent("Painting_finish", "Painting_finish");
        }
    }
    
    
    
    // if you use SimpleAudioEngine, it must be pause
     CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    
    Director::getInstance()->getEventDispatcher()->setEnabled(true);
//    AdLoadingLayerBase::showLoading<AdLoadingLayerDoll>(false);
    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    if (MusicHelper::m_bIsPlayBackgroudMusic)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }

}
