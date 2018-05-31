//
//  FavoriteSprite.cpp
//  IMG1001
//
//  Created by houhuang on 15/11/17.
//
//

#include "FavoriteSprite.hpp"
#include "../helper/ScreenAdapterHelper.h"
#include "../DownLoad/DownLoadPage.h"
#include "../helper/DeviceHelper.h"

FavoriteSprite::~FavoriteSprite()
{
    NotificationCenter::getInstance()->removeObserver(this, st_datamanager_dynamicLoadTexture);
    NotificationCenter::getInstance()->removeObserver(this, st_FavoriteSprite_getLikeNumber_suc);
}

FavoriteSprite::FavoriteSprite()
{
    m_pRotateSprite = NULL;
    m_iImageName = -1;
    m_iLikeNumber = -1;
    m_bIsHide = true;
    m_pTextureSprite = NULL;
    m_pTargetTexture = NULL;
    m_bIsPost = false;
    m_pLikeLabel = NULL;
    m_pHeartSpr = NULL;
    m_bIsGetLike = false;
    m_bIsHeart = false;
    m_bIsPostLayer = false;
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(FavoriteSprite::LoadMapSprite), st_datamanager_dynamicLoadTexture, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(FavoriteSprite::getLikeNumberSuc), st_FavoriteSprite_getLikeNumber_suc, NULL);

    
}

FavoriteSprite* FavoriteSprite::create(int imageName, int likeNum)
{
    FavoriteSprite* lNode = new FavoriteSprite();
    if (lNode && lNode->init(imageName,likeNum))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;
}

bool FavoriteSprite::init(int imageName, int likeNum)
{
    if (!Node::init()) return false;
    
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    m_iImageName = imageName;
    m_iLikeNumber = likeNum;
    
    Sprite* border = Sprite::create("map_frame.png");
    this->addChild(border, -1);
    this->setContentSize(Size(border->getContentSize().width, border->getContentSize().height));
    border->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    
    return true;

}

FavoriteSprite* FavoriteSprite::create(string imageName, int imageID)
{
    FavoriteSprite* lNode = new FavoriteSprite();
    if (lNode && lNode->init(imageName,imageID))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;
}

bool FavoriteSprite::init(string imageName, int imageID)
{
    if (!Node::init()) return false;
    
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    m_iImageName = imageID;
    _postImageName = imageName;
    
    m_bIsPostLayer = true;
    
    Sprite* border = Sprite::create("map_frame.png");
    this->addChild(border, -1);
    this->setContentSize(Size(border->getContentSize().width, border->getContentSize().height));
    border->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    
    return true;
}



void FavoriteSprite::display()
{
    if (!m_bIsHide)
    {
        return;
    }
    
//    if (!m_bIsGetLike)
//    {
//        m_bIsGetLike = true;
//        DownLoadPage::getInstance()->getLikeNumber(m_iImageName);
//    }
    
    if (m_iImageName > 0)
    {
        DownLoadPage::getInstance()->updateNeedGetLikeImage(m_iImageName);
        
        
        DownLoadPage::getInstance()->getLikeNumber(m_iImageName);
    }
    
    this->setImageFullPath();
    
    m_bIsHide = false;
    
    Sprite* rotateSprite = Sprite::create("load_rotate_icon.png");
    rotateSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(rotateSprite, 10);
    rotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));
    m_pRotateSprite = rotateSprite;
    
    if (FileUtils::getInstance()->isFileExist(m_sImageFullPath))
    {
//        Sprite* lSprite = Sprite::create(m_sImageFullPath);
//        lSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
//        this->addChild(lSprite);
//        lSprite->setOpacity(0);
//        lSprite->runAction(FadeIn::create(0.3f));
        
//        if (DeviceHelper::getInstance()->getIs2048Device())
//        {
//            lSprite->setScale(2.0f);
//        }
        
        DataManager::getInstance()->m_vNeedLoadImage.push_back(m_sImageFullPath);
        DataManager::getInstance()->startLoadImage();
        
        if (!m_bIsHeart && m_iImageName > 0)
        {
            m_bIsHeart = true;
            Sprite* heart = Sprite::create("heart.png");
            heart->setPosition(Vec2(ScreenAdapterHelper::getValue(40), ScreenAdapterHelper::getValue(-10)));
            this->addChild(heart);
//            heart->setVisible(false);
            m_pHeartSpr = heart;
            
            //        stringstream ss;
            //        ss<<"";
            //        ss<<DownLoadPage::getInstance()->m_mFavoriteInfo[m_iImageName];
            
            Label* label = Label::createWithTTF("0", st_font_AR, ScreenAdapterHelper::getValue(50));
            label->ignoreAnchorPointForPosition(false);
            label->setAnchorPoint(Vec2(0, 0.5));
            label->setPosition(Vec2(heart->getContentSize().width * 1.2, heart->getContentSize().height/2));
            heart->addChild(label);
            label->setColor(Color3B::BLACK);
            m_pLikeLabel = label;
        }
        
        
//        DataManager::getInstance()->dynamicLoadTexture(m_sImageFullPath);
    }

