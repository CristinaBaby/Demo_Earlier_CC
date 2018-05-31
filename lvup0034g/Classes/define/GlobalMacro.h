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
#define project_name                            "Messy Holiday Trip - Hidden Objects"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // iOS

#define apple_id                                ""

#define st_flurry_key                           ""


#define st_iap_sku_unlock_all                   ""
#define st_iap_sku_ad                           ""
#define st_iap_sku_hint_5                       ""




#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Android
/*  android os */
#define st_android_package_name                 "net.levelupapps.android_hog_messyholidaytrip"

#define st_iap_sku_unlock_all                   ""
#define st_iap_sku_ad                           ""
#define st_iap_sku_hint_5                       "net.levelupapps.android_hog_messyholidaytrip.billing_hints"


#if (ST_TARGET_MARKET == ST_TARGET_MARKET_AMAZON)

// Amazon version
#define st_flurry_key                           ""

#elif (ST_TARGET_MARKET == ST_TARGET_MARKET_GOOGLE_PLAY)

// Google Play version
#define st_flurry_key                           "TTFJYS9TFSZJSDPVCPD3"

#endif


#endif
// ------------------------------------------------------------------


//notification event info
#define st_refresh_choose_map_box_locked_notification "refresh_choose_map_box_locked_notification"
#define st_refresh_hint_notification      "refresh_hint_notification"
#define st_save_resumedata_notification   "save_resumedata_notification"
#define st_stop_response_menu_touch_notification "stop_response_menu_touch_notification"
#define st_resume_response_menu_touch_notification "resume_response_menu_touch_notification"
#define st_game_scene_pause_game_notification "game_scene_pause_game_notification"
#define st_pauselayer_request_ad_notification "pauselayer_request_ad_notification"
//define designed screen size
#define st_design_screen_width 1024
#define st_design_screen_height 768

//define level data
#define st_level_time_orgin_data  120

//define screen center position
#define st_screen_center_position Vec2(512,384)

//define game setting parameter
#define st_icon_col_number 5
#define st_icon_name_col_number 6
#define st_icon_scroll_width 600
#define st_hitbonus_time_duration 5
#define st_set_game_icon_number 16
#define st_init_set_hint_number_value 5
#define st_hint_time_distance  30
#define st_rateus_day_1  24
#define st_rateus_day_2  (24 * 3)

//define Choose Map Layer row and col
#define st_choose_map_row_num 3
#define st_choose_map_col_num 3


//definr property function
#define st_property(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void)\
{\
return varName; \
}\
public: virtual void set##funName(varType var) \
{\
varName=var; \
}\

//define screen function
#define SCENE_FUNC(__TYPE__) \
static cocos2d::Scene* scene() \
{ \
cocos2d::Scene* lScene=cocos2d::Scene::create();\
__TYPE__ *pRet = new __TYPE__(); \
if (pRet && pRet->init()) \
{ \
pRet->autorelease(); \
lScene->addChild(pRet);\
return lScene; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}\

//define map position struct
typedef struct _name_vec_
{
   std::string imagename;
   Vec2 position;
  cocos2d::Size size;
}name_vec;

//define three mode
typedef enum _game_mode_
{
    K_GAME_MODE_UNKNOWN=0,
    K_GAME_MODE_PICTURE,
    K_GAME_MODE_SILHOUETTE,
    K_GAME_MODE_NAME,
    K_GAME_MODE_MAP
}game_mode;



typedef enum _ParamType_
{
   K_INT_TYPE,
   K_STRING_TYPE,
   K_BOOL_TYPE
}ParamType;

typedef struct _skuInfo_
{
    std::string str_store;
    std::string str_ios_iap;
    std::string str_android_iap;
    std::string str_bg_name;
}SkuInfo;

typedef struct _saveGameData_
{
    bool isAlreadySaveGame;
    bool isAlreadyShowTutorial;
    bool isFirstLoadLevelInfo;
    bool isFirstLoadSaveGameData;
    bool isRemoveAd;
    int hintNumber;
    long recordGameStartTime;
    int isNeverRateUs;
}SaveGameData;

typedef struct _LevelData_
{
    int starStarNum;
    bool isEnabled;
    bool isLocked;
    bool isFinishPictureMode;
    bool isFinishSilhoutetteMode;
    bool isFinishWordMode;
    int highestScore;
}LevelData;

//define level info struct
typedef struct _levelInfo_
{
    std::string str_id;
    std::string str_thumbimage;
    std::string str_isfree;
    std::string str_background;
    std::string str_foldername;
    struct _levelInfo_* nextLevelInfoInfo;
    LevelData levelData;
}levelInfo;
typedef struct _gameInfo_
{
    levelInfo m_levelInfoData;
    levelInfo* m_levelInfo;
    game_mode m_gameMode;
    int m_time;
}GameInfo;
typedef struct _LevelBoxPositionInfo_
{
    Vec2 thumbImagePosition;
    Vec2 leftStarPosition;
    Vec2 centerStarPosition;
    Vec2 rightStarPosition;
    Vec2 lockPosition;
}LevelBoxPositionInfo;

typedef struct _SpriteNamesInfo_
{
    std::vector<std::string> names;
    int length;
}SpriteNamesInfo;

typedef struct _ResumeGameInfo_
{
    std::vector<std::string> names;
    int length;
    int progressValue;
    int gameScore;

}ResumeGameInfo;



//UserDefault
#define boolSavedForKey_False(pKey)                 DataManager::getInstance()->boolDataForKey_False(pKey)
#define boolSavedForKey_True(pKey)                  DataManager::getInstance()->boolDataForKey_True(pKey)
#define setBoolSavedForKey(pKey,pValue)             DataManager::getInstance()->setBoolDataForKey(pKey,pValue)
#define intSavedForKey(pKey)                        DataManager::getInstance()->intDataForKey(pKey)
#define setIntSavedForKey(pKey,pValue)              DataManager::getInstance()->setIntDataForKey(pKey,pValue)
#define stringSavedForKey(pKey)                     DataManager::getInstance()->stringDataForKey(pKey);
#define setStringSavedForKey(pKey,pValue)           DataManager::getInstance()->setStringDataForKey(pKey,pValue)

//function
#define check_assert(target)                        CCASSERT(target!=NULL, "target is NULL")
#endif /* defined(__HIDD__Macro__) */
