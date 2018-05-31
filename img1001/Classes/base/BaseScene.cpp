//
//  BaseScene.cpp
//  IMG1001
//
//  Created by houhuang on 15/10/19.
//
//

#include "BaseScene.hpp"
#include "../define/Globalmacro.h"
#include "../layer/Dialog.h"
#include "../DownLoad/DownLoadPage.h"
#include "../data/DataManager.h"
#include "../helper/ScreenAdapterHelper.h"

enum
{
    st_newImageDatas_tag = 999,

};

BaseScene::BaseScene()
{
    m_pLabelSprite = NULL;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseScene::waitMessage), st_baseScene_finfNewVersion_notifition, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseScene::waitMessageDownLoadSuc), st_baseScene_downloadSuc_notifition, NULL);
}

BaseScene::~BaseScene()
{
    NotificationCenter::getInstance()->removeObserver(this, st_baseScene_finfNewVersion_notifition);
    NotificationCenter::getInstance()->removeObserver(this, st_baseScene_downloadSuc_notifition);
}

bool BaseScene::init()
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;
    
    
    
    return true;
}

void BaseScene::waitMessage(Ref* ref)
{
    STAlertDialog* lDialog = dynamic_cast<STAlertDialog*>(this->getChildByTag(st_newImageDatas_tag));
    if (lDialog)
    {
        return;
    }
    
    STAlertDialog* dialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["DownloadNewResource"], DM_getInstance->getLanguageMes()["Later"], DM_getInstance->getLanguageMes()["DownloadNow"]);
    dialog->setLeftListener(this, callfuncN_selector(BaseScene::removeDialog));
    dialog->setRightListener(this, callfuncN_selector(BaseScene::downLoadResource));
    dialog->setTag(st_newImageDatas_tag);
    this->addChild(dialog, 101);
}

void BaseScene::waitMessageDownLoadSuc(Ref* ref)
{

    STAlertDialog* dialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["DownloadSuc"], DM_getInstance->getLanguageMes()["OK"], "");
    dialog->setLeftListener(this, callfuncN_selector(BaseScene::removeDialog));
    this->addChild(dialog, 101);
    
    
}

void BaseScene::downLoadResource(Node* pNode)
{
    DataManager::m_bIsEnableCheck = false;
    
    Dialog* lDialog = (Dialog*)pNode;
    if (lDialog)
    {
        lDialog->removeFromParentAndCleanup(true);
    }
    
    DownLoadPage::getInstance()->startDownload();
    
    Sprite* lSprite = Sprite::create("pop_download.png");
    ScreenAdapterHelper::setPosition(lSprite, Vec2(384, 512));
    this->addChild(lSprite, 101);
    m_pLabelSprite = lSprite;
    lSprite->setOpacity(0);

    Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["Downloading"], st_font_AR, ScreenAdapterHelper::getValue(30));
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
    lSprite->addChild(label);
    
    m_pLabelSprite->runAction(FadeIn::create(0.5f));
    
    this->scheduleOnce(schedule_selector(BaseScene::removeLabel), 2.0f);
    
}

void BaseScene::removeLabel(float dt)
{
    if (m_pLabelSprite)
    {
        m_pLabelSprite->removeFromParentAndCleanup(true);
    }
}

void BaseScene::removeDialog(Node* pNode)
{
    //updateScene
    this->updateScene();
    NotificationCenter::getInstance()->postNotification(st_filterAndFrames_downLoadSuc);
    
    DataManager::m_bIsEnableCheck = false;
    
    Dialog* lDialog = (Dialog*)pNode;
    if (lDialog)
    {
        lDialog->removeFromParentAndCleanup(true);
    }
}








