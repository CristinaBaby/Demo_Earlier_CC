//
//  AppConfigs.h
//  ColorBook
//
//  Created by luoxiaopeng on 4/21/15.
//
//

#pragma once

#define THUMB_WIDTH                                     352

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define IAP_PREMIUM_SUBSCRIPTION                        "net.applabsinc.coloringbookforwomengirl.unlockall"
#else
#define IAP_PREMIUM_SUBSCRIPTION                        "net.applabsinc.android_coloranimaltopiafree.unlock"
#endif

#define COLORBOOK_IAP                                   "colorbook_iap"