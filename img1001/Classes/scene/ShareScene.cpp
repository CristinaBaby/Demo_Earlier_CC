//
//  ShareScene.cpp
//  ColorFree
//
//  Created by houhuang on 15/9/9.
//
//

#include "ShareScene.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../scene/EditScene.h"
#include "../scene/ChooseGalleryScene.h"
#include "../define/AppConfigs.h"
#include "../modules/FacebookTool.h"
#include "../modules/STSystemFunction.h"
//#include "../modules/AnalyticX.h"
#include "../modules/STInstagramFunction.h"
#include "../layer/Dialog.h"
#include "../DownLoad/DownLoadPage.h"
#include "../helper/DeviceHelper.h"
#include "../data/DataManager.h"
#include "../crossplatformapi/headers/analytics/Analytics.h"

#include <stdio.h>
#include <vector>
#include <thread>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#endif

enum
{
    st_shareScene_btn_back = 20,
    st_shareScene_btn_home,
    st_shareScene_btn_faceBook,
    st_shareScene_btn_instagram,
    st_shareScene_btn_email,
    st_shareScene_btn_save,
    st_shareScene_btn_more,
};

static Image* _targetImage = NULL;

Scene* ShareScene::scene(Sprite* pSprite)
{
    Scene* scene = Scene::create();
    ShareScene* layer = new ShareScene();
    if (layer && layer->init(pSprite))
    {
        layer->autorelease();
        scene->addChild(layer);
        return scene;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

ShareScene::ShareScene()
{
    m_sImagePath = "";
    m_pShareSprite = NULL;
    
    m_pBackBtn = NULL;
    m_pHomeBtn = NULL;
    
    m_pPostingLayer = NULL;
    m_sMyPostImageName = "";
    m_pPostMenu = NULL;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ShareScene::waitPostSuccessful), st_myPost_success, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ShareScene::waitPostFaild), st_myPost_failed, NULL);
    
}

ShareScene::~ShareScene()
{
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_failed);
    NotificationCenter::getInstance()->removeObserver(this, st_myPost_success);
}

bool ShareScene::init(Sprite* pSprite)
{
    if (!BaseScene::init()) return false;
    
//    LayerColor* bg = LayerColor::create(Color4B(236, 105, 65, 255), ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(700));
//    bg->ignoreAnchorPointForPosition(false);
//    bg->setAnchorPoint(Vec2(0.5, 0.5));
//    ScreenAdapterHelper::setPosition(bg, Vec2(384, 650));
//    this->addChild(bg);
//    bg->setScaleY(ScreenAdapterHelper::getRealDesignScale());
    
    Sprite* lSprite = Sprite::createWithTexture(pSprite->getTexture());
    ScreenAdapterHelper::setPosition(lSprite, Vec2(384, 600));
//    lSprite->setScale(0.8f);
    this->addChild(lSprite);
    lSprite->setFlippedY(true);
    this->initUI();
    m_pShareSprite = lSprite;
    
    pSprite->release();
    this->saveShareImage();

//    bg->setScaleY(0.7 * ScreenAdapterHelper::getRealDesignScale());
    lSprite->setScale(0.66 * ScreenAdapterHelper::getRealDesignScale());
    
    MenuItemImage* post = MenuItemImage::create("btn_active.png", "btn_active.png", CC_CALLBACK_1(ShareScene::reponsePostBtn, this));
    ScreenAdapterHelper::setPosition(post, Vec2(384, 100), KVBoarderTypeBottom, KHBoarderTypeNone);
    
    Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["PostToInspiration"], st_font_AR, ScreenAdapterHelper::getValue(50));
    label->setPosition(Vec2(post->getContentSize().width/2, post->getContentSize().height/2));
    post->addChild(label);
    label->setColor(Color3B::WHITE);
    
    
    Menu* menu = Menu::create(post, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    m_pPostMenu = menu;
    
//    Label* shareOn = Label::createWithTTF("Share On", st_font_PTS, ScreenAdapterHelper::getValue(30));
//    shareOn->ignoreAnchorPointForPosition(false);
//    shareOn->setAnchorPoint(Vec2(0, 0.5));
//    ScreenAdapterHelper::setPosition(shareOn, Vec2(90, 240), KVBoarderTypeBottom, KHBoarderTypeLeft);
//    this->addChild(shareOn);
//    shareOn->setColor(Color3B::BLACK);
    
    
    return true;
}

