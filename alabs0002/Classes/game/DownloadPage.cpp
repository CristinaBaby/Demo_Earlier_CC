//
//  DownloadPage.cpp
//  ColorGarden
//
//  Created by lidong on 15/10/15.
//
//

#include "DownLoadPage.h"
#include "MyArtDataManager.hpp"
#include "CSVParse.h"
//#include <curl/curl.h>
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
//#include <sys/stat.h>
#include <errno.h>
//#include <dirent.h>
#include <vector>
#endif


#include <curl/curl.h>
#include "cocostudio/CocoStudio.h"



using namespace cocos2d;
using namespace network;
using namespace std;
USING_NS_CC_EXT;

static DownLoadPage* _instance = NULL;
string DownLoadPage::m_sInspirationInfo = "";

bool DownLoadPage::m_bCurlIspostFaild = false;
int DownLoadPage::m_iCurrentPostImageId = -1;

int DownLoadPage::m_iNextPageImageNumber = 0;

DownLoadPage* DownLoadPage::getInstance()
{
    if (_instance == NULL)
    {
        _instance = new DownLoadPage();
    }
    
    return _instance;
}

DownLoadPage::DownLoadPage():_downloadCount(0)
{

    m_sPostImageName = "";
    _isRequestingAsk = false;
    m_bIsFinishDownloadImage = false;
    m_bIsDownloadLike = false;
    m_sGetLikeOfImageName = -1;
//    m_iCurrentPostImageId = -1;
    m_iCurrentDownloadIndex = 0;
    
    this->readClickLikeInfo();
}

DownLoadPage::~DownLoadPage()
{
    while (_assertManagers.size()>0) {
        AssetsManager* lAssertsManager=_assertManagers.at(_assertManagers.size()-1);
        _assertManagers.pop_back();
        CC_SAFE_DELETE(lAssertsManager);
    }
    this->saveClickLikeInfo();
}


#pragma mark - Init Set

void DownLoadPage::postImage(const string& image)
{
    m_bCurlIspostFaild = false;
    m_sPostImageName = image;
    string lPath;

    lPath = FileUtils::getInstance()->getWritablePath() + image;

    CURLcode cURLcode;
    CURL *cURL;
    
    //Init
    cURLcode = curl_global_init(CURL_GLOBAL_SSL);
    if(CURLE_OK != cURLcode)
    {
        curl_global_cleanup();
        return ;
    }
    
    cURL = curl_easy_init();
    if(!cURL)
    {
        curl_easy_cleanup(cURL);
        curl_global_cleanup();
        return ;
    }
    curl_httppost *post = NULL;
    curl_httppost *last = NULL;
    
    //上传的文件的全路径并指定类型
    curl_formadd(&post, &last,CURLFORM_COPYNAME,"user-file",
                 CURLFORM_FILE,lPath.c_str(),
                 CURLFORM_CONTENTTYPE,"Image/png",CURLFORM_END);
    
    ////设置多个参数
    //curl_formadd(&post, &last,CURLFORM_COPYNAME,"username",
    //            CURLFORM_COPYCONTENTS,"你好",CURLFORM_END);
    
//    curl_easy_setopt(cURL, CURLOPT_URL,"http://10.0.18.2:8000/red/user/upload?bundle_id=colorfree&version=v1.0");
    

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    curl_easy_setopt(cURL, CURLOPT_URL,"http://color-origin.applabsinc.net/red/user/upload?bundle_id=android_enchantedforest&version=v1.0");
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    curl_easy_setopt(cURL, CURLOPT_URL,"http://color-origin.imagineappsltd.com/red/user/upload?bundle_id=colorfree&version=v1.0");
//#endif
    
    curl_easy_setopt(cURL, CURLOPT_TIMEOUT,10);//超时时间10秒
    curl_easy_setopt(cURL, CURLOPT_HTTPPOST,post);//请求方式POST
    curl_easy_setopt(cURL, CURLOPT_WRITEFUNCTION,reponsePostImage); //设置回调
    curl_easy_setopt(cURL, CURLOPT_VERBOSE,1); //非零值表示你想CURL报告每一件意外的事情
    
    
    CURLcode res=curl_easy_perform(cURL);
//    log("%u",res);
    
    log("1111111111111111111111111111");
    if (res != CURLE_OK)
    {log("222222222222222222222222222");
        if (m_bCurlIspostFaild == false)
        {
            log("33333333333333333333333333333");
            m_bCurlIspostFaild = true;
            NotificationCenter::getInstance()->postNotification(st_myPost_failed);
        }

    }
    
    curl_easy_cleanup(cURL);
    
    curl_global_cleanup();
}


