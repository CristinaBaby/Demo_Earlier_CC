//
//  ShareScene.cpp
//  ColorGarden
//
//  Created by lidong on 15/8/6.
//
//

#include "ShareScene.h"
#include "GameScene.h"
#include "GameLayer.h"
#include "../define/GlobalMacro.h"
#include "../define/ResoureMacro.h"
#include "../helper/ControlHelper.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../scene/ChooseLevelScene.h"
#include "../game/DataManager.h"
#include "../modules/STSystemFunction.h"
#include "../modules/FacebookTool.h"
#include "../modules/STInstagramFunction.h"
#include "../helper/ImageDownload.h"
#include "ColorAdsManager.h"

#include <stdio.h>
#include <vector>
#include <thread>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#endif

enum
{
   eShareScene_MenuItem_Back_Tag=10,
   eShareScene_MenuItem_Home_Tag,
   eShareScene_MenuItem_FaceBook_Tag,
   eShareScene_MenuItem_Instagram_Tag,
   eShareScene_MenuItem_Email_Tag,
   eShareScene_MneuItem_Save_Tag
};

cocos2d::Scene* ShareScene::scene(int pIndex,GameScene* pGameScene)
{
    cocos2d::Scene* lScene=cocos2d::Scene::create();
    ShareScene *pRet = new ShareScene();
    if (pRet && pRet->init(pIndex, pGameScene))
    {
        pRet->setGameScene(pGameScene);
        pRet->autorelease();
        lScene->addChild(pRet);
        return lScene;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ShareScene::init(int pIndex, GameScene* pGameScene)
{
    if(!BaseLayer::init()) return false;
   
    this->setIndex(pIndex);
    
    LayerColor* layerColor=LayerColor::create(Color4B(49, 49, 49, 255),this->getContentSize().width,ScreenAdapterHelper::getValue(90));
    this->addChild(layerColor);
    layerColor->setScale(ScreenAdapterHelper::getRealDesignScale());
    layerColor->setPosition(Vec2(0, this->getContentSize().height-ScreenAdapterHelper::getValue(90)));
    
//    LayerColor* layerColor=LayerColor::create(Color4B(49, 49, 49, 255));
//    this->addChild(layerColor);
    
    LayerColor* layer = LayerColor::create(DataManager::getInstance()->m_cCurrentBGColor, ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(630) * ScreenAdapterHelper::getRealDesignScale());
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2(0.5, 0.5));
    ScreenAdapterHelper::setPosition(layer, Vec2(384, 620 + (ScreenAdapterHelper::getRealDesignScale() -1 )*200));
    this->addChild(layer, -1);
    
    LayerColor* la = LayerColor::create(DataManager::getInstance()->m_cCurrentBGColor, ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(620));
    la->ignoreAnchorPointForPosition(false);
    la->setAnchorPoint(Vec2(0, 1));
    layer->addChild(la);
    
    setGameScene(pGameScene);
    
    this->loadUI();
    
    return true;
}

void ShareScene::loadUI()
{
    
    float lY=995 - ScreenAdapterHelper::getRealDesignScale() * 17;
    
    Label* label=ControlHelper::getLabel(st_font_futumd, "Share My Painting", 50);
    this->addChild(label);
    ScreenAdapterHelper::setPosition(label, Vec2(384, lY),true,false,false,false);
    label->setScale(0.8);
    
    MenuItemSprite* lMenuItemBack=ControlHelper::menuItemSprite("back.png");
    lMenuItemBack->setTag(eShareScene_MenuItem_Back_Tag);
    lMenuItemBack->setCallback(CC_CALLBACK_1(ShareScene::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemBack, Vec2(50, lY),true,false,false,false);
    lMenuItemBack->setScale(0.8);
    
    MenuItemSprite* lMenuItemHome=ControlHelper::menuItemSprite("home.png");
    lMenuItemHome->setTag(eShareScene_MenuItem_Home_Tag);
    lMenuItemHome->setCallback(CC_CALLBACK_1(ShareScene::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemHome, Vec2(768-50, lY),true,false,false,false);
    lMenuItemHome->setScale(0.8);
    
    
    int lResY=200 * ScreenAdapterHelper::getRealDesignScale();
    
    MenuItemSprite* lMenuItemFaceBook=ControlHelper::menuItemSprite("facebook.png");
    lMenuItemFaceBook->setTag(eShareScene_MenuItem_FaceBook_Tag);
    lMenuItemFaceBook->setCallback(CC_CALLBACK_1(ShareScene::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemFaceBook, Vec2(155, lResY),false,true,false,false);
    
    MenuItemSprite* lMenuItemInstagram=ControlHelper::menuItemSprite("instagram.png");
    lMenuItemInstagram->setTag(eShareScene_MenuItem_Instagram_Tag);
    lMenuItemInstagram->setCallback(CC_CALLBACK_1(ShareScene::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemInstagram,  Vec2(315, lResY),false,true,false,false);
    
    MenuItemSprite* lMenuItemEmail=ControlHelper::menuItemSprite("email.png");
    lMenuItemEmail->setTag(eShareScene_MenuItem_Email_Tag);
    lMenuItemEmail->setCallback(CC_CALLBACK_1(ShareScene::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemEmail, Vec2(475, lResY),false,true,false,false);
    
    MenuItemSprite* lMenuItemSave=ControlHelper::menuItemSprite("save.png");
    lMenuItemSave->setTag(eShareScene_MneuItem_Save_Tag);
    lMenuItemSave->setCallback(CC_CALLBACK_1(ShareScene::doAction, this));
    ScreenAdapterHelper::setPosition(lMenuItemSave,  Vec2(635, lResY),false,true,false,false);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
      _menu=Menu::create(lMenuItemBack,lMenuItemHome,lMenuItemFaceBook,lMenuItemInstagram,lMenuItemEmail,lMenuItemSave, NULL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
      ScreenAdapterHelper::setPosition(lMenuItemInstagram,  Vec2(224, lResY),false,true,false,false);
      ScreenAdapterHelper::setPosition(lMenuItemEmail,  Vec2(384, lResY),false,true,false,false);
      ScreenAdapterHelper::setPosition(lMenuItemSave,  Vec2(544, lResY),false,true,false,false);
     _menu=Menu::create(lMenuItemBack,lMenuItemHome,lMenuItemInstagram,lMenuItemEmail,lMenuItemSave, NULL);
#endif
    
//  _menu=Menu::create(lMenuItemBack,lMenuItemHome,lMenuItemFaceBook,lMenuItemInstagram,lMenuItemEmail,lMenuItemSave, NULL);
    
    this->addChild(_menu);
    _menu->setPosition(Vec2(0, 0));
    _menu->setAnchorPoint(Vec2(0, 0));
    

    Sprite* lShadeSprite=ControlHelper::AddSprite("shadow.png", this);
    ScreenAdapterHelper::setPosition(lShadeSprite, Vec2(392, 572));
    
    Sprite* lFrame=ControlHelper::AddSprite("frame.png", this);
    ScreenAdapterHelper::setPosition(lFrame, Vec2(384, 590));
    lFrame->setScale(0.5f);
    
    ResourceInfo* lInfo;
    if(this->getIndex()<=DataManager::getInstance()->_resourceInfos.size())
    {
        lInfo=&DataManager::getInstance()->_resourceInfos.at(this->getIndex()-1);
    }
    else
    {
        lInfo=&ImageDownload::getInstance()->_downloadFinishResourceInfo.at(this->getIndex()-DataManager::getInstance()->_resourceInfos.size()-1);
    }
    
    auto texture = new Texture2D();
    auto gameLayer = (GameLayer*)_gameScene->getGameLayer();
    _saveImage = gameLayer->getSaveImage();
    texture->initWithImage(_saveImage);
    texture->autorelease();
    auto lSprite = Sprite::createWithTexture(texture);
    lFrame->addChild(lSprite, 1, 0);
    lSprite->setPosition(Vec2(lFrame->getContentSize().width/2, lFrame->getContentSize().height/2));
    lSprite->setScale(0.99f);

    
    string filename2;
    if(this->getIndex()<=DataManager::getInstance()->_resourceInfos.size())
    {
        ResourceInfo* lInfo=&DataManager::getInstance()->_resourceInfos.at(this->getIndex()-1);
        filename2=lInfo->image_name;
    }
    else
    {
        ResourceInfo* lInfo=&ImageDownload::getInstance()->_downloadFinishResourceInfo.at(this->getIndex()-DataManager::getInstance()->_resourceInfos.size()-1);
        filename2=lInfo->image_name;
    }
    
    Sprite* mSprite=ControlHelper::AddSprite(filename2.c_str(), lFrame,2,0);
    mSprite->setPosition(Vec2(lFrame->getContentSize().width/2, lFrame->getContentSize().height/2));
    mSprite->setScale(0.99f);
    Color3B lColor=DataManager::getInstance()->getSaveBackColor(this->getIndex());
    LayerColor* layerColor=LayerColor::create(Color4B(lColor.r, lColor.g, lColor.b, 255), lSprite->getContentSize().width, lSprite->getContentSize().height);
   // ScreenAdapterHelper::setPosition(layerColor, Vec2(-11, 12));
    lSprite->addChild(layerColor,-1);
    

    _frameSprite=lFrame;
}

void ShareScene::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
    
    this->scheduleOnce(schedule_selector(ShareScene::delayeSaveImage), 0.1f);
}

void ShareScene::delayeSaveImage(float dt)
{
    this->saveShareImage();
}

void ShareScene::saveShareImage()
{
    STSystemFunction sf;
    
    string lPath;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    lPath= FileUtils::getInstance()->getWritablePath() + "share.png";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    lPath = sf.getSdCardPath()+"/"+"share.png";
#endif
    
    CCLOG("lPATH=%s",lPath.c_str());
    
    _imagePath=lPath;
    

    _saveImage->saveToFile(lPath,false);
}

void ShareScene::doAction(cocos2d::Ref *pSender)
{
    MenuItemSprite* lMenuItem=(MenuItemSprite*)pSender;
    switch (lMenuItem->getTag()) {
        case eShareScene_MenuItem_Back_Tag:
        {
            _menu->setEnabled(false);
            
            getGameScene()->resumeTouchEvent();
            
            Director::getInstance()->popScene();
            
            xColorAds->back((Scene*)_gameScene->getParent());
        }
            break;
        case eShareScene_MenuItem_Home_Tag:
        {
            _menu->setEnabled(false);
            
            Director::getInstance()->popScene();
            auto scene = ChooseLevelScene::scene(this->getIndex());
            Director::getInstance()->replaceScene(scene);
            
            xColorAds->back(scene);
        }
            break;
        case eShareScene_MenuItem_FaceBook_Tag:
        {
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            string lBodyStr="Check out my artwork painted with ";
            lBodyStr=lBodyStr+project_name;
            lBodyStr.append("\n");
            
            string linkStr="https://itunes.apple.com/us/app/id";
            linkStr+=apple_id;
            linkStr+="?mt=8</\n";
            
            FacebookTool::postToFacebook(lBodyStr.c_str(),_imagePath.c_str(),linkStr.c_str());
            
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            
            string lBodyStr="Check out my artwork painted with ";
            lBodyStr=lBodyStr+project_name;
            lBodyStr.append("\n");
            
            string linkStr="https://play.google.com/store/apps/details?id=";
            linkStr.append(st_android_package_name).append("\n");
   
            FacebookTool::postToFacebook(lBodyStr.c_str(),_imagePath.c_str(),linkStr.c_str());
#endif
            
            xColorAds->share();
        }
            break;
        case eShareScene_MenuItem_Instagram_Tag:
        {
         
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            STSystemFunction sf;
            sf.sendPictureToInstagram(_imagePath);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            STInstagramFunction st;
            st.sendImageToInstagram(_imagePath.c_str());
#endif
            xColorAds->share();
        }
            break;
        case eShareScene_MenuItem_Email_Tag:
        {
            string lBodyStr="Check out my artwork painted with ";
            lBodyStr=lBodyStr+project_name;
            lBodyStr.append("\n");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            
   
            string linkStr="<a>https://itunes.apple.com/us/app/id";
            linkStr+=apple_id;
            linkStr+="?mt=8</a>\n";
            lBodyStr.append(linkStr);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            lBodyStr.append("http://");
           // lBodyStr.append("https://play.google.com/store/apps/details?id=")
           // .append(st_android_package_name).append("\n");
#endif
            STSystemFunction sf;
             sf.sendEmailAndFilePic(project_name, lBodyStr, _imagePath);
            
            xColorAds->share();
        }
            break;
        case eShareScene_MneuItem_Save_Tag:
        {
            Vec2 lOrginPos=_frameSprite->getPosition();
            _frameSprite->setScale(1.0f);
            
            STSystemFunction sf;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            sf.saveLocalImage(_saveImage);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            
            std::string writePath = sf.getSdCardPath()+"/Coloring Book/";
            if(!FileUtils::sharedFileUtils()->isFileExist(writePath))
            {
                this->createDirectory(writePath.c_str());
                log("MX: android writeable path: %s", writePath.c_str());
            }
            
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string _time = StringUtils::format("%ld", (now.tv_sec * 1000 + now.tv_usec / 1000));
            std::string fullPath = writePath + _time + ".png";
            
            bool isSaveSuccess = _saveImage->saveToFile(fullPath.c_str());
            if(isSaveSuccess)
                sf.makeToast("Saved successfully!");
            else
                sf.makeToast("Saved failed!");
            
            sf.refreshDCIM(fullPath);
#endif
            _frameSprite->setPosition(lOrginPos);
            _frameSprite->setScale(0.5f);
            
            xColorAds->share();
        }
            break;
        default:
            break;
    }
    
}

bool ShareScene::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, nullptr);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#endif

}

void ShareScene::delayTimeExcute(float dt)
{
      STSystemFunction sf;
      sf.sendPictureToInstagram(_imagePath);
}

