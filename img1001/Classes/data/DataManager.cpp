//
//  DataManager.cpp
//  HH002
//
//  Created by houhuang on 15/8/26.
//
//

#include "DataManager.h"
#include "../utilities/CSVParse.h"
#include "../define/Globalmacro.h"
#include "../define/AppConfigs.h"
#include "../DownLoad/DownLoadPage.h"

#define st_MyArtWorksName   "myArt"

#define st_english_csv "English.csv"
#define st_french_csv "french.csv"
#define st_spanish_csv "spanish.csv"
#define st_italian_csv "italian.csv";

static  DataManager* _instance = NULL;

float   DataManager::tableOffset_Gallery = 0.0f;
bool    DataManager::is_TableOffset_Gallery = false;

float   DataManager::tableOffset = 0.0f;
bool    DataManager::isTableOffset = false;
bool    DataManager::m_bIsFilters = true;
bool    DataManager::m_bCurrentImageIsUsed = false;

int     DataManager::m_iAvailableFrames = 10;
int     DataManager::m_iAvailableFilters = 10;
int     DataManager::m_iCurrentFramesIndex = 0;

int     DataManager::m_iFilterAndFramesButton = 0;
int     DataManager::m_iCurrentChooseFilter = -1;

int     DataManager::m_iAdsCount = 0;
bool    DataManager::m_bIsAds = false;

bool    DataManager::stopLoadThread=false;

bool    DataManager::m_bEnableRateUs = false;
bool    DataManager::m_bIsEnableCheck = true;
bool    DataManager::m_bIsOpenKeyboard = false;

bool    DataManager::m_bIsAddMyAtrworks = false;
bool    DataManager::m_bIsShowMyArtworksDeleteBtn = false;
bool    DataManager::m_bIsMyArtworksInGameScene = false;
int     DataManager::m_iCurrentClickNumber = 0;
int     DataManager::m_iCurrentCancelNumber = 0;

float   DataManager::myArtworksOffset = 0.0f;
bool    DataManager::IsMyArtworksOffsety = false;

float   DataManager::inspirationOffset = 0.0f;
bool    DataManager::isInspirationOffset = false;

float   DataManager::favoriteOffset = 10.0f;
bool    DataManager::isFavoriteOffset = false;

float   DataManager::postOffset = 10.0f;
bool    DataManager::isPostOffset = false;

bool    DataManager::m_bIsShowColorBoard = true;

bool    DataManager::m_bIsGalleryLayer = true;
bool    DataManager::m_bIsFavoriteLayer = true;

bool    DataManager::m_bIsImagereset = false;
bool    DataManager::m_bIsImageundo = false;
bool    DataManager::m_bIsClickText = false;
bool    DataManager::m_bIsClickTextFont = false;
bool    DataManager::m_bIsClickTextColor = false;
bool    DataManager::m_bIsClickGameSceneBack = false;
bool    DataManager::m_bIsClickGameSceneDone = false;

DataManager::DataManager()
{
    ColorData colorData;
    
    colorData.R = 231;
    colorData.G = 76;
    colorData.B = 60;
//    m_vRecentUseColor.push_back(colorData);
//    
//    m_cSelectedColor = colorData;
//    
//    colorData.R = 230;
//    colorData.G = 126;
//    colorData.B = 34;
//    m_vRecentUseColor.push_back(colorData);
//    
//    colorData.R = 255;
//    colorData.G = 205;
//    colorData.B = 2;
//    m_vRecentUseColor.push_back(colorData);
    
    m_cSelectedColor = colorData;
    
    m_iSelectedColorIndex = 0;
    
    m_vColorDatas = this->getCsvColorDatas("Color RGB.csv");
    m_vSignatureColorInfo = this->getCsvColorDatas("Text RGB.csv");
    
//    this->updateCsvInfo();

//    this->initFontTypeInfoDatas("FontTypeInfo.csv");
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("loadAnimation.plist");
    
    m_pAnimate = NULL;
    currentCalleryInfo=NULL;
    m_bIsLoadFinish = false;

}

DataManager* DataManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new DataManager();
    }
    
    return _instance;
}

void DataManager::destory()
{
    delete _instance;
    _instance = NULL;
}



vector<GalleryInfo> DataManager::getCsvDatas(string fileName)
{
//    log("%s",fileName.c_str());
    
    std::vector<GalleryInfo> lDatas;
    
    CSVParse* lCsv = CSVParse::create(fileName.c_str());
    const unsigned int row = lCsv->getRows();
    for (int i = 1; i < row; i++)
    {
        string lID = lCsv->getDatas(i, "ID");
        string lImage = lCsv->getDatas(i, "ImageName");
        string lGalleryName = lCsv->getDatas(i, "GalleryName");
        string lIsLock = lCsv->getDatas(i, "IsLock");
        string lUserDefault = lCsv->getDatas(i, "Userdefault");
        string lIsNewImage = lCsv->getDatas(i, "IsNewImage");
        string lIsUsedNewImage = lCsv->getDatas(i, "IsUsedNewImage");
        
        string lIap_ID;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        lIap_ID = lCsv->getDatas(i, "Iap_ID");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        lIap_ID = lCsv->getDatas(i, "Iap_ID_A");
#endif
              
        GalleryInfo gallerydata;
        gallerydata.ID = atoi(lID.c_str());
        gallerydata.image = lImage;
        gallerydata.galleryname = lGalleryName;
        gallerydata.userdefault = lUserDefault;
        gallerydata.Iap_ID = lIap_ID;
        gallerydata.isUsedNewImage = lIsUsedNewImage;
        
        if (lIsLock == "TRUE")
        {
            gallerydata.isLock = true;
        }else
        {
            gallerydata.isLock = false;
        }
        
        if (lIsNewImage == "TRUE")
        {
            gallerydata.isNewImage = true;
        }else
        {
            gallerydata.isNewImage = false;
        }
        
        lDatas.push_back(gallerydata);
    }
    
    return lDatas;

}

