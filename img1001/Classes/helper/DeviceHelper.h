//
//  DeviceHelper.h
//  HH002
//
//  Created by houhuang on 15/8/25.
//
//

#ifndef __HH002__DeviceHelper__
#define __HH002__DeviceHelper__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
USING_NS_CC;

enum{
    /*
     *根据适配后尺寸分类,本项目设计尺寸为 768 x 1024
     *iPhone4/iPhone4s --- 640 x 960
     *iPhone5/iPhone5s/iPhone6/iPhone6s --- 640 x 1136
     *iPhone6 Plus/ iPhone6S Plus --- 960 x 1704
     */
    st_DeviceType_iPhone4 = 30,
    st_DeviceType_iPhonePlus,
    st_DeviceType_iPhoneOther,
    st_DeviceType_iPad,
    st_DeviceType_Android_iphone,
    st_DeviceType_Android_ipad,
    
};

class DeviceHelper
{
public:
    st_property(bool, _is2048Device, Is2048Device);
    
    DeviceHelper();
    static DeviceHelper* getInstance();
    
    static int getDeviceType();
    
    void initCurrentDeviceType();
    int getCurrentDeviceType();
    
protected:
    int m_iCurrentType;
};


#endif /* defined(__HH002__DeviceHelper__) */
