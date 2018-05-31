//
//  PurchaseHelper.cpp
//  ColorFree
//
//  Created by houhuang on 15/9/9.
//
//

#include "PurchaseHelper.h"
#include "../data/DataManager.h"
#include "../define/AppConfigs.h"
#include "../define/Globalmacro.h"
//#include "../modules/AnalyticX.h"

static PurchaseHelper* s_PurchaseHelper = NULL;

PurchaseHelper* PurchaseHelper::getInstance()
{
    if (s_PurchaseHelper == NULL)
    {
        s_PurchaseHelper = new PurchaseHelper();
    }
    return s_PurchaseHelper;
}

PurchaseHelper::PurchaseHelper():_delegate(NULL)
{

}

void PurchaseHelper::destory()
{
    CC_SAFE_DELETE(s_PurchaseHelper);
}

void PurchaseHelper::setDelegate(STIABDelegate* delegate)
{
    _delegate = delegate;
}

void PurchaseHelper::purchaseSuccessful(const char* pid)
{
    purchasing(pid);
    if (_delegate)
    {
        _delegate->purchaseSuccessful(pid);
    }
}

void PurchaseHelper::purchaseFailed(const char* pid, int errorCode)
{
    if (_delegate)
    {
        _delegate->purchaseFailed(pid, errorCode);
    }
}

void PurchaseHelper::restoreSuccessful(const char* pid)
{
    restoring(pid);
    if (_delegate)
    {
        _delegate->restoreSuccessful(pid);
    }
}

void PurchaseHelper::restoreFailed(const char* pid, int errorCode)
{
    if (_delegate)
    {
        _delegate->restoreFailed(pid, errorCode);
    }
}

void PurchaseHelper::restoring(const char* pid)
{
    //restore
    for (int i = 0; i < DataManager::getInstance()->m_vRestoreInfo.size(); ++i)
    {
        if (strcmp(pid, DataManager::getInstance()->m_vRestoreInfo.at(i).iap_ID.c_str()) == 0)
        {
            if (!DataManager::getInstance()->m_vRestoreInfo.at(i).userdefault.empty())
            {
                UserDefault::getInstance()->setBoolForKey(DataManager::getInstance()->m_vRestoreInfo.at(i).userdefault.c_str(), true);
                UserDefault::getInstance()->destroyInstance();
            }
        }
    }
    
    if (strcmp(pid, st_purchase_color_neon_iap) == 0)
    {
        UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_NEON, true);
    }
    else if (strcmp(pid, st_purchase_color_sea_iap) == 0)
    {
        UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_SEA, true);

    }
    else if (strcmp(pid, st_purchase_color_ukiyoe_iap) == 0)
    {
        UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_UKIYOE, true);
    }
    
    //解决restore ColorGalleryText不刷新问题
    DataManager::getInstance()->initColorGalleryText();
}