void DataManager::getRestoreInfoDatas(string fileName)
{
    if (!m_vRestoreInfo.empty())
    {
        m_vRestoreInfo.clear();
    }
    
    CSVParse* lCsv = CSVParse::create(fileName.c_str());
    const unsigned int row = lCsv->getRows();
    
    for (int i = 1; i < row; i++)
    {
        string lUserDefault = lCsv->getDatas(i, "userdefault");
        string lIap_ID;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        lIap_ID = lCsv->getDatas(i, "Iap_ID");
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        lIap_ID = lCsv->getDatas(i, "Iap_ID_A");
        
#endif
        
        RestoreInfo lRestoreInfo;
        lRestoreInfo.userdefault = lUserDefault;
        lRestoreInfo.iap_ID = lIap_ID;
        
        m_vRestoreInfo.push_back(lRestoreInfo);
    }
}

void DataManager::initFontTypeInfoDatas(string fileName)
{
    if (!m_vFontTypeInfo.empty())
    {
        m_vFontTypeInfo.clear();
    }
    
    CSVParse* lCsv = CSVParse::create(fileName.c_str());
    const unsigned int row = lCsv->getRows();
    
    for (int i = 1; i < row; i++)
    {
        string lFontName = lCsv->getDatas(i, "FontName");
        string lFontTypeFileName = lCsv->getDatas(i, "FontTypeName");
        string lFontIsWhite = lCsv->getDatas(i, "IsWhite");
        string lId = lCsv->getDatas(i, "ID");
        
        FontTypeInfo lFontTypeInfo;
        lFontTypeInfo.fontName = lFontName;
        lFontTypeInfo.fontTypeFileName = lFontTypeFileName;
        
        if (lFontIsWhite == "TRUE")
        {
            lFontTypeInfo.fontIsWhite = true;
        }else
        {
            lFontTypeInfo.fontIsWhite = false;
        }
    
        stringstream ss;
        ss<<"";
        ss<<lId;
        ss>>lFontTypeInfo.ID;
        
        m_vFontTypeInfo.push_back(lFontTypeInfo);
    }
}

vector<std::string> DataManager::getCsvInfo(string fileName)
{
    
    std::vector<std::string> lDatas;
    
    CSVParse* lCsv = CSVParse::create(fileName.c_str());
    const unsigned int row = lCsv->getRows();
    
    for (int i = 1; i < row; i++)
    {
        string lCsvInfo = lCsv->getDatas(i, "CsvName");

        lDatas.push_back(lCsvInfo);
    }
    return lDatas;
    
}

vector<ColorData> DataManager::getCsvColorDatas(std::string fileName)
{
    std::vector<ColorData> lColorData;
    
    CSVParse* lCsv = CSVParse::create(fileName.c_str());
    const unsigned int row = lCsv->getRows();
    for (int i = 1; i < row; ++i)
    {
        std::string lID = lCsv->getDatas(i, "ID");
        std::string lR = lCsv->getDatas(i, "R");
        std::string lG = lCsv->getDatas(i, "G");
        std::string lB = lCsv->getDatas(i, "B");
        ColorData colorData;
        
        stringstream ss1;
        ss1<<"";
        ss1<<lID;
        ss1>>colorData.ID;
        
        stringstream ss2;
        ss2<<"";
        ss2<<lR;
        ss2>>colorData.R;
        
        stringstream ss3;
        ss3<<"";
        ss3<<lG;
        ss3>>colorData.G;
        
        stringstream ss4;
        ss4<<"";
        ss4<<lB;
        ss4>>colorData.B;
//        colorData.ID = stoi(lID);
//        colorData.R = stoi(lR);
//        colorData.G = stoi(lG);
//        colorData.B = stoi(lB);
        
        lColorData.push_back(colorData);
    }
    
    return lColorData;
}



int DataManager::getCsvSize(string fileName)
{
    CSVParse* lCsv = CSVParse::create(fileName.c_str());
    
    return lCsv->getRows();
}

bool DataManager::isShowingTutorial()
{
    return UserDefault::getInstance()->getBoolForKey(st_showingtutorial, false);
}

void DataManager::setIsShowingTutorial(bool pBool)
{
    UserDefault::getInstance()->setBoolForKey(st_showingtutorial, pBool);
    UserDefault::getInstance()->destroyInstance();
}

