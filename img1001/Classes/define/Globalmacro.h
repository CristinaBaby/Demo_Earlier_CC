//
//  Globalmacro.h
//  HH002
//
//  Created by houhuang on 15/8/25.
//
//

#ifndef __HH002__Globalmacro__
#define __HH002__Globalmacro__

#include <stdio.h>

#define st_loading_DownLoadResoure_Fai          "loading_DownLoadResoure_Fai"
#define st_loading_DownLoadResoure_Suc          "loading_DownLoadResoure_Suc"

#define st_datamanager_dynamicLoadTexture       "datamanager_loadtexture"

#define st_filterAndFrames_LoadRewardSu         "filterAndFrames_LoadRewardSu"
#define st_filterAndFrames_notifiction          "filterAndFrames_notifction"
#define st_filterAndFrames_downLoadSuc          "st_filterAndFrames_downLoadSuc"

#define st_colorScrollLayer_notifiction         "colorScrollLayer_notifiction"
#define st_scrollmaplayer_notifiction           "scroll_layer_notifiction"
#define st_pauselayer_request_ad_notification   ""

//my post
#define st_myPost_success                       "st_myPost_success"
#define st_myPost_failed                        "st_myPost_failed"

//Inspiration
#define st_inspiration_downloadSuc                  "st_inspiration_downloadSuc"
#define st_inspirationLayer_downLoadJson_suc        "st_inspirationLayer_downLoadJson_suc"
#define st_inspirationLayer_firstDownLoadJson_suc   "st_inspirationLayer_firstDownLoadJson_suc"
#define st_inspirationLayer_downLoadJson_fai        "st_inspirationLayer_downLoadJson_fai"
#define st_FavoriteSprite_getLikeNumber_suc         "st_FavoriteSprite_getLikeNumber_suc"

//DownLoad new Version
#define st_baseScene_finfNewVersion_notifition  "st_baseScene_findNewVersion_notifition"
#define st_baseScene_downloadSuc_notifition     "st_baseScene_downloadSuc_notifition"

#define st_MyArtworks_deleteImage               "st_MyArtworks_deleteImage"
#define st_color_pack_Impressionism             "st_color_pack_Impressionism"
#define st_color_pack_Flower                    "st_color_pack_Flower"

//UserDefault Datas/ inspiration post favorite
#define st_Inspiration_length                   "st_Inspiration_length"
#define st_Posts_length                         "st_Posts_length"
#define st_PostsId_length                       "st_PostsId_length"
#define st_Favorite_length                      "st_Favorite_length"
#define st_MyArtworks_length                    "st_MyArtworks_length"


#define st_dialog_tag   999

#define st_design_screen_width  768
#define st_design_screen_height 1024

#define st_map_sprite_width 580
#define st_map_height       850

#define st_editlayer_width  590

#define DM_getSaveBackColor     DataManager::getInstance()->getSaveBackColor(DataManager::getInstance()->m_pCurrentImage.imageName, DataManager::getInstance()->m_pCurrentImage.ID)
#define DM_saveBackColor(d,a)   DataManager::getInstance()->saveBackColor(d,a)

#define DM_getInstance          DataManager::getInstance()

#define st_map_islock   1
#define st_showingtutorial "is_showing_tutorial"
#define st_tableview_offset "TableView_Offset"

#define st_color_scroll_pagedot 8
#define st_is_setting_back_color    "is_setting_backcolor"
//#define st_Return_intoGame  "return_into_game"

#define st_IsPlayBackgrioundMusic   "is_playMusic"

#define st_font_PTS "PTSans_0.ttf"
#define st_font_AR  "Alice-Regular.ttf"

#define st_gallery_csv      "Gallery.csv"
#define st_Animals_csv      "Animals.csv"
#define st_Flowers_csv      "Flowers.csv"
#define st_Ladies_csv       "Ladies.csv"
#define st_Mandala_csv      "Mandala.csv"
#define st_Mysterious_csv   "Mysterious.csv"
#define st_Ocean_csv        "Ocean.csv"

#define DM_FlurryDatas   DataManager::getInstance()->m_vFlurryDatas

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

#endif /* defined(__HH002__Globalmacro__) */