size_t DownLoadPage::reponsePostImage(uint8_t *dataBack, size_t size, size_t nmemb, void *user_p)
{
    string szData = string((char*)dataBack);
    
    string lBuffer = szData.substr(0, szData.find('}')+1);
    
    log("%s",lBuffer.c_str());
    
    log("444444444444444444444444444444444444");
    //解析Json 文件
    rapidjson::Document _doc;
    
    _doc.Parse<0>(lBuffer.c_str());

    rapidjson::Value& lDoc = _doc["status"];
    
    if (strcmp(lDoc.GetString(), "ok") == 0)
    {
        rapidjson::Value& lId = _doc["id"];
        
        m_iCurrentPostImageId = lId.GetInt();
        
//        log("-----%d",m_iCurrentPostImageId);
        
        if (m_bCurlIspostFaild == false)
        {
            MyArtData* lMyArtData = &MyArtDataManager::getInstance()->m_vMyArtDatas.at(MyArtDataManager::getInstance()->m_vMyArtDatas.size() - 1);
            lMyArtData->serveId = m_iCurrentPostImageId;
            
            m_bCurlIspostFaild = true;
            NotificationCenter::getInstance()->postNotification(st_myPost_success);
            
        }

        return size*nmemb;
        log("post success");
//        DM_getInstance->m_vMyPostImage.push_back(m_sPostImageName);
    }else
    {
        if (m_bCurlIspostFaild == false)
        {
            m_bCurlIspostFaild = true;
            NotificationCenter::getInstance()->postNotification(st_myPost_failed);
            log("post faild");
        }
        
    }
 

    return 0;
}

void DownLoadPage::postClickLike(int imageID)
{

    stringstream ss;
    ss<<"";
    ss<<"http://color-origin.applabsinc.net/red/res/images/inspiration/";
    ss<<imageID;
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ss<<"/like?bundle_id=android_enchantedforest&version=v1.0";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    ss<<"/like?bundle_id=colorfree&version=v1.0";
//#endif
    
    HttpRequest* lRequest=new HttpRequest;
    lRequest->setUrl(ss.str().c_str());
    
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this,lRequest](network::HttpClient* client, network::HttpResponse* response) {
        if (!response)
        {
            CCLOG("response failed");
            
            
//            NotificationCenter::getInstance()->postNotification(st_inspirationLayer_downLoadJson_fai);
            lRequest->release();
            return;
        }
        
        if (!response->isSucceed())
        {
            CCLOG("response failed   获取网络资源失败");
//            NotificationCenter::getInstance()->postNotification(st_inspirationLayer_downLoadJson_fai);
            lRequest->release();
            return;
        }
        
        std::vector<char> *buffer = response->getResponseData();
        std::string bufffff(buffer->begin(),buffer->end());
        m_sInspirationInfo = bufffff;
        
        rapidjson::Document _doc;
        _doc.Parse<0>(bufffff.c_str());

        rapidjson::Value& lDoc = _doc["status"];
        
        if (strcmp(lDoc.GetString(), "ok") == 0)
        {
            log("post success");
        }else
        {
            log("post faild");
        }
        
        lRequest->release();
    });
    HttpClient::getInstance()->send(lRequest);
    

}

