//
//  Resoure.h
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#ifndef HIDD_Resoure_h
#define HIDD_Resoure_h

#define st_color_system_version                "color_system_version"

//Sound
/*--------------------- sounds--------------------*/
#define st_tap_button_sound                    "sounds/tap_button.mp3"
#define st_hint_sound                          "sounds/hint.mp3"
#define st_gamebg_music                        "sounds/in_game.mp3"
#define st_game_complete_sound                 "sounds/game_complete.mp3"
#define st_game_over_sound                     "sounds/game_over.mp3"
#define st_game_skill_bonus_bound              "sounds/skill_bonus.mp3"
#define st_game_correct_sound                  "sounds/tap_correct.mp3"
#define st_game_error_sound                    "sounds/tap_error.mp3"
#define st_sound_popup_sound                   "sounds/popup.mp3"

#define st_color_map                           "color_map"
#define st_is_setting_back_color               "is_setting_back_color"
#define st_is_showing_tutorial                 "is_showing_tutorial"


//fonts
/* -------------------- fonts -------------------- */

#define st_font_futumd                         "fonts/CrimsonText-Semibold.ttf"
#define st_font_Helvetica_Neue                 ""
#define st_maptile_font_size                   22
#define st_time_font_size                      22
#define st_dialog_text_font_size               33
#define st_gameover_text_font_size             40
#define st_gamecomplete_text_font_size         40
#define st_gamecomplete_text2_font_size        35
#define st_hint_number_font_size               20
//csv
#define st_decription_csv                       "Description.csv"
#define st_displayname_csv                      "DisplayName.csv"
#define st_strings_csv                          "strings/strings.csv"
#define st_sku_csv                              "sku/sku.csv"
// tmx
#define st_objects_name                         "Objects"
#define st_object_name                          "Object"
#define st_bg_objects_name                      "BGObject"
#define st_level_suffix                         ".tmx"
#define st_jpg_suffix                           ".jpg"
#define st_png_suffix                           ".png"
#define st_word_suffix                          "_w"
#define st_picture_suffix                       "_p"
#define st_silhouette_suffix                    "_s"
#define st_map_name_property                    "Name"
#define st_map_id_property                      "ID"


#define st_loading_tmx                          "loading.tmx"
#define st_home_tmx                             "home.tmx"
#define st_home_1_tmx                           "home1.tmx"
#define st_tutorial_tmx                         "tutorial.tmx"
#define st_map_tmx                              "maps.tmx"
#define st_choose_mode_tmx                      "choosemode.tmx"
#define st_gameui_tmx                           "in-game.tmx"
#define st_pause_tmx                            "pause.tmx"
#define st_setting_tmx                          "settings.tmx"
#define st_deluxe_tmx                           "deluxe.tmx"
#define st_gameover_tmx                         "gameover.tmx"
#define st_gamecomplete_tmx                     "gamecomplete.tmx"


//plist file
#define st_load_home_plist                      "load-home.plist"
#define st_pop_window_plist                     "pop-window.plist"
#define st_choose_map_plist                     "choose-map.plist"
#define st_ingame_plist                         "in-game.plist"
#define st_deluxe_plist                         "sku.plist"
#define st_word_plsit_txt                       "word"
#define st_silhouette_plsit_txt                 "silhouette"
#define st_picture_plist_txt                    "picture"
#define st_plist_suffix                         ".plist"

#define st_load_home_pvr                        "load-home.pvr.ccz"
#define st_pop_window_pvr                       "pop-window.pvr.ccz"
#define st_choose_map_pvr                       "choose-map.pvr.ccz"
#define st_ingame_pvr                           "in-game.pvr.ccz"
#define st_deluxe_pvr                           "sku.pvr.ccz"


//loading scene image
#define st_loading_bg_image                     "loading.png"
#define st_loading_progress_bg_image            "ui09_bar_bg.png"
#define st_loading_progress_image               "ui09_loading.png"
#define st_loading_progress_bar_image           "ui09_bar.png"

