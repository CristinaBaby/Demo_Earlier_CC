#include "AppDelegate.h"
#include "AppConfigs.h"
#include "AudioHelper.h"
#include "UICpp.h"
#include "audio/include/SimpleAudioEngine.h"
#include "GraphManager.hpp"
#include "MyArtDataManager.hpp"
#include "AdsLoadingLayer.h"
#include "AdsLoadingManager.h"
#include "ColorAdsManager.h"
#include "GameScene.h"
#include "DownLoadPage.h"
#include "DataPromotionManager.hpp"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = cocos2d::GLViewImpl::create("ColorBook");
        director->setOpenGLView(glview);
    }
    
    log("%s",FileUtils::getInstance()->getWritablePath().c_str());
    
    xDPM_getInstance->promotionToV2_0();
    xDPM_getInstance->promotionToV2_1();
    namespaceST::STVisibleRect::setupVisibleRect(director->getOpenGLView()->getFrameSize(), Size(640, 960));

    //set resource search path
    vector<string> searchPaths;
    searchPaths.push_back(FileUtils::getInstance()->getWritablePath() + "color");
    searchPaths.push_back(FileUtils::getInstance()->getWritablePath());
    searchPaths.push_back("cocos");
    searchPaths.push_back("sound");
    searchPaths.push_back("data");
    searchPaths.push_back("values");
    searchPaths.push_back("graphs");
    searchPaths.push_back("graphs/thumbs");
    
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    Director::getInstance()->setProjection(Director::Projection::_2D);
    

    xMyArtData->readMyArtData();
 
    xAdLoading->init(AdsLoadingLayer::create(false));
    xAds->preloadAllAds();
    xAds->showAds(kTypeBannerAds);
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    xScene->forward(SceneManager::Tag::CHAPTER);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    xScene->forward(SceneManager::Tag::LOADING);
#endif
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    xMyArtData->saveMyArtData();
    DownLoadPage::getInstance()->saveClickLikeInfo();
    
    auto scene = Director::getInstance()->getRunningScene();
    if (scene && scene->getName() == "GameScene") {
        ((GameScene*)scene)->applicationDidEnterBackground();
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
    xColorAds->setShowed(false);
}