void ShareScene::initUI()
{
    Label* title = Label::createWithTTF(DM_getInstance->getLanguageMes()["ShareYourArt"], st_font_AR, ScreenAdapterHelper::getValue(60));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setColor(Color3B(236, 64, 122));
    ScreenAdapterHelper::setPosition(title, Vec2(384, 50), KVBoarderTypeTop, KHBoarderTypeNone);
    this->addChild(title);
    
    UIButton* back = UIButton::create("btn_back.png");
    back->initListener(this, callfuncN_selector(ShareScene::doAction));
    ScreenAdapterHelper::setPosition(back, Vec2(60, 50), KVBoarderTypeTop, KHBoarderTypeLeft);
    back->setTag(st_shareScene_btn_back);
    this->addChild(back);
    m_pBackBtn = back;
    
    UIButton* home = UIButton::create("btn_home.png");
    home->initListener(this, callfuncN_selector(ShareScene::doAction));
    ScreenAdapterHelper::setPosition(home, Vec2(60, 50), KVBoarderTypeTop, kHBoarderTypeRight);
    home->setTag(st_shareScene_btn_home);
    this->addChild(home);
    m_pHomeBtn = home;

    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    UIButton* faceBook = UIButton::create("facebook.png");
//    faceBook->initListener(this, callfuncN_selector(ShareScene::doAction));
//    ScreenAdapterHelper::setPosition(faceBook, Vec2(150, 150), KVBoarderTypeBottom, KHBoarderTypeNone);
//    faceBook->setTag(st_shareScene_btn_faceBook);
//    this->addChild(faceBook);
//    
//    Label* faceBook_label = Label::createWithTTF(DM_getInstance->getLanguageMes()["FACEBOOK"], st_font_PTS, ScreenAdapterHelper::getValue(20));
//    faceBook_label->setAnchorPoint(Vec2(0.5, 0.5));
//    ScreenAdapterHelper::setPosition(faceBook_label, Vec2(150, 50), KVBoarderTypeBottom, KHBoarderTypeNone);
//    faceBook_label->setColor(Color3B::BLACK);
//    this->addChild(faceBook_label);
//    
//    
//    UIButton* instagram = UIButton::create("instagram.png");
//    instagram->initListener(this, callfuncN_selector(ShareScene::doAction));
//    ScreenAdapterHelper::setPosition(instagram, Vec2(150 + 156, 150), KVBoarderTypeBottom, KHBoarderTypeNone);
//    instagram->setTag(st_shareScene_btn_instagram);
//    this->addChild(instagram);
//    
//    Label* instagram_label = Label::createWithTTF(DM_getInstance->getLanguageMes()["INSTAGRAM"], st_font_PTS, ScreenAdapterHelper::getValue(20));
//    instagram_label->setAnchorPoint(Vec2(0.5, 0.5));
//    ScreenAdapterHelper::setPosition(instagram_label, Vec2(150 + 156, 50), KVBoarderTypeBottom, KHBoarderTypeNone);
//    instagram_label->setColor(Color3B::BLACK);
//    this->addChild(instagram_label);
//    
//    
//    UIButton* email = UIButton::create("email.png");
//    email->initListener(this, callfuncN_selector(ShareScene::doAction));
//    ScreenAdapterHelper::setPosition(email, Vec2(150 + 156 * 2, 150), KVBoarderTypeBottom, KHBoarderTypeNone);
//    email->setTag(st_shareScene_btn_email);
//    this->addChild(email);
//    
//    Label* email_label = Label::createWithTTF(DM_getInstance->getLanguageMes()["EMAIL"], st_font_PTS, ScreenAdapterHelper::getValue(20));
//    email_label->setAnchorPoint(Vec2(0.5, 0.5));
//    ScreenAdapterHelper::setPosition(email_label, Vec2(150 + 156 * 2, 50), KVBoarderTypeBottom, KHBoarderTypeNone);
//    email_label->setColor(Color3B::BLACK);
//    this->addChild(email_label);
//    
//    
//    UIButton* save = UIButton::create("save.png");
//    save->initListener(this, callfuncN_selector(ShareScene::doAction));
//    ScreenAdapterHelper::setPosition(save, Vec2(150 + 156 * 3, 150), KVBoarderTypeBottom, KHBoarderTypeNone);
//    save->setTag(st_shareScene_btn_save);
//    this->addChild(save);
//    
//    Label* save_label = Label::createWithTTF(DM_getInstance->getLanguageMes()["SAVE"], st_font_PTS, ScreenAdapterHelper::getValue(20));
//    save_label->setAnchorPoint(Vec2(0.5, 0.5));
//    ScreenAdapterHelper::setPosition(save_label, Vec2(150 + 156 * 3, 50), KVBoarderTypeBottom, KHBoarderTypeNone);
//    save_label->setColor(Color3B::BLACK);
//    this->addChild(save_label);
    
    
    UIButton* more = UIButton::create("more_share.png");
    more->initListener(this, callfuncN_selector(ShareScene::doAction));
    ScreenAdapterHelper::setPosition(more, Vec2(512, 240), KVBoarderTypeBottom, KHBoarderTypeNone);
    more->setTag(st_shareScene_btn_more);
    this->addChild(more);
    
    
    UIButton* save = UIButton::create("save.png");
    save->initListener(this, callfuncN_selector(ShareScene::doAction));
    ScreenAdapterHelper::setPosition(save, Vec2(256, 240), KVBoarderTypeBottom, KHBoarderTypeNone);
    save->setTag(st_shareScene_btn_save);
    this->addChild(save);

    
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

//    UIButton* instagram = UIButton::create("instagram.png");
//    instagram->initListener(this, callfuncN_selector(ShareScene::doAction));
//    ScreenAdapterHelper::setPosition(instagram, Vec2(150 , 150), KVBoarderTypeBottom, KHBoarderTypeNone);
//    instagram->setTag(st_shareScene_btn_instagram);
//    this->addChild(instagram);
//    
//    Label* instagram_label = Label::createWithTTF(DM_getInstance->getLanguageMes()["INSTAGRAM"], st_font_PTS, ScreenAdapterHelper::getValue(20));
//    instagram_label->setAnchorPoint(Vec2(0.5, 0.5));
//    ScreenAdapterHelper::setPosition(instagram_label, Vec2(150 , 50), KVBoarderTypeBottom, KHBoarderTypeNone);
//    instagram_label->setColor(Color3B::BLACK);
//    this->addChild(instagram_label);
//    
//    
//    UIButton* email = UIButton::create("email.png");
//    email->initListener(this, callfuncN_selector(ShareScene::doAction));
//    ScreenAdapterHelper::setPosition(email, Vec2(150 + 234, 150), KVBoarderTypeBottom, KHBoarderTypeNone);
//    email->setTag(st_shareScene_btn_email);
//    this->addChild(email);
//    
//    Label* email_label = Label::createWithTTF(DM_getInstance->getLanguageMes()["EMAIL"], st_font_PTS, ScreenAdapterHelper::getValue(20));
//    email_label->setAnchorPoint(Vec2(0.5, 0.5));
//    ScreenAdapterHelper::setPosition(email_label, Vec2(150 + 234, 50), KVBoarderTypeBottom, KHBoarderTypeNone);
//    email_label->setColor(Color3B::BLACK);
//    this->addChild(email_label);
//    
//    
//    UIButton* save = UIButton::create("save.png");
//    save->initListener(this, callfuncN_selector(ShareScene::doAction));
//    ScreenAdapterHelper::setPosition(save, Vec2(150 + 234 * 2, 150), KVBoarderTypeBottom, KHBoarderTypeNone);
//    save->setTag(st_shareScene_btn_save);
//    this->addChild(save);
//    
//    Label* save_label = Label::createWithTTF(DM_getInstance->getLanguageMes()["SAVE"], st_font_PTS, ScreenAdapterHelper::getValue(20));
//    save_label->setAnchorPoint(Vec2(0.5, 0.5));
//    ScreenAdapterHelper::setPosition(save_label, Vec2(150 + 234 * 2, 50), KVBoarderTypeBottom, KHBoarderTypeNone);
//    save_label->setColor(Color3B::BLACK);
//    this->addChild(save_label);
//#endif


}

