//
//  ScrollGalleryLayer_ScrollView.cpp
//  IMG1001
//
//  Created by houhuang on 15/9/24.
//
//

#include "ScrollGalleryLayer_ScrollView.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../data/DataManager.h"
#include "../compoment/GallerySprite.h"
#include "../helper/PurchaseHelper.h"
#include "../scene/ChooseMapScene.h"
#include "../define/AppConfigs.h"
#include "../scene/MyArtworksScene.hpp"
#include "../modules/STSystemFunction.h"

enum
{
    btn_restore = 22,
    st_MyArtworks_tag,
    btn_privacy,
};

ScrollGalleryLayer_ScrollView::ScrollGalleryLayer_ScrollView()
{
    _stInAppBilling = new STInAppBilling();
    _stInAppBilling->setIABDelegate(PurchaseHelper::getInstance());
}

ScrollGalleryLayer_ScrollView::~ScrollGalleryLayer_ScrollView()
{
    m_pScrollView = NULL;
    _galleryArrays.clear();
    CC_SAFE_DELETE(_stInAppBilling);
}

bool ScrollGalleryLayer_ScrollView::init()
{
    if (!Layer::init()) return false;
  
    if (DM_getInstance->m_vMyArtWorksImageName.size() != 0)
    {
        m_fOffset = ScreenAdapterHelper::getValue(650);
    }else
    {
        m_fOffset = 0.0f;
    }
    
    m_vGalleryInfo  = &DataManager::getInstance()->_galleryInfo;
    m_iGalleryCount = (int)m_vGalleryInfo->size();
    
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 1));
    this->setContentSize(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(1024)));
    
    m_pScrollView = ScrollView::create(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(950 * ScreenAdapterHelper::getRealDesignScale())));
    m_pScrollView->setContentSize(Size(ScreenAdapterHelper::getValue(700),ScreenAdapterHelper::getValue(650) * m_iGalleryCount + m_fOffset));
    
    m_pScrollView->setDirection(ScrollView::Direction::VERTICAL);
    m_pScrollView->ignoreAnchorPointForPosition(false);
    m_pScrollView->setAnchorPoint(Vec2(0.5, 1));
    m_pScrollView->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));

    this->addChild(m_pScrollView);
    this->initContentLayer();

    if (DataManager::is_TableOffset_Gallery)
    {
        DataManager::isTableOffset = false;
        if (DataManager::tableOffset_Gallery < m_pScrollView->minContainerOffset().y)
        {
            m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
        }else if (DataManager::tableOffset_Gallery > m_pScrollView->maxContainerOffset().y)
        {
            m_pScrollView->setContentOffset(m_pScrollView->maxContainerOffset());
        }else
        {
            m_pScrollView->setContentOffset(Vec2(0, DataManager::tableOffset_Gallery));
        }
    }
    else
    {
        m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
    }
    return true;
    
}

void ScrollGalleryLayer_ScrollView::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    
    
    this->scheduleOnce(schedule_selector(ScrollGalleryLayer_ScrollView::resumeSpriteTouch), 0.5f);
 }

void ScrollGalleryLayer_ScrollView::onExit()
{
    Layer::onExit();
    
    DataManager::is_TableOffset_Gallery = true;
    DataManager::tableOffset_Gallery = m_pScrollView->getContentOffset().y;
    
}

void ScrollGalleryLayer_ScrollView::resumeSpriteTouch(float dt)
{
    for (int i=0; i<_galleryArrays.size(); i++) {
        GallerySprite_New* lNode=_galleryArrays.at(i);
        lNode->setTouchEnable(true);
    }
}

