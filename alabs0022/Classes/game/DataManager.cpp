//
//  DataManager.cpp
//  ColorGarden
//
//  Created by lidong on 15/8/5.
//
//

#include "DataManager.h"
#include "../utilities/CSVParse.h"
#include "../define/ResoureMacro.h"
#include "../define/GlobalMacro.h"
static DataManager* _instance=NULL;

float DataManager::m_fChooseTableViewOffset = 0.0f;
bool  DataManager::m_bTableViewIsOffset = false;
int   DataManager::m_iChooseCurrentPage = 1;
bool  DataManager::m_bDownloadingImage = false;

DataManager* DataManager::getInstance()
{
   if(_instance==NULL)
   {
       _instance=new DataManager();
   }
   return _instance;
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

std::vector<ColorData>& DataManager::readCsvData(string filename)
{
    if(_colorDatas.empty())
    {
        CSVParse* lCsv=CSVParse::create(filename.c_str());
        const unsigned int row=lCsv->getRows();
        for (int r=1; r<row; r++) {
            
            string lID=lCsv->getDatas(r, "ID");
            string lR=lCsv->getDatas(r, "R");
            string lG=lCsv->getDatas(r, "G");
            string lB=lCsv->getDatas(r, "B");
            string lIsNewColor = lCsv->getDatas(r, "IsNewColor");
            
            ColorData colorData;
            colorData.ID=atoi(lID.c_str());
            colorData.R=atoi(lR.c_str());
            colorData.G=atoi(lG.c_str());
            colorData.B=atoi(lB.c_str());
            
            if (lIsNewColor == "TRUE")
            {
                colorData.isNewColor = true;
            }else
            {
                colorData.isNewColor = false;
            }
            
            _colorDatas.push_back(colorData);
        }
    }
     return _colorDatas;
}

void DataManager::readCsvResourceInfo(string filename)
{
    if(_resourceInfos.empty())
    {
        _resourceInfos.clear();
            CSVParse* lCsv=CSVParse::create(filename.c_str());
            const unsigned int row=lCsv->getRows();
            for (int r=1; r<row; r++) {
        //name,name_white
                int image_index=r;
                ResourceInfo lInfo;
                lInfo.image_name = lCsv->getDatas(r, "name");
                lInfo.image_name_w = lInfo.image_name + "_mask.png";
                lInfo.image_name += ".png";
                lInfo.image_index=image_index;
                _resourceInfos.push_back(lInfo);
        }
    }
}


void DataManager::saveBackColor(int pIndex,ColorData pColorData)
{
    stringstream ss;
    ss<<st_color_map;
    ss<<pIndex;
    ss<<"_R";
    UserDefault::getInstance()->setIntegerForKey(ss.str().c_str(),pColorData.R);
    
    stringstream ss2;
    ss2<<st_color_map;
    ss2<<pIndex;
    ss2<<"_G";
    UserDefault::getInstance()->setIntegerForKey(ss2.str().c_str(),pColorData.G);
    
    stringstream ss3;
    ss3<<st_color_map;
    ss3<<pIndex;
    ss3<<"_B";
    UserDefault::getInstance()->setIntegerForKey(ss3.str().c_str(),pColorData.B);
    
    UserDefault::getInstance()->destroyInstance();
    
}

void DataManager::saveBackColorDirect(int pIndex,ColorData pColorData)
{
    stringstream ss;
    ss<<st_color_map;
    ss<<pIndex;
    ss<<"_R";
    UserDefault::getInstance()->setIntegerForKey(ss.str().c_str(),pColorData.R);
    
    stringstream ss2;
    ss2<<st_color_map;
    ss2<<pIndex;
    ss2<<"_G";
    UserDefault::getInstance()->setIntegerForKey(ss2.str().c_str(),pColorData.G);
    
    stringstream ss3;
    ss3<<st_color_map;
    ss3<<pIndex;
    ss3<<"_B";
    UserDefault::getInstance()->setIntegerForKey(ss3.str().c_str(),pColorData.B);
}

Color3B DataManager::getSaveBackColor(int pIndex)
{
    Color3B color;
    
    stringstream ss;
    ss<<st_color_map;
    ss<<pIndex;
    ss<<"_R";
   
    color.r = UserDefault::getInstance()->getIntegerForKey(ss.str().c_str());
    
    
    stringstream ss2;
    ss2<<st_color_map;
    ss2<<pIndex;
    ss2<<"_G";
    color.g = UserDefault::getInstance()->getIntegerForKey(ss2.str().c_str());
    
    stringstream ss3;
    ss3<<st_color_map;
    ss3<<pIndex;
    ss3<<"_B";
    
    color.b = UserDefault::getInstance()->getIntegerForKey(ss3.str().c_str());
    
    return color;
}

void DataManager::initSetting()
{
    bool lIsSetupBackColorData=UserDefault::getInstance()->getBoolForKey(st_is_setting_back_color);
    if(lIsSetupBackColorData==false)
    {
        UserDefault::getInstance()->setBoolForKey(st_is_setting_back_color, true);
        ColorData lColorData;
    
        lColorData.R=255;
        lColorData.G=255;
        lColorData.B=255;
        
        for (int i=0; i<100; i++) {
            lColorData.ID=i+1;
            DataManager::getInstance()->saveBackColor(i+1, lColorData);
        }
    }
}

void DataManager::loadColorScvData()
{
    this->readCsvResourceInfo("orgin_color.csv");
}

bool DataManager::isShowingTutorial()
{
    return  UserDefault::getInstance()->getBoolForKey(st_is_showing_tutorial);
}

void DataManager::setIsShowingTutorial(bool pBool)
{
    UserDefault::getInstance()->setBoolForKey(st_is_showing_tutorial, pBool);
    UserDefault::getInstance()->destroyInstance();
}

float DataManager::getColorSystemVersion()
{
   return UserDefault::getInstance()->getFloatForKey(st_color_system_version, 1.0f);
}

void DataManager::setColorSystemVersion(float pVersion)
{
    UserDefault::getInstance()->setFloatForKey(st_color_system_version, pVersion);
    UserDefault::getInstance()->destroyInstance();
}

void DataManager::ansyLoadTexture(string fileName)
{
    Director::getInstance()->getTextureCache()->addImageAsync(fileName, CC_CALLBACK_1(DataManager::ansyLoadFinish, this));
}
void DataManager::ansyLoadFinish(Ref* pSender)
{
    Texture2D* lTexture=(Texture2D*)pSender;
    if(lTexture!=NULL)
    {
        NotificationCenter::getInstance()->postNotification(st_texture_load_finish_notification);
    }
   
}

void DataManager::initCurrentBGColor()
{
    int num = rand() % 3;
    switch (num)
    {
        case 0:
        {
            m_cCurrentBGColor = Color4B(255, 112, 67, 255);
        }
            break;
            
        case 1:
        {
            m_cCurrentBGColor = Color4B(14, 191, 176, 255);
        }
            break;
            
        case 2:
        {
            m_cCurrentBGColor = Color4B(133, 90, 211, 255);
        }
            break;
            
        default:
            break;
    }
}



