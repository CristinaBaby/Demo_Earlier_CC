//
//  AppGlobal.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-24.
//
//

#ifndef __LVUP002__AppGlobal__
#define __LVUP002__AppGlobal__

#include <cocos2d.h>
#include "FileModifiers.h"



/* -------------------- fonts -------------------- */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define font_Helvetica_Neue                     "Lucida Bright"

#else

#define font_Helvetica_Neue                     "fonts/Lucida Bright.ttf"

#endif // end fonts definition
// -------------------------------------------------



#define GET_CCDIRCTOR                           CCDirector::sharedDirector()


#define CCRectPointSizeMake(point, size)        CCRect( ((CCPoint)(point)).x, ((CCPoint)(point)).y, ((CCSize)(size)).width, ((CCSize)(size)).height )


#define VISIBLE_RECT                            CCRectPointSizeMake((GET_CCDIRCTOR->getVisibleOrigin()), (GET_CCDIRCTOR->getVisibleSize()))


#define fontsize(size)                          (AppGlobal::IS_2048) ? size : (float)(size / 2)


#define ccp2(X,Y)                               (AppGlobal::IS_2048 ? (CCPointMake((float)(X), (float)(Y))):(CCPointMake((float)(X / 2), (float)(Y / 2))))

#define ccp2_1024(X,Y)                          (AppGlobal::IS_2048 ? (CCPointMake((float)(X) * 2, (float)(Y) * 2)):(CCPointMake((float)(X), (float)(Y))))

#define CCSizeMake2(width, height)              (AppGlobal::IS_2048 ? CCSize((float)(width), (float)(height)):CCSize((float)(width / 2), (float)(height / 2)))

#define ccp_center_screen                       CCPointMake(VISIBLE_RECT.getMidX(), VISIBLE_RECT.getMidY())

#define ccp_horizontal_center(Y)                (AppGlobal::IS_2048 ? (CCPointMake(VISIBLE_RECT.getMidX(), (float)(Y))) : (CCPointMake(VISIBLE_RECT.getMidX(), (float)(Y / 2))))

#define ccp_vertical_center(X)                  (AppGlobal::IS_2048 ? (CCPointMake((float)(X), VISIBLE_RECT.getMidY())) : (CCPointMake((float)(X / 2), VISIBLE_RECT.getMidY())))

#define ccp_fixed_X(X, Y)                       (AppGlobal::IS_2048 ? (CCPointMake((float)(X), (float)(Y))) : (CCPointMake((float)(X), (float)(Y / 2))))

#define ccp_fixed_Y(X, Y)                       (AppGlobal::IS_2048 ? (CCPointMake((float)(X), (float)(Y))) : (CCPointMake((float)(X / 2), (float)(Y))))

#define ccp_get_center(size)                    CCPointMake( (((CCSize)(size)).width / 2), (((CCSize)(size)).height / 2) )

#define get_offset(delta)                       ((AppGlobal::IS_2048) ? delta : (float)(delta / 2))




#define ANIMATION_DURING                        0.4



/**
 * @brief Search mode, affect the content of bottom bar
 */
typedef enum
{
    Picture, Silhouette, Name,
} SearchMode;

/**
 * @brief maps' file storage location
 */
typedef enum {Inerval, External} FileLocation;


enum
{
    tag_scene_home = 10, tag_scene_game,
};


class AppGlobal
{
    AppGlobal();
public:
    static bool                         IS_2048;
    static float                        UI_SCALE;
    
    static cocos2d::CCArray             *stAllLevelsInformation;
    static cocos2d::CCDictionary        *stMapsConfig;
    static cocos2d::CCArray             *stSKUInformation;
    static cocos2d::CCDictionary        *stAllStrings;
};

#endif /* defined(__LVUP002__AppGlobal__) */
