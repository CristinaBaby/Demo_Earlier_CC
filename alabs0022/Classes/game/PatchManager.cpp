//
//  PatchManager.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "PatchManager.h"
#include "DataManager.h"
#include "CSVParse.h"
#include "XCFileStream.h"
#include "ImageDownload.h"


#define PATCH_VERSION   "patch_version"

static PatchManager *_sharedPatch = nullptr;

PatchManager* PatchManager::getInstance()
{
    if (! _sharedPatch)
    {
        _sharedPatch = new PatchManager();
    }
    
    return _sharedPatch;
}

void PatchManager::destroyInstance()
{
    xScheduler->unscheduleAllForTarget(this);
    CC_SAFE_DELETE(_sharedPatch);
}

PatchManager::PatchManager()
{
    _writablePath = FileUtils::getInstance()->getWritablePath();
    _fromVersion = UserDefault::getInstance()->getIntegerForKey(PATCH_VERSION, 1);
    
    Patch patch;
    patch.fromVersion = 1;
    patch.toVersion = 2;
    patch.callback = CC_CALLBACK_2(PatchManager::app_2, this);
    _patchsApp.push_back(patch);
    
    patch.callback = CC_CALLBACK_2(PatchManager::choose_2, this);
    _patchsChoose.push_back(patch);
    
    findPatch();
}

void PatchManager::findPatch()
{
    if (_fromVersion == 1) {
        _patchApp = _patchsApp.at(0).callback;
        _patchChoose = _patchsChoose.at(0).callback;
    }
}

//patchs ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PatchManager::app_2(int iData, void* vData)
{
    vector<list<string>> m_vColorCSVData;
    if (FileUtils::getInstance()->isFileExist(_writablePath + "color/color.csv"))
    {
        CSVParse* lCsv = CSVParse::create("color.csv");
        for (int i = 0 ; i < lCsv->getRows(); i++)
        {
            string str = lCsv->getData(i, 0);
            string newStr = str.substr(0, 9);
            list<string> lList;
            lList.push_back(newStr);
            m_vColorCSVData.push_back(lList);
        }
        
        CXCFileStream cx;
        cx.WriteCsvData(_writablePath + "color/" + "color_1.csv", m_vColorCSVData);
    }
}

void PatchManager::choose_2(int iData, void* vData)
{
    for (int i = 0; i < 60; ++i)
    {
        if (DataManager::getInstance()->_resourceInfos.size()>=i)
        {
            stringstream ss;
            ss<<"";
            ss<<"map";
            ss<<i;
            ss<<"_white.png";
            
            string oldPath = FileUtils::getInstance()->getWritablePath() + ss.str();
            if (FileUtils::getInstance()->isFileExist(oldPath))
            {
                
                
                char buffer[20];
                sprintf(buffer, "map%d_history.png",i);
                
                char buffer2[20];
                sprintf(buffer2, "map%d_save.png",i);
                
                string newPath = FileUtils::getInstance()->getWritablePath() + buffer;
                string newPath2 = FileUtils::getInstance()->getWritablePath() + buffer2;
                
                //改文件名
                rename(oldPath.c_str(), newPath.c_str());
                
                //创建并复制文件
                Data data = FileUtils::getInstance()->getDataFromFile(newPath);
                FILE *fp = fopen(newPath2.c_str(), "wb+");
                fwrite(data.getBytes(), sizeof(unsigned char), data.getSize(), fp);
                fclose(fp);
                
            }
        }
        
        if (ImageDownload::getInstance()->_downloadFinishResourceInfo.size() > i)
        {
            //            log("+++++%s",ImageDownload::getInstance()->_downloadFinishResourceInfo.at(i).image_name.c_str());
            
            string str = ImageDownload::getInstance()->_downloadFinishResourceInfo.at(i).image_name;
            string newStr = str.substr(0,9);
            log("%s",newStr.c_str());
            
            stringstream aa;
            aa<<"";
            aa<<newStr;
            aa<<"w.png";
            
            string oldPath = FileUtils::getInstance()->getWritablePath() + aa.str();
            if (FileUtils::getInstance()->isFileExist(oldPath))
            {
                
                string buffer = newStr + "_history.png";
                
                string buffer2 = newStr + "_save.png";
                //                char buffer[20];
                //                sprintf(buffer, "map%d_history.png",i);
                
                //                char buffer2[20];
                //                sprintf(buffer2, "map%d_save.png",i);
                
                string newPath = FileUtils::getInstance()->getWritablePath() + buffer;
                string newPath2 = FileUtils::getInstance()->getWritablePath() + buffer2;
                
                //改文件名
                rename(oldPath.c_str(), newPath.c_str());
                
                //创建并复制文件
                Data data = FileUtils::getInstance()->getDataFromFile(newPath);
                FILE *fp = fopen(newPath2.c_str(), "wb+");
                fwrite(data.getBytes(), sizeof(unsigned char), data.getSize(), fp);
                fclose(fp);
            }
        }
    }
}


//patchs end ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PatchManager::patchApp(int iData, void* vData)
{
    if (_patchApp) {
        _patchApp(iData, vData);
    }
}

void PatchManager::patchChoose(int iData, void* vData)
{
    if (_patchChoose) {
        _patchChoose(iData, vData);
    }
}

void PatchManager::patchFinish()
{
    //完成后, 设置version
    UserDefault::getInstance()->setIntegerForKey(PATCH_VERSION, _toVersion);
    
    _patchApp = nullptr;
    _patchChoose = nullptr;
}

