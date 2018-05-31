//
//  ResourceHelper.cpp
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#include "ResourceHelper.h"
#include "DeviceHelper.h"

USING_NS_CC;

static ResourceHelper* _instance=NULL;

ResourceHelper* ResourceHelper::getInstance()
{
    if(_instance==NULL)
    {
        _instance=new ResourceHelper();
    }
    
    return _instance;
}

void ResourceHelper::releaseResourceHelper()
{
    if(_instance!=NULL)
    {
        delete _instance;
        _instance=NULL;
    }
}

ResourceHelper::ResourceHelper()
{
}

void ResourceHelper::setImageRootDir(string dir)
{
    ss.clear();
    ss=ss+dir+"/";
}


const char* ResourceHelper::getUIMapImagePath(const char *imagename)
{
    string str="ui-1024X768/";
    str=str+imagename;
    return str.c_str();
}

const char* ResourceHelper::getImagePath(const char *imageName)
{
    string str=ss+"/"+imageName;
    return str.c_str();
}

void ResourceHelper::setupResourceDir()
{
    ResourceHelper* lRes=ResourceHelper::getInstance();
    Size lSize= Director::getInstance()->getWinSize();
   
    std::vector<string> lSearchPath;
    lSearchPath.push_back(FileUtils::getInstance()->getWritablePath());
    

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
         string str("ui-1024X768");
         lRes->setImageRootDir(str);
        
         lSearchPath.push_back("ui-1024X768");
         lSearchPath.push_back("ui-1024X768/maps");
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
     string str("ui-1024X768");
     lRes->setImageRootDir(str);
    
      lSearchPath.push_back("ui-1024X768");
      lSearchPath.push_back("ui-1024X768/maps");

#endif
    
    string ldir;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)   
    ldir=FileUtils::getInstance()->getWritablePath()+"color";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
     ldir=FileUtils::getInstance()->getWritablePath()+"/color";
#endif
    
    lSearchPath.push_back(ldir);
    
    FileUtils::getInstance()->setSearchPaths(lSearchPath);

    
    
}