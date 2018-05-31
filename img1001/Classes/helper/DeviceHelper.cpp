//
//  DeviceHelper.cpp
//  HH002
//
//  Created by houhuang on 15/8/25.
//
//

#include "DeviceHelper.h"

static DeviceHelper* _instance = nullptr;

DeviceHelper* DeviceHelper::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new DeviceHelper();
    }
    
    return _instance;
}

DeviceHelper::DeviceHelper()
{
    _is2048Device = false;
  
}

int DeviceHelper::getDeviceType()
{
    Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    
    Size winSize = Director::getInstance()->getWinSize();
    log("%f, %f",winSize.width, winSize.height);
    
    if (frameSize.width == 640 && frameSize.height == 960)
    {
        return st_DeviceType_iPhone4;
    }else if (frameSize.width == 640 && frameSize.height == 1136)
    {
        return st_DeviceType_iPhoneOther;
    }else if (frameSize.width == 960 && frameSize.height == 1704)
    {
        return st_DeviceType_iPhonePlus;
    }else if (frameSize.width == 768 && frameSize.height == 1024 || frameSize.width == 1536 && frameSize.height == 2048)
    {
        return st_DeviceType_iPad;
    }else
    {
        return -1;
    }
        
}

void DeviceHelper::initCurrentDeviceType()
{
    Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    
    Size winSize = Director::getInstance()->getWinSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    log("%f, %f",frameSize.width, frameSize.height);
    log("%f, %f",winSize.width, winSize.height);
    log("%f, %f",visibleSize.width, visibleSize.height);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    if (frameSize.width == 640 && frameSize.height == 960)
    {
        m_iCurrentType = st_DeviceType_iPhone4;
    }else if (frameSize.width == 640 && frameSize.height == 1136)
    {
        
        if (frameSize.width == 768)
        {
            m_iCurrentType = st_DeviceType_iPhonePlus;
        }else
        {
            m_iCurrentType = st_DeviceType_iPhoneOther;
        }
    }else if (frameSize.width == 960 && frameSize.height == 1704)
    {
        m_iCurrentType = st_DeviceType_iPhonePlus;
    }else if ((frameSize.width == 768 && frameSize.height == 1024) || (frameSize.width == 1536 && frameSize.height == 2048))
    {
        m_iCurrentType = st_DeviceType_iPad;
    }else
    {
        m_iCurrentType = -1;
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (frameSize.height / frameSize.width > 1.4)
    {
        m_iCurrentType = st_DeviceType_Android_iphone;
    }else
    {
        m_iCurrentType = st_DeviceType_Android_ipad;
    }
#endif
    
}

int DeviceHelper::getCurrentDeviceType()
{
    return m_iCurrentType;
}