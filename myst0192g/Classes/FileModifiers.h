//
//  ParseLabel.h
//  HIDD
//
//  Created by Steven.T on 13-6-2.
//
//

#ifndef proj_ios_ParseLabel_h
#define proj_ios_ParseLabel_h


// ===========================================================

/*
 * market macros. when we will publish amazon version, we need add "ST_MARKET_AMAZON" 
 * value in "Application.mk" file, which under the "jni" directory, with param "-DST_MARKET_AMAZON"
 */

// distinguish google play from amazon
#define ST_TARGET_MARKET_APPLE                                 0
#define ST_TARGET_MARKET_GOOGLE_PLAY                           1
#define ST_TARGET_MARKET_AMAZON                                2


#if defined(ST_MARKET_AMAZON)
#undef  ST_TARGET_MARKET
#define ST_TARGET_MARKET        ST_TARGET_MARKET_AMAZON
#elif defined(ST_MARKET_GOOGLE_PLAY)
#undef  ST_TARGET_MARKET
#define ST_TARGET_MARKET        ST_TARGET_MARKET_GOOGLE_PLAY
#endif

// ===========================================================


#define UI_ROOT_2048                                    "ui-2048X1536"
#define UI_ROOT_1024                                    "ui-1024X768"




// file seperator
#define st_file_seperator                               "/"
#define st_file_hyphen                                  "-"

// all maps' root directory
#define st_maps_root                                    "maps"

// MapsConfig.csv file
#define st_maps_config_file                             "MapsConfig.csv"
// the first level label
#define st_maps_config_version                          "Version"
#define st_maps_config_capacity                         "Capacity"
#define st_maps_config_free                             "Free"

// define the labels in "description.plist" file
#define st_maps_descfile_name                           "Description.csv"
// the second level label under the Level
#define st_level_free                                   "free"
#define st_level_location                               "location"
#define st_level_background                             "background"
#define st_level_bg_thumb                               "thumbnail"
#define st_level_folder_id                              "folderid"
#define st_level_res_id                                 "resid"
// the column names in DisplayName.csv file
#define st_strings_root                                 "strings"
#define st_disname_file                                 "DisplayName.csv"
#define st_disname_id                                   "id"
#define st_disname_Name                                 "Name"
#define st_strings_file                                 "strings.csv"
//
#define st_sku_store                                    "Store"
#define st_sku_ios                                      "iOS"
#define st_sku_android                                  "Android"
#define st_sku_bgname                                   "BGName"

// protocol of parse map file.
#define st_suffix_jpg                                   ".jpg"
#define st_suffix_png                                   ".png"
#define st_suffix_csv                                   ".csv"
// level's identifer + this suffix, such as "a1.tmx"
#define st_map_suffix                                   ".tmx"
// the describe file of texture sheet of hidden objects
#define st_map_hidd_objs_suffix                         "_word.plist"
#define st_map_hidd_objs_pvr_ccz                        "_word.pvr.ccz"
#define st_map_hidd_objs_png                            "_word.png"
#define st_map_hidd_objs_pic_suffix                     "_picture.plist"
#define st_map_hidd_objs_pic_png                        "_picture.pvr.ccz"
#define st_map_hidd_objs_silhouette_pic_suffix          "_silhouette.plist"
#define st_map_hidd_objs_silhouette_pic_png             "_silhouette.pvr.ccz"
#define st_map_tips_objs_pic_suffix                     "_p.png"
#define st_map_tips_objs_silhouette_pic_suffix          "_s.png"

// map objects' coord attr
#define st_map_obj_x                                    "x"
#define st_map_obj_y                                    "y"
// hidden objects' pic name without suffix
#define st_map_obj_name                                 "Name"
#define st_map_obj_id                                   "ID"
// object group name about hidden objects
#define st_map_group_obj_name                           "Objects"
// object group name about background
#define st_map_group_bg_name                            "BGObject"
// object group name about animation
#define st_map_group_anim_name                          "Animation"
// animation's first frame name
#define st_map_anim_ff                                  "FirstFrame"
// animation file with suffix
#define st_map_anim_tf                                  "TargetFile"


#define key_first_play_game                             "key_first_play_game"
#define key_iap_purchased_flag                          "iap_purchased_flag"
#define key_iap_ad_purchased_flag                       "key_iap_ad_purchased_flag"
#define key_music_volume                                "key_music_volume"
#define key_sound_volume                                "key_sound_volume"
#define key_rate_us_timestamp                           "key_rate_us_timestamp"
#define key_rate_us_never                               "key_rate_us_never"
// highest score key for save to CCUserDefault
#define key_high_score_normal_prefix                    "NormalHighScore"
#define key_high_score_challenge_prefix                 "ChallengHighScore"
#define key_high_score_blitz_prefix                     "BlitzHighScore"
#define key_hint_quantity                               "key_hint_quantity"
#define key_level_stars                                 "key_level_stars_"
#define key_level_picture                               "key_level_picture_"
#define key_level_silhouette                            "key_level_silhouette_"
#define key_level_word                                  "key_level_word_"
#define key_complete_flag                               "key_complete_flag"


#define strings_button_yes                              "button_yes"
#define strings_button_no                               "button_no"
#define strings_button_ok                               "button_ok"
#define strings_quit_content                            "quit_content"
#define strings_delete_content                          "delete_content"
#define strings_restart_content                         "restart_content"
#define strings_exit_content                            "exit_content"
#define strings_restore_no_transaction                  "restore_no_transaction"
#define strings_restore_successfully                    "restore_successfully"
#define strings_restore_failed                          "restore_failed"
#define strings_billing_not_supported_message           "billing_not_supported_message"
#define strings_already_purchased                       "already_purchased"
#define strings_rate_us1                                "rate_us1"
#define strings_rate_us2                                "rate_us2"
#define strings_email_subject                           "email_subject"
#define strings_email_body                              "email_body"
#define strings_not_play_previous                       "tips_not_play_pre"


/**
 *  help to user UserDefault
 */
#define STSaveStringToUD                        STUserDefault::getInstance()->setStringForKey
#define STSaveIntegerToUD                       STUserDefault::getInstance()->setIntegerForKey
#define STSaveBooleanToUD                       STUserDefault::getInstance()->setBoolForKey
#define STSaveFloatToUD                         STUserDefault::getInstance()->setFloatForKey
#define STLoadStringFromUD                      STUserDefault::getInstance()->getStringForKey
#define STLoadIntegerFromUD                     STUserDefault::getInstance()->getIntegerForKey
#define STLoadBooleanFromUD                     STUserDefault::getInstance()->getBoolForKey
#define STLoadFloatFromUD                       STUserDefault::getInstance()->getFloatForKey

#endif
