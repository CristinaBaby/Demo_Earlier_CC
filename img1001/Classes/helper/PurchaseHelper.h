//
//  PurchaseHelper.h
//  ColorFree
//
//  Created by houhuang on 15/9/9.
//
//

#ifndef __ColorFree__PurchaseHelper__
#define __ColorFree__PurchaseHelper__

#include <stdio.h>
#include "cocos2d.h"
#include "../modules/STIABDelegate.h"
USING_NS_CC;

class PurchaseHelper : public Ref, public STIABDelegate
{
    PurchaseHelper();
    STIABDelegate* _delegate;
    
public:
    static PurchaseHelper* getInstance();
    static void destory();
    
public:
    void setDelegate(STIABDelegate*);
    
    void purchaseSuccessful(const char* pid);
    void purchaseFailed(const char* pid, int errorCode);
    void restoreSuccessful(const char* pid);
    void restoreFailed(const char* pid, int errorCode);
    
private:
    void purchasing(const char* pid);
    void restoring(const char* pid);
};

#endif /* defined(__ColorFree__PurchaseHelper__) */