void DataManager::updateRecentColor(ColorData colorData)
{
    if (colorData.R == m_vRecentUseColor.at(0).R && colorData.G == m_vRecentUseColor.at(0).G && colorData.B == m_vRecentUseColor.at(0).B )
    {
        
    }else if (colorData.R == m_vRecentUseColor.at(1).R && colorData.G == m_vRecentUseColor.at(1).G && colorData.B == m_vRecentUseColor.at(1).B)
    {
        ColorData colorData0 = m_vRecentUseColor.at(0);
        ColorData colorData2 = m_vRecentUseColor.at(2);
        
        m_vRecentUseColor.clear();
        m_vRecentUseColor.push_back(colorData);
        m_vRecentUseColor.push_back(colorData0);
        m_vRecentUseColor.push_back(colorData2);
        
    }else
    {
        ColorData colorData0 = m_vRecentUseColor.at(0);
        ColorData colorData1 = m_vRecentUseColor.at(1);
        
        m_vRecentUseColor.clear();
        
        m_vRecentUseColor.push_back(colorData);
        m_vRecentUseColor.push_back(colorData0);
        m_vRecentUseColor.push_back(colorData1);
    
    }
    
}


void DataManager::saveBackColor(string gallery, int pIndex, ColorData pColorData)
{
    char buffer[20];
    sprintf(buffer,"%s%d_R", gallery.c_str(),pIndex);

    UserDefault::getInstance()->setIntegerForKey(buffer, pColorData.R);
    
    char buffer2[20];
    sprintf(buffer2,"%s%d_G", gallery.c_str(),pIndex);
    
    UserDefault::getInstance()->setIntegerForKey(buffer2, pColorData.G);
    
    char buffer3[20];
    sprintf(buffer3,"%s%d_B", gallery.c_str(),pIndex);
    
    UserDefault::getInstance()->setIntegerForKey(buffer3, pColorData.B);
    
}

Color3B DataManager::getSaveBackColor(string gallery, int pIndex)
{
    Color3B color;
    stringstream ss;
    ss<<"";
    ss<<gallery;
    ss<<pIndex;
    ss<<"_R";
    
    color.r = UserDefault::getInstance()->getIntegerForKey(ss.str().c_str());
    
    stringstream ss1;
    ss1<<"";
    ss1<<gallery;
    ss1<<pIndex;
    ss1<<"_G";
    color.g = UserDefault::getInstance()->getIntegerForKey(ss1.str().c_str());
    
    stringstream ss2;
    ss2<<"";
    ss2<<gallery;
    ss2<<pIndex;
    ss2<<"_B";
    color.b = UserDefault::getInstance()->getIntegerForKey(ss2.str().c_str());
    
    return color;
}

void DataManager::initSettingBackColor()
{
//    bool isSettingBackColor = UserDefault::getInstance()->getBoolForKey(st_is_setting_back_color);
//    if (isSettingBackColor == false)
//    {
//        UserDefault::getInstance()->setBoolForKey(st_is_setting_back_color, true);
////        if (m_vGalleryName.empty())
//        {
//            m_vGalleryName = DataManager::getInstance()->getCsvDatas("Gallery.csv");
//        }
//    
//        
//        ColorData lColorData;
//        lColorData.R = 255;
//        lColorData.G = 255;
//        lColorData.B = 255;
//        for (int i = 0; i < m_vGalleryName.size(); i ++)
//        {
//            for (int j = 0; j < 50; j ++)
//            {
//                lColorData.ID = j+1;
//                DataManager::getInstance()->saveBackColor(m_vGalleryName.at(i).galleryname, j+1, lColorData);
//            }
//        }
//    }
//    
//    
//    UserDefault::getInstance()->destroyInstance();
    
}

void DataManager::updateBackgroundColor(string gallery, int pIndex, ColorData pColorData)
{
    stringstream ss;
    ss<<"";
    ss<<gallery;
    ss<<pIndex;
    ss<<"_R";
    UserDefault::getInstance()->setIntegerForKey(ss.str().c_str(), pColorData.R);
    
    stringstream ss1;
    ss1<<"";
    ss1<<gallery;
    ss1<<pIndex;
    ss1<<"_G";
    UserDefault::getInstance()->setIntegerForKey(ss1.str().c_str(), pColorData.G);
    
    stringstream ss2;
    ss2<<"";
    ss2<<gallery;
    ss2<<pIndex;
    ss2<<"_B";
    UserDefault::getInstance()->setIntegerForKey(ss2.str().c_str(), pColorData.B);
    
    UserDefault::getInstance()->destroyInstance();
}

void DataManager::updateDownLoadResource()
{
    this->updateCsvInfo();
    
//    //update background color
//    m_vGalleryName = DataManager::getInstance()->getCsvDatas("Gallery.csv");
//    
//    ColorData lColorData;
//    lColorData.R = 255;
//    lColorData.G = 255;
//    lColorData.B = 255;
//    for (int i = 0; i < m_vGalleryName.size(); i ++)
//    {
//        for (int j = 0; j < 200; j ++)
//        {
//            lColorData.ID = j+1;
//            DataManager::getInstance()->updateBackgroundColor(m_vGalleryName.at(i).galleryname, j+1, lColorData);
//        }
//    }
}

