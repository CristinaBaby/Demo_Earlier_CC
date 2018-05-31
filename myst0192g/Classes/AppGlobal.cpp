//
//  AppGlobal.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-24.
//
//

#include "AppGlobal.h"

USING_NS_CC;

bool AppGlobal::IS_2048 = false;
float AppGlobal::UI_SCALE = 0.f;

CCArray*       AppGlobal::stAllLevelsInformation            = NULL;
CCDictionary*  AppGlobal::stMapsConfig                      = NULL;
CCDictionary*  AppGlobal::stAllStrings                      = NULL;
CCArray*       AppGlobal::stSKUInformation                  = NULL;