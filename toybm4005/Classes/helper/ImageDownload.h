//
//  ImageDownload.hpp
//  ColorGarden
//
//  Created by lidong on 15/11/19.
//
//

#ifndef ImageDownload_hpp
#define ImageDownload_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "DataManager.h"

typedef enum _DownState_
{
   START_STATE=0,
   STRAT_CHECK_VERSION_STATE,
   DOWNLOAD_FAIL_STATE,
   EXIST_NEW_VERSION_STATE,
   NO_NEW_VERSION_STATE,
   CANCEL_DOWNLOAD_STATE,
   START_DOWNLOAD_STATE,
   FINISH_DOWNLOAD_STATE,
   ASK_NET_FAIL_STATE
}DownState;


class ImageDownload  {
    
    std::string _csvDataBuffer;
    int         _index;
    
public:
    std::string _savePath;
    float _targetVersion;
    
    std::vector<ResourceInfo> _resourceInfos;
    std::vector<ResourceInfo> _resourceInfos2;
    
    DownState _currentDownState;
    
    int _downloadNum;
    
    std::vector<ResourceInfo> _downloadFinishResourceInfo;
    
public:
    bool init();
    static ImageDownload* getInstance();
    void excuteFunc(DownState pDownState);
    DownState getCurrentState();
    void setCurrentState(DownState pState);
    void refreshData();
protected:
    void downloadCsv();
    void checkIsNeedUpdate();
    void readCsvData();
    void readCsvData2();
    bool createDirectory(const char *path);
    void startDownload();
    void startDownload(string filename);

};


#endif /* ImageDownload_hpp */