void DataManager::updateCsvInfo()
{
    
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("frame_icon.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("frame_icon.plist");
    
    this->initFramesData();
    this->updataFramesDataBase(false);
    
    this->initFilterData();
    this->updataFilterDataBase(false);
    
    this->getRestoreInfoDatas("RestoreInfo.csv");
    
    _galleryInfo  = this->getCsvDatas("Gallery.csv");
    
    m_vCsvInfo = this->getCsvInfo("CsvInfo.csv");
    
    if (!_allMapDatas.empty())
    {
        _allMapDatas.clear();
    }
    
    if (!_allMapDatas_spare.empty())
    {
        _allMapDatas_spare.clear();
    }
    
    for (int i = 0; i < m_vCsvInfo.size(); ++i)
    {
        std::vector<GalleryInfo> lDatas;
        lDatas = this->getCsvDatas(m_vCsvInfo.at(i));
        _allMapDatas_spare.push_back(lDatas);
        //            _allMapDatas.push_back(&_allMapDatas_spare.at(_allMapDatas_spare.size()-1));
    }
    
    for (int i = 0; i < _allMapDatas_spare.size(); ++i)
    {
        _allMapDatas.push_back(&_allMapDatas_spare.at(i));
    }
    
    currentCalleryInfo = NULL;
    
}

void DataManager::initFramesData()
{
    if (!m_vFramesDatas.empty())
    {
        m_vFramesDatas.clear();
    }
    CSVParse* lCsv ;
#ifdef REMOVE_REWARD_ADS
    bool lBool = false;
    
    lCsv = CSVParse::create("Frames_new.csv");
    if (lCsv == NULL)
    {
        lBool = true;
        lCsv = CSVParse::create("Frames_new.csv");
    }
#else
    lCsv = CSVParse::create("Frames_new.csv");
#endif
        int row = lCsv->getRows();
        
        for (int i = 1; i < row; ++i)
        {
            string lImage = lCsv->getDatas(i, "ImageName");
            string lIsNotUsed = lCsv->getDatas(i, "IsNotUsed");
            string lChildScale = lCsv->getDatas(i, "ChildScale");
            string lFrameName = lCsv->getDatas(i, "FrameName");
            string lUserdefault = lCsv->getDatas(i, "UserDefault");
            string lTag = lCsv->getDatas(i, "Tag");

            bool lIsLock1;
            if (lIsNotUsed == "FALSE")
            {
                lIsLock1 = false;
            }else
            {
                lIsLock1 = true;
            }
            
#ifdef REMOVE_REWARD_ADS
            if (lBool == true)
            {
                lIsLock1 = false;
            }
#endif
            
            FramesInfo frame;
            frame.imageName = lImage;
            frame.isNotUsed = lIsLock1;
//            frame.childScale = stof(lChildScale);
            stringstream ss;
            ss<<"";
            ss<<lChildScale;
            ss>>frame.childScale;
            
            frame.frameName = lFrameName;
            frame.userdefault = lUserdefault;
            
            if (lTag != "")
            {
//                frame.tag = stoi(lTag);
                
                stringstream ss1;
                ss1<<"";
                ss1<<lTag;
                ss1>>frame.tag;
            }else
            {
                frame.tag = -1;
            }
            
            if (lFrameName == "Original")
            {
                frame.labelIsBlack = false;
            }else
            {
                frame.labelIsBlack = true;
            }
            m_vFramesDatas.push_back(frame);
        }
//
        for (int i = 0; i < m_vFramesDatas.size(); ++i)
        {
            if (UserDefault::getInstance()->getBoolForKey(m_vFramesDatas.at(i).userdefault.c_str()))
            {
                m_vFramesDatas.at(i).isNotUsed = false;
            }
            

        }
    
}

void DataManager::updataFramesDataBase(bool isNotUsed)
{
    if (!m_vFramesDatasBase.empty())
    {
        m_vFramesDatasBase.clear();
    }

#ifdef REMOVE_REWARD_ADS
    for (int i = 0; i < m_vFramesDatas.size(); ++i)
    {
        FramesInfo lFramesInfo = m_vFramesDatas.at(i);
        m_vFramesDatasBase.push_back(lFramesInfo);
    }
#else
    for (int i = 0; i < 7; ++i)
    {
        FramesInfo lFramesInfo = m_vFramesDatas.at(i);
        m_vFramesDatasBase.push_back(lFramesInfo);
    }
    //
    for (int i = 0; i < m_vFramesDatas.size(); ++i)
    {
        FramesInfo lFramesInfo = m_vFramesDatas.at(i);
        if (UserDefault::getInstance()->getBoolForKey(lFramesInfo.userdefault.c_str()))
        {
            if (isNotUsed)
            {
                //                lFramesInfo.isNotUsed = true;
            }
            m_vFramesDatasBase.push_back(lFramesInfo);
        }
    }
    
    log("%lu",m_vFramesDatasBase.size());
#endif

}

void DataManager::initFilterData()
{
    if (m_vFilterDatas.empty())
    {
        CSVParse* lCsv;
        
#ifdef REMOVE_REWARD_ADS
        lCsv = CSVParse::create("Filters_new.csv");
#else
        lCsv = CSVParse::create("Filters_new.csv");
#endif
        int row = lCsv->getRows();
        
        for (int i = 1; i < row; ++i)
        {
            string lImageName = lCsv->getDatas(i, "ImageName");
            string lChangeIndex = lCsv->getDatas(i, "ChangeIndex");
            string lIsNotUsed = lCsv->getDatas(i, "IsNotUsed");
            string lUserdefault = lCsv->getDatas(i, "Userdefault");
            string lTag = lCsv->getDatas(i, "Tag");

            
            bool lIsLock1;
            if (lIsNotUsed == "FALSE")
            {
                lIsLock1 = false;
            }else
            {
                lIsLock1 = true;
            }
            
//#ifdef REMOVE_REWARD_ADS
//            lIsLock1 = false;
//#endif
            
            FilterInfo lFilterInfo;
            lFilterInfo.filterName = lImageName;
//            lFilterInfo.changeIndex = stoi(lChangeIndex);
            stringstream ss1;
            ss1<<"";
            ss1<<lChangeIndex;
            ss1>>lFilterInfo.changeIndex;
            
            lFilterInfo.isNotUsed = lIsLock1;
            lFilterInfo.userdefault = lUserdefault;
            
            if (lTag != "")
            {
//                lFilterInfo.tag = stoi(lTag);
                stringstream ss1;
                ss1<<"";
                ss1<<lTag;
                ss1>>lFilterInfo.tag;
            }else
            {
                lFilterInfo.tag = -1;
            }

            if (lImageName == "Original")
            {
                lFilterInfo.labelIsBlack = false;
            }else
            {
                lFilterInfo.labelIsBlack = true;
            }
            
            
            m_vFilterDatas.push_back(lFilterInfo);
            
            
        }
        
        for (int i = 0; i < m_vFilterDatas.size(); ++i)
        {
            if (UserDefault::getInstance()->getBoolForKey(m_vFilterDatas.at(i).userdefault.c_str()))
            {
                m_vFilterDatas.at(i).isNotUsed = false;
            }
        }
            

    }
    
}

void DataManager::updataFilterDataBase(bool isNotUsed)
{
    if (!m_vFilterDatasBase.empty())
    {
        m_vFilterDatasBase.clear();
    }
    
#ifdef REMOVE_REWARD_ADS
    for (int i = 0; i < m_vFilterDatas.size(); ++i)
    {
        FilterInfo lFilterInfo = m_vFilterDatas.at(i);
        m_vFilterDatasBase.push_back(lFilterInfo);
    }
#else
    for (int i = 0; i < 5; ++i)
    {
        FilterInfo lFilterInfo = m_vFilterDatas.at(i);
        m_vFilterDatasBase.push_back(lFilterInfo);
    }
    
    for (int i = 0; i < m_vFilterDatas.size(); ++i)
    {
        FilterInfo lFramesInfo = m_vFilterDatas.at(i);
        if (UserDefault::getInstance()->getBoolForKey(lFramesInfo.userdefault.c_str()))
        {
            if (isNotUsed)
            {
                //                lFramesInfo.isNotUsed = true;
            }
            m_vFilterDatasBase.push_back(lFramesInfo);
        }
    }
#endif

}

void DataManager::initPurchaseInfo()
{
    if (m_vPurchaseInfo.empty())
    {
        CSVParse* lCsv = CSVParse::create("ui-1024X768/csv/purchase.csv");
        int row = lCsv->getRows();
        
        for (int i = 1; i < row; ++i)
        {
            string lID = lCsv->getDatas(i, "ID");
            string lIOS = lCsv->getDatas(i, "IOS");
            string lANDRIOD = lCsv->getDatas(i, "ANDRIOD");
            
            PurchaseInfo purchaes;
//            purchaes.ID = stoi(lID);
            stringstream ss;
            ss<<"";
            ss<<lID;
            ss>>purchaes.ID;
            
            purchaes.purchase_ios_iap = lIOS;
            purchaes.purchase_andriod_iap = lANDRIOD;
            
            m_vPurchaseInfo.push_back(purchaes);
        }
    }
}

void DataManager::initCacheTexture()
{
    for (int i = 0; i < _galleryInfo.size(); ++i)
    {
        Director::getInstance()->getTextureCache()->addImage(_galleryInfo.at(i).image);
    }
    
    Director::getInstance()->getTextureCache()->addImage("frame2.png");
    Director::getInstance()->getTextureCache()->addImage("bg.png");
}

void DataManager::initColorGalleryText()
{
    if (!m_vColorGalleryText.empty())
    {
        m_vColorGalleryText.clear();
    }
    
    m_vColorGalleryText.push_back(DM_getInstance->getLanguageMes()["Basic"]);
    m_vColorGalleryText.push_back(DM_getInstance->getLanguageMes()["Sweet"]);
    m_vColorGalleryText.push_back(DM_getInstance->getLanguageMes()["Autumn"]);
    m_vColorGalleryText.push_back(DM_getInstance->getLanguageMes()["Impressionism"]);
    m_vColorGalleryText.push_back(DM_getInstance->getLanguageMes()["Flower"]);
    
    if (UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_NEON))
    {
        m_vColorGalleryText.push_back(DM_getInstance->getLanguageMes()["Neon"]);
    }else
    {
        string str = DM_getInstance->getLanguageMes()["Neon"] + " $0.99";
        m_vColorGalleryText.push_back(str);
    }
    
    if (UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_SEA))
    {
        m_vColorGalleryText.push_back(DM_getInstance->getLanguageMes()["Nature"]);
    }else
    {
        string str = DM_getInstance->getLanguageMes()["Nature"] + " $0.99";
        m_vColorGalleryText.push_back(str);
    }
    
    if (UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_UKIYOE))
    {
        m_vColorGalleryText.push_back(DM_getInstance->getLanguageMes()["Ukiyoe"]);
    }else
    {
        string str = DM_getInstance->getLanguageMes()["Ukiyoe"] + " $0.99";
        m_vColorGalleryText.push_back(str);
    }
}