void DownLoadPage::getLikeNumber(int imageID)
{

    if (m_bIsDownloadLike)
    {
        return;
    }
    
    m_sGetLikeOfImageName = imageID;
    m_bIsDownloadLike = true;
    
    stringstream ss;
    ss<<"";
    ss<<"http://color-origin.applabsinc.net/red/res/images/inspiration/";
    ss<<imageID;

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ss<<"/info?bundle_id=android_enchantedforest&version=v1.0";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    ss<<"/info?bundle_id=colorfree&version=v1.0";
//#endif

    HttpRequest* lRequest=new HttpRequest;
    lRequest->setUrl(ss.str().c_str());
    
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this,lRequest](network::HttpClient* client, network::HttpResponse* response) {
        if (!response)
        {
            CCLOG("response failed");
            
            
            //            NotificationCenter::getInstance()->postNotification(st_inspirationLayer_downLoadJson_fai);
            lRequest->release();
            return;
        }
        
        if (!response->isSucceed())
        {
            CCLOG("response failed   获取网络资源失败");
            //            NotificationCenter::getInstance()->postNotification(st_inspirationLayer_downLoadJson_fai);
            lRequest->release();
            return;
        }
        
        std::vector<char> *buffer = response->getResponseData();
        std::string bufffff(buffer->begin(),buffer->end());
        
        rapidjson::Document _doc;
        _doc.Parse<0>(bufffff.c_str());
        
        rapidjson::Value& lDoc = _doc["data"];
        rapidjson::Value& lId = lDoc["id"];
        rapidjson::Value& lLike = lDoc["like"];

//        log("%d",lLike.GetInt());
        
//        m_mFavoriteInfo[lId.GetInt()] = lLike.GetInt();
//        m_mFavoriteInfo.insert(std::map<int, int>::value_type(lId.GetInt(), lLike.GetInt()));

        FavoriteInfo lFavoriteInfo;
        lFavoriteInfo.imageID = lId.GetInt();
        lFavoriteInfo.likeNumber = lLike.GetInt();
        
        for (int i = 0; i < m_vFavoriteInfo.size(); ++i)
        {
            if (m_vFavoriteInfo.at(i).imageID == lFavoriteInfo.imageID)
            {
                std::vector<FavoriteInfo>::iterator iter = m_vFavoriteInfo.begin() + i;
                m_vFavoriteInfo.erase(iter);

            }
        }
        
        m_vFavoriteInfo.push_back(lFavoriteInfo);
        
        
        NotificationCenter::getInstance()->postNotification(st_FavoriteSprite_getLikeNumber_suc);
        m_bIsDownloadLike = false;
        
        for (int i = 0; i < m_vNeedGetLikeImage.size(); ++i)
        {
            if (m_vNeedGetLikeImage.at(i) == m_sGetLikeOfImageName)
            {
                std::vector<int>::iterator iter = m_vNeedGetLikeImage.begin() + i;
                m_vNeedGetLikeImage.erase(iter);
            }
        }
        
        if (m_vNeedGetLikeImage.size() > 0)
        {
            this->getLikeNumber(m_vNeedGetLikeImage.at(0));
        }
        
        
        lRequest->release();
    });
    HttpClient::getInstance()->send(lRequest);
}

void DownLoadPage::firstDownLoadInspirationInfo()
{

    if(_isRequestingAsk)
    {
        return;
    }
    
    _isRequestingAsk=true;
    
    m_vInspirationInfo.clear();
    
    
    
    string lDir=FileUtils::getInstance()->getWritablePath()+"Inspiration";
    this->createDirectory(lDir.c_str());

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    string ss = "http://color-origin.applabsinc.net/red/res/images/inspiration?bundle_id=android_enchantedforest&version=v1.0";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    string ss = "http://color-origin.imagineappsltd.com/red/res/images/inspiration?bundle_id=colorfree&version=v1.0";
//#endif

    HttpRequest* lRequest=new HttpRequest;
    //lRequest->setUrl("http://color-origin.applabsinc.net/red/res/images/inspiration?bundle_id=adultcoloringbook&version=v1.0");
    lRequest->setUrl(ss.c_str());
    
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this,lRequest](network::HttpClient* client, network::HttpResponse* response) {

        
        if (!response || !response->isSucceed())
        {
            CCLOG("response failed   获取网络资源失败");
            NotificationCenter::getInstance()->postNotification(st_inspirationLayer_downLoadJson_fai);
        }
        else
        {
            
            std::vector<char> *buffer = response->getResponseData();
            std::string bufffff(buffer->begin(),buffer->end());
            log("%s",bufffff.c_str());
            
            rapidjson::Document _doc;
            _doc.Parse<0>(bufffff.c_str());
            
            rapidjson::Value& lTotalNum = _doc["total"];
            rapidjson::Value& lDatas = _doc["data"];
            
            for(int i=0;i<lTotalNum.GetInt();i++)
            {
                rapidjson::Value& lPerInfo = lDatas[i];
                rapidjson::Value& lUrl = lPerInfo["url"];
                rapidjson::Value& lId = lPerInfo["id"];
                rapidjson::Value& lLike = lPerInfo["like"];
                //                rapidjson::Value& lUploadTime=lPerInfo["upload_time"];
                InspirationInfo lInfo;
                lInfo.imageUrl=lUrl.GetString();
                lInfo.ID=lId.GetInt();
                lInfo.likeNumber=lLike.GetInt();
                //                lInfo.upload_time=lUploadTime.GetString();
                
                m_vInspirationInfo.push_back(lInfo);
                
                if(i==lTotalNum.GetInt()-1)
                {
                    _lastDownRequestID=lInfo.ID;
                }
            }
            
            NotificationCenter::getInstance()->postNotification(st_inspirationLayer_firstDownLoadJson_suc);
        }
        
        
        _isRequestingAsk=false;
        
        lRequest->release();
        
    });
    HttpClient::getInstance()->send(lRequest);
}

