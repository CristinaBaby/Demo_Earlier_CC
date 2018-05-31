//
//  PurchaseHelper.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-17.
//
//

#include "PurchaseHelper.h"
#include "../define/GlobalMacro.h"


USING_NS_CC;


static PurchaseHelper* s_m_PurchaseHelper = NULL;

PurchaseHelper* PurchaseHelper::getInstance()
{
    if (NULL == s_m_PurchaseHelper)
    {
        s_m_PurchaseHelper = new PurchaseHelper();
    }
    
    return s_m_PurchaseHelper;
}

void PurchaseHelper::destroy()
{
    CC_SAFE_DELETE(s_m_PurchaseHelper);
}


PurchaseHelper::PurchaseHelper()
: _delegate(NULL)
{
}

void PurchaseHelper::setDelegate(STIABDelegate *delegate)
{
    _delegate = delegate;
}

void PurchaseHelper::purchasing(const char* pid)
{
    /*
    if (strcmp(pid, st_iap_sku_ad) == 0)
    {
        // 去除广告
        SaveGameData& lSaveGameData=DataManager::getInstance()->shareSaveGameData();
        lSaveGameData.isRemoveAd=true;
        DataManager::getInstance()->excuteSaveGameData();

    }
    else if (strcmp(pid, st_iap_sku_unlock_all) == 0)
    {
        // 全部解锁
        DataManager::getInstance()->unlockAllLockedLevel();
        
        // 去除广告
        SaveGameData& lSaveGameData=DataManager::getInstance()->shareSaveGameData();
        lSaveGameData.isRemoveAd=true;
        DataManager::getInstance()->excuteSaveGameData();
        
        NotificationCenter::getInstance()->postNotification(st_refresh_choose_map_box_locked_notification);
        
    }
    else if(strcmp(pid, st_iap_sku_hint_5) == 0)
    {
        // 5个hint道具
        
        SaveGameData& lSaveGameData=DataManager::getInstance()->shareSaveGameData();
        lSaveGameData.hintNumber+=5;
        DataManager::getInstance()->excuteSaveGameData();
        
        NotificationCenter::getInstance()->postNotification(st_refresh_hint_notification);
        
    }
     
     */
}


void PurchaseHelper::purchaseSuccessful(const char* pid)
{
    purchasing(pid);
    
    // 做代理，转发事件，上面只处理共同事件
    if (_delegate)
    {
        _delegate->purchaseSuccessful(pid);
    }
}

void PurchaseHelper::purchaseFailed(const char *pid, int errorCode)
{
    if (_delegate)
    {
        _delegate->purchaseFailed(pid, errorCode);
    }
}

void PurchaseHelper::restoreSuccessful(const char* pid)
{
    purchasing(pid);
    
    if (_delegate)
    {
        _delegate->restoreSuccessful(pid);
    }
}

void PurchaseHelper::restoreFailed(const char* pid, int errorCode)
{
    CCLOG("PurchaseHelper Restore Failed");
    
    if (_delegate)
    {
        _delegate->restoreFailed(pid, errorCode);
    }
}

