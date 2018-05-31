//
//  ServerSprite.cpp
//  IMG1001
//
//  Created by houhuang on 15/11/17.
//
//

#include "ServerSprite.hpp"
//#include "../helper/ScreenAdapterHelper.h"
#include "DownLoadPage.h"
//#include "../modules/STSystemFunction.h"
//#include "../helper/DeviceHelper.h"
#include "ThreadManager.hpp"
#include "STSystemFunction.h"
#include "UITools.h"

ServerSprite::~ServerSprite()
{
    NotificationCenter::getInstance()->removeObserver(this, st_inspiration_downloadSuc);
    NotificationCenter::getInstance()->removeObserver(this, st_multiThread_loadImage_finish);
}

ServerSprite::ServerSprite()
{
    m_pRotateSprite = NULL;
    m_sImageUrl = "";
    m_bIsclickLike = false;
    m_pLikeSprite = NULL;
    m_pLikeLabel = NULL;
    m_pTextureSprite = NULL;
    m_pTargetTexture = NULL;
    m_pMenuItem = NULL;
    m_bIsDownloadFinish = false;
    m_bIsHide = true;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ServerSprite::waitDownLoadSuc), st_inspiration_downloadSuc, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ServerSprite::LoadMapSprite), st_multiThread_loadImage_finish, NULL);
}

ServerSprite* ServerSprite::create(const string& imageUrl, int id, int likeNumber)
{
    ServerSprite* lNode = new ServerSprite();
    if (lNode && lNode->init(imageUrl, id, likeNumber))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;
}

bool ServerSprite::init(const string& imageUrl, int id, int likeNumber)
{
    if (!Node::init()) return false;
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    _imageID = id;
    _likeNumber = likeNumber;
    m_sImageUrl = imageUrl;
    
    Sprite* border = Sprite::create("cocos/my_arts/frame2.png");
    
    this->addChild(border, -1);
    this->setContentSize(Size(border->getContentSize().width, border->getContentSize().height));
    border->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));

    for (int i = 0; i < DownLoadPage::getInstance()->m_vAlreadyClickLick.size(); ++i)
    {
        if (DownLoadPage::getInstance()->m_vAlreadyClickLick.at(i) == id)
        {
            m_bIsclickLike = true;
            break;
        }
    }
    
    return true;

}

void ServerSprite::display()
{
    if (!m_bIsHide)
    {
        return;
    }
    
    m_bIsHide = false;
    
    Sprite* rotateSprite = Sprite::create("cocos/other/load_rotate_icon.png");
    rotateSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(rotateSprite, 10);
    rotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));
    m_pRotateSprite = rotateSprite;

    if (FileUtils::getInstance()->isFileExist(this->getImageFullPath()) )
    {
        m_bIsDownloadFinish = true;
//        Sprite* lSprite = Sprite::create(ss.str());
//        lSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
//        this->addChild(lSprite);

        if (!m_pMenuItem)
        {
            
            MenuItemImage* lMenuItem = MenuItemImage::create("cocos/my_arts/heart1.png", "cocos/my_arts/heart1.png", CC_CALLBACK_1(ServerSprite::reponseMenu, this));
            lMenuItem->ignoreAnchorPointForPosition(false);
            lMenuItem->setAnchorPoint(Vec2(0, 1));
            lMenuItem->setPosition(Vec2(500, 0));
            m_pMenuItem = lMenuItem;
            
            Sprite* heart = Sprite::create("cocos/my_arts/heart.png");
            heart->setPosition(Vec2(lMenuItem->getContentSize().width/2, lMenuItem->getContentSize().height/2));
            lMenuItem->addChild(heart);
            heart->setVisible(m_bIsclickLike);
            m_pLikeSprite = heart;
            
            Label* label = Label::createWithTTF("0", "fonts/verdana_0.ttf", 40);
            label->ignoreAnchorPointForPosition(false);
            label->setAnchorPoint(Vec2(1, 0.5));
            label->setPosition(Vec2(-lMenuItem->getContentSize().width * 0.3, lMenuItem->getContentSize().height/2));
            lMenuItem->addChild(label);
            label->setColor(Color3B::WHITE);
            m_pLikeLabel = label;
            stringstream ss;
            ss<<"";
            ss<<_likeNumber;
            label->setString(ss.str());

            
            Menu* lMenu = Menu::create(lMenuItem, NULL);
            lMenu->setPosition(Vec2::ZERO);
            this->addChild(lMenu);
//            lMenu->setScale(0.8f);
        }
        
        
//        if (m_pRotateSprite)
//        {
//            m_pRotateSprite->removeFromParentAndCleanup(true);
//            m_pRotateSprite = NULL;
//        }
        
        
        Texture2D* lTexture2D = Director::getInstance()->getTextureCache()->getTextureForKey(this->getImageFullPath());
        if (lTexture2D)
        {
            this->imageLoadedCallback(lTexture2D);
        }else
        {
            xThread->m_vNeedLoadImage.push_back(this->getImageFullPath());
            xThread->startLoadImage();
//            DataManager::getInstance()->m_vNeedLoadImage.push_back(this->getImageFullPath());
//            DataManager::getInstance()->startLoadImage();

        }
        

    }
    else
    {
        DownloadImageInfo imageInfo;
        imageInfo.imageID = _imageID;
        imageInfo.imageUrl = m_sImageUrl;
        
        DownLoadPage::getInstance()->m_mNeedDownloadImage.push_back(imageInfo);
//        DownLoadPage::getInstance()->downLoadImage(m_sImageUrl, _imageID);
        DownLoadPage::getInstance()->startLoadImage();
    }

}