void DownLoadPage::downLoadInspirationInfo(size_t pIdx)
{
    if(pIdx<m_vInspirationInfo.size())
    {
        return;
    }
    if(_isRequestingAsk)
    {
        return;
    }
    
    _isRequestingAsk=true;
    
    
    string lDir=FileUtils::getInstance()->getWritablePath()+"Inspiration";
    this->createDirectory(lDir.c_str());

    stringstream ss;
    
    
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ss<<"http://color-origin.applabsinc.net/red/res/images/inspiration?bundle_id=android_enchantedforest&version=v1.0&direction=next&image_id=";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    ss<<"http://color-origin.imagineappsltd.com/red/res/images/inspiration?bundle_id=colorfree&version=v1.0&direction=next&image_id=";
//#endif
    
    ss<<_lastDownRequestID;

    HttpRequest* lRequest=new HttpRequest;
    //lRequest->setUrl("http://color-origin.applabsinc.net/red/res/images/inspiration?bundle_id=adultcoloringbook&version=v1.0");
    lRequest->setUrl(ss.str().c_str());
    
    lRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    lRequest->setResponseCallback([this,lRequest](network::HttpClient* client, network::HttpResponse* response) {
        
        
        
        if (!response || !response->isSucceed())
        {
            CCLOG("response failed   获取网络资源失败");
        }
        else
        {
            
            std::vector<char> *buffer = response->getResponseData();
            std::string bufffff(buffer->begin(),buffer->end());
            log("%s",bufffff.c_str());
            
            rapidjson::Document _doc;
            _doc.Parse<0>(bufffff.c_str());
            
            rapidjson::Value& lTotalNum = _doc["total"];
            rapidjson::Value& lDatas = _doc["data"];
            
            for(int i=0;i<lTotalNum.GetInt();i++)
            {
                rapidjson::Value& lPerInfo = lDatas[i];
                rapidjson::Value& lUrl = lPerInfo["url"];
                rapidjson::Value& lId = lPerInfo["id"];
                rapidjson::Value& lLike = lPerInfo["like"];
//                rapidjson::Value& lUploadTime=lPerInfo["upload_time"];
                InspirationInfo lInfo;
                lInfo.imageUrl=lUrl.GetString();
                lInfo.ID=lId.GetInt();
                lInfo.likeNumber=lLike.GetInt();
//                lInfo.upload_time=lUploadTime.GetString();
                
                m_vInspirationInfo.push_back(lInfo);
                
                if(i==lTotalNum.GetInt()-1)
                {
                    _lastDownRequestID=lInfo.ID;
                }
            }
            
            int total = lTotalNum.GetInt();
            if (total != 0)
            {
                m_iNextPageImageNumber = total;
                NotificationCenter::getInstance()->postNotification(st_inspirationLayer_downLoadJson_suc);
            }
        }
        
        
        _isRequestingAsk=false;
        
        lRequest->release();
    
    });
    HttpClient::getInstance()->send(lRequest);
}

void DownLoadPage::startLoadImage()
{
    if (m_mNeedDownloadImage.size() > 0)
    {
        DownloadImageInfo imageInfo = m_mNeedDownloadImage.at(0);
        this->downLoadImage(imageInfo.imageUrl, imageInfo.imageID);
    }
    
}

