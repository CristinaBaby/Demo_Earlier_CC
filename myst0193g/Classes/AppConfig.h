//
//  AppConfig.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-24.
//
//

#ifndef __LVUP002__AppConfig__
#define __LVUP002__AppConfig__


// ------------------- debug info ------------------

//以下PA内部测试时全部打开
// unlimited hint
//#define DEBUG_HINT

//#define DEBUG_ALL_OBJECTS

//提交QA是注释
//#define DEBUG_IAP

// all levels is valid，提交QA是注释
//#define DEBUG_ALL_LEVELS

//提交QA是注释
//#define DEBUG_REMOVE_ADS

//只有一个购买hint的iap(关卡全免)时打开
#define ONE_IAP
// -------------------------------------------------

#define st_resume_response_menu_touch_notification "resume_response_menu_touch_notification"

#define st_pauselayer_request_ad_notification "pauselayer_request_ad_notification"

// ------------------------------------------------------------------
#define project_name                            "Hidden Objects -Crime Investigation Unit-CIU"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS

#define apple_id                                "920282693"

#define st_flurry_key                           "C76N78TJVFRXM2Z3MTXY"


#define st_iap_sku_unlock_all                   "net.timeoutapps.shopping_mania.unlockall"
#define st_iap_sku_ad                           ""
#define st_iap_sku_hint_5                       "net.timeoutapps.shopping_mania.hints"


// -------------- ADs --------------
// iPhone
#define IPHONE_BANNER_ID                        ""
#define IPHONE_INTERSTITIAL_ID                  "4cd12cb4c1974092b073546ca7f82ee5"
// iPad
#define IPAD_BANNER_ID                          ""
#define IPAD_INTERSTITIAL_ID                    "148e88692e724d42b9de9d13a382477e"
// Rect ad
#define IPHONE_RECT_ID                          "e8d6d368660949de9d99635f80878269"
#define IPAD_RECT_ID                            "e9e95bce607849d3b0480ddc54d3a5cb"

#define IPHONE_FULLSCREEN_ID_CROSSPROMO         ""
#define IPAD_FULLSCREEN_ID_CROSSPROMO           ""
// ---------------------------------


#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
/*  android os */
#define st_android_package_name                 "com.moremysteries.android_hog_crimeinvestigationunit"

#define st_iap_sku_unlock_all                   "net.timeoutapps.android_hog_shoppingmania.billing_unlock_all"
#define st_iap_sku_ad                           ""
#define st_iap_sku_hint_5                       "com.moremysteries.android_hog_crimeinvestigationunit.billing_hints"


#if (ST_TARGET_MARKET == ST_TARGET_MARKET_AMAZON)

// Amazon version
#define st_flurry_key                           "R8FCRK57C6FQN3BNDWFH"

#elif (ST_TARGET_MARKET == ST_TARGET_MARKET_GOOGLE_PLAY)

// Google Play version
#define st_flurry_key                           "2675RCP9QCV5Q4RVQ99Q"

#endif


#endif
// ------------------------------------------------------------------


#endif /* defined(__LVUP002__AppConfig__) */