void DataManager::initLoadingAnimate()
{
//    Vector<SpriteFrame*> _spriteVec;
//    Animation* lAnimation = Animation::create();
//    for (int i = 0; i < 148; ++i)
//    {
//        char animationName[100];
//        std::sprintf(animationName, "letters_00%03d.png", i);
//
//       lAnimation->addSpriteFrameWithFile(animationName);
//        
//    }
//    
//    lAnimation->setDelayPerUnit(3.0 / 148.0);
//    lAnimation->setRestoreOriginalFrame(false);
//    
//    m_pAnimate = Animate::create(lAnimation);
    
    if (m_vLoadingAnimate.empty())
    {
        m_vLoadingAnimate.push_back("C.png");
        m_vLoadingAnimate.push_back("o.png");
        m_vLoadingAnimate.push_back("L.png");
        m_vLoadingAnimate.push_back("o.png");
        m_vLoadingAnimate.push_back("r.png");
        
        m_vLoadingAnimate.push_back("D.png");
        m_vLoadingAnimate.push_back("i.png");
        m_vLoadingAnimate.push_back("a.png");
        m_vLoadingAnimate.push_back("r.png");
        m_vLoadingAnimate.push_back("y.png");
    }
}

Animate* DataManager::getLoadingAnimate()
{
    return m_pAnimate;
}

