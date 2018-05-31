//
//  BaseLayer.cpp
//  ColorGarden
//
//  Created by lidong on 15/11/20.
//
//

#include "BaseLayer.h"
#include "../define/GlobalMacro.h"
#include "../layer/Dialog.h"
#include "../helper/ImageDownload.h"

BaseLayer::BaseLayer()
{
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(BaseLayer::responseIsDownloadNotification),st_check_is_download_notification,NULL);
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(BaseLayer::downloadFinish),st_download_finish_notification,NULL);
    
     NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(BaseLayer::noNewVersionReadCsvFinish),st_no_new_version_readcsv_finish_notification,NULL);
    
    
    _isAlreadyShowScene=false;
    
}

BaseLayer::~BaseLayer()
{
    NotificationCenter::getInstance()->removeObserver(this, st_check_is_download_notification);
    NotificationCenter::getInstance()->removeObserver(this, st_download_finish_notification);
    NotificationCenter::getInstance()->removeObserver(this, st_no_new_version_readcsv_finish_notification);
}

bool BaseLayer::init()
{
    if(!Layer::init()) return false;
    
    return true;
}

void BaseLayer::responseIsDownloadNotification(Ref* pSender)
{
    STAlertDialog* lAlertDialog=STAlertDialog::create("New Artworks Available! \nDownload Now!!!", "Cancel", "Ok");
    lAlertDialog->setLeftListener(this,callfuncN_selector(BaseLayer::removeDialog));
    lAlertDialog->setRightListener(this, callfuncN_selector(BaseLayer::startDownload));
    this->addChild(lAlertDialog,INT_MAX-1);
}

void BaseLayer::removeDialog(Node* pSender)
{
    pSender->removeFromParentAndCleanup(true);
    ImageDownload::getInstance()->excuteFunc(CANCEL_DOWNLOAD_STATE);
}

void BaseLayer::startDownload(cocos2d::Node *pSender)
{
    NotificationCenter::getInstance()->postNotification(st_begin_download_image);
    DataManager::m_bDownloadingImage = true;
    pSender->removeFromParentAndCleanup(true);
    ImageDownload::getInstance()->excuteFunc(EXIST_NEW_VERSION_STATE);

}

void BaseLayer::downloadFinish(Ref *pSender)
{
    DataManager::m_bDownloadingImage = false;
    
    string lStr;
    lStr.clear();
    
    ImageDownload::getInstance()->excuteFunc(FINISH_DOWNLOAD_STATE);
    
    STAlertDialog* lAlertDialog=STAlertDialog::create("Downloaded successfully!","Ok",lStr.c_str());
    lAlertDialog->setLeftListener(this, callfuncN_selector(BaseLayer::downloadFinishRemoveDialog));
    this->addChild(lAlertDialog,INT_MAX-1,INT_MAX-1);
}

void BaseLayer::downloadFinishRemoveDialog(cocos2d::Node *pSender)
{
    pSender->removeFromParentAndCleanup(true);
    
    this->setTouchEnabled(false);
    
//    ImageDownload::getInstance()->excuteFunc(FINISH_DOWNLOAD_STATE);


    refresh();
}

void BaseLayer::noNewVersionReadCsvFinish(Ref *pSender)
{
    refresh();
}

