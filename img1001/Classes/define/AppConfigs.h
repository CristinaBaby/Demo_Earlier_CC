//
//  AppConfig.h
//  IMG1001
//
//  Created by houhuang on 15/9/10.
//
//


#ifndef IMG1001_AppConfigs_h
#define IMG1001_AppConfigs_h

//#define DEBUG_IAP
//#define DEBUG_ADS
//#define TEST_VERSION

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define REMOVE_REWARD_ADS

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define REMOVE_REWARD_ADS

#endif



#define project_name                            "ColorFree: Coloring Book Mania"
#define apple_id                                "1034634147"


#define st_android_package_name                 "com.imagineappsltd.colorfree"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)


#define st_purchase_color_sea_iap       "com.imagineappsltd.colorfree.unlockpalettesea"
#define st_purchase_color_neon_iap      "com.imagineappsltd.colorfree.unlockpaletteneon"
#define st_purchase_color_ukiyoe_iap    "com.imagineappsltd.colorfree.unlockpaletteukiyoe"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define st_purchase_color_sea_iap       "com.imagineappsltd.android_colorfree.unlockpalettenature"
#define st_purchase_color_neon_iap      "com.imagineappsltd.android_colorfree.unlockpaletteneon"
#define st_purchase_color_ukiyoe_iap    "com.imagineappsltd.android_colorfree.unlockpaletteukiyoe"

#endif

#define IS_PURCHASE_COLOR_NEON      "purchase_color_neon"
#define IS_PURCHASE_COLOR_SEA       "puechase_color_sea"
#define IS_PURCHASE_COLOR_UKIYOE    "puechase_color_ukiyoe"


#endif