void ScrollGalleryLayer_ScrollView::initContentLayer()
{
    Layer* lContentLayer = Layer::create();
    lContentLayer->setContentSize(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(650) * m_iGalleryCount + ScreenAdapterHelper::getValue(300) + m_fOffset + ScreenAdapterHelper::getValue(40)));
    m_pScrollView->setContainer(lContentLayer);
    
    _galleryArrays.clear();
    
    if (DM_getInstance->m_vMyArtWorksImageName.size() != 0)
    {
        GallerySprite_New* lGallerySprite = GallerySprite_New::create();
        lGallerySprite->addListener(callfuncN_selector(ScrollGalleryLayer_ScrollView::reponseMapSprite), this);
        lGallerySprite->setPosition(Vec2(lContentLayer->getContentSize().width / 2, lContentLayer->getContentSize().height - lGallerySprite->getContentSize().height / 2));
        lContentLayer->addChild(lGallerySprite);
        lGallerySprite->setIndex(st_MyArtworks_tag);
//        lGallerySprite->setTouchEnable(true);
        
        Label* galleryName = Label::createWithTTF(DM_getInstance->getLanguageMes()["MyArtworks"], st_font_AR, ScreenAdapterHelper::getValue(44));
        galleryName->setPosition(Vec2(lGallerySprite->getContentSize().width/2, lGallerySprite->getContentSize().height * 0.13f));
        lGallerySprite->addChild(galleryName);
        galleryName->setColor(Color3B(0, 0, 0));
        
        _galleryArrays.push_back(lGallerySprite);
    }
    
    
    
    for (int i = 0; i < m_vGalleryInfo->size(); ++i)
    {
        GalleryInfo* lGallery = &m_vGalleryInfo->at(i);
        
        GallerySprite_New* lGallerySprite = GallerySprite_New::create(lGallery);
        lGallerySprite->setPosition(Vec2(lContentLayer->getContentSize().width / 2, lContentLayer->getContentSize().height - ScreenAdapterHelper::getValue(650)/2 - i * ScreenAdapterHelper::getValue(650) - m_fOffset));
        lGallerySprite->addListener(callfuncN_selector(ScrollGalleryLayer_ScrollView::reponseMapSprite), this);
        lContentLayer->addChild(lGallerySprite);
        
        string galleryNameStr = lGallery->galleryname;
//        Label* galleryName = Label::createWithTTF(DM_getInstance->getLanguageMes()[galleryNameStr.c_str()], st_font_AR, ScreenAdapterHelper::getValue(44));
        Label* galleryName = Label::createWithTTF(galleryNameStr.c_str(), st_font_AR, ScreenAdapterHelper::getValue(44));
        galleryName->setPosition(Vec2(lGallerySprite->getContentSize().width/2, lGallerySprite->getContentSize().height * 0.13f));
        lGallerySprite->addChild(galleryName);
        galleryName->setColor(Color3B(0, 0, 0));
        
        if (lGallery->isNewImage == true && !UserDefault::getInstance()->getBoolForKey(lGallery->isUsedNewImage.c_str()))
        {
            Sprite* newIcon = Sprite::create("image_newIcon.png");
            newIcon->setAnchorPoint(Vec2(1, 1));
            //578
            newIcon->setPosition(Vec2(ScreenAdapterHelper::getValue(540), ScreenAdapterHelper::getValue(585)));
            lGallerySprite->addChild(newIcon, 10);
        }
        
        _galleryArrays.push_back(lGallerySprite);
    }
    
    MenuItemImage* restore = MenuItemImage::create("btn_active.png", "btn_inactive.png", CC_CALLBACK_1(ScrollGalleryLayer_ScrollView::reponseTouchButton, this));
    ScreenAdapterHelper::setPosition(restore, Vec2(384, 200), KVBoarderTypeBottom, KHBoarderTypeNone);
    restore->setPosition(Vec2(lContentLayer->getContentSize().width/2, restore->getPosition().y + ScreenAdapterHelper::getValue(60)));
    restore->setTag(btn_restore);
    
    Label* restoreTitle = Label::createWithTTF(DM_getInstance->getLanguageMes()["RESTORE"], st_font_PTS, ScreenAdapterHelper::getValue(30));
    restoreTitle->setPosition(Vec2(restore->getContentSize().width/2, restore->getContentSize().height/2));
    restore->addChild(restoreTitle);
    
    MenuItemImage* privacy = MenuItemImage::create("Privacy.png", "Privacy.png", CC_CALLBACK_1(ScrollGalleryLayer_ScrollView::reponseTouchButton, this));
    privacy->setPosition(Vec2(lContentLayer->getContentSize().width/2, restore->getPosition().y - ScreenAdapterHelper::getValue(100)));
    privacy->setTag(btn_privacy);
    
    
    Menu* menu = Menu::create( restore, privacy,NULL);
    menu->setPosition(Vec2::ZERO);
    lContentLayer->addChild(menu);
}

void ScrollGalleryLayer_ScrollView::reponseTouchButton(Ref* ref)
{
    MenuItemImage* lMenuItem= (MenuItemImage*)ref;
    
    switch (lMenuItem->getTag())
    {
        case btn_restore:
        {

            _stInAppBilling->restore();

        }
            break;
            
        case btn_privacy:
        {
            
            STSystemFunction sf;
            sf.go2PrivacyPage();
            
        }
            break;
            
        default:
            break;
    }
}

void ScrollGalleryLayer_ScrollView::reponseMapSprite(Node* ref)
{
    for (int i=0; i<_galleryArrays.size(); i++) {
        GallerySprite_New* lNode=_galleryArrays.at(i);
        lNode->setTouchEnable(false);
    }
    
    GallerySprite_New* lGallerySpr = dynamic_cast<GallerySprite_New*>(ref);
    lGallerySpr->setTouchEnable(false);
    
    int index = lGallerySpr->getIndex();
    
    if (index == st_MyArtworks_tag)
    {

        DataManager::is_TableOffset_Gallery = true;
        DataManager::tableOffset_Gallery = m_pScrollView->getContentOffset().y;
        
        Scene* scene = MyArtworksScene::scene();
        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.35f, scene));
        return;
    }
    

    std::vector<GalleryInfo>* lInfo=DataManager::getInstance()->_allMapDatas.at(index-1);
    
    if(DataManager::getInstance()->currentCalleryInfo!=lInfo && DataManager::getInstance()->currentCalleryInfo!=NULL)
    {
        std::vector<GalleryInfo>* mInfo =  DataManager::getInstance()->currentCalleryInfo;
        for (int i=0; i<mInfo->size(); i++)
        {
            GalleryInfo* lGalleryInfo=&mInfo->at(i);
            string lImageName=lGalleryInfo->image;
            stringstream ss;
            ss<<"";
            ss<<lImageName;
            ss<<i+1;
            ss<<"_white.png";
            lImageName=ss.str();
            
            Director::getInstance()->getTextureCache()->removeTextureForKey(lImageName);
        }

    }
    
    
    if (!m_vGalleryInfo->at(index - 1).isUsedNewImage.empty())
    {
        UserDefault::getInstance()->setBoolForKey(m_vGalleryInfo->at(index - 1).isUsedNewImage.c_str(), true);
        UserDefault::getInstance()->destroyInstance();
    }
    
        DataManager::getInstance()->currentCalleryInfo=lInfo;
        
        string str = m_vGalleryInfo->at(index-1).galleryname;
        
        DataManager::getInstance()->m_iCurrentGalleryID = index;
        
        DataManager::is_TableOffset_Gallery = true;
        DataManager::tableOffset_Gallery = m_pScrollView->getContentOffset().y;
        
        Scene* scene = ChooseMapScene::scene(index, str.c_str());
        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.35f, scene));
}