void DownLoadPage::downLoadImage(string url ,int imageName)
{
    if(m_bIsFinishDownloadImage)
    {
        return;
    }
    
    m_bIsFinishDownloadImage = true;
    
    m_iImageID = imageName;
    
    HttpRequest* lRequest=new HttpRequest;
    lRequest->setUrl(url.c_str());
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
        
        stringstream ss;
        ss<<"";
        ss<<FileUtils::getInstance()->getWritablePath();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ss<<"Inspiration/";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        ss<<"Inspiration/";
//#endif
        ss<<lRequest->getTag();
        
        std::vector<char> *buffer = response->getResponseData();
        std::string bufffff(buffer->begin(),buffer->end());
        
//        std::string lPath=_savePath+"/"+lRequest->getTag();
        FILE *fp = fopen(ss.str().c_str(), "wb+");
        fwrite(bufffff.c_str(),1,buffer->size(), fp);
        fclose(fp);
        
        NotificationCenter::getInstance()->postNotification(st_inspiration_downloadSuc);
        m_bIsFinishDownloadImage = false;
        
        for (int i = 0; i < m_mNeedDownloadImage.size(); ++i)
        {
            if (m_iImageID == m_mNeedDownloadImage.at(i).imageID)
            {
                vector<DownloadImageInfo>::iterator iter = m_mNeedDownloadImage.begin() + i;
                m_mNeedDownloadImage.erase(iter);
            }
        }
        
        this->startLoadImage();
        
        lRequest->release();
    });
    
    stringstream ss;
    ss<<"";
    ss<<imageName;
    ss<<".png";
    
    lRequest->setTag(ss.str().c_str());
    HttpClient::getInstance()->send(lRequest);
    
}

bool DownLoadPage::createDirectory(const char *path)
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

void DownLoadPage::saveInspirationDatas()
{
    this->removeNullFile();
    for (int j = 0; j < m_vInsImageNameInfo.size(); j++)
    {
        char buffer[6];
        sprintf(buffer, "Ii%02d",j);
        
        stringstream ss;
        ss<<"";
        ss<<FileUtils::getInstance()->getWritablePath();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ss<<"Inspiration/";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        ss<<"Inspiration/";
//#endif
        ss<<m_vInsImageNameInfo.at(j);
        ss<<".png";
        
        if (FileUtils::getInstance()->isFileExist(ss.str()))
        {
            UserDefault::getInstance()->setIntegerForKey(buffer, m_vInsImageNameInfo.at(j));
        }
    }
    
    //like number
    for (int j = 0; j < m_vInsImageNameInfo.size(); j++)
    {
        char buffer[6];
        sprintf(buffer, "Il%02d",j);
        
        stringstream ss;
        ss<<"";
        ss<<FileUtils::getInstance()->getWritablePath();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ss<<"Inspiration/";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        ss<<"Inspiration/";
//#endif
        ss<<m_vInsImageNameInfo.at(j);
        ss<<".png";
        
        if (FileUtils::getInstance()->isFileExist(ss.str()))
        {
            UserDefault::getInstance()->setIntegerForKey(buffer, m_vInsImageLikeNum.at(j));
        }
    }
    
    UserDefault::getInstance()->setIntegerForKey(st_Inspiration_length, (int)m_vInsImageNameInfo.size());
}

void DownLoadPage::readInspitationDatas()
{
    if (!m_vInsImageNameInfo.empty())
    {
        m_vInsImageNameInfo.clear();
    }
    
    if (!m_vInsImageLikeNum.empty())
    {
        m_vInsImageLikeNum.clear();
    }
    
    int lenght = UserDefault::getInstance()->getIntegerForKey(st_Inspiration_length);
    
    for (int i = 0; i < lenght; i++)
    {
        char buffer[6];
        sprintf(buffer, "Ii%02d",i);
        
        int str = UserDefault::getInstance()->getIntegerForKey(buffer, -1);
        if (str != -1)
        {
            m_vInsImageNameInfo.push_back(str);
        }else
        {
            break;
        }
    }
    
    for (int i = 0; i < lenght; i++)
    {
        char buffer[6];
        sprintf(buffer, "Il%02d",i);
        
        int str = UserDefault::getInstance()->getIntegerForKey(buffer, -1);
        if (str != -1)
        {
            m_vInsImageLikeNum.push_back(str);
        }else
        {
            break;
        }
    }
}