//home scene image
#define st_home_bg_image                        "ui01_menu_bg.jpg"
#define st_home_logo_image                      "ui01_logo.png"
#define st_home_btn_play_image                  "ui01_btn_play.png"
#define st_home_btn_play_h_image                "ui01_btn_play_h.png"
#define st_home_btn_deluxe_image                "ui01_btn_deluxe.png"
#define st_home_btn_deluxe_h_image              "ui01_btn_deluxe_h.png"
#define st_home_settings_image                  "ui01_btn_settings.png"
#define st_home_settings_h_image                "ui01_btn_settings_h.png"
#define st_home_setting_image                   "ui01_btn_setting.png"
#define st_home_setting_h_image                 "ui01_btn_setting_h.png"
#define st_home_btn_more_image                  "ui01_btn_more.png"
#define st_home_btn_more_h_image                "ui01_btn_more_h.png"
#define st_home_btn_resume_image                "ui01_btn_resume.png"
#define st_home_btn_resume_h_image              "ui01_btn_resume_h.png"
#define st_home_btn_new_game_image              "ui01_btn_new_game.png"
#define st_home_btn_new_game_h_image            "ui01_btn_new_game_h.png"

/// tutorial scene image
#define st_tutorial_1_image                     "tutorial_1.png"
#define st_tutorial_2_image                     "tutorial_2.png"
#define st_tutorial_3_image                     "tutorial_3.png"
#define st_tutorial_4_image                     "tutorial_4.jpg"
#define st_tutorial_5_image                     "tutorial_5.jpg"
#define st_tutorial_6_image                     "tutorial_6.jpg"
#define st_tutorial_skip_image                  "tutorial_skip.png"

//Choose Map Scene Image
#define st_choose_map_bg_image                  "ui03_bg.jpg"
#define st_choose_map_back_btn_image            "ui03_back.png"
#define st_choose_map_back_btn_h_image          "ui03_back_h.png"
#define st_choose_map_dot1_image                "dot_1.png"
#define st_choose_map_dot2_image                "dot_2.png"
#define st_choose_map_frame_image               "ui03_frame.png"
#define st_choose_map_frame_1_image             "ui03_frame_1.png"
#define st_choose_map_frame_2_image             "ui03_frame_2.png"
#define st_choose_map_lock_image                "ui03_lock.png"
#define st_choose_map_star_image                "ui03_star.png"
#define st_choose_map_star_u_image              "ui03_star_u.png"
#define st_choose_map_star_2_image              "ui03_star_2.png"
#define st_choose_map_thumb_image               "ui03_map_thumb.png"

//Choose Mode Scene Image
#define st_choose_model_back_btn_image          "ui03_back.png"
#define st_choose_model_back_btn_h_image        "ui03_back_h.png"
#define st_choose_mode_bg_image                 "ui01_menu_bg.jpg"
#define st_choose_mode_btn_image                "ui02_button.png"
#define st_choose_mode_btn_h_image              "ui02_button_h.png"
#define st_choose_mode_btn_2_image              "ui02_button_2.png"
#define st_choose_mode_btn_3_image              "ui02_button_3.png"
#define st_choose_mode_picture_image            "ui02_picture.png"
#define st_choose_mode_silhouette_image         "ui02_silhouette.png"
#define st_choose_mode_word_image               "ui02_word.png"
#define st_choose_mode_choosemode_image         "ui02_choose_mode.png"
#define st_choose_mode_back_image               "ui03_back.png"

//Game UI Layer
#define st_gameui_bar_image                     "ui04_bar.png"
#define st_gameui_frame_image                   "ui04_frame.png"
#define st_gameui_frame_h_image                 "ui04_frame_h.png"
#define st_gameui_hint_image                    "ui04_hint.png"
#define st_gameui_hint_u_image                  "ui04_hint_u.png"
#define st_gameui_next_image                    "ui04_next.png"
#define st_gameui_next_h_image                  "ui04_next_h.png"
#define st_gameui_number_image                  "ui04_number.png"
#define st_gameui_pause_image                   "ui04_pause.png"
#define st_gameui_plus_image                    "ui04_plus.png"
#define st_gameui_previous_image                "ui04_previous.png"
#define st_gameui_previoue_h_image              "ui04_previous_h.png"
#define st_gameui_wrong_image                   "ui04_wrong.png"
#define st_gameui_font_image                    "ui04_font.png"


