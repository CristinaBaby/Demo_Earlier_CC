//
//  ImageDownload.cpp
//  ColorGarden
//
//  Created by lidong on 15/11/19.
//
//

#include "ImageDownload.h"
#include "GlobalMacro.h"
#include "DataManager.h"
#include "../layer/Dialog.h"
#include "CSVParse.h"
#include <stdlib.h>
#include "../define/ResoureMacro.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <vector>
#endif

using namespace cocos2d;
using namespace network;
using namespace std;
USING_NS_CC_EXT;

static ImageDownload* _instance = NULL;

#define st_csv_file_name "color_1.csv"
#define st_csv_file_name_spare "color_spare.csv"
#define st_version_file_name "version_1.php"


#define st_versionCsvUrl "http://liveupdate.toyboxapps.com/coloring/IMG0023/color/color_1.csv"
#define st_versionUrl "http://liveupdate.toyboxapps.com/coloring/IMG0023/color/version_1.php"
#define st_download_dir "http://liveupdate.toyboxapps.com/coloring/IMG0023/color"

bool ImageDownload::_isDownloading = false;


ImageDownload* ImageDownload::getInstance()
{
    if (_instance == NULL)
    {
        _instance = new ImageDownload();
        _instance->init();
    }
    
    return _instance;
}

bool ImageDownload::init()
{
    _savePath=FileUtils::getInstance()->getWritablePath();
    _downloadNum=0;
    
    _resourceInfos.clear();
    _downloadFinishResourceInfo.clear();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _savePath+="color";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _savePath+="/color";
#endif
    
    this->createDirectory(_savePath.c_str());
    
    _currentDownState=START_STATE;
    
    std::vector<std::string> paths=FileUtils::getInstance()->getSearchPaths();
    paths.push_back(_savePath);
    
   
    
    return true;
}

void ImageDownload::excuteFunc(DownState pDownState)
{
    _currentDownState=pDownState;
    switch (_currentDownState) {
        case START_STATE:
        {
          
        }
            break;
        case STRAT_CHECK_VERSION_STATE:
        {
             this->checkIsNeedUpdate();
        }
            break;
        
        case EXIST_NEW_VERSION_STATE:
        {
            _isDownloading = true;
            this->downloadCsv();
        }
            break;
        case DOWNLOAD_FAIL_STATE:
        case NO_NEW_VERSION_STATE:
        case CANCEL_DOWNLOAD_STATE:
        {
            this->readCsvData();
            this->refreshData();
            
            NotificationCenter::getInstance()->postNotification(st_no_new_version_readcsv_finish_notification);
            
        }
            break;

        case START_DOWNLOAD_STATE:
        {
            this->startDownload();
        }
            break;
        case FINISH_DOWNLOAD_STATE:
        {
            _isDownloading = false;
            
            std::string lPath=_savePath+"/"+st_csv_file_name;
            FILE *fp = fopen(lPath.c_str(), "wb+");
            fwrite(_csvDataBuffer.c_str(),1,_index, fp);
            fclose(fp);
            
            this->readCsvData();
            
            this->refreshData();
        }
            break;
        default:
            break;
    }
}

DownState ImageDownload::getCurrentState()
{
    return _currentDownState;
}

void ImageDownload::setCurrentState(DownState pState)
{
    _currentDownState=pState;
}

void ImageDownload::checkIsNeedUpdate()
{
    HttpRequest* lRequest=new HttpRequest;
    string lPath=st_download_dir;
    lPath+="/";
    lPath+=st_version_file_name;
    
    lRequest->setUrl(lPath.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this,lRequest](network::HttpClient* client, network::HttpResponse* response) {
        if (!response)
        {
            CCLOG("response failed");
            lRequest->release();
            
            ImageDownload::getInstance()->excuteFunc(DOWNLOAD_FAIL_STATE);
            
            return;
        }
        
        if (!response->isSucceed())
        {
            CCLOG("err0r=%s",response->getErrorBuffer());
            CCLOG("response failed   获取网络资源失败");
            lRequest->release();
            
             ImageDownload::getInstance()->excuteFunc(DOWNLOAD_FAIL_STATE);
            return;
        }
        std::vector<char> *buffer = response->getResponseData();
        std::string bufffff(buffer->begin(),buffer->end());
        if(!bufffff.empty())
        {
            float versionNumber=atof(bufffff.c_str());
            versionNumber=versionNumber*10;
            int versionValue=(int)versionNumber;
            
            float lSystemVersion=DataManager::getInstance()->getColorSystemVersion();
            int lSystemVersionValue=(int)(lSystemVersion*10);
            
            if(versionValue>lSystemVersionValue)
            {
                _targetVersion=versionNumber*0.1f;
                _currentDownState=EXIST_NEW_VERSION_STATE;

                Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
                   NotificationCenter::getInstance()->postNotification(st_check_is_download_notification);
                });

            }
            else
            {
                this->excuteFunc(NO_NEW_VERSION_STATE);
            }
        }
        
        lRequest->release();
    });
    
    HttpClient::getInstance()->send(lRequest);
}