void PurchaseHelper::purchasing(const char* pid)
{
    log("IAP ID = %s",pid);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (DataManager::m_bIsMyArtworksInGameScene)
    {
        if (strcmp(pid, st_purchase_color_neon_iap) == 0)
        {
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Buy_unlockpaletteneon") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_NEON, true);
            NotificationCenter::getInstance()->postNotification(st_colorScrollLayer_notifiction);
        }
        else if (strcmp(pid, st_purchase_color_sea_iap) == 0)
        {
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Buy_unlockpalettesea") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_SEA, true);
            NotificationCenter::getInstance()->postNotification(st_colorScrollLayer_notifiction);
        }
        else if (strcmp(pid, st_purchase_color_ukiyoe_iap) == 0)
        {
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Buy_unlockpaletteukiyoe") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_UKIYOE, true);
            NotificationCenter::getInstance()->postNotification(st_colorScrollLayer_notifiction);
        }
    }
    else
    {
        if (strcmp(pid, DataManager::getInstance()->m_CurrentPurchase->Iap_ID.c_str()) == 0)
        {
            if (!DataManager::getInstance()->m_CurrentPurchase->userdefault.empty())
            {
                UserDefault::getInstance()->setBoolForKey(DataManager::getInstance()->m_CurrentPurchase->userdefault.c_str(), true);
            }
            
            NotificationCenter::getInstance()->postNotification(st_scrollmaplayer_notifiction);
            
            if (DataManager::getInstance()->m_CurrentPurchase->galleryname == "Animals")
            {
                for (int i = 0; i < DM_FlurryDatas.size(); ++i)
                {
                    FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                    if (strcmp(lFlurryInfo->message.c_str(), "Buy_unlockanimalspack ") == 0)
                    {
                        lFlurryInfo->number++;
                    }
                }
            }else if (DataManager::getInstance()->m_CurrentPurchase->galleryname == "Flowers")
            {
                for (int i = 0; i < DM_FlurryDatas.size(); ++i)
                {
                    FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                    if (strcmp(lFlurryInfo->message.c_str(), "Buy_unlockflowerspack") == 0)
                    {
                        lFlurryInfo->number++;
                    }
                }
            }else if (DataManager::getInstance()->m_CurrentPurchase->galleryname == "Ladies")
            {
                for (int i = 0; i < DM_FlurryDatas.size(); ++i)
                {
                    FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                    if (strcmp(lFlurryInfo->message.c_str(), "Buy_unlockladiespack") == 0)
                    {
                        lFlurryInfo->number++;
                    }
                }
            }else if (DataManager::getInstance()->m_CurrentPurchase->galleryname == "Mandalas")
            {
                for (int i = 0; i < DM_FlurryDatas.size(); ++i)
                {
                    FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                    if (strcmp(lFlurryInfo->message.c_str(), "Buy_unlockmandalaspack") == 0)
                    {
                        lFlurryInfo->number++;
                    }
                }
            }
        }
    }
    
    
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (DataManager::m_bIsMyArtworksInGameScene)
    {
        if (strcmp(pid, st_purchase_color_neon_iap) == 0)
        {
            
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "IAP_unlockpaletteneon") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_NEON, true);
            NotificationCenter::getInstance()->postNotification(st_colorScrollLayer_notifiction);
        }
        else if (strcmp(pid, st_purchase_color_sea_iap) == 0)
        {
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "IAP_unlockpalettesea") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_SEA, true);
            NotificationCenter::getInstance()->postNotification(st_colorScrollLayer_notifiction);
        }
        else if (strcmp(pid, st_purchase_color_ukiyoe_iap) == 0)
        {
            
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "IAP_unlockpaletteukiyoe") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_UKIYOE, true);
            NotificationCenter::getInstance()->postNotification(st_colorScrollLayer_notifiction);
        }
    }
    else
    {
        if (strcmp(pid, DataManager::getInstance()->m_CurrentPurchase->Iap_ID.c_str()) == 0)
        {
            if (!DataManager::getInstance()->m_CurrentPurchase->userdefault.empty())
            {
                UserDefault::getInstance()->setBoolForKey(DataManager::getInstance()->m_CurrentPurchase->userdefault.c_str(), true);
            }
            
            NotificationCenter::getInstance()->postNotification(st_scrollmaplayer_notifiction);
            
            if (DataManager::getInstance()->m_CurrentPurchase->galleryname == "Animals")
            {
                for (int i = 0; i < DM_FlurryDatas.size(); ++i)
                {
                    FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                    if (strcmp(lFlurryInfo->message.c_str(), "IAP_unlockanimalspack ") == 0)
                    {
                        lFlurryInfo->number++;
                    }
                }
            }else if (DataManager::getInstance()->m_CurrentPurchase->galleryname == "Flowers")
            {
                for (int i = 0; i < DM_FlurryDatas.size(); ++i)
                {
                    FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                    if (strcmp(lFlurryInfo->message.c_str(), "IAP_unlockflowerspack") == 0)
                    {
                        lFlurryInfo->number++;
                    }
                }
            }else if (DataManager::getInstance()->m_CurrentPurchase->galleryname == "Ladies")
            {
                for (int i = 0; i < DM_FlurryDatas.size(); ++i)
                {
                    FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                    if (strcmp(lFlurryInfo->message.c_str(), "IAP_unlockladiespack") == 0)
                    {
                        lFlurryInfo->number++;
                    }
                }
            }else if (DataManager::getInstance()->m_CurrentPurchase->galleryname == "Mandalas")
            {
                for (int i = 0; i < DM_FlurryDatas.size(); ++i)
                {
                    FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                    if (strcmp(lFlurryInfo->message.c_str(), "IAP_unlockmandalaspack") == 0)
                    {
                        lFlurryInfo->number++;
                    }
                }
            }
        }
        
        if (strcmp(pid, st_purchase_color_neon_iap) == 0)
        {
            
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "IAP_unlockpaletteneon") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_NEON, true);
            NotificationCenter::getInstance()->postNotification(st_colorScrollLayer_notifiction);
        }
        else if (strcmp(pid, st_purchase_color_sea_iap) == 0)
        {
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "IAP_unlockpalettesea") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_SEA, true);
            NotificationCenter::getInstance()->postNotification(st_colorScrollLayer_notifiction);
        }
        else if (strcmp(pid, st_purchase_color_ukiyoe_iap) == 0)
        {
            
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "IAP_unlockpaletteukiyoe") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            UserDefault::getInstance()->setBoolForKey(IS_PURCHASE_COLOR_UKIYOE, true);
            NotificationCenter::getInstance()->postNotification(st_colorScrollLayer_notifiction);
        }
    }
  
#endif
    
    //解决restore ColorGalleryText不刷新问题
    DataManager::getInstance()->initColorGalleryText();
}