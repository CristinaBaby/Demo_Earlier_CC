//
//  PurchaseHelper.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-17.
//
//

#include "PurchaseHelper.h"
#include "EncryptDataHelper.h"
#include "../AppConfig.h"
#include "../FileModifiers.h"


USING_NS_CC;


bool PurchaseHelper::isPurchased = false;
bool PurchaseHelper::isAdPurchased = false;


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
    if (strcmp(pid, st_iap_sku_ad) == 0)
    {
        // 全部解锁
        isPurchased   = true;
        isAdPurchased = true;
        EncryptDataHelper::setPurchaseFlag(key_iap_purchased_flag, isPurchased);
        EncryptDataHelper::setPurchaseFlag(key_iap_ad_purchased_flag, isAdPurchased);
        
    }
    else if (strcmp(pid, st_iap_sku_unlock_all) == 0)
    {
        // 解锁关卡
        //        isAdPurchased = true;
        //        EncryptDataHelper::setPurchaseFlag(key_iap_ad_purchased_flag, isAdPurchased);
        isPurchased   = true;
        EncryptDataHelper::setPurchaseFlag(key_iap_purchased_flag, isPurchased);
    }
    else if(strcmp(pid, st_iap_sku_hint_5) == 0)
    {
        // 5个hint道具
        unsigned hintQuantity = EncryptDataHelper::getHintQuantity(key_hint_quantity);
        hintQuantity += 5;
        EncryptDataHelper::setHintQuantity(key_hint_quantity, hintQuantity);
    }

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

