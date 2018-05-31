//
//  MyArtSprite.cpp
//  ALABS0010
//
//  Created by houhuang on 16/3/31.
//
//

#include "MyArtSprite.hpp"
#include "ThreadManager.hpp"
#include "UITools.h"
#include "UICpp.h"
#include "MyArtDataManager.hpp"
#include "ChapterManager.hpp"
#include "DownLoadPage.h"

#define st_MyArtSprite_length   500
#define st_MyArtSprite_offset   7

MyArtSprite* MyArtSprite::create(const string& imageName, const Color3B& bgColor, const int serveId)
{
    MyArtSprite* lSprite = new MyArtSprite();
    if (lSprite && lSprite->init(imageName, bgColor, serveId))
    {
        lSprite->autorelease();
        return lSprite;
    }
    
    CC_SAFE_DELETE(lSprite);
    return NULL;
}

MyArtSprite::MyArtSprite()
{
    m_sNeedLoadImageT = "";
    m_sNeedLoadImageP = "";
    m_sNeedLoadImage = "";
    m_pRotateSprite = NULL;
    m_pTexture2D = NULL;
    m_pLikeLabel = NULL;
    m_iServeId = 0;
    m_sPreName = "";
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MyArtSprite::waitGetlikeSuc), st_FavoriteSprite_getLikeNumber_suc, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MyArtSprite::loadGraphSprite), st_multiThread_loadImage_finish, NULL);
}

MyArtSprite::~MyArtSprite()
{
    NotificationCenter::getInstance()->removeObserver(this, st_FavoriteSprite_getLikeNumber_suc);
    NotificationCenter::getInstance()->removeObserver(this, st_multiThread_loadImage_finish);
    removeTouchListener();
    DownLoadPage::getInstance()->m_vNeedGetLikeImage.clear();
}

bool MyArtSprite::init(const string& imageName, const Color3B& bgColor, const int serveId)
{
    if (!Node::init()) return false;
    
    m_iServeId = serveId;
    
    m_sNeedLoadImage = imageName;
//    m_sNeedLoadImageT = imageName + "_preview.png";
//    m_sNeedLoadImageP = imageName + "_preview.png";
    
//    log("%s,  %s", m_sNeedLoadImageP.c_str(), m_sNeedLoadImageT.c_str());
    m_cBgColor = bgColor;
    this->setContentSize(Size(st_MyArtSprite_length, st_MyArtSprite_length));
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    Texture2D* lTexture2D = Director::getInstance()->getTextureCache()->getTextureForKey("cocos/my_arts/paper.png");
    Sprite* bg = Sprite::createWithTexture(lTexture2D);
    bg->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(bg);
    
    Sprite* lRotateSprite = Sprite::create("cocos/other/load_rotate_icon.png");
    lRotateSprite->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(lRotateSprite, 10);
    lRotateSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 250)));
    m_pRotateSprite = lRotateSprite;
    
    auto deleteBtn = Button::create("cocos/my_arts/delete.png");
    deleteBtn->setPosition(Vec2(this->getContentSize().width * 0.1, -this->getContentSize().height * 0.1));
    deleteBtn->setPressedActionEnabled(true);
    deleteBtn->addTouchEventListener(CC_CALLBACK_2(MyArtSprite::onButton, this));
    addChild(deleteBtn);
    
    Sprite* heart = Sprite::create("cocos/my_arts/heart.png");
    heart->setPosition(Vec2(this->getContentSize().width * 0.87f, -this->getContentSize().height * 0.1));
    this->addChild(heart);
    heart->setScale(1.2f);
    
    Label* label = Label::createWithTTF("0", "fonts/verdana_0.ttf", 48);
    label->ignoreAnchorPointForPosition(false);
    label->setAnchorPoint(Vec2(1, 0.5));
    label->setPosition(Vec2(this->getContentSize().width * 0.77f, -this->getContentSize().height * 0.1));
    this->addChild(label);
    m_pLikeLabel = label;

    if (serveId != 0)
    {
        DownLoadPage::getInstance()->updateNeedGetLikeImage(serveId);
    }
    
    return true;
}

void MyArtSprite::waitGetlikeSuc(Ref* ref)
{
    for (int i = 0; i < DownLoadPage::getInstance()->m_vFavoriteInfo.size(); ++i)
    {
        FavoriteInfo lInfo = DownLoadPage::getInstance()->m_vFavoriteInfo.at(i);
        if (lInfo.imageID == m_iServeId)
        {
            stringstream ss;
            ss<<"";
            ss<<lInfo.likeNumber;
            m_pLikeLabel->setString(ss.str());
        }
    }
}