void DataManager::startLoadImage()
{
    if (m_vNeedLoadImage.size() > 0)
    {
        string str = m_vNeedLoadImage.at(0);
        this->dynamicLoadTexture(str);
    }
}

void DataManager::dynamicLoadTexture(string imageName)
{
    if (m_bIsLoadFinish)
    {
        return;
    }
    
    m_bIsLoadFinish = true;
    Director::getInstance()->getTextureCache()->addImageAsync(imageName, CC_CALLBACK_1(DataManager::imageLoadedCallback, this));
    m_sCurrentImageName = imageName;
}

void DataManager::imageLoadedCallback(Ref* pSender)
{
    NotificationCenter::getInstance()->postNotification(st_datamanager_dynamicLoadTexture);
    
    for (int i = 0; i < m_vNeedLoadImage.size(); ++i)
    {
        if (m_sCurrentImageName == m_vNeedLoadImage.at(i))
        {
            std::vector<std::string>::iterator iter = m_vNeedLoadImage.begin() + i;
            m_vNeedLoadImage.erase(iter);
            break;
        }
        
    }
    
    m_bIsLoadFinish = false;
    
    this->startLoadImage();
}


void DataManager::initFlurryDatas()
{
    if (m_vFlurryDatas.empty())
    {
        CSVParse* lCsv = CSVParse::create("FlurryDatas.csv");
        int row = lCsv->getRows();
        
        for (int i = 1; i < row; ++i)
        {
            string lMessage;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            lMessage = lCsv->getDatas(i, "Name-IOS");
            
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            lMessage = lCsv->getDatas(i, "Name-Android");
            
#endif
            string lNumber  = lCsv->getDatas(i, "Number");
            
            FlurryInfo flurryInfo;
            flurryInfo.message = lMessage;
//            flurryInfo.number  = stoi(lNumber);
            stringstream ss;
            ss<<"";
            ss<<lNumber;
            ss>>flurryInfo.number;
            
            m_vFlurryDatas.push_back(flurryInfo);
        }
    }
}

#pragma mark My AtrtWorks

void DataManager::readMyArtworksDatas()
{
    if (!m_vMyArtWorksImageName.empty())
    {
        m_vMyArtWorksImageName.clear();
    }
    
    int length = UserDefault::getInstance()->getIntegerForKey(st_MyArtworks_length, -1);
    
//    log("%d",length);
    
    if (length == -1)
    {
        for (int i = 0; i < 200; ++i)
        {
            char buffer1[12];
            sprintf(buffer1, "ArtName%03d",i);
            
            char buffer2[12];
            sprintf(buffer2, "ArtNum%03d",i);
            
            if (UserDefault::getInstance()->getStringForKey(buffer1, "N") != "N")
            {
                m_vMyArtWorksImageName.push_back(UserDefault::getInstance()->getStringForKey(buffer1));
            }else
            {
                return;
            }
            
            if (UserDefault::getInstance()->getIntegerForKey(buffer2, 0) != 0)
            {
                m_vMyArtWorksImageNumber.push_back(UserDefault::getInstance()->getIntegerForKey(buffer2));
            }
        }
    }else
    {
        for (int i = 0; i < length; ++i)
        {
            char buffer1[12];
            sprintf(buffer1, "ArtName%03d",i);
            
            char buffer2[12];
            sprintf(buffer2, "ArtNum%03d",i);
            
            if (UserDefault::getInstance()->getStringForKey(buffer1, "N") != "N")
            {
                m_vMyArtWorksImageName.push_back(UserDefault::getInstance()->getStringForKey(buffer1));
            }
            
            if (UserDefault::getInstance()->getIntegerForKey(buffer2, 0) != 0)
            {
                m_vMyArtWorksImageNumber.push_back(UserDefault::getInstance()->getIntegerForKey(buffer2));
            }
        }
    }
    
    
    
}

