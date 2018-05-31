//
//  DeviceHelper.cpp
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#include "DeviceHelper.h"

static DeviceHelper* _instance=NULL;

DeviceHelper* DeviceHelper::getInstance()
{
    if(_instance==NULL)
    {
        _instance=new DeviceHelper();
    }
    
    return _instance;
}

DeviceHelper::DeviceHelper()
{
    _is2048Device=false;
    m_iCurrentDeviceType = 0;
}

void DeviceHelper::setCurrentDeviceType()
{
    Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    float _scale = frameSize.height / frameSize.width;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if ((frameSize.width == 768 && frameSize.height == 1024) || (frameSize.height == 2048 && frameSize.width == 1536) )
    {
        m_iCurrentDeviceType = kType_Device_Pad;
    }else
    {
        m_iCurrentDeviceType = kType_Device_Phone;
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (_scale > 1.49)
    {
        m_iCurrentDeviceType = kType_Device_Phone;
    }else
    {
        m_iCurrentDeviceType = kType_Device_Pad;
    }
    
#endif
    
    log("screen height = %f, screen width = %f, heigth/width = %f", frameSize.height, frameSize.width, _scale);
}

int DeviceHelper::getCurrentDeviceType()
{
    return m_iCurrentDeviceType;
}