//
//  DeviceHelper.h
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#ifndef __HIDD__DeviceHelper__
#define __HIDD__DeviceHelper__

#include <stdio.h>
#include "../define/GlobalMacro.h"

USING_NS_CC;

/*
 *判断设备型号,将设备分成Pad和phone
 *Android根据分辨率给设备分类（pad or phone）
 */
enum
{
    kType_Device_Pad = 222,
    kType_Device_Phone,
};

class DeviceHelper {
    
public:
    st_property(bool,_is2048Device,Is2048Device);
public:
    DeviceHelper();
    static DeviceHelper* getInstance();
    
    void setCurrentDeviceType();
    int getCurrentDeviceType();
    
public:
    int m_iCurrentDeviceType;
};

#endif /* defined(__HIDD__DeviceHelper__) */
