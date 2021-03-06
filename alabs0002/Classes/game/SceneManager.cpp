//
//  SceneManager.cpp
//  SingleEye
//
//  Created by luoxiaopeng on 1/21/14.
//
//
#include "SceneManager.h"
#include "ChapterScene.h"
#include "GraphScene.h"
#include "GameScene.h"
#include "MyArtsScene.h"
#include "ShareScene.h"
#include "InspirationScene.h"
#include "Dialog.h"
#include "DownloadManager.hpp"
#include "StringsManager.h"
#include "TransitionMoveOutR.h"
#include "MyArtDataManager.hpp"
#include "AdsLoadingManager.h"
#include "PalletteManager.hpp"
#include "LoadingScene.hpp"


SceneManager* SceneManager::getInstance()
{
    static SceneManager instance;
    return &instance;
}

SceneManager::SceneManager()
{
    m_pOldScene = NULL;
}

SceneManager::~SceneManager()
{
    m_vData.clear();
}

void SceneManager::replaceScene(SceneData &data)
{
    if (_updateCSV && data.aTag == Tag::CHAPTER) {
        _updateCSV = false;
        xDownload->refreshData();
    }
    
    Scene *pScene = NULL;
    switch (data.aTag) {
        case Tag::CHAPTER:
        {
            pScene = ChapterScene::create();
        }
            break;
        case Tag::GRAPH:
        {
            pScene = GraphScene::create();
        }
            break;
        case Tag::GAME:
        {
            if (data.isForward) {
                xPallette->reset();
//                if (m_vData.at(m_vData.size() - 1).aTag != SceneManager::Tag::MY_ARTS)
                {
                    int rateUs = UserDefault::getInstance()->getIntegerForKey(rateUsCount, 0);
                    UserDefault::getInstance()->setIntegerForKey(rateUsCount, ++rateUs);
                }
            }
            
            pScene = GameScene::create();
        }
            break;
        case Tag::MY_ARTS:
        {
            pScene = MyArtsScene::create();
            
            if (data.isForward) {
                quickFlurry("btn_myartwork");
            }
        }
            break;
        case Tag::SHARE:
        {
            if (data.isForward) {
                xAdLoading->show(ADS_TYPE::kTypeInterstitialAds, nullptr);
            }
            
            pScene = ShareScene::create();
        }
            break;
        case Tag::INSPIRATION:
        {
            pScene = InspirationScene::create();
        }
            break;
        case Tag::SETTING:
        {
//            pScene = SettingScene::create();
        }
            break;
            
        case Tag::LOADING:
        {
            pScene = LoadingScene::create();
        }
            break;
            
        default:
            break;
    }
    
    if (!m_vData.empty()) {
#if 0   //暂时不用这个功能
        TransitionScene * transition;
        if (data.isForward) {
            if(data.aTag == Tag::SETTING || data.aTag == Tag::MY_ARTS)
            {
                transition = TransitionMoveInR::create(0.5f, pScene);
            }
            else
            {
                transition = TransitionSlideInR::create(0.5f, pScene);
            }
        }
        else
        {
            if (m_eLastScene == Tag::SETTING || m_eLastScene == Tag::MY_ARTS)
            {
                transition = TransitionMoveOutL::create(0.5f, pScene);
            }
            else
            {
                transition = TransitionSlideInL::create(0.5f, pScene);
            }
        }
#endif
        Director::getInstance()->replaceScene(pScene);
    } else {
        //第1个场景
        Director::getInstance()->runWithScene(pScene);
    }
    
    m_eLastScene = data.aTag;
}

void SceneManager::back()
{    
    if (isOldScene()) {
        return;
    }
    
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {
        runningScene->unscheduleAllSelectors();
    }
    
    m_vData.pop_back();
    CCAssert(!m_vData.empty(), "error");

    SceneData data = m_vData.at(m_vData.size() - 1);
    data.isForward = false;
    replaceScene(data);
}

void SceneManager::back(Tag aTag)
{
    if (isOldScene()) {
        return;
    }
    
    CCAssert(!m_vData.empty(), "error");
    
    int index = m_vData.size();

    while (index > 0) {
        SceneData data = m_vData.at(index - 1);
        data.isForward = false;
        
        if (data.aTag == aTag) {
            replaceScene(data);
            break;
        }
        else
        {
            //删除该记录
            index--;
            m_vData.pop_back();
            CCAssert(index != 0, "no this scene.");
        }
    }
}

void SceneManager::forward(Tag aTag)
{   
    forward(aTag, 0, NULL);
}

void SceneManager::forward(Tag aTag, int iData, void* pData)
{
    if (isOldScene()) {
        return;
    }
    
    SceneData data;
    
    data.aTag = aTag;
    data.iData = iData;
    data.pData = pData;
    data.isForward = true;
        
    replaceScene(data);
    m_vData.push_back(data);
}

SceneManager::Tag SceneManager::getLastSceneTag()
{
    if (m_vData.size() > 1) {
        SceneData data = m_vData.at(m_vData.size() - 2);        
        return data.aTag;
    }
    else
    {
        return Tag::NONE;
    }
}

SceneManager::Tag SceneManager::getRunningSceneTag()
{      
    return m_eLastScene;
}

bool SceneManager::isOldScene()
{
    bool bRet = false;
    
    Scene *pRunningScene = Director::getInstance()->getRunningScene();
    
    if (pRunningScene != NULL) {
        if (m_pOldScene == pRunningScene) {
            bRet = true;
        }
        else
        {
            m_pOldScene = pRunningScene;
        }
    }
        
    return bRet;
}

void SceneManager::updateCSV()
{
    xDownload->setColorSystemVersion();
    xDownload->saveCsvFile();
    
    if (m_vData.at(m_vData.size() - 1).aTag == Tag::CHAPTER) {
        xDownload->refreshData();
    }
    else
    {
        _updateCSV = true;
    }
}