void ShareScene::onEnterTransitionDidFinish()
{
    BaseScene::onEnterTransitionDidFinish();
//    this->saveShareImage();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void ShareScene::saveShareImage()
{
    m_pShareSprite->setScale(2.0/3.0);
    float scale = 1.5f;
    if (DeviceHelper::getInstance()->getIs2048Device())
    {
        m_pShareSprite->setScale(1.0/3.0);
        scale = 3.0;
    }
    
    Vec2 lOrginPos = m_pShareSprite->getPosition();
    RenderTexture* lRenderTexture = RenderTexture::create(m_pShareSprite->getContentSize().width/scale, m_pShareSprite->getContentSize().height/scale);
    lRenderTexture->begin();
    m_pShareSprite->setPosition(Vec2((m_pShareSprite->getContentSize().width/2)/scale, (m_pShareSprite->getContentSize().height/2)/scale));
    m_pShareSprite->visit();
    lRenderTexture->end();
    
    Director::getInstance()->getRenderer()->render();
    
    STSystemFunction sf;
    Image* lImage = lRenderTexture->newImage();
    string lPath;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    lPath = FileUtils::getInstance()->getWritablePath() + "share.png";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    lPath = sf.getSdCardPath()+"/"+"share_colordiary.png";
#endif
    
    m_sImagePath = lPath;
    lImage->saveToFile(lPath, false);
    lImage->autorelease();
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(lPath.c_str());
    m_pShareSprite->setPosition(lOrginPos);
    m_pShareSprite->setScale(0.66 * ScreenAdapterHelper::getRealDesignScale());
    
}

string ShareScene::getSaveImagePath()
{
    m_pShareSprite->setScale(2.0/3.0);
    float scale = 1.5f;
    if (DeviceHelper::getInstance()->getIs2048Device())
    {
        m_pShareSprite->setScale(1.0/3.0);
        scale = 3.0;
    }
    Vec2 lOrginPos = m_pShareSprite->getPosition();
    RenderTexture* lRenderTexture = RenderTexture::create(m_pShareSprite->getContentSize().width/scale, m_pShareSprite->getContentSize().height/scale);
    lRenderTexture->begin();
    m_pShareSprite->setPosition(Vec2((m_pShareSprite->getContentSize().width/2)/scale, (m_pShareSprite->getContentSize().height/2)/scale));
    m_pShareSprite->visit();
    lRenderTexture->end();
    
    Director::getInstance()->getRenderer()->render();
    
    STSystemFunction sf;
    Image* lImage = lRenderTexture->newImage();
    string lPath;
    
    char buffer[25];
    sprintf(buffer,"postImage%04lu.png",DM_getInstance->m_vMyPostImage.size()+1);
    m_sMyPostImageName = buffer;
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    lPath = FileUtils::getInstance()->getWritablePath() + buffer;
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    lPath = sf.getSdCardPath()+"/"+buffer;
//#endif
    
    
    
    lImage->saveToFile(lPath, false);
    lImage->autorelease();
    
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(lPath.c_str());
    m_pShareSprite->setPosition(lOrginPos);
    m_pShareSprite->setScale(0.66 * ScreenAdapterHelper::getRealDesignScale());
    return buffer;
}

void ShareScene::reponsePostBtn(Ref* ref)
{
    STSystemFunction sf;
    if(!sf.checkNetworkAvailable() && DownLoadPage::getInstance()->m_vInspirationInfo.size() == 0)
    {
        sf.popAlertDialog(DM_getInstance->getLanguageMes()["FBNetworkFaild"].c_str());
        return;
    }
    
    SwallowListenerLayer* lSwallowLayer = SwallowListenerLayer::create(Color4B(0, 0, 0, 200));
    this->addChild(lSwallowLayer);
    lSwallowLayer->setTouchEnable(true);
    m_pPostingLayer = lSwallowLayer;
    
//    Sprite* posting_bg = Sprite::create("btn_active.png");
//    ScreenAdapterHelper::setPosition(posting_bg, Vec2(384, 100), KVBoarderTypeBottom, KHBoarderTypeNone);
//    lSwallowLayer->addChild(posting_bg);
    
    Label* label = Label::createWithTTF(DM_getInstance->getLanguageMes()["Posting"], st_font_AR, ScreenAdapterHelper::getValue(50));
    label->setPosition(Vec2(lSwallowLayer->getContentSize().width/2, lSwallowLayer->getContentSize().height/2));
    label->setColor(Color3B::WHITE);
    lSwallowLayer->addChild(label);
    
    this->getSaveImagePath();
    this->scheduleOnce(schedule_selector(ShareScene::delayePostImage), 0.2f);
    if (m_pPostMenu)
    {
        m_pPostMenu->setEnabled(false);
    }
}

void ShareScene::delayePostImage(float dt)
{
    DownLoadPage::getInstance()->postImage(m_sMyPostImageName);
    if (m_pPostMenu)
    {
        m_pPostMenu->setEnabled(true);
    }
}

void ShareScene::waitPostSuccessful(Ref* ref)
{

    
    if (m_pPostingLayer)
    {
        m_pPostingLayer->removeFromParentAndCleanup(true);
        m_pPostingLayer = NULL;
    }
    
    DM_getInstance->m_vMyPostImage.push_back(m_sMyPostImageName);
    DownLoadPage::getInstance()->m_vPostImageId.push_back(DownLoadPage::m_iCurrentPostImageId);
    if (DM_getInstance->m_vMyPostImage.size() > 200)
    {
        std::vector<std::string>::iterator iter = DM_getInstance->m_vMyPostImage.begin();
        DM_getInstance->m_vMyPostImage.erase(iter);

        std::vector<int>::iterator iter2 = DownLoadPage::getInstance()->m_vPostImageId.begin();
        DownLoadPage::getInstance()->m_vPostImageId.erase(iter2);
    }
    
    DM_getInstance->saveMyPostsDatas();
    
//    thread lThread = std::thread(&ShareScene::threadSaveDatas, this);
//    lThread.detach();
    
    STAlertDialog* dialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["GoToInspiration"], DM_getInstance->getLanguageMes()["Later"], DM_getInstance->getLanguageMes()["GoToInsBtn"]);
    dialog->setLeftListener(this, callfuncN_selector(ShareScene::removePostDialog));
    dialog->setRightListener(this, callfuncN_selector(ShareScene::reponsePostSuccessful));
    this->addChild(dialog, 100);
}