void DataManager::saveMyArtworksDatas()
{
//    for (int i = 0; i < 200; ++i)
//    {
//        char buffer1[12];
//        sprintf(buffer1, "ArtName%03d",i);
//        
//        char buffer2[12];
//        sprintf(buffer2, "ArtNum%03d",i);
//
//        UserDefault::getInstance()->setStringForKey(buffer1, "N");
//        UserDefault::getInstance()->setIntegerForKey(buffer2, 0);
//    }
    
    for (int j = 0; j < m_vMyArtWorksImageName.size(); ++j)
    {
        char buffer1[12];
        sprintf(buffer1, "ArtName%03d",j);
        
        char buffer2[12];
        sprintf(buffer2, "ArtNum%03d",j);
        
        UserDefault::getInstance()->setStringForKey(buffer1, m_vMyArtWorksImageName.at(j));
        UserDefault::getInstance()->setIntegerForKey(buffer2, m_vMyArtWorksImageNumber.at(j));
    }
    
    UserDefault::getInstance()->setIntegerForKey(st_MyArtworks_length, (int)m_vMyArtWorksImageName.size());
//    UserDefault::getInstance()->destroyInstance();
}

void DataManager::addImageToArtworks(const string& galleryName, int imageNumber)
{
//    for (std::vector<std::string>::iterator iter=m_vMyArtWorksImageName.begin(); iter!=m_vMyArtWorksImageName.end(); iter++)
//    {
//        string lStr=*iter;
//        
//        if (m_vMyArtWorksImageName.at(i) == galleryName && m_vMyArtWorksImageNumber.at(i) == imageNumber)
//        {
//            m_vMyArtWorksImageName.erase(m_vMyArtWorksImageName.begin()+i);
//        }
//    }
    
    this->deleteArtworksImage(galleryName, imageNumber);
    
    m_vMyArtWorksImageName.push_back(galleryName);
    m_vMyArtWorksImageNumber.push_back(imageNumber);
}

void DataManager::deleteArtworksImage(const string& galleryName, int imageNumber)
{
    for (int i = 0; i < m_vMyArtWorksImageName.size(); ++i)
    {
        if (m_vMyArtWorksImageName.at(i) == galleryName && m_vMyArtWorksImageNumber.at(i) == imageNumber)
        {
            std::vector<std::string>::iterator iter = m_vMyArtWorksImageName.begin()+i;
            m_vMyArtWorksImageName.erase(iter);
            
            std::vector<int>::iterator iter2 = m_vMyArtWorksImageNumber.begin() + i;
            m_vMyArtWorksImageNumber.erase(iter2);
        }
    }
}