//Game Pause Layer

#define st_game_paue_bg_image                   "ui06_pause_bg.png"
#define st_game_pause_play_image                "ui06_play.png"
#define st_game_pause_play_h_image              "ui06_play_h.png"
#define st_game_pause_replay_image              "ui06_replay.png"
#define st_game_pause_replay_h_image            "ui06_replay_h.png"
#define st_game_pause_menu_image                "ui06_menu.png"
#define st_game_pause_menu_h_image              "ui06_menu_h.png"
#define st_game_pause_settings_image            "ui06_settings.png"
#define st_game_pause_settings_h_image          "ui06_settings_h.png"

//Setting Layer
#define st_setting_box_image                    "ui05_box.png"
#define st_setting_bar_bg_image                 "ui07_bar_bg.png"
#define st_setting_progress_image               "ui07_bar.png"
#define st_setting_bar_btn_image                "ui07_button.png"
#define st_setting_bar_bg_2_image               "ui07_bar_bg_2.png"
#define st_setting_progress_2_image             "ui07_bar_2.png"
#define st_setting_bar_btn_2_image              "ui07_button_2.png"
#define st_setting_close_image                  "ui07_close.png"
#define st_setting_close_h_image                "ui07_close_h.png"
#define st_setting_delete_image                 "ui07_delete.png"
#define st_setting_delete_h_image               "ui07_delete_h.png"
#define st_setting_music_image                  "ui07_music.png"
#define st_setting_text_image                   "ui07_settings.png"
#define st_setting_sound_image                  "ui07_sound.png"
#define st_setting_tell_friend_image            "ui07_tell_a_friend.png"
#define st_setting_tell_friend_h_image          "ui07_tell_a_friend_h.png"

//Dialog
#define st_dislog_default_image                 "ui10_popup.png"
#define st_dialog_button_image                  "button.png"
#define st_dialog_button_h_image                "button.png"

//Deluxe Layer
#define st_deluxe_title_image                   "ui08_deluxe.png"
#define st_deluxe_restore_image                 "ui08_btn_restore.png"
#define st_deluxe_restore_h_image               "ui08_btn_restore_h.png"
#define st_deluxe_btn_buy_image                 "ui08_btn_buy.png"
#define st_deluxe_btn_buy_h_image               "ui08_btn_buy_h.png"
#define st_deluxe_close_image                   "ui07_close.png"
#define st_deluxe_close_h_image                 "ui07_close_h.png"
#define st_deluxe_hint_image                    "ui08_hint.png"
#define st_deluxe_noad_image                    "ui08_noad.png"
#define st_deluxe_picutres_image                "ui08_pictures.png"
#define st_deluxe_box_image                     "ui05_box.png"

//GameOver Layer
#define st_gameover_box_image                   "ui05_box.png"
#define st_gameover_title_image                 "ui05_game_over.png"
#define st_gameover_textbg_image                "ui05_text_bg.png"
#define st_gameover_score_image                 "ui05_score.png"
#define st_gameover_home_image                  "ui06_menu.png"
#define st_gameover_home_h_image                "ui06_menu_h.png"
#define st_gameover_replay_image                "ui05_replay.png"
#define st_gameover_replay_h_image              "ui05_replay_h.png"
#define st_gameover_score_value_image           "ui05_score_value.png"

//GameComplete Layer
#define st_gamecomplete_box_image               "ui05_box.png"
#define st_gamecomplete_score_image             "ui05_score.png"
#define st_gamecomplete_time_image              "ui05_time.png"
#define st_gamecomplete_title_image             "ui05_game_complete.png"
#define st_gamecomplete_highscore_image         "ui05_high_score.png"
#define st_gamecomplete_home_image              "ui06_menu.png"
#define st_gamecomplete_home_h_image            "ui06_menu_h.png"
#define st_gamecomplete_replay_image            "ui05_replay.png"
#define st_gamecomplete_replay_h_image          "ui05_replay_h.png"
#define st_gamecomplete_next_image              "ui05_next.png"
#define st_gamecomplete_next_h_image            "ui05_next_h.png"
#define st_gamecomplete_time_value_image        "ui05_time_value.png"
#define st_gamecomplete_score_value_image       "ui05_score_value.png"
#define st_gamecomplete_highscore_value_image    "ui05_highscore_value.png"


