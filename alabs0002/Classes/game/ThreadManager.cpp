//
//  ThreadManager.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/29.
//
//

#include "ThreadManager.hpp"



static ThreadManager* _instance = NULL;

ThreadManager::ThreadManager()
{
    m_bIsload = false;
    m_sCurrentLoadImage = "";
}

ThreadManager::~ThreadManager()
{
    
}

ThreadManager* ThreadManager::getInstance()
{
    if (!_instance)
    {
        _instance = new ThreadManager();
    }
    return _instance;
}

void ThreadManager::startLoadImage()
{
    if (!m_vNeedLoadImage.empty())
    {
        string str = m_vNeedLoadImage.at(0);
        this->dynamicLoadImage(str);
    }
}

void ThreadManager::dynamicLoadImage(const string& imageName)
{
    if (!m_bIsload)
    {
        m_bIsload = true;
        Director::getInstance()->getTextureCache()->addImageAsync(imageName, CC_CALLBACK_1(ThreadManager::imageLoadedCallback, this));
    }
}

void ThreadManager::imageLoadedCallback(Ref* ref)
{
    NotificationCenter::getInstance()->postNotification(st_multiThread_loadImage_finish);
    
    m_bIsload = false;
    
    if (m_vNeedLoadImage.empty())
    {
        return;
    }
        
    m_vNeedLoadImage.pop_front();
    
    
    if (!m_vNeedLoadImage.empty()) {
        startLoadImage();
    }
}


