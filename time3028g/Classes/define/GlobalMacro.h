//
//  Macro.h
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#ifndef __HIDD__Macro__
#define __HIDD__Macro__

#include <stdio.h>
#include "cocos2d.h"
#include "../hidd/macro/macro.h"
using namespace cocos2d;
using namespace std;

//game debug setting
//show all hidd objects

//#define st_is_show_all_icons

//unlock all levels
//#define st_unlock_all_level

//let all levels maybe play
//#define st_enable_all_level

//unlimited hint model
//#define st_debug_hint_unlimited


// -------------- ADs --------------
// iPhone
#define IPHONE_BANNER_ID                        ""
#define IPHONE_INTERSTITIAL_ID                  ""
// iPad
#define IPAD_BANNER_ID                          ""
#define IPAD_INTERSTITIAL_ID                    ""
// Rect ad
#define IPHONE_RECT_ID                          ""
#define IPAD_RECT_ID                            ""
#define IPHONE_FULLSCREEN_ID_CROSSPROMO         ""
#define IPAD_FULLSCREEN_ID_CROSSPROMO           ""
// ---------------------------------

// ------------------------------------------------------------------
#define project_name                            "Top Secret Vacation"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS

#define apple_id                                ""

#define st_flurry_key                           ""


#define st_iap_sku_unlock_all                   ""
#define st_iap_sku_ad                           ""
#define st_iap_sku_hint_5                       ""




#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
/*  android os */
#define st_android_package_name                 "net.timeoutapps.android_hog_topsecretvacation"

#define st_iap_sku_unlock_all                   ""
#define st_iap_sku_ad                           ""
#define st_iap_sku_hint_5                       "net.timeoutapps.android_hog_topsecretvacation.billing_hints"


#if (ST_TARGET_MARKET == ST_TARGET_MARKET_AMAZON)

// Amazon version
#define st_flurry_key                           ""

#elif (ST_TARGET_MARKET == ST_TARGET_MARKET_GOOGLE_PLAY)

// Google Play version
#define st_flurry_key                           ""

#endif


#endif
// ------------------------------------------------------------------

#endif /* defined(__HIDD__Macro__) */