void ServerSprite::removeTextureCache()
{
    for (int i = 0; i < DownLoadPage::getInstance()->m_mNeedDownloadImage.size(); ++i)
    {
        DownloadImageInfo info = DownLoadPage::getInstance()->m_mNeedDownloadImage.at(i);
        if (info.imageID == _imageID)
        {
            std::vector<DownloadImageInfo>::iterator iter = DownLoadPage::getInstance()->m_mNeedDownloadImage.begin() + i;
            DownLoadPage::getInstance()->m_mNeedDownloadImage.erase(iter);
            
            break;
        }
    }
    
    for (int i = 0; i < xThread->m_vNeedLoadImage.size(); ++i)
    {
        string str = xThread->m_vNeedLoadImage.at(i);
        if (str == this->getImageFullPath())
        {
            deque<string>::iterator iter = xThread->m_vNeedLoadImage.begin() + i;
            xThread->m_vNeedLoadImage.erase(iter);

            break;
        }
    }
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void ServerSprite::hideThis()
{
    if (m_bIsHide)
    {
        return;
    }
    
    m_bIsHide = true;
    if (m_pTextureSprite)
    {
        
        m_pTextureSprite->removeFromParentAndCleanup(true);
        m_pTextureSprite = NULL;
        
        stringstream ss;
        ss<<"";
        ss<<_imageID;
        ss<<".png";
        Director::getInstance()->getTextureCache()->removeTextureForKey(ss.str());
    }
    
    if (m_pRotateSprite)
    {
        m_pRotateSprite->removeFromParentAndCleanup(true);
        m_pRotateSprite = NULL;
    }
    
}

void ServerSprite::waitDownLoadSuc(Ref* ref)
{
    if (m_bIsHide)
    {
        if (m_pTextureSprite)
        {
            
            m_pTextureSprite->removeFromParentAndCleanup(true);
            m_pTextureSprite = NULL;
            
            stringstream ss;
            ss<<"";
            ss<<_imageID;
            ss<<".png";
            Director::getInstance()->getTextureCache()->removeTextureForKey(ss.str());
        }
        
        if (m_pRotateSprite)
        {
            m_pRotateSprite->removeFromParentAndCleanup(true);
            m_pRotateSprite = NULL;
        }
        
        return;
    }
    
    if (FileUtils::getInstance()->isFileExist(this->getImageFullPath()) && m_bIsDownloadFinish == false)
    {
        m_bIsDownloadFinish = true;
        
        MenuItemImage* lMenuItem = MenuItemImage::create("cocos/my_arts/heart1.png", "cocos/my_arts/heart1.png", CC_CALLBACK_1(ServerSprite::reponseMenu, this));
        lMenuItem->ignoreAnchorPointForPosition(false);
        lMenuItem->setAnchorPoint(Vec2(0, 1));
        lMenuItem->setPosition(Vec2(500, 0));
        m_pMenuItem = lMenuItem;
        
        Sprite* heart = Sprite::create("cocos/my_arts/heart.png");
        heart->setPosition(Vec2(lMenuItem->getContentSize().width/2, lMenuItem->getContentSize().height/2));
        lMenuItem->addChild(heart);
        heart->setVisible(m_bIsclickLike);
        m_pLikeSprite = heart;
        
        Label* label = Label::createWithTTF("", "fonts/verdana_0.ttf", 40);
        label->ignoreAnchorPointForPosition(false);
        label->setAnchorPoint(Vec2(1, 0.5));
        label->setPosition(Vec2(-lMenuItem->getContentSize().width * 0.3, lMenuItem->getContentSize().height/2));
        lMenuItem->addChild(label);
        label->setColor(Color3B::WHITE);
        m_pLikeLabel = label;
        stringstream ss;
        ss<<"";
        ss<<_likeNumber;
        label->setString(ss.str());
        
        Menu* lMenu = Menu::create(lMenuItem, NULL);
        lMenu->setPosition(Vec2::ZERO);
        this->addChild(lMenu);
        
//        if (m_pRotateSprite)
//        {
//            m_pRotateSprite->removeFromParentAndCleanup(true);
//            m_pRotateSprite = NULL;
//        }
        
        xThread->m_vNeedLoadImage.push_back(this->getImageFullPath());
        xThread->startLoadImage();
//        DataManager::getInstance()->m_vNeedLoadImage.push_back(this->getImageFullPath());
//        DataManager::getInstance()->startLoadImage();
//        DataManager::getInstance()->dynamicLoadTexture(this->getImageFullPath());
    }
}

void ServerSprite::reponseMenu(Ref* ref)
{
    if (m_bIsclickLike == false)
    {
        m_bIsclickLike = true;
        if (m_pLikeSprite)
        {
            STSystemFunction sf;
            if(!sf.checkNetworkAvailable())
            {
                sf.popAlertDialog("error network");
                return;
            }
            
            DownLoadPage::getInstance()->postClickLike(_imageID);
            DownLoadPage::getInstance()->m_vAlreadyClickLick.push_back(_imageID);
            
            m_pLikeSprite->setVisible(true);
            
            stringstream ss;
            ss<<"";
            ss<<_likeNumber + 1;
            m_pLikeLabel->setString(ss.str());
            
//            DM_getInstance->m_vMyFavoriteDatas.push_back(_imageID);
            DownLoadPage::getInstance()->getLikeNumber(_imageID);
            
//            DM_getInstance->m_vMyFavoriteLikeNum.push_back(_likeNumber + 1);
            
//            if (DM_getInstance->m_vMyFavoriteDatas.size() > 200)
//            {
//                std::vector<int>::iterator iter = DM_getInstance->m_vMyFavoriteDatas.begin();
//                DM_getInstance->m_vMyFavoriteDatas.erase(iter);
//                
//                std::vector<int>::iterator iter2 = DM_getInstance->m_vMyFavoriteLikeNum.begin();
//                DM_getInstance->m_vMyFavoriteLikeNum.erase(iter2);
//            }
//            
//            DM_getInstance->saveFavoriteDatas();
        }
    }
}

void ServerSprite::imageLoadedCallback(Ref* pSender)
{
    Texture2D* lTexture = (Texture2D*)pSender;
    
    Sprite* lRenderSprite = Sprite::createWithTexture(lTexture);
    
    Director::getInstance()->getTextureCache()->removeTexture(lTexture);
    
    if (lRenderSprite != NULL && m_pTextureSprite == NULL)
    {
        lRenderSprite->getTexture()->setAntiAliasTexParameters();
        lRenderSprite->setPosition(Vec2(this->getContentSize().width/2 - 1, this->getContentSize().height/2+ 3));
        
//        if (DeviceHelper::getInstance()->getIs2048Device())
//        {
//            lRenderSprite->setScale(2.0f);
//        }

        this->addChild(lRenderSprite);
        lRenderSprite->setScale(0.53f);
        
        m_pTextureSprite = lRenderSprite;
        
        if (m_pRotateSprite)
        {
            m_pRotateSprite->removeFromParentAndCleanup(true);
            m_pRotateSprite = NULL;
        }
    }
}

void ServerSprite::LoadMapSprite(Ref* pSender)
{
    //解决快速滑动时，连续调用display()和hideThis()导致图片缓存未释放问题
    
//    if (m_bIsHide)
//    {
//        if (m_pTextureSprite)
//        {
//            
//            m_pTextureSprite->removeFromParentAndCleanup(true);
//            m_pTextureSprite = NULL;
//            
//            stringstream ss;
//            ss<<"";
//            ss<<_imageID;
//            ss<<".png";
//            Director::getInstance()->getTextureCache()->removeTextureForKey(ss.str());
//        }
//        
//        if (m_pRotateSprite)
//        {
//            m_pRotateSprite->removeFromParentAndCleanup(true);
//            m_pRotateSprite = NULL;
//        }
//        
//        return;
//    }
//    
//    if (m_bIsHide)
//    {
//        Director::getInstance()->getTextureCache()->removeTextureForKey(this->getImageFullPath());
//        
//        return;
//    }
    
    if (m_pTargetTexture == NULL)
    {
        Texture2D* lTexture;
        lTexture = Director::getInstance()->getTextureCache()->getTextureForKey(this->getImageFullPath());
        
        if (lTexture != NULL)
        {
            this->imageLoadedCallback(lTexture);
        }
    }
}

string ServerSprite::getImageFullPath()
{
    
    string path = FileUtils::getInstance()->getWritablePath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    path+="Inspiration/";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    path+="Inspiration/";
#endif
    
    stringstream ss;
    ss<<"";
    ss<<path;
    ss<<_imageID;
    ss<<".png";
    
    return ss.str();
}


ServerSprite* ServerSprite::create(int id, int likeNumber)
{
    ServerSprite* lNode = new ServerSprite();
    if (lNode && lNode->init(id, likeNumber))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;
}

bool ServerSprite::init(int id, int likeNumber)
{
    if (!Node::init()) return false;
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    _imageID = id;
    _likeNumber = likeNumber;
//    m_sImageUrl = imageUrl;
    
    Sprite* border = Sprite::create("map_frame.png");
    this->addChild(border, -1);
    this->setContentSize(Size(border->getContentSize().width, border->getContentSize().height));
    border->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));

    
//    for (int i = 0; i < DM_getInstance->m_vMyFavoriteDatas.size(); ++i)
//    {
//        if (DM_getInstance->m_vMyFavoriteDatas.at(i) == id)
//        {
//            m_bIsclickLike = true;
//        }
//    }
    
    return true;
    
}

