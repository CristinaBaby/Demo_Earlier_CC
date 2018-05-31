//
//  DownloadPage.hpp
//  ColorGarden
//
//  Created by lidong on 15/10/15.
//
//

#ifndef DownloadPage_hpp
#define DownloadPage_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../cocos2d/cocos/network/HttpRequest.h"
#include "../../cocos2d/cocos/network/HttpClient.h"
//#include "network/HttpRequest.h"
//#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace network;

typedef struct _versionCsvInfo
{
    std::string versionStr;
    std::string imageSizeStr;
    std::string packFullName;
    
}VersionCsvInfo;

typedef struct _InspirationInfo
{
    std::string imageUrl;
    long int    ID;
    int         likeNumber;

}InspirationInfo;

typedef struct _DownloadImageInfo
{
    std::string imageUrl;
    int imageID;
    
}DownloadImageInfo;

typedef struct _FavoriteInfo
{
    long int imageID;
    int likeNumber;
}FavoriteInfo;

class DownLoadPage  : public Layer, cocos2d::extension::AssetsManagerDelegateProtocol{
    
public:
    static DownLoadPage* getInstance();
    
public:
    DownLoadPage();
    ~DownLoadPage();
protected:
    void initDownloadDir();
public:
    string getCurrentResourceVersion();
    void setCurrentResourceVersion(string versionStr);
    void updateNewVersionCsv();
    void checkVersionCsv();
    static void reset(Ref *pSender);
    void reSetSreachDir();
    void reSetSreachDir2(int index);
    void startDownload();
    void excuteDownload();
    
    //post
    void postImage(const string& image);
    static size_t reponsePostImage(uint8_t *dataBack, size_t size, size_t nmemb, void *user_p);
    
    //inspiration
    void downLoadInspirationInfo(size_t pIdx);
    void firstDownLoadInspirationInfo();
    bool createDirectory(const char *path);
    void downLoadImage(string url, int imageName);
    
    void startLoadImage();
    
    void postClickLike(int imageID);
    void getLikeNumber(int imageID);
    
    void updateNeedGetLikeImage(int imageID);
    
    void saveInspirationDatas();
    void readInspitationDatas();
    void removeNullFile();
    
public:
    virtual void onError(cocos2d::extension::AssetsManager::ErrorCode errorCode); //错误信息
    virtual void onProgress(int percent); //更新下载进度
    virtual void onSuccess(); //下载成功
private:
    std::string _pathToSave;
    std::vector<VersionCsvInfo>  _downloadPackageInfoVec;
    std::vector<VersionCsvInfo> _needDownVector;
    
    int _downloadCount;
    
    std::vector<AssetsManager*> _assertManagers;
    
    std::string m_sPostImageName;
    
public:
    string inspirationSavePath;
    static string m_sInspirationInfo;
    std::vector<InspirationInfo> m_vInspirationInfo;
    int m_iImageID;
    
    std::vector<int> m_vInsImageNameInfo;
    std::vector<int> m_vInsImageLikeNum;
    
    static bool m_bCurlIspostFaild;
    int _lastDownRequestID;
    bool _isRequestingAsk;
    
    std::vector<DownloadImageInfo> m_mNeedDownloadImage;
    bool  m_bIsFinishDownloadImage;
    
//    std::map<int, int> m_mFavoriteInfo;
    std::vector<FavoriteInfo> m_vFavoriteInfo;
    std::vector<int> m_vNeedGetLikeImage;
    bool m_bIsDownloadLike;
    int m_sGetLikeOfImageName;
    
    std::vector<int> m_vPostImageId;
    static int m_iCurrentPostImageId;
    
    int m_iCurrentDownloadIndex;
    
    static int m_iNextPageImageNumber;
};

#endif /* DownloadPage_hpp */