//Csv item name
#define st_level_free                           "free"
#define st_level_id                             "resid"
#define st_level_thumbimagename                 "thumbnail"
#define st_level_backroundname                  "background"
#define st_level_foldername                     "folderid"

#define st_level_model                          "level_model"
#define st_level_time                           "level_time"

#define st_level_display_id                     "id"
#define st_level_display_name                   "Name"

#define st_strings_id                           "id"
#define st_strings_name                         "Name"

//string.csv item key
#define st_string_button_yes_key                "button_yes"
#define st_string_button_no_key                 "button_no"
#define st_string_button_ok_key                 "button_ok"
#define st_string_tips_not_play_pre_key         "tips_not_play_pre"
#define st_string_delete_content_key            "delete_content"
#define st_string_restart_content_key           "restart_content"
#define st_string_exit_content_key              "exit_content"
#define st_string_quit_content_key              "quit_content"
#define st_string_restore_no_transaction_key    "restore_no_transaction"
#define st_string_restore_successfully_key      "restore_successfully"
#define st_string_restore_failed_key            "restore_failed"
#define st_string_billing_not_supported_message_key "billing_not_supported_message"
#define st_string_already_purchased_key         "already_purchased"
#define st_string_rate_us1_key                  "rate_us1"
#define st_string_rate_us2_key                  "rate_us2"
#define st_string_email_subject_key             "email_subject"
#define st_string_email_body_key                "email_body"

//sku.csv
#define st_sku_store                            "Store"
#define st_sku_ios                              "iOS"
#define st_sku_android                          "Android"
#define st_sku_bgname                           "BGName"

//Save Data
#define st_savedata_isHaveSavedGame_bool        "gamedata_isHaveSavedGame"
#define st_savedata_isAlradyShowTutorial_bool   "savedata_isAlradyShowTutorial"
#define st_savedata_isfirstloadlevelinfo_bool   "savedata_isfirstloadlevelinfo"
#define st_savedata_isfirstloadsavegamedata_bool "savedata_isfirstloadsavegamedata"
#define st_savedata_isremove_ad_bool            "savedata_isremove_ad"
#define st_savedata_hint_number_int             "savedata_hint_number"
#define st_savedata_record_time_int             "savedata_record_time"
#define st_savedata_isnever_rateus_bool         "savedata_isnever_rateus"
#define st_saveleveldata_starnumber_suffix      "_saveleveldata_star_number"
#define st_saveleveldata_enable_suffix          "_saveleveldata_enabled"
#define st_saveleveldata_locked_suffix          "_saveleveldata_locked"
#define st_saveleveldata_highestscore_suffix    "_saveleveldata_highestscore"
#define st_saveleveldata_picture_model          "_saveleveldata_picture_model"
#define st_saveleveldata_silhouette_model       "_saveleveldata_silhouette_mode"
#define st_saveleveldata_word_model             "_saveleveldata_word_model"
#define st_saveresumeinfodata_name              "saveresumeinfodata_name"
#define st_saveresumeinfodata_length            "saveresumeinfodata_length"
#define st_saveresumeinfodata_progress          "saveresumeinfodata_progress"
#define st_saveresumeinfodata_score             "saveresumeinfodata_score"
#define st_spritenames_info_length              "spritenames_info_length"
#define st_spritenames_info_name                "spritenames_info_name"

//Partical System
#define st_partical_hint_plist                  "particles/hint.plist"
#define st_partical_tap_correct_plist           "particles/tap_correct.plist"
#define st_partical_skill_bonus_plist           "particles/skill_bonus.plist"

#endif