void DownLoadPage::removeNullFile()
{
    for (int i = 0; i < m_vInsImageNameInfo.size(); ++i)
    {
        stringstream ss;
        ss<<"";
        ss<<FileUtils::getInstance()->getWritablePath();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ss<<"Inspiration/";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        ss<<"Inspiration/";
//#endif
        ss<<m_vInsImageNameInfo.at(i);
        ss<<".png";
        
        if (!FileUtils::getInstance()->isFileExist(ss.str()))
        {
            std::vector<int>::iterator iter = m_vInsImageNameInfo.begin()+i;
            m_vInsImageNameInfo.erase(iter);
            
            std::vector<int>::iterator iter2 = m_vInsImageLikeNum.begin()+i;
            m_vInsImageLikeNum.erase(iter2);
        }
    }
}

void DownLoadPage::updateNeedGetLikeImage(int imageID)
{
    for (int i = 0; i < m_vNeedGetLikeImage.size(); ++i)
    {
        if (m_vNeedGetLikeImage.at(i) == imageID)
        {
            return;
        }
    }
    
    m_vNeedGetLikeImage.push_back(imageID);
    this->getLikeNumber(imageID);
}

void DownLoadPage::readClickLikeInfo()
{
    string csvFilePath = FileUtils::getInstance()->getWritablePath() + "Inspiration/alreadyLike.csv";
    
    m_vAlreadyClickLick.clear();
    
    if (FileUtils::getInstance()->isFileExist(csvFilePath))
    {
        CSVParse* lCsv = CSVParse::create(csvFilePath.c_str());
        if (!lCsv)
        {
            return;
        }
        
        int row = lCsv->getRows();
        for (int i = 0; i < row; ++i)
        {
            int id = stoi(lCsv->getData(i, 0));
            m_vAlreadyClickLick.push_back(id);
        }
    }
}

void DownLoadPage::saveClickLikeInfo()
{
    std::vector<list<string>> vLikeInfo;
    for (int i = 0; i < m_vAlreadyClickLick.size(); ++i)
    {
        list<string> vList;
    
        stringstream ss;
        ss<<"";
        ss<<m_vAlreadyClickLick.at(i);
        vList.push_back(ss.str());
        vLikeInfo.push_back(vList);
    }
    
    this->saveMyArtDataTofile(vLikeInfo);
}

void DownLoadPage::saveMyArtDataTofile(const vector<list<string>>& lCsvData)
{
    string csvFilePath = FileUtils::getInstance()->getWritablePath() + "Inspiration/alreadyLike.csv";
    
    ofstream _streamToFile(csvFilePath);
    ///    判断打开文件是否成功
    if (!_streamToFile.is_open())
        return;
    /// 本地变量
    static char chQuote = '"';
    static char chComma = ',';
    /// Loop through each list of string in vector
    for (vector<list<string> >::const_iterator vIt = lCsvData.begin(); vIt != lCsvData.end(); vIt ++) {
        /// Loop through each string in list
        for (list<string>::const_iterator lIt = vIt->begin(); lIt != vIt->end(); lIt ++) {
            /// Separate this value from previous
            if (vIt->begin() != lIt)
                _streamToFile.put(chComma);
            /// 考虑string中可能有,或"的情况，这就要特殊包装。
            bool bComma = (lIt->find(chComma) != lIt->npos);
            bool bQuote = (lIt->find(chQuote) != lIt->npos);
            /// 真的含有,或"的情况
            if (bComma || bQuote) {
                _streamToFile.put(chQuote);
                
                
                if (bQuote) {
                    for (string::const_iterator chIt = lIt->begin(); chIt != lIt->end(); chIt ++ ) {
                        // Pairs of quotes interpreted as single quote
                        if (chQuote == *chIt)
                            _streamToFile.put(chQuote);
                        
                        _streamToFile.put(*chIt);
                    }
                }
                else
                    _streamToFile << *lIt;
                
                
                _streamToFile.put(chQuote);
            }
            else
                _streamToFile << *lIt;
        }
        /// 换行
        _streamToFile << endl;
    }
}