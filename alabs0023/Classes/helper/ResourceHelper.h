//
//  ResourceHelper.h
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#ifndef __HIDD__ResourceHelper__
#define __HIDD__ResourceHelper__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#define st_path(filename)  ResourceHelper::getInstance()->getImagePath(filename)
#define st_uimap_path(filename) ResourceHelper::getInstance()->getUIMapImagePath(filename)

class ResourceHelper {
    string ss;
public:
    ResourceHelper();
public:
    static ResourceHelper* getInstance();
    static void releaseResourceHelper();
    
    void setupResourceDir();
    const char* getImagePath(const char* imageName);
    const char* getUIMapImagePath(const char* imagename);
public:
    void setImageRootDir(string dir);
};

#endif /* defined(__HIDD__ResourceHelper__) */