void MyArtSprite::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    xDialog->show("delete_myart", "cancel", "delete", CC_CALLBACK_1(MyArtSprite::onDialog, this));
}

void MyArtSprite::onDialog(const string& name)
{
    if(name == "delete")
    {
        log("%s",m_sNeedLoadImage.c_str());
        xMyArtData->deleteImageFromVec(m_sPreName);
        
        auto & graph = xChapter->getGraph(m_sNeedLoadImage);
        graph.deleteHistory(m_sPreName);
        
        NotificationCenter::getInstance()->postNotification(st_deleteMyArtSprite);
    }
    else if (name == "no")
    {
        
    }
}

void MyArtSprite::display()
{
    Texture2D* lTexture2DT = Director::getInstance()->getTextureCache()->getTextureForKey(m_sNeedLoadImageT);
//    Texture2D* lTexture2DP = Director::getInstance()->getTextureCache()->getTextureForKey(m_sNeedLoadImageP);
    if (lTexture2DT)
    {
        m_pTexture2D = lTexture2DT;
        this->imageLoadedCallback(NULL);
    }else
    {
        xThread->m_vNeedLoadImage.push_back(m_sNeedLoadImageT);
//        xThread->m_vNeedLoadImage.push_back(m_sNeedLoadImageP);
        xThread->startLoadImage();
    }
}

void MyArtSprite::imageLoadedCallback(Ref* ref)
{
    Texture2D* lTexture2DT = Director::getInstance()->getTextureCache()->getTextureForKey(m_sNeedLoadImageT);
//    Texture2D* lTexture2DP = Director::getInstance()->getTextureCache()->getTextureForKey(m_sNeedLoadImageP);

    if (lTexture2DT)
    {
        Sprite* lSprite2 = Sprite::createWithTexture(lTexture2DT);
        
        lSprite2->setPosition(Vec2(this->getContentSize().width/2 - st_MyArtSprite_offset - 3, this->getContentSize().height/2 + st_MyArtSprite_offset));
        this->addChild(lSprite2);
        lSprite2->setScale(1.2f);
//        lSprite2->setScale(quickAdaptScale(lSprite2, this));
        
        if (m_pRotateSprite)
        {
            m_pRotateSprite->removeFromParentAndCleanup(true);
            m_pRotateSprite = NULL;
        }
        
    }
}

void MyArtSprite::loadGraphSprite(Ref* ref)
{
    if (m_pTexture2D == NULL)
    {
        Texture2D* lTexture2D = Director::getInstance()->getTextureCache()->getTextureForKey(m_sNeedLoadImageT);
        if (lTexture2D != NULL)
        {
            this->imageLoadedCallback(NULL);
        }
    }
}

void MyArtSprite::removeTexture()
{
    Director::getInstance()->getTextureCache()->removeTextureForKey(m_sNeedLoadImageT);
}

string MyArtSprite::getSpriteName()
{
    return m_sNeedLoadImage;
}


void MyArtSprite::addTouchListener(Ref* target, SEL_CallFuncN callBack,bool isSwallow /* =true */)
{
    _selector = callBack;
    _target = target;
    
    this->removeTouchListener();
    
    auto dispatcher = Director::Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MyArtSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyArtSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyArtSprite::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(MyArtSprite::onTouchCancelled, this);
    listener->setSwallowTouches(isSwallow);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void MyArtSprite::removeTouchListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}


bool MyArtSprite::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    firstPos = lPos;
    lPos = Director::getInstance()->convertToGL(lPos);
    lPos = this->getParent()->convertToNodeSpace(lPos);
    if (this->getBoundingBox().containsPoint(lPos))
    {
        _isClick = true;
        return true;
    }
    
    return false;
}

void MyArtSprite::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void MyArtSprite::onTouchEnded(Touch *touch, Event *unused_event)
{
    Vec2 lPos = touch->getLocationInView();
    
    float distance = sqrt(pow(lPos.x - firstPos.x, 2) + pow(lPos.y - firstPos.y, 2));
    if (distance < 25)
    {
        if (_target && _selector)
        {
            (_target->*_selector)(this);
        }
    }
    
}

void MyArtSprite::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}

void MyArtSprite::setPreName(const string& name)
{
    m_sNeedLoadImageT = name;
    m_sPreName = name;
}

string MyArtSprite::getPreName()
{
    return m_sPreName;
}

Color3B MyArtSprite::getBGColor()
{
    return m_cBgColor;
}