void DataManager::readMyPostsDatas()
{
    if (!m_vMyPostImage.empty())
    {
        m_vMyPostImage.clear();
    }
    
    if (!DownLoadPage::getInstance()->m_vPostImageId.empty())
    {
        DownLoadPage::getInstance()->m_vPostImageId.clear();
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int length = UserDefault::getInstance()->getIntegerForKey(st_Posts_length, 0);
    
    for (int i = 0; i < length; i++)
    {
        char buffer[6];
        sprintf(buffer, "po%03d",i);
        
        m_vMyPostImage.push_back(UserDefault::getInstance()->getStringForKey(buffer));
        
        
        char buffer2[6];
        sprintf(buffer2, "pI%03d",i);
        
        DownLoadPage::getInstance()->m_vPostImageId.push_back(UserDefault::getInstance()->getIntegerForKey(buffer2));
        
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int length = UserDefault::getInstance()->getIntegerForKey(st_Posts_length, 0);
    
    for (int i = 0; i < length; i++)
    {
        char buffer[6];
        sprintf(buffer, "po%03d",i);
        
        m_vMyPostImage.push_back(UserDefault::getInstance()->getStringForKey(buffer));
        
    }
    
    int length2 = UserDefault::getInstance()->getIntegerForKey(st_PostsId_length, 0);
    
    for (int i = 0; i < length2; i++)
    {
        char buffer2[6];
        sprintf(buffer2, "pI%03d",i);
        
        DownLoadPage::getInstance()->m_vPostImageId.push_back(UserDefault::getInstance()->getIntegerForKey(buffer2));
    }
#endif

}

void DataManager::saveMyPostsDatas()
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for (int j = 0; j < m_vMyPostImage.size(); j++)
    {
        char buffer[6];
        sprintf(buffer, "po%03d",j);
        UserDefault::getInstance()->setStringForKey(buffer, m_vMyPostImage.at(j));
        
        char buffer2[6];
        sprintf(buffer2, "pI%03d",j);
        UserDefault::getInstance()->setIntegerForKey(buffer2, DownLoadPage::getInstance()->m_vPostImageId.at(j));
    }
    
    UserDefault::getInstance()->setIntegerForKey(st_Posts_length, (int)m_vMyPostImage.size());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    for (int j = 0; j < m_vMyPostImage.size(); j++)
    {
        char buffer[6];
        sprintf(buffer, "po%03d",j);
        UserDefault::getInstance()->setStringForKey(buffer, m_vMyPostImage.at(j));
    }
    
    UserDefault::getInstance()->setIntegerForKey(st_Posts_length, (int)m_vMyPostImage.size());
    
    for (int j = 0; j < DownLoadPage::getInstance()->m_vPostImageId.size(); j++)
    {
        char buffer2[6];
        sprintf(buffer2, "pI%03d",j);
        UserDefault::getInstance()->setIntegerForKey(buffer2, DownLoadPage::getInstance()->m_vPostImageId.at(j));
    }
    
    UserDefault::getInstance()->setIntegerForKey(st_PostsId_length, (int)DownLoadPage::getInstance()->m_vPostImageId.size());
#endif

    
}

void DataManager::deleteMyPostsImage(const string& imageName)
{
    for (int i = 0; i < m_vMyPostImage.size(); ++i)
    {
        if (m_vMyPostImage.at(i) == imageName)
        {
            std::vector<std::string>::iterator iter = m_vMyPostImage.begin()+i;
            m_vMyPostImage.erase(iter);
            
            if (i <= DownLoadPage::getInstance()->m_vPostImageId.size())
            {
                std::vector<int>::iterator iter2 = DownLoadPage::getInstance()->m_vPostImageId.begin()+i;
                DownLoadPage::getInstance()->m_vPostImageId.erase(iter2);
            }

            this->saveMyPostsDatas();
        }
    }
}

void DataManager::readFavoriteDatas()
{
    if (!m_vMyFavoriteDatas.empty())
    {
        m_vMyFavoriteDatas.clear();
    }
    
//    if (!m_vMyFavoriteLikeNum.empty())
//    {
//        m_vMyFavoriteLikeNum.clear();
//    }
    
    int length = UserDefault::getInstance()->getIntegerForKey(st_Favorite_length, 0);
//
//    for (int i = 0; i < length; i++)
//    {
//        char buffer[6];
//        //favorite Number
//        sprintf(buffer, "fn%03d",i);
//        
//        m_vMyFavoriteLikeNum.push_back(UserDefault::getInstance()->getIntegerForKey(buffer));
//        
//    }
    
    for (int i = 0; i < length; i++)
    {
        char buffer[6];
        //favorite Image
        sprintf(buffer, "fi%03d",i);

//        DownLoadPage::getInstance()->getLikeNumber(UserDefault::getInstance()->getIntegerForKey(buffer));
        
        m_vMyFavoriteDatas.push_back(UserDefault::getInstance()->getIntegerForKey(buffer));
        
    }
}

void DataManager::saveFavoriteDatas()
{
    for (int j = 0; j < m_vMyFavoriteDatas.size(); j++)
    {
        char buffer[6];
        sprintf(buffer, "fi%03d",j);
        UserDefault::getInstance()->setIntegerForKey(buffer, m_vMyFavoriteDatas.at(j));
    }

//    for (int j = 0; j < m_vMyFavoriteLikeNum.size(); j++)
//    {
//        char buffer[6];
//        sprintf(buffer, "fn%03d",j);
//        UserDefault::getInstance()->setIntegerForKey(buffer, m_vMyFavoriteLikeNum.at(j));
//    }
    
    UserDefault::getInstance()->setIntegerForKey(st_Favorite_length, (int)m_vMyFavoriteDatas.size());
}

void DataManager::deleteFavoriteImage(int imageName)
{
    for (int i = 0; i < m_vMyFavoriteDatas.size(); ++i)
    {
        if (m_vMyFavoriteDatas.at(i) == imageName)
        {
            std::vector<int>::iterator iter = m_vMyFavoriteDatas.begin()+i;
            m_vMyFavoriteDatas.erase(iter);
            
//            std::vector<int>::iterator iter2 = m_vMyFavoriteLikeNum.begin()+i;
//            m_vMyFavoriteLikeNum.erase(iter2);
        }
    }
}

std::map<std::string, std::string>& DataManager::getLanguageMes()
{
    LanguageType lType = Application::getInstance()->getCurrentLanguage();
    
    
//    _currentLanguage=lType;
    
    string lCsvName=st_english_csv;
    switch (lType) {
        case cocos2d::LanguageType::FRENCH:
        {
            lCsvName=st_french_csv;
        }
            break;
        case cocos2d::LanguageType::SPANISH:
        {
            lCsvName=st_spanish_csv;
        }
            break;
        case cocos2d::LanguageType::ITALIAN:
        {
            lCsvName=st_italian_csv
        }
        default:
        {
            lCsvName=st_english_csv;
        }
            break;
    }

    if(m_mLanguageInfo.empty())
    {
        CSVParse* lCsv=CSVParse::create(lCsvName.c_str());
        const unsigned int row=lCsv->getRows();
        for (int r=1; r<row; r++) {
            string lKey=lCsv->getDatas(r, "Key");
            string lValue=lCsv->getDatas(r, "Content");
            m_mLanguageInfo.insert(pair<string, string>(lKey,lValue));
        }
    }
    return m_mLanguageInfo;
}

std::string DataManager::dialogNewline(std::string str, int length)
{
    unsigned int beginPos = 0;
    string resultStr;
    std::vector<std::string> str_vec;
    
    do
    {
        string findStr = str.substr(beginPos, length + 1);

        int pos = findStr.find_last_of(" ");

        if (beginPos + length >= str.length())
        {
            str_vec.push_back(str.substr(beginPos, length));
        }else
        {
            str_vec.push_back(str.substr(beginPos, pos + 1));
        }
        
        
        if (beginPos + length >= str.length() || pos <= 0)
        {
//            str_vec.push_back(str.substr(beginPos, length));
            break;
        }else
        {
            
            beginPos += pos + 1;
        }
        
    }while (true);
    
    
    for (int i = 0; i < str_vec.size(); ++i)
    {
        if (i < str_vec.size() - 1)
        {
            resultStr.append(str_vec.at(i)).append("\n");
        }else
        {
            resultStr.append(str_vec.at(i));
        }
        
    }
    
    log("%s",resultStr.c_str());
    return resultStr;
}