//    Sprite* heart = Sprite::create("heart.png");
//    heart->setPosition(Vec2(ScreenAdapterHelper::getValue(40), ScreenAdapterHelper::getValue(-10)));
//    this->addChild(heart);
//    
//    stringstream ss;
//    ss<<"";
//    ss<<m_iLikeNumber;
//    
//    Label* label = Label::createWithTTF(ss.str(), st_font_AR, ScreenAdapterHelper::getValue(50));
//    label->ignoreAnchorPointForPosition(false);
//    label->setAnchorPoint(Vec2(0, 0.5));
//    label->setPosition(Vec2(heart->getContentSize().width * 1.2, heart->getContentSize().height/2));
//    heart->addChild(label);
//    label->setColor(Color3B::BLACK);
//    
//    
//    for (int i = 0; i < DownLoadPage::getInstance()->m_vInspirationInfo.size(); ++i)
//    {
//        InspirationInfo lInsInfo = DownLoadPage::getInstance()->m_vInspirationInfo.at(i);
//        if (lInsInfo.ID == m_iImageName)
//        {
//            stringstream ss;
//            ss<<"";
//            if (lInsInfo.likeNumber > m_iLikeNumber)
//            {
//                ss<<lInsInfo.likeNumber;
//            }else
//            {
//                ss<<m_iLikeNumber;
//            }
//
//            label->setString(ss.str());
//        }
//    }
}

void FavoriteSprite::getLikeNumberSuc(Ref* pSender)
{
    stringstream ss;
    ss<<"";
    for (int i = 0; i < DownLoadPage::getInstance()->m_vFavoriteInfo.size(); ++i)
    {
        if (DownLoadPage::getInstance()->m_vFavoriteInfo.at(i).imageID == m_iImageName)
        {
            ss<<DownLoadPage::getInstance()->m_vFavoriteInfo.at(i).likeNumber;
            log("%d",DownLoadPage::getInstance()->m_vFavoriteInfo.at(i).likeNumber);
            break;
        }
    }
    
//    log("%s",ss.str().c_str());
    
    if (m_pLikeLabel)
    {
        m_pLikeLabel->setString(ss.str());
    }
    
    log("------%lu",DownLoadPage::getInstance()->m_vNeedGetLikeImage.size());
}

void FavoriteSprite::hideThis()
{
    if (m_bIsHide)
    {
        return;
    }
    
    for (int i =0; i < DownLoadPage::getInstance()->m_vNeedGetLikeImage.size(); ++i)
    {
        if (m_iImageName == DownLoadPage::getInstance()->m_vNeedGetLikeImage.at(i))
        {
            std::vector<int>::iterator iter = DownLoadPage::getInstance()->m_vNeedGetLikeImage.begin() + i;
            DownLoadPage::getInstance()->m_vNeedGetLikeImage.erase(iter);
//            break;
        }
    }
    
    m_bIsHide = true;
    if (m_pTextureSprite)
    {
        
        m_pTextureSprite->removeFromParentAndCleanup(true);
        m_pTextureSprite = NULL;
        
        stringstream ss;
        ss<<"";
        ss<<m_iImageName;
        ss<<".png";
        Director::getInstance()->getTextureCache()->removeTextureForKey(ss.str());
        
        stringstream ss2;
        ss<<"";
        ss<<_postImageName;
        ss<<".png";
        Director::getInstance()->getTextureCache()->removeTextureForKey(ss2.str());
    }
    
    if (m_pRotateSprite)
    {
        m_pRotateSprite->removeFromParentAndCleanup(true);
        m_pRotateSprite = NULL;
    }
    
}

void FavoriteSprite::setImageFullPath()
{
    if (m_bIsPostLayer)
    {
        string fullPath = FileUtils::getInstance()->getWritablePath() + _postImageName;
        if (FileUtils::getInstance()->isFileExist(fullPath))
        {
            //  Director::getInstance()->getTextureCache()->removeTextureForKey(fullPath);
            m_sImageFullPath = fullPath;
        }
        return;
    }
    
    string path = FileUtils::getInstance()->getWritablePath();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        path+="Inspiration/";
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        path+="Inspiration/";
    #endif
    stringstream ss;
        ss<<"";
        ss<<path;
        ss<<m_iImageName;
        ss<<".png";

    m_sImageFullPath = ss.str();
}

string FavoriteSprite::getImageFullPath()
{
    return m_sImageFullPath;
}

int FavoriteSprite::getImageName()
{
    return m_iImageName;
}

void FavoriteSprite::imageLoadedCallback(Ref* pSender)
{
    Texture2D* lTexture = (Texture2D*)pSender;
    
    Sprite* lRenderSprite = Sprite::createWithTexture(lTexture);
    
    Director::getInstance()->getTextureCache()->removeTexture(lTexture);
    
    if (lRenderSprite != NULL)
    {
        lRenderSprite->getTexture()->setAntiAliasTexParameters();
        lRenderSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
        lRenderSprite->setOpacity(0);
        lRenderSprite->runAction(FadeIn::create(0.3f));
        
        if (DeviceHelper::getInstance()->getIs2048Device())
        {
            lRenderSprite->setScale(2.0f);
        }
        
        this->addChild(lRenderSprite);
        if (m_pHeartSpr)
        {
//            m_pHeartSpr->setVisible(true);
        }
        
        m_pTextureSprite = lRenderSprite;
        
        if (m_pRotateSprite)
        {
            m_pRotateSprite->removeFromParentAndCleanup(true);
            m_pRotateSprite = NULL;
        }
    }
}

void FavoriteSprite::LoadMapSprite(Ref* pSender)
{
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