void ImageDownload::downloadCsv()
{
    HttpRequest* lRequest=new HttpRequest;
    
    string lPath=st_download_dir;
    lPath+="/";
    lPath+=st_csv_file_name;
    
    lRequest->setUrl(lPath.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this,lRequest](network::HttpClient* client, network::HttpResponse* response) {
    
        if (!response)
        {
            CCLOG("response failed");
            lRequest->release();
            //NotificationCenter::getInstance()->postNotification(st_load_csv_fail_notification);
            return;
        }
        
        if (!response->isSucceed())
        {
            CCLOG("error=%s",response->getErrorBuffer());
            
            CCLOG("response failed   获取网络资源失败");
           // NotificationCenter::getInstance()->postNotification(st_load_csv_fail_notification);
            lRequest->release();
            return;
        }
        
        std::vector<char> *buffer = response->getResponseData();
        std::string bufffff(buffer->begin(),buffer->end());
        _csvDataBuffer=bufffff;
        _index = buffer->size();
    
        std::string lPath=_savePath+"/"+st_csv_file_name_spare;
        FILE *fp = fopen(lPath.c_str(), "wb+");
        fwrite(bufffff.c_str(),1,buffer->size(), fp);
        fclose(fp);
        
        this->readCsvData2();
        
       // DataManager::getInstance()->setColorSystemVersion(_targetVersion);
          lRequest->release();
        
        this->startDownload();
        
        
        
        
    });
    HttpClient::getInstance()->send(lRequest);
    
}

void ImageDownload::readCsvData()
{
    _resourceInfos.clear();
    
    std::string lPath=_savePath+"/"+st_csv_file_name;
    
    CSVParse* lCsv=CSVParse::create(lPath.c_str());
    
    if(lCsv==NULL)
    {
        return;
    }
    
    const unsigned int row=lCsv->getRows();
    for (int r=1; r<row; r++) {
        ResourceInfo lInfo;
        lInfo.image_name =lCsv->getDatas(r, "name");
        lInfo.image_index=DataManager::getInstance()->_resourceInfos.size()+r;
        
        lInfo.image_name_w = lInfo.image_name + "_mask.png";
        lInfo.image_name += ".png";
        _resourceInfos.push_back(lInfo);
    }
}

void ImageDownload::readCsvData2()
{
    _resourceInfos2.clear();
    
    std::string lPath=_savePath+"/"+st_csv_file_name_spare;
    
    CSVParse* lCsv=CSVParse::create(lPath.c_str());
    
    if(lCsv==NULL)
    {
        return;
    }
    
    const unsigned int row=lCsv->getRows();
    for (int r=1; r<row; r++) {
        ResourceInfo lInfo;
        lInfo.image_name =lCsv->getDatas(r, "name");
        lInfo.image_index=DataManager::getInstance()->_resourceInfos.size()+r;
        
        lInfo.image_name_w = lInfo.image_name + "_mask.png";
        lInfo.image_name += ".png";
        _resourceInfos.push_back(lInfo);
    }
}

bool ImageDownload::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, nullptr);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#endif
    
}

void ImageDownload::startDownload()
{
    _downloadNum=0;
    for (int i=0; i<_resourceInfos.size(); i++) {
        ResourceInfo* lInfo=&_resourceInfos.at(i);
        std::string lPath=_savePath+"/"+lInfo->image_name;
        if(!FileUtils::getInstance()->isFileExist(lPath))
        {
            _downloadNum++;
            this->startDownload(lInfo->image_name);
            CCLOG("_downloadNum=%d",_downloadNum);
        }
        
        std::string lPathW=_savePath+"/"+lInfo->image_name_w;
        if(!FileUtils::getInstance()->isFileExist(lPathW))
        {
            _downloadNum++;
            this->startDownload(lInfo->image_name_w);
            CCLOG("_downloadNum=%d",_downloadNum);
        }
       
    }
    
    if(_downloadNum==0)
    {
        _currentDownState=FINISH_DOWNLOAD_STATE;
        DataManager::getInstance()->setColorSystemVersion(_targetVersion);
        NotificationCenter::getInstance()->postNotification(st_download_finish_notification);
    }
}

void ImageDownload::startDownload(string filename)
{

    string lPath=st_download_dir;
    lPath+="/";
    lPath+=filename;

    HttpRequest* lRequest=new HttpRequest;
    lRequest->setUrl(lPath.c_str());
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this,lRequest](network::HttpClient* client, network::HttpResponse* response) {
        if (!response)
        {
            CCLOG("response failed");
            lRequest->release();
            //NotificationCenter::getInstance()->postNotification(st_load_csv_fail_notification);
            return;
        }

        if (!response->isSucceed())
        {
            CCLOG("error=%s",response->getErrorBuffer());
            
            CCLOG("response failed   获取网络资源失败");
            // NotificationCenter::getInstance()->postNotification(st_load_csv_fail_notification);
            lRequest->release();
            return;
        }
        
        std::vector<char> *buffer = response->getResponseData();
        std::string bufffff(buffer->begin(),buffer->end());
        
        std::string lPath=_savePath+"/"+lRequest->getTag();
        FILE *fp = fopen(lPath.c_str(), "wb+");
        fwrite(bufffff.c_str(),1,buffer->size(), fp);
        fclose(fp);
        
        lRequest->release();
        
        _downloadNum--;
        
        CCLOG("_downloadNum=%d",_downloadNum);
        
        if(_downloadNum<=0)
        {
            _currentDownState=FINISH_DOWNLOAD_STATE;
            DataManager::getInstance()->setColorSystemVersion(_targetVersion);
            NotificationCenter::getInstance()->postNotification(st_download_finish_notification);
        }
    });
    
    lRequest->setTag(filename.c_str());
    HttpClient::getInstance()->send(lRequest);
}

void ImageDownload::refreshData()
{
    _downloadFinishResourceInfo.clear();
    for (int i=0; i<_resourceInfos.size(); i++) {
        ResourceInfo lInfo=_resourceInfos.at(i);
        _downloadFinishResourceInfo.push_back(lInfo);
    }
}
