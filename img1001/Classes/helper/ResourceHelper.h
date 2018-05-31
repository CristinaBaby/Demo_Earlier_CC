//
//  ResourceHelper.h
//  HH002
//
//  Created by houhuang on 15/8/26.
//
//

#ifndef __HH002__ResourceHelper__
#define __HH002__ResourceHelper__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#define st_record_dir_length "record_dir_length"
#define st_record_dir_name "record_dir_name"

class ResourceHelper
{
public:
    ResourceHelper();
    
    static ResourceHelper* getInstance();
    static void releaseResourceHelper();
    void setUpEnviroment();
    void setupResourceDir();
    
    const char* getIamgePath(const char* iamgeName);
    const char* getUIMapImagePath(const char* iamgeName);

    void setImageRootDir(string dir);
    
private:
    string m_sString;

};

#endif /* defined(__HH002__ResourceHelper__) */