void ShareScene::threadSaveDatas()
{
    DM_getInstance->saveMyPostsDatas();
}

void ShareScene::waitPostFaild(Ref* ref)
{
    if (m_pPostingLayer)
    {
        m_pPostingLayer->removeFromParentAndCleanup(true);
        m_pPostingLayer = NULL;
    }
    
    STAlertDialog* dialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["PostFaild"], DM_getInstance->getLanguageMes()["Later"], DM_getInstance->getLanguageMes()["TryAgain"]);
    dialog->setLeftListener(this, callfuncN_selector(ShareScene::removePostDialog));
    dialog->setRightListener(this, callfuncN_selector(ShareScene::reponsePostFaild));
    this->addChild(dialog, 100);
}

void ShareScene::reponsePostSuccessful(Node* node)
{
    string filterName = "Post_";
    for (int i = 0; i < DM_getInstance->m_vFilterDatasBase.size(); ++i)
    {
        FilterInfo lFiterInfo = DM_getInstance->m_vFilterDatasBase.at(i);
        if (!lFiterInfo.labelIsBlack)
        {
            filterName += DM_getInstance->getLanguageMes()[lFiterInfo.filterName]; ;
            break;
        }
    }
    
    string frameName = "Post_";
    for (int i = 0; i < DM_getInstance->m_vFramesDatasBase.size(); ++i)
    {
        FramesInfo lFiterInfo = DM_getInstance->m_vFramesDatasBase.at(i);
        if (!lFiterInfo.labelIsBlack)
        {
            frameName += DM_getInstance->getLanguageMes()[lFiterInfo.frameName];
            break;
        }
    }
    
    DataManager::m_bIsClickText;
    
    stringstream ss;
    ss<<"";
    ss<<"Post_";
    ss<<DM_getInstance->m_pCurrentImage.imageName;
    ss<<DM_getInstance->m_pCurrentImage.ID;
    Analytics::getInstance()->sendEvent(ss.str().c_str(), ss.str().c_str());
    
    Analytics::getInstance()->sendEvent(filterName.c_str(), filterName.c_str());
    Analytics::getInstance()->sendEvent(frameName.c_str(), frameName.c_str());
    
    if (DataManager::m_bIsClickText)
    {
        DataManager::m_bIsClickText = false;
        Analytics::getInstance()->sendEvent("Post_Text", "Post_Text");
    }
    
    STAlertDialog* lDialog = dynamic_cast<STAlertDialog*>(node);
    if (lDialog)
    {
        lDialog->removeFromParentAndCleanup(true);
    }
    
    DM_getInstance->m_bIsGalleryLayer = false;
    Scene* scene = ChooseGalleryScene::scene();
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.35f, scene));
}

