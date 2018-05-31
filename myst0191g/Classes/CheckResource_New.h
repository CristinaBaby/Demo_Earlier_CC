//
//  CheckFile.h
//  TestGame
//
//  Created by lidong on 15-1-5.
//
//

#ifndef __TestGame__CheckFile__
#define __TestGame__CheckFile__

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "cocos2d.h"
#define VERSION_SUPPORT_215





//调用函数
void checkResource(int mapLevel);
//

void scanPlistContentFile_Func(std::vector<std::string> plistVector,std::string mPlistFilename,std::string dir);
void scanPlistContentFile_Func(std::string* pScanPlistContentFiles,int scanPlistContentLength,std::string mPlistFilename,std::string dir);

void checkMapLevel(std::string dir,int currentLevel,bool checkIsIpadHD);
void checkMap(std::string dir,int mapLevel,bool checkIsIpadHD);

void checkResourceUI_2048(std::string dir,int mapLevel);
void checkResourceUI_1024(std::string dir,int mapLevel);


#endif /* defined(__TestGame__CheckFile__) */
