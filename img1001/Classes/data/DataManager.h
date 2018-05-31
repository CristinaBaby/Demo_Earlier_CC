//
//  DataManager.h
//  HH002
//
//  Created by houhuang on 15/8/26.
//
//

#ifndef __HH002__DataManager__
#define __HH002__DataManager__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

typedef struct _galleryInfo
{
    int ID;
    string image;
    string galleryname;
    bool isLock;
    string userdefault;
    string Iap_ID;
    
    bool isNewImage;
    string isUsedNewImage;
    
}GalleryInfo;

typedef struct _ColorData
{
    int ID;
    int R;
    int G;
    int B;

}ColorData;

typedef struct _CurrentImage
{
    int ID;
    string imageName;
//    string imageName_w;
}CurrentImage;

typedef struct _FramesInfo
{
    string imageName;
    string frameName;
    bool isNotUsed;
    float childScale;
    string userdefault;
    int tag;
    
    bool labelIsBlack;
}FramesInfo;

typedef struct _FilterInfo
{
    string  filterName;
    int     changeIndex;
    bool    isNotUsed;
    string  userdefault;
    int     tag;
    
    bool labelIsBlack;
}FilterInfo;

typedef struct _PurchaseInfo
{
    int ID;
    string purchase_ios_iap;
    string purchase_andriod_iap;
}PurchaseInfo;

enum
{
    st_purchase_flower_iap_index = 0,
    
    st_purchase_filter_pack1 = 111,
    st_purchase_filter_pack2,
    st_purchase_frames_pack1,
    st_purchase_frames_pack2,
    st_purchase_frames_pack3,

    st_rewardAds_filters1 = 150,
    st_rewardAds_filters2,
    st_rewardAds_frames1,
    st_rewardAds_frames2,
    st_rewardAds_frames3,
};


typedef struct _FlurryDatas
{
    string  message;
    int     number;
}FlurryInfo;

typedef struct _RestoreInfo
{
    string userdefault;
    string iap_ID;
    
}RestoreInfo;

typedef struct _FontTypeInfo
{
    int    ID;
    string fontName;
    string fontTypeFileName;
    
    bool   fontIsWhite;
}FontTypeInfo;

class DataManager
{
 
public:
    std::vector<GalleryInfo>* currentCalleryInfo;
    
public:
    std::vector<GalleryInfo> _galleryInfo;
    std::vector<GalleryInfo> _animalInfo;
    std::vector<GalleryInfo> _flowersInfo;
    std::vector<GalleryInfo> _ladiesInfo;
    std::vector<GalleryInfo> _mandalaInfo;
    std::vector<GalleryInfo> _mysteriousInfo;
    std::vector<GalleryInfo> _oceanInfo;
    
    
    std::vector<std::vector<GalleryInfo>*> _allMapDatas;
    std::vector<std::vector<GalleryInfo>> _allMapDatas_spare;
public:
    DataManager();
    static DataManager* getInstance();
    static void destory();
    std::vector<GalleryInfo> getCsvDatas(string fileName);
    std::vector<ColorData> getCsvColorDatas(std::string fileName);
    
    void getRestoreInfoDatas(string fileName);
    void initFontTypeInfoDatas(string fileName);
    
    vector<std::string> getCsvInfo(string fileName);
    
    int getCsvSize(string fileName);
    
public:
    bool isShowingTutorial();
    void setIsShowingTutorial(bool pBool);
    
    void updateRecentColor(ColorData colorData);
    
    void initFramesData();
    void updataFramesDataBase(bool isNotUsed);
    
    void initFilterData();
    void updataFilterDataBase(bool isNotUsed);
    
    //background color
    void saveBackColor(string gallery, int pIndex, ColorData pColorData);
    Color3B getSaveBackColor(string gallery, int pIndex);
    
    //init background
    void initSettingBackColor();
    
    void initPurchaseInfo();
    
    void initCacheTexture();
    
    void initColorGalleryText();
    
    void initFlurryDatas();
    
    void initLoadingAnimate();
    Animate* getLoadingAnimate();
    
    //多线程加载资源
    void startLoadImage();
    void dynamicLoadTexture(string imageName);
    void imageLoadedCallback(Ref* pSender);
    
    //在线更新
    void updateDownLoadResource();
    void updateCsvInfo();
    void updateBackgroundColor(string gallery, int pIndex, ColorData pColorData);
    
    
    //My Artworks
    void readMyArtworksDatas();
    void saveMyArtworksDatas();
    
    void addImageToArtworks(const string& galleryName, int imageNumber);
    void deleteArtworksImage(const string& galleryName, int imageNumber);
    
    //My post
    void readMyPostsDatas();
    void saveMyPostsDatas();
    
    void deleteMyPostsImage(const string& imageName);
    
    //favorite
    void readFavoriteDatas();
    void saveFavoriteDatas();
    void deleteFavoriteImage(int imageName);
    
    /*
     *multi language
     */
    std::map<std::string, std::string>& getLanguageMes();
    
    //accodring to string number dialog newline
    std::string dialogNewline(std::string str, int length);
protected:
    std::vector<GalleryInfo>    m_vGalleryDatas;
//    std::vector<ColorData>      m_vColorDatas;
   
    Animate*    m_pAnimate;

    
public:
    //最近使用的三种颜色处理
    int   m_iSelectedColorIndex;
    ColorData   m_cSelectedColor;
    std::vector<ColorData> m_vRecentUseColor;

    //MyArtwork
    static bool m_bIsAddMyAtrworks;
    static bool m_bIsShowMyArtworksDeleteBtn;
    static bool m_bIsMyArtworksInGameScene;
    
    static int  m_iCurrentClickNumber;
    static int  m_iCurrentCancelNumber;
    
    //实现返回map场景，回到进入时的位子
    static float    tableOffset;
    static float    tableOffset_Gallery;
    
    static bool     isTableOffset;
    static bool     is_TableOffset_Gallery;

    static float    myArtworksOffset;
    static bool     IsMyArtworksOffsety;
    
    static float    inspirationOffset;
    static bool     isInspirationOffset;
    
    static float    favoriteOffset;
    static bool     isFavoriteOffset;
    
    static float    postOffset;
    static bool     isPostOffset;
    
    static bool     m_bIsFilters;
    static int m_iAvailableFrames;
    static int m_iAvailableFilters;
    static int m_iCurrentFramesIndex;
    
    static int m_iFilterAndFramesButton;
    static int m_iCurrentChooseFilter;
    
    static int m_iAdsCount;
    static bool m_bIsAds;
    
    //flurry Datas
    static bool m_bCurrentImageIsUsed;
    static bool m_bIsImagereset;
    static bool m_bIsImageundo;
    static bool stopLoadThread;
    static bool m_bEnableRateUs;
    static bool m_bIsClickText;
    static bool m_bIsClickTextFont;
    static bool m_bIsClickTextColor;
    static bool m_bIsClickGameSceneBack;
    static bool m_bIsClickGameSceneDone;
    
    //是否检测有无新版本
    static bool m_bIsEnableCheck;

    static bool m_bIsOpenKeyboard;
    
    static bool m_bIsShowColorBoard;
    
    //关于界面切换控制
    static bool m_bIsGalleryLayer;
    static bool m_bIsFavoriteLayer;
    
    CurrentImage m_pCurrentImage;
    
    int m_iCurrentGalleryID;
    
    std::vector<GalleryInfo>    m_vGalleryName;
    
    //保存Frames datas
    std::vector<FramesInfo>     m_vFramesDatas;
    std::vector<FramesInfo>     m_vFramesDatasBase;
    
    //保存Filters datas
    std::vector<FilterInfo>     m_vFilterDatas;
    std::vector<FilterInfo>     m_vFilterDatasBase;
    
    //保存iapID
    std::vector<PurchaseInfo>   m_vPurchaseInfo;
    
    //读取颜色数据
    std::vector<ColorData>      m_vColorDatas;

    std::vector<std::string>    m_vColorGalleryText;
    
    //Flurry datas
    std::vector<FlurryInfo>     m_vFlurryDatas;
    
    //loading animate
    std::vector<std::string>    m_vLoadingAnimate;
    
    //RestoreInfo
    std::vector<RestoreInfo>    m_vRestoreInfo;
    GalleryInfo*                m_CurrentPurchase;
    std::vector<std::string>    m_vCsvInfo;
    
    //FontTypeInfo
    std::vector<FontTypeInfo>   m_vFontTypeInfo;
    
    std::vector<ColorData>      m_vSignatureColorInfo;
    
    //my artworks datas
    std::vector<std::string>    m_vMyArtWorksImageName;
    std::vector<int>            m_vMyArtWorksImageNumber;
    
    //my post datas
    std::vector<std::string>    m_vMyPostImage;
    
    //my favorite datas
    std::vector<int>            m_vMyFavoriteDatas;
    std::vector<int>            m_vMyFavoriteLikeNum;
    
    //multi language info
    std::map<std::string, std::string>  m_mLanguageInfo;
    
    //need dynamic load image
    std::vector<std::string>    m_vNeedLoadImage;
    std::string                 m_sCurrentImageName;
    bool                        m_bIsLoadFinish;
};

#endif /* defined(__HH002__DataManager__) */