void ShareScene::reponsePostFaild(Node* node)
{
    STAlertDialog* lDialog = dynamic_cast<STAlertDialog*>(node);
    if (lDialog)
    {
        lDialog->removeFromParentAndCleanup(true);
    }
    
    this->reponsePostBtn(NULL);
    
//    DownLoadPage::getInstance()->postImage(m_sMyPostImageName);
}

void ShareScene::removePostDialog(Node* node)
{
    STAlertDialog* lDialog = dynamic_cast<STAlertDialog*>(node);
    if (lDialog)
    {
        lDialog->removeFromParentAndCleanup(true);
    }
}

void ShareScene::doAction(Node* pNode)
{
    UIButton* button = (UIButton*)pNode;
    
    switch (button->getTag())
    {
        case st_shareScene_btn_back:
        {
//            button->setButtonEnable(false);
            m_pBackBtn->setButtonEnable(false);
            m_pHomeBtn->setButtonEnable(false);
            
            Director::getInstance()->popSceneWithTransition<TransitionSlideInL>(0.35f);
        }
            break;
            
        case st_shareScene_btn_home:
        {
//            button->setButtonEnable(false);
            m_pBackBtn->setButtonEnable(false);
            m_pHomeBtn->setButtonEnable(false);
            
            DataManager::getInstance()->m_bIsAds = true;
            
            Scene* scene = ChooseGalleryScene::scene();
            Director::getInstance()->replaceScene(scene);
        }
            break;
            
        case st_shareScene_btn_faceBook:
        {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Sharing_Facebook") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            string lBodyStr = "Check out my artwork painted with ";
            lBodyStr = lBodyStr + project_name;
            lBodyStr.append("\n");
            
            string linkStr = "http://itunes.apple.com/us/app/id";
            linkStr = linkStr + apple_id;
            linkStr.append("?mt=8</\n");
            
            FacebookTool::postToFacebook(lBodyStr.c_str(), m_sImagePath.c_str(), linkStr.c_str());

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Share_Facebook") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            string lBodyStr = "Check out my artwork painted with ";
            lBodyStr = lBodyStr + project_name;
            lBodyStr.append("\n");
            
            string linkStr = "https://play.google.com/store/apps/details?id=";
            linkStr.append(st_android_package_name).append("\n");
            
            FacebookTool::postToFacebook(lBodyStr.c_str(), m_sImagePath.c_str(), linkStr.c_str());
            
#endif
            
        }
            break;
            
        case st_shareScene_btn_instagram:
        {
 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Sharing_Inatagram") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            STSystemFunction sf;
            sf.sendPictureToInstagram(m_sImagePath);
            

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Share_Inatagram") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            STInstagramFunction st;
            st.sendImageToInstagram(m_sImagePath.c_str());
#endif
            
        }
            break;
            
        case st_shareScene_btn_email:
        {

            string lBodyStr = "Check out my artwork painted with ";
            lBodyStr = lBodyStr + project_name;
            lBodyStr.append("\n");
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Sharing_Email") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            string linkStr = "<a>https://itunes.apple.com/us/app/id";
            linkStr += apple_id;
            linkStr += "?mt=8</a>\n";
            lBodyStr.append(linkStr);
            
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Share_Email") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            lBodyStr.append("http://");
            
#endif
            
            STSystemFunction sf;
            sf.sendEmailAndFilePic(project_name, lBodyStr, m_sImagePath);
            
        }
            break;
            
        case st_shareScene_btn_save:
        {

            Vec2 lOrginPos = m_pShareSprite->getPosition();
            RenderTexture* lRenderTexture = RenderTexture::create(m_pShareSprite->getContentSize().width, m_pShareSprite->getContentSize().height);
            lRenderTexture->begin();
            m_pShareSprite->setPosition(Vec2(m_pShareSprite->getContentSize().width/2, m_pShareSprite->getContentSize().height/2));
            m_pShareSprite->visit();
            lRenderTexture->end();
            lRenderTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
            
            Director::getInstance()->getRenderer()->render();
            
            Image* lImage = lRenderTexture->newImage();
            
            if (_targetImage != NULL)
            {
                delete _targetImage;
                _targetImage = NULL;
            }
            
            _targetImage = lImage;
            
            
            STSystemFunction sf;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Sharing_Save") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            sf.saveLocalImage(lImage);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            for (int i = 0; i < DM_FlurryDatas.size(); ++i)
            {
                FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
                if (strcmp(lFlurryInfo->message.c_str(), "Share_Save") == 0)
                {
                    lFlurryInfo->number++;
                }
            }
            
            std::string writePath = sf.getSdCardPath()+"/Color Diary/";
            if (!FileUtils::sharedFileUtils()->isFileExist(writePath))
            {
                this->createDirectory(writePath.c_str());
            }
            
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string _time = StringUtils::format("%ld", (now.tv_sec * 1000 + now.tv_usec / 1000));
            std::string fullPath = writePath + _time + ".png";
            
            //  CCLOG("------------lPath=%s",fullPath.c_str());
            
            bool isSaveSuccess = _targetImage->saveToFile(fullPath.c_str());
            if(isSaveSuccess)
                sf.makeToast("Successfully Saved!");
            else
                sf.makeToast("Unable to save. Please try again later.");
            
            sf.refreshDCIM(fullPath);
#endif
            
            m_pShareSprite->setPosition(lOrginPos);
            
        }
            break;
            
        case st_shareScene_btn_more:
        {
            
            STSystemFunction sf;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            sf.sendPictureToMore(m_sImagePath);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            STInstagramFunction st;
            st.sendImageToInstagram(m_sImagePath.c_str());
#endif
            
            
        }
            break;
            
        default:
            break;
    }
}

bool ShareScene::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, nullptr);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#endif
    
}