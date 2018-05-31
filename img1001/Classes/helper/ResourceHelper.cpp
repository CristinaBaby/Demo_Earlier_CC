//
//  ResourceHelper.cpp
//  HH002
//
//  Created by houhuang on 15/8/26.
//
//

#include "ResourceHelper.h"
#include "DeviceHelper.h"
using namespace std;

static ResourceHelper* _instance = nullptr;

ResourceHelper::ResourceHelper()
{

}

ResourceHelper* ResourceHelper::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new ResourceHelper();
    }
    
    return _instance;
}

void ResourceHelper::releaseResourceHelper()
{
    if (_instance != nullptr)
    {
        delete _instance;
        _instance = nullptr;
    }
}

void ResourceHelper::setUpEnviroment()
{
    Size lSize= Director::getInstance()->getWinSize();
    log("%f,%f",lSize.width,lSize.height);
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (lSize.width > 768 && lSize.height > 1024)
    {
        DeviceHelper::getInstance()->setIs2048Device(true);
    }
    else
    {
       DeviceHelper::getInstance()->setIs2048Device(false);
    }
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
       DeviceHelper::getInstance()->setIs2048Device(false);
    #endif
}


void ResourceHelper::setupResourceDir()
{
    ResourceHelper* lRes = ResourceHelper::getInstance();
//    Size lSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    Size lSize= Director::getInstance()->getWinSize();
    
    vector<string> lSearchPath;
    
    int lRecordDirLength = UserDefault::getInstance()->getIntegerForKey(st_record_dir_length);
    
    for (int i=lRecordDirLength-1; i>=0; i--) {
        stringstream ss;
        ss<<st_record_dir_name;
        ss<<"_";
        ss<<i+1;
        string lDirName=UserDefault::getInstance()->getStringForKey(ss.str().c_str());
        
        string lTargteDir= FileUtils::getInstance()->getWritablePath()+lDirName;
        
        lSearchPath.push_back(lTargteDir);
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    if(DeviceHelper::getInstance()->getIs2048Device())
    {
        string str("ui-2048X1536");
        lRes->setImageRootDir(str);
        lSearchPath.push_back("ui-2048X1536");
        lSearchPath.push_back("ui-2048X1536/maps");
        lSearchPath.push_back("ui-2048X1536/ui-button");
        
        lSearchPath.push_back("ui-2048X1536/maps/Animals");
        lSearchPath.push_back("ui-2048X1536/maps/Flowers");
        lSearchPath.push_back("ui-2048X1536/maps/Ladies");
        lSearchPath.push_back("ui-2048X1536/maps/Mandala");
        lSearchPath.push_back("ui-2048X1536/maps/Mysterious");
        lSearchPath.push_back("ui-2048X1536/maps/Ocean");
        
        lSearchPath.push_back("ui-2048X1536/Frames");
        lSearchPath.push_back("ui-2048X1536/Filters");
        
        lSearchPath.push_back("ui-2048X1536/loading_animation");
        
        lSearchPath.push_back("ui-1024X768");
        lSearchPath.push_back("ui-1024X768/ui-button");
        lSearchPath.push_back("fonts");
        lSearchPath.push_back("ui-1024X768/csv");
        lSearchPath.push_back("ui-1024X768/ColorRGB");
        lSearchPath.push_back("ui-1024X768/music");
        lSearchPath.push_back("ui-1024X768/Filters");

    }
     else
    {
        string str("ui-1024X768");
        lRes->setImageRootDir(str);
        
        lSearchPath.push_back("ui-1024X768");
        lSearchPath.push_back("ui-1024X768/maps");
        lSearchPath.push_back("ui-1024X768/ui-button");
        lSearchPath.push_back("fonts");
        lSearchPath.push_back("ui-1024X768/csv");
        lSearchPath.push_back("ui-1024X768/ColorRGB");
        lSearchPath.push_back("ui-1024X768/maps/Animals");
        lSearchPath.push_back("ui-1024X768/maps/Flowers");
        lSearchPath.push_back("ui-1024X768/maps/Ladies");
        lSearchPath.push_back("ui-1024X768/maps/Mandala");
        lSearchPath.push_back("ui-1024X768/maps/Mysterious");
        lSearchPath.push_back("ui-1024X768/maps/Ocean");
        lSearchPath.push_back("ui-1024X768/Frames");
        lSearchPath.push_back("ui-1024X768/Filters");
        lSearchPath.push_back("ui-1024X768/loading_animation");
        
        lSearchPath.push_back("ui-1024X768/music");
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string str("ui-1024X768");
    lRes->setImageRootDir(str);
    
    lSearchPath.push_back("ui-1024X768");
    lSearchPath.push_back("ui-1024X768/maps");
    lSearchPath.push_back("ui-1024X768/ui-button");
    lSearchPath.push_back("fonts");
    lSearchPath.push_back("ui-1024X768/csv");
    lSearchPath.push_back("ui-1024X768/ColorRGB");
    
    lSearchPath.push_back("ui-1024X768/maps/Animals");
    lSearchPath.push_back("ui-1024X768/maps/Flowers");
    lSearchPath.push_back("ui-1024X768/maps/Ladies");
    lSearchPath.push_back("ui-1024X768/maps/Mandala");
    lSearchPath.push_back("ui-1024X768/maps/Mysterious");
    lSearchPath.push_back("ui-1024X768/maps/Ocean");
    lSearchPath.push_back("ui-1024X768/Frames");
    lSearchPath.push_back("ui-1024X768/Filters");
    lSearchPath.push_back("ui-1024X768/loading_animation");
    
    lSearchPath.push_back("ui-1024X768/music");
    
#endif

    
    FileUtils::getInstance()->setSearchPaths(lSearchPath);
}

const char* ResourceHelper::getIamgePath(const char* iamgeName)
{
    string str = m_sString + "/" + iamgeName;
    
    return str.c_str();
}

const char* ResourceHelper::getUIMapImagePath(const char* iamgeName)
{
    string str = "ui-1024X768/";
    str = str + iamgeName;

    return str.c_str();
}

void ResourceHelper::setImageRootDir(string dir)
{
    m_sString.clear();
    m_sString = m_sString + dir + "/";
}