//
//  DataManager.h
//  ColorGarden
//
//  Created by lidong on 15/8/5.
//
//

#ifndef __ColorGarden__DataManager__
#define __ColorGarden__DataManager__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;


typedef struct _ResourceInfo_
{
    int image_index;
    std::string image_name;
    std::string image_name_w;
}ResourceInfo;


typedef struct _ColorData
{
    int ID;
    int R;
    int G;
    int B;
    bool isNewColor;
}ColorData;



class DataManager  {
    
public:
    bool isAlreadySelectedColor;
    ColorData selectedColorData;
    
    std::vector<ResourceInfo> _resourceInfos;

    std::vector<ColorData> _colorDatas;
public:
    static DataManager* getInstance();
    std::vector<ColorData>& readCsvData(string filename);
    void readCsvResourceInfo(string filename);
    void saveBackColor(int pIndex,ColorData pColorData);
    void saveBackColorDirect(int pIndex,ColorData pColorData);
    Color3B getSaveBackColor(int pIndex);
    void initSetting();
    bool isShowingTutorial();
    void setIsShowingTutorial(bool pBool);
    
    float getColorSystemVersion();
    void setColorSystemVersion(float pVersion);
    
    void loadColorScvData();
    
    void ansyLoadTexture(string fileName);
    void ansyLoadFinish(Ref* pSender);
    
    void initCurrentBGColor();
    
    vector<ColorData> getCsvColorDatas(std::string fileName);
public:
    Color4B m_cCurrentBGColor;

    static bool     m_bTableViewIsOffset;
    static float    m_fChooseTableViewOffset;
    static int      m_iChooseCurrentPage;
    static bool     m_bDownloadingImage;
    static bool     m_bIspainted;
};

#endif /* defined(__ColorGarden__DataManager__) */
