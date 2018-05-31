//
//  ColorScrollLayer.cpp
//  ColorFree
//
//  Created by houhuang on 15/8/31.
//
//

#include "ColorScrollLayer.h"
#include "../../define/Globalmacro.h"
#include "../../helper/ScreenAdapterHelper.h"
#include "../../compoment/ColorSprite.h"
#include "../../game/GameUILayer.h"
#include "ColorScrollLayerMask.h"
#include "../../compoment/UIButton.h"
#include "../../helper/PurchaseHelper.h"
#include "../../define/AppConfigs.h"
#include "../../compoment/LayerButton.h"

enum
{
    st_btn_updown_tag = 10,
    st_btn_left_tag,
    st_btn_right_tag,
    
    st_resently_color_tag = 77,
    
    st_btn_purchase_color_neon = 90,
    st_btn_purchase_color_sea,
    st_btn_purchase_color_ukiyoe,
    st_color_pack_newIcon_tag,
    st_color_pack_lock_tag,
};

ColorScrollLayer::ColorScrollLayer()
{
    _index          = 0;
    m_iMaxPage      = 0;
    m_iCurrentPage  = 0;
    m_vFirstPoint   = Vec2(0, 0);
    m_vOffset       = Vec2(0, 0);
    m_sViewSize     = Size(0, 0);
    
    m_pScrollView   = NULL;
    m_pPageDot      = NULL;
    m_pContentLayer = NULL;
    m_pLeft         = NULL;
    m_pRight        = NULL;
    m_pMenu         = NULL;
    
    m_pPreColorSprite   = NULL;
    m_pColorGalleryText = NULL;
    
    m_pColorNeonLayer   = NULL;
    m_pColorSeaLayer    = NULL;
    m_pcolorUkiyoeLayer = NULL;
    
    m_bIsUpTowardsMove  = true;
    m_bIsInitColorSpriteVector = false;
    m_bIsMoveFinish     = true;
    m_bIsClickScroll    = false;
    m_bIsVerticalMove   = false;
    m_bIsCurrentClick   = true;
    
    m_vColorDatas = &DataManager::getInstance()->m_vColorDatas;
    m_vColorGalleryText = &DataManager::getInstance()->m_vColorGalleryText;
    
    _stInAppBilling = new STInAppBilling();
    _stInAppBilling->setIABDelegate(PurchaseHelper::getInstance());
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ColorScrollLayer::reponsePurchseSuccessfulNofiction), st_colorScrollLayer_notifiction, NULL);
}

ColorScrollLayer::~ColorScrollLayer()
{
    m_pPreColorSprite = NULL;
    m_pArrowSprite = NULL;
    m_pColorBtn = NULL;
    m_bIsClickBtn = false;
    m_bIsUpTowards = false;
    
    m_vColorSpriteNeon.clear();
    m_vColorSpriteSea.clear();
    m_vColorSpriteUkiyoe.clear();
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);

    CC_SAFE_DELETE(_stInAppBilling);
    
    NotificationCenter::getInstance()->removeObserver(this, st_colorScrollLayer_notifiction);
}

ColorScrollLayer* ColorScrollLayer::create(Size size)
{
    ColorScrollLayer* layer = new ColorScrollLayer();
    if (layer && layer->init(size))
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool ColorScrollLayer::init(Size size)
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) return false;

    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setContentSize(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(300)));
    
    DataManager::m_bIsShowColorBoard = true;
    
    m_pScrollView = ScrollView::create();
//    m_pScrollView->setContentSize(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(300)));
    m_pScrollView->setViewSize(Size(ScreenAdapterHelper::getValue(650), ScreenAdapterHelper::getValue(250)));
    m_pScrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    m_pScrollView->ignoreAnchorPointForPosition(false);
    m_pScrollView->setAnchorPoint(Vec2(0.5, 1));
    m_pScrollView->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    m_pScrollView->setTouchEnabled(false);
    this->addChild(m_pScrollView);
//    m_pScrollView->setSwallowsTouches(true);
    
    m_pPageDot = PageDot::create("page_turn2.png", "page_turn1.png");
    m_pPageDot->setPageCount(st_color_scroll_pagedot);
    m_pPageDot->setSelectPage(0);
    m_pPageDot->setPosition(Vec2(this->getContentSize().width/2 - ScreenAdapterHelper::getValue(80), ScreenAdapterHelper::getValue(10)));
    this->addChild(m_pPageDot);
    
    m_iMaxPage = m_pPageDot->getPageCount();
    
    m_pContentLayer = Layer::create();
//    m_pContentLayer = LayerColor::create(Color4B(0, 250, 0, 100));
    m_pContentLayer->setContentSize(Size(m_pScrollView->getViewSize().width * m_iMaxPage, m_pScrollView->getViewSize().height));
    m_pScrollView->addChild(m_pContentLayer);

//    Sprite* line = Sprite::create("seperator-2.png");
//    line->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
//    line->setPosition(Vec2(line->getPosition().x - ScreenAdapterHelper::getValue(3), line->getPosition().y));
//    line->setScaleX(0.95);
//    this->addChild(line);
    
    
    this->initUILayer();
    
    this->initContentLayer();
    
    this->addListener();
    
    _isFirstLoad=false;
    
    return true;
}

void ColorScrollLayer::onEnter()
{
    LayerColor::onEnter();
    
    if(!_isFirstLoad)
    {
        _isFirstLoad=true;
        _orginPos=this->getPosition();
    }
}

void ColorScrollLayer::initUILayer()
{
    Sprite* colorBtn = Sprite::create("color_btn.png");
    colorBtn->setAnchorPoint(Vec2(0.5, 1));
    colorBtn->setPosition(Vec2(this->getContentSize().width/2 - ScreenAdapterHelper::getValue(4), this->getContentSize().height + ScreenAdapterHelper::getValue(63)));
    this->addChild(colorBtn);
    m_pColorBtn = colorBtn;
    
    Sprite* arrowBtn = Sprite::create("down.png");
    arrowBtn->setAnchorPoint(Vec2(0, 0.5));
    arrowBtn->setPosition(Vec2(ScreenAdapterHelper::getValue(15), colorBtn->getContentSize().height / 2));
    colorBtn->addChild(arrowBtn, 10);
    m_pArrowSprite = arrowBtn;

    string str = m_vColorGalleryText->at(0);
    m_pColorGalleryText = Label::createWithTTF(str.c_str(), st_font_AR, ScreenAdapterHelper::getValue(44));
    m_pColorGalleryText->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height + ScreenAdapterHelper::getValue(30)));
    m_pColorGalleryText->setColor(Color3B::BLACK);
    this->addChild(m_pColorGalleryText, 10);

}

void ColorScrollLayer::doAction(Ref* pSender)
{
    MenuItemImage* clickMenu = (MenuItemImage*)pSender;
    switch (clickMenu->getTag())
    {
        case st_btn_updown_tag:
        {
//            m_pHideScroll->setEnabled(false);
//            this->scrollMoveAction();
        }
            break;
//        case st_btn_left_tag:
//        {
//            m_pMenu->setEnabled(false);
//            m_bIsMoveFinish = false;
//            --m_iCurrentPage;
//            
//            if (m_iCurrentPage < 1)
//            {
//                m_pLeft->setNormalImage(Sprite::create("btn_noleft.png"));
//            }else
//            {
//                m_pRight->setNormalImage(Sprite::create("btn_right.png"));
//            }
//            
//            if (m_iCurrentPage < 0)
//            {
//                m_iCurrentPage = 0;
//                
//            }else
//            {
//                
//                m_pScrollView->setContentOffsetInDuration(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0), 0.2f);
//            }
//            
//            m_pPageDot->setSelectPage(m_iCurrentPage);
//            
//            string str = m_vColorGalleryText->at(m_iCurrentPage);
//            m_pColorGalleryText->setString(str);
//            
//            this->updataColorPackText();
//            
//            scheduleOnce(schedule_selector(ColorScrollLayer::menuDelayEnable), 0.2f);
//        }
//            break;
//        case st_btn_right_tag:
//        {
//            m_pMenu->setEnabled(false);
//            ++m_iCurrentPage;
//            
//            if (m_iCurrentPage >= m_iMaxPage - 1)
//            {
//                m_pRight->setNormalImage(Sprite::create("btn_noright.png"));
//            }else
//            {
//                m_pLeft->setNormalImage(Sprite::create("btn_left.png"));
//            }
//            
//            if (m_iCurrentPage >= m_iMaxPage)
//            {
//                m_iCurrentPage = m_iMaxPage - 1;
//                
//            }else
//            {
//                m_pScrollView->setContentOffsetInDuration(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0), 0.2f);
//            }
//            
//            m_pPageDot->setSelectPage(m_iCurrentPage);
//            
//            string str = m_vColorGalleryText->at(m_iCurrentPage);
//            m_pColorGalleryText->setString(str);
//            
//            this->updataColorPackText();
//            
//            
//            
//            
//            scheduleOnce(schedule_selector(ColorScrollLayer::menuDelayEnable), 0.2f);
//        }
//            break;
            
        default:
            break;
    }

}

void ColorScrollLayer::addListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event)
    {
        if(touch->getID() > 0)
        {
            return false;
        }
        
        m_bIsCurrentClick = true;
        m_bIsUpTowards = !DataManager::m_bIsShowColorBoard;
        m_bIsClickBtn = false;
        m_bIsVerticalMove = false;
        m_vOffset = m_pScrollView->getContentOffset();
        
        Vec2 lPos = touch->getLocationInView();
        lPos = Director::getInstance()->convertToGL(lPos);
        lPos = this->convertToNodeSpace(lPos);
        m_vFirstPoint = lPos;
//        if (DataManager::m_bIsShowColorBoard == true)
        {
            if (m_bIsMoveFinish)
            {
                //避免多点触摸问题
                if (!m_bIsClickScroll)
                {
                    if (m_pColorBtn->getBoundingBox().containsPoint(lPos))
                    {
                        m_bIsClickBtn = true;
                        return true;
                    }
                    
                    if (m_pScrollView->boundingBox().containsPoint(lPos))
                    {
                        m_bIsClickScroll = true;
                        m_bIsMoveFinish = false;
//                        m_pMenu->setEnabled(false);
                        
                        return true;
                    }
                }
                
            }
            
        }
        
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* touch, Event* event)
    {
        Vec2 lPos = touch->getLocationInView();
        lPos = Director::getInstance()->convertToGL(lPos);
        lPos = this->convertToNodeSpace(lPos);
        
        float distance = lPos.x - m_vFirstPoint.x;
        float distanceY = lPos.y - m_vFirstPoint.y;
        
        m_pScrollView->setContentOffset(Vec2(distance + m_vOffset.x, 0));
        
        if (distanceY > 150 && m_bIsUpTowards && m_bIsCurrentClick && abs(distance) < 50)
        {
            m_bIsCurrentClick = false;
            m_bIsUpTowards = false;
            this->scrollMoveAction();
            m_pScrollView->setContentOffset(Vec2(distance - m_vOffset.x, 0));

        }else if (distanceY < -300 && m_bIsUpTowards == false && m_bIsCurrentClick && abs(distance) < 50)
        {
            m_bIsCurrentClick = false;
            m_bIsUpTowards = true;
            this->scrollMoveAction();
            m_pScrollView->setContentOffset(Vec2(distance - m_vOffset.x, 0));

        }
        
        
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event)
    {
        Vec2 endPoint = touch->getLocationInView();
        endPoint = Director::getInstance()->convertToGL(endPoint);
        endPoint = this->convertToNodeSpace(endPoint);
        
        float distance = endPoint.x - m_vFirstPoint.x;
        float distanceY = endPoint.y - m_vFirstPoint.y;
        
        if (m_bIsClickBtn)
        {
            if (abs(distanceY) < 10 && abs(distance) < 10 )
            {
                this->scrollMoveAction();
            }
            
            m_pScrollView->setContentOffset(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0));
            return ;
        }
        
        if (fabs(distance) > 200 && m_bIsVerticalMove == false)
        {
            if (distance > 0)
            {
                --m_iCurrentPage;
//                m_pRight->setNormalImage(Sprite::create("btn_right.png"));
            }
            else if (distance < 0)
            {
                ++m_iCurrentPage;
//                m_pLeft->setNormalImage(Sprite::create("btn_left.png"));
            }
            
//            if (m_iCurrentPage == m_iMaxPage - 1)
//            {
//                m_pRight->setNormalImage(Sprite::create("btn_noright.png"));
//            }
//            if (m_iCurrentPage == 0)
//            {
//                m_pLeft->setNormalImage(Sprite::create("btn_noleft.png"));
//            }
        }
        
        if (m_iCurrentPage < 0)
        {
            m_iCurrentPage = 0;
        }else if (m_iCurrentPage >= m_iMaxPage)
        {
            m_iCurrentPage = m_iCurrentPage - 1;
        }
        
        m_pScrollView->setContentOffsetInDuration(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0), 0.2f);
        m_pPageDot->setSelectPage(m_iCurrentPage);
        
        string str = m_vColorGalleryText->at(m_iCurrentPage);
        m_pColorGalleryText->setString(str);
        
        this->updataColorPackText();
        
        this->scheduleOnce(schedule_selector(ColorScrollLayer::areadyMoveFinish), 0.2f);
        
        m_bIsClickScroll = false;
    };
    
    
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void ColorScrollLayer::areadyMoveFinish(float dt)
{
    m_bIsMoveFinish = true;
//    m_pMenu->setEnabled(true);
}

void ColorScrollLayer::initContentLayer()
{
    
    for (int j = 0 ; j < 3; ++j)
    {
        for (int i = 0; i < 8; ++i)
        {
            ColorData colorData = m_vColorDatas->at(_index);
            
            ColorSprite* lSprite = ColorSprite::create("sqare_color.png", "sqare_stroke.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            
            lSprite->setPosition(Vec2((lSprite->getContentSize().width/2 + ScreenAdapterHelper::getValue(39))*(i+0.5),(3-j)*2*lSprite->getContentSize().height/2 - lSprite->getContentSize().height/2)*1.02 );
            lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(colorData.R, colorData.G, colorData.B));
            lSprite->setColorData(colorData);
            m_pContentLayer->addChild(lSprite);
            lSprite->setGalleryName("Basic");
            m_vColorSpriteAll.push_back(lSprite);
            
            ++_index;
            if (_index >= 24)
            {
                break;
            }
            
        }
    }
    
    for (int j = 0 ; j < 2; ++j)
    {
        for (int i = 0; i < 8; ++i)
        {
            ColorData colorData = m_vColorDatas->at(_index);
            
            ColorSprite* lSprite = ColorSprite::create("sqare_color.png", "sqare_stroke.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            lSprite->setPosition(Vec2((lSprite->getContentSize().width/2 + ScreenAdapterHelper::getValue(39))*(i+0.5) + ScreenAdapterHelper::getValue(638),(3-j)*2*lSprite->getContentSize().height/2 - lSprite->getContentSize().height/2)*1.02);
            lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(colorData.R, colorData.G, colorData.B));
            lSprite->setColorData(colorData);
            m_pContentLayer->addChild(lSprite);
            lSprite->setGalleryName("Sweet");
            m_vColorSpriteAll.push_back(lSprite);
            
//            if (colorData.R == DataManager::getInstance()->m_cSelectedColor.R && colorData.G == DataManager::getInstance()->m_cSelectedColor.G && colorData.B == DataManager::getInstance()->m_cSelectedColor.B)
//            {
//                m_pPreColorSprite = LayerColor::create(Color4B::WHITE, ScreenAdapterHelper::getValue(40), ScreenAdapterHelper::getValue(40));
//                
//                if (DataManager::getInstance()->m_cSelectedColor.R > 245 && DataManager::getInstance()->m_cSelectedColor.G > 245 && DataManager::getInstance()->m_cSelectedColor.B > 245)
//                {
//                    m_pPreColorSprite->setColor(Color3B::BLACK);
//                }
//                
//                m_pPreColorSprite->ignoreAnchorPointForPosition(false);
//                m_pPreColorSprite->setAnchorPoint(Vec2(0.5, 0.5));
//                m_pPreColorSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
//                m_pPreColorSprite->setOpacity(0);
//                lSprite->addChild(m_pPreColorSprite);
//                
//                m_pPreColorSprite->runAction(FadeIn::create(0.5f));
//            }
            
            ++_index;
            if (_index >= 40)
            {
                break;
            }
        }
    }
    
    for (int j = 0 ; j < 3; ++j)
    {
        for (int i = 0; i < 8; ++i)
        {
            ColorData colorData = m_vColorDatas->at(_index);
            
            ColorSprite* lSprite = ColorSprite::create("sqare_color.png", "sqare_stroke.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            lSprite->setPosition(Vec2((lSprite->getContentSize().width/2 + ScreenAdapterHelper::getValue(39))*(i+0.5) + ScreenAdapterHelper::getValue(638) * 2,(3-j)*2*lSprite->getContentSize().height/2 - lSprite->getContentSize().height/2)*1.02);
            lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(colorData.R, colorData.G, colorData.B));
            lSprite->setColorData(colorData);
            m_pContentLayer->addChild(lSprite);
            lSprite->setGalleryName("Autumn");
            m_vColorSpriteAll.push_back(lSprite);
//            if (colorData.R == DataManager::getInstance()->m_cSelectedColor.R && colorData.G == DataManager::getInstance()->m_cSelectedColor.G && colorData.B == DataManager::getInstance()->m_cSelectedColor.B)
//            {
//                m_pPreColorSprite = LayerColor::create(Color4B::WHITE, ScreenAdapterHelper::getValue(40), ScreenAdapterHelper::getValue(40));
//                
//                if (DataManager::getInstance()->m_cSelectedColor.R > 245 && DataManager::getInstance()->m_cSelectedColor.G > 245 && DataManager::getInstance()->m_cSelectedColor.B > 245)
//                {
//                    m_pPreColorSprite->setColor(Color3B::BLACK);
//                }
//                
//                m_pPreColorSprite->ignoreAnchorPointForPosition(false);
//                m_pPreColorSprite->setAnchorPoint(Vec2(0.5, 0.5));
//                m_pPreColorSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
//                m_pPreColorSprite->setOpacity(0);
//                lSprite->addChild(m_pPreColorSprite);
//                
//                m_pPreColorSprite->runAction(FadeIn::create(0.5f));
//            }
            
            ++_index;
            if (_index >= 60)
            {
                break;
            }
            
        }
    }
    
    for (int j = 0 ; j < 3; ++j)
    {
        for (int i = 0; i < 8; ++i)
        {
            ColorData colorData = m_vColorDatas->at(_index);
            
            ColorSprite* lSprite = ColorSprite::create("sqare_color.png", "sqare_stroke.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            
            lSprite->setPosition(Vec2((lSprite->getContentSize().width/2 + ScreenAdapterHelper::getValue(39))*(i+0.5) + ScreenAdapterHelper::getValue(638) * 3,(3-j)*2*lSprite->getContentSize().height/2 - lSprite->getContentSize().height/2)*1.02 );
            lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(colorData.R, colorData.G, colorData.B));
            lSprite->setColorData(colorData);
            m_pContentLayer->addChild(lSprite);
            lSprite->setGalleryName("Impressionism");
            m_vColorSpriteAll.push_back(lSprite);
            
            ++_index;
            if (_index >= 80)
            {
                break;
            }
            
        }
    }
    
    for (int j = 0 ; j < 3; ++j)
    {
        for (int i = 0; i < 8; ++i)
        {
            ColorData colorData = m_vColorDatas->at(_index);
            
            ColorSprite* lSprite = ColorSprite::create("sqare_color.png", "sqare_stroke.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            
            lSprite->setPosition(Vec2((lSprite->getContentSize().width/2 + ScreenAdapterHelper::getValue(39))*(i+0.5) + ScreenAdapterHelper::getValue(638) * 4,(3-j)*2*lSprite->getContentSize().height/2 - lSprite->getContentSize().height/2)*1.02 );
            lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(colorData.R, colorData.G, colorData.B));
            lSprite->setColorData(colorData);
            m_pContentLayer->addChild(lSprite);
            lSprite->setGalleryName("Flower");
            m_vColorSpriteAll.push_back(lSprite);
            
            ++_index;
            if (_index >= 98)
            {
                break;
            }
            
        }
    }
    
    if (m_vColorSpriteNeon.empty())
    {
        m_bIsInitColorSpriteVector = false;
    }else
    {
        m_bIsInitColorSpriteVector = true;
    }
    
    for (int j = 0 ; j < 3; ++j)
    {
        for (int i = 0; i < 8; ++i)
        {
            
            ColorData colorData = m_vColorDatas->at(_index);
            
            ColorSprite* lSprite = ColorSprite::create("sqare_color.png", "sqare_stroke.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            lSprite->setPosition(Vec2((lSprite->getContentSize().width/2 + ScreenAdapterHelper::getValue(39))*(i+0.5) + ScreenAdapterHelper::getValue(638) * 5,(3-j)*2*lSprite->getContentSize().height/2 - lSprite->getContentSize().height/2)*1.02);
            lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(colorData.R, colorData.G, colorData.B));
            lSprite->setColorData(colorData);
            m_pContentLayer->addChild(lSprite);

            if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_NEON))
            {
                lSprite->setEnableTouch(false);
                if (!m_bIsInitColorSpriteVector)
                {
                    m_vColorSpriteNeon.push_back(lSprite);
                }
            }

            lSprite->setGalleryName("Neon");
            m_vColorSpriteAll.push_back(lSprite);
            
            ++_index;
            if (_index >= 118)
            {
                break;
            }
            
        }
    }
    
    if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_NEON))
    {
        LayerButton* btnNeon = LayerButton::create(Size(ScreenAdapterHelper::getValue(650), ScreenAdapterHelper::getValue(250)), Color4B(0, 0, 0, 0));
        btnNeon->addListener(callfuncN_selector(ColorScrollLayer::reponseUnlockColorButton), this);
        btnNeon->setAnchorPoint(Vec2(0, 0));
        btnNeon->setPosition(Vec2(5 * ScreenAdapterHelper::getValue(650), 0));
        m_pContentLayer->addChild(btnNeon);
        btnNeon->setTag(st_btn_purchase_color_neon);
        m_pColorNeonLayer = btnNeon;
        
    }

    for (int j = 0 ; j < 3; ++j)
    {
        for (int i = 0; i < 8; ++i)
        {
            ColorData colorData = m_vColorDatas->at(_index);
            
            ColorSprite* lSprite = ColorSprite::create("sqare_color.png", "sqare_stroke.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            lSprite->setPosition(Vec2((lSprite->getContentSize().width/2 + ScreenAdapterHelper::getValue(39))*(i+0.5) + ScreenAdapterHelper::getValue(638) * 6,(3-j)*2*lSprite->getContentSize().height/2 - lSprite->getContentSize().height/2)*1.02);
            lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(colorData.R, colorData.G, colorData.B));
            lSprite->setColorData(colorData);
            m_pContentLayer->addChild(lSprite);
            
            if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_SEA))
            {
                lSprite->setEnableTouch(false);
                if (!m_bIsInitColorSpriteVector)
                {
                    m_vColorSpriteSea.push_back(lSprite);
                }
            }

            lSprite->setGalleryName("Sea");
            m_vColorSpriteAll.push_back(lSprite);
//
            
            ++_index;
            if (_index >= 138)
            {
                break;
            }
            
        }
    }
    
    if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_SEA))
    {
        LayerButton* btnSea = LayerButton::create(Size(ScreenAdapterHelper::getValue(650), ScreenAdapterHelper::getValue(250)), Color4B(0, 0, 0, 0));
        btnSea->addListener(callfuncN_selector(ColorScrollLayer::reponseUnlockColorButton), this);
        btnSea->setAnchorPoint(Vec2(0, 0));
        btnSea->setPosition(Vec2(6 * ScreenAdapterHelper::getValue(650), 0));
        m_pContentLayer->addChild(btnSea);
        btnSea->setTag(st_btn_purchase_color_sea);

        m_pColorSeaLayer = btnSea;
    }
    
    
    for (int j = 0 ; j < 3; ++j)
    {
        for (int i = 0; i < 8; ++i)
        {
            ColorData colorData = m_vColorDatas->at(_index);
            
            ColorSprite* lSprite = ColorSprite::create("sqare_color.png", "sqare_stroke.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            lSprite->setPosition(Vec2((lSprite->getContentSize().width/2 + ScreenAdapterHelper::getValue(39))*(i+0.5) + ScreenAdapterHelper::getValue(638) * 7,(3-j)*2*lSprite->getContentSize().height/2 - lSprite->getContentSize().height/2)*1.02);
            lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(colorData.R, colorData.G, colorData.B));
            lSprite->setColorData(colorData);
            m_pContentLayer->addChild(lSprite);
            
            if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_UKIYOE))
            {
                lSprite->setEnableTouch(false);
                if (!m_bIsInitColorSpriteVector)
                {
                    m_vColorSpriteUkiyoe.push_back(lSprite);
                }
            }
            
            lSprite->setGalleryName("Ukiyoe");
            m_vColorSpriteAll.push_back(lSprite);
            
            ++_index;
            if (_index >= 158)
            {
                break;
            }
            
        }
    }

    
    if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_UKIYOE))
    {
        LayerButton* btnUkiyoe = LayerButton::create(Size(ScreenAdapterHelper::getValue(650), ScreenAdapterHelper::getValue(250)), Color4B(0, 0, 0, 0));
        btnUkiyoe->addListener(callfuncN_selector(ColorScrollLayer::reponseUnlockColorButton), this);
        btnUkiyoe->setAnchorPoint(Vec2(0, 0));
        btnUkiyoe->setPosition(Vec2(7 * ScreenAdapterHelper::getValue(650), 0));
        m_pContentLayer->addChild(btnUkiyoe);
        btnUkiyoe->setTag(st_btn_purchase_color_ukiyoe);
        
        m_pcolorUkiyoeLayer = btnUkiyoe;
    }
    
    
    for (int i = 0; i < m_vColorSpriteAll.size(); ++i)
    {
        ColorSprite* lSprite = m_vColorSpriteAll.at(i);
        ColorData colorData = lSprite->getColorData();
        
        if (colorData.R == DataManager::getInstance()->m_cSelectedColor.R && colorData.G == DataManager::getInstance()->m_cSelectedColor.G && colorData.B == DataManager::getInstance()->m_cSelectedColor.B)
            {
                m_pPreColorSprite = LayerColor::create(Color4B::WHITE, ScreenAdapterHelper::getValue(40), ScreenAdapterHelper::getValue(40));
            
                if (DataManager::getInstance()->m_cSelectedColor.R > 245 && DataManager::getInstance()->m_cSelectedColor.G > 245 && DataManager::getInstance()->m_cSelectedColor.B > 245)
                {
                    m_pPreColorSprite->setColor(Color3B::BLACK);
                }
            
                m_pPreColorSprite->ignoreAnchorPointForPosition(false);
                m_pPreColorSprite->setAnchorPoint(Vec2(0.5, 0.5));
                m_pPreColorSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
                m_pPreColorSprite->setOpacity(0);
                lSprite->addChild(m_pPreColorSprite);
                            
                m_pPreColorSprite->runAction(FadeIn::create(0.5f));
            }
    }
    
}

void ColorScrollLayer::menuDelayEnable(float dt)
{
    m_pMenu->setEnabled(true);
    m_bIsMoveFinish = true;
}

void ColorScrollLayer::removeDialog(Node* pNode)
{
    pNode->removeFromParentAndCleanup(true);
}

void ColorScrollLayer::unlockColorPack(Node* pNode)
{
    STAlertDialog* dialog = (STAlertDialog*)pNode;
    switch (dialog->getTag())
    {
        case st_btn_purchase_color_neon:
        {
#ifdef DEBUG_IAP
            PurchaseHelper::getInstance()->purchaseSuccessful(st_purchase_color_neon_iap);
#else
            _stInAppBilling->purchase(st_purchase_color_neon_iap);
#endif
            
            
        }
            break;
            
        case st_btn_purchase_color_sea:
        {
#ifdef DEBUG_IAP
            PurchaseHelper::getInstance()->purchaseSuccessful(st_purchase_color_sea_iap);
#else
            _stInAppBilling->purchase(st_purchase_color_sea_iap);
#endif
        }
            break;
            
        case st_btn_purchase_color_ukiyoe:
        {
#ifdef DEBUG_IAP
            PurchaseHelper::getInstance()->purchaseSuccessful(st_purchase_color_ukiyoe_iap);
#else
            _stInAppBilling->purchase(st_purchase_color_ukiyoe_iap);
#endif
        }
            break;
            
        default:
            break;
    }
    

}

void ColorScrollLayer::reponseUnlockColorButton(Node* pNode)
{
//    UIButton* lUIButton = (UIButton*)pNode;
    
//    STAlertDialog* dialog = STAlertDialog::create("Buy the color", "Later", "$0.99");
    STAlertDialog* dialog = STAlertDialog::create(DM_getInstance->getLanguageMes()["PurchasePalette"], DM_getInstance->getLanguageMes()["Later"], DM_getInstance->getLanguageMes()["Buy"]);
    
    dialog->setLeftListener(this, callfuncN_selector(ColorScrollLayer::removeDialog));
    dialog->setRightListener(this, callfuncN_selector(ColorScrollLayer::unlockColorPack));
    this->getParent()->addChild(dialog, 100);
    dialog->setTag(pNode->getTag());
    
}

void  ColorScrollLayer::reponsePurchseSuccessfulNofiction(Ref* ref)
{
    //update color pack text
    DataManager::getInstance()->initColorGalleryText();
    m_pColorGalleryText->setString(m_vColorGalleryText->at(m_iCurrentPage));
    this->updataColorPackText();
    
    if (UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_NEON))
    {
        if (this->getParent()->getChildByTag(st_btn_purchase_color_neon))
        {
            this->getParent()->getChildByTag(st_btn_purchase_color_neon)->removeFromParentAndCleanup(true);
        }
        
        if (m_pColorNeonLayer)
        {
            m_pColorNeonLayer->removeFromParentAndCleanup(true);
            m_pColorNeonLayer = nullptr;
            for (int i = 0 ; i < m_vColorSpriteNeon.size(); ++i)
            {
                ColorSprite* lColorSprite = m_vColorSpriteNeon.at(i);
                lColorSprite->setEnableTouch(true);
            }
        }
        
    }
    if (UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_SEA))
    {
        
        if (this->getParent()->getChildByTag(st_btn_purchase_color_sea))
        {
            this->getParent()->getChildByTag(st_btn_purchase_color_sea)->removeFromParentAndCleanup(true);
        }
        
        if (m_pColorSeaLayer)
        {
            m_pColorSeaLayer->removeFromParentAndCleanup(true);
            m_pColorSeaLayer = nullptr;
            for (int i = 0 ; i < m_vColorSpriteSea.size(); ++i)
            {
                ColorSprite* lColorSprite = m_vColorSpriteSea.at(i);
                lColorSprite->setEnableTouch(true);
            }
        }
        
    }
    if (UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_UKIYOE))
    {
        if (this->getParent()->getChildByTag(st_btn_purchase_color_ukiyoe))
        {
            this->getParent()->getChildByTag(st_btn_purchase_color_ukiyoe)->removeFromParentAndCleanup(true);
        }
        
        if (m_pcolorUkiyoeLayer)
        {
            m_pcolorUkiyoeLayer->removeFromParentAndCleanup(true);
            m_pcolorUkiyoeLayer = nullptr;
            for (int i = 0 ; i < m_vColorSpriteUkiyoe.size(); ++i)
            {
                ColorSprite* lColorSprite = m_vColorSpriteUkiyoe.at(i);
                lColorSprite->setEnableTouch(true);
            }
        }
        
    }
}

void ColorScrollLayer::reponseColorSpriteTouch(Node* pSender)
{
    ColorSprite* lSprite = (ColorSprite*)pSender;
    
    if (lSprite->getGalleryName() == "Flower")
    {
        UserDefault::getInstance()->setBoolForKey(st_color_pack_Flower, true);
        UserDefault::getInstance()->destroyInstance();
    }
    if (lSprite->getGalleryName() == "Impressionism")
    {
        UserDefault::getInstance()->setBoolForKey(st_color_pack_Impressionism, true);
        UserDefault::getInstance()->destroyInstance();
    }
    
    for (int i = 0; i < DM_FlurryDatas.size(); ++i)
    {
        stringstream ss;
        ss<<"";
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ss<<"Plate_";
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ss<<"Pallete_";
        
#endif
        ss<<lSprite->getGalleryName();
        
        FlurryInfo* lFlurryInfo = &DM_FlurryDatas.at(i);
        if (strcmp(lFlurryInfo->message.c_str(), ss.str().c_str()) == 0)
        {
            lFlurryInfo->number++;
        }
    }
   
    if (m_pPreColorSprite)
    {
        m_pPreColorSprite->removeFromParentAndCleanup(true);
        m_pPreColorSprite = NULL;
    }
    
    DataManager::getInstance()->m_cSelectedColor = lSprite->getColorData();
    
    m_pPreColorSprite = LayerColor::create(Color4B::WHITE, ScreenAdapterHelper::getValue(40), ScreenAdapterHelper::getValue(40));
    
    if (DataManager::getInstance()->m_cSelectedColor.R > 245 && DataManager::getInstance()->m_cSelectedColor.G > 245 && DataManager::getInstance()->m_cSelectedColor.B > 245)
    {
        m_pPreColorSprite->setColor(Color3B::BLACK);
    }
    
    m_pPreColorSprite->ignoreAnchorPointForPosition(false);
    m_pPreColorSprite->setAnchorPoint(Vec2(0.5, 0.5));
    m_pPreColorSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
    m_pPreColorSprite->setOpacity(0);
    lSprite->addChild(m_pPreColorSprite);
    
    m_pPreColorSprite->runAction(FadeIn::create(0.5f));
    
    DataManager::getInstance()->m_iSelectedColorIndex = 0;
    
//    DataManager::getInstance()->updateRecentColor(lSprite->getColorData());
    GameUILayer* lGameUILayer = dynamic_cast<GameUILayer*>(this->getParent());
    lGameUILayer->updateResentlyColor();

   
}

void ColorScrollLayer::deleteCurrentColor()
{
    if (m_pPreColorSprite)
    {
        m_pPreColorSprite->removeFromParentAndCleanup(true);
        m_pPreColorSprite = NULL;
    }
}


void ColorScrollLayer::scrollMoveAction()
{
    GameUILayer* lGameUILayer = dynamic_cast<GameUILayer*>(this->getParent());
    lGameUILayer->reponseMaskAnimation(DataManager::m_bIsShowColorBoard);
    
    if (DataManager::m_bIsShowColorBoard)
    {
        DataManager::m_bIsShowColorBoard = false;
        m_bIsUpTowardsMove = false;
//        m_pHideScroll->setNormalImage(Sprite::create("down-arrow.png"));
//        m_pHideScroll->setSelectedImage(Sprite::create("down-arrow.png"));
        m_pArrowSprite->setTexture("up.png");
        this->stopAllActions();
        
        MoveTo* lMoveTo=MoveTo::create(0.3f, _orginPos+Vec2(0, -ScreenAdapterHelper::getValue(270)));
        CallFunc* lCallFunc = CallFunc::create(CC_CALLBACK_0(ColorScrollLayer::resumeMenuEnable, this));
        this->runAction(Sequence::create(EaseExponentialOut::create(lMoveTo), NULL));
    }
    else
    {
        DataManager::m_bIsShowColorBoard = true;
        m_bIsUpTowardsMove = true;
//        m_pHideScroll->setNormalImage(Sprite::create("up-arrow.png"));
//        m_pHideScroll->setSelectedImage(Sprite::create("up-arrow.png"));
        m_pArrowSprite->setTexture("down.png");
        this->stopAllActions();
        
        MoveTo* lMoveTo=MoveTo::create(0.3f, _orginPos);
        CallFunc* lCallFunc = CallFunc::create(CC_CALLBACK_0(ColorScrollLayer::resumeMenuEnable, this));
        this->runAction(Sequence::create(EaseExponentialIn::create(lMoveTo), NULL));
    }
    
    
}

void ColorScrollLayer::resumeMenuEnable()
{
    m_pHideScroll->setEnabled(true);
}

void ColorScrollLayer::updataColorPackText()
{
    Sprite* _sprite = (Sprite*)this->getChildByTag(st_color_pack_newIcon_tag);
    Sprite* _sprite2 = (Sprite*)this->getChildByTag(st_color_pack_lock_tag);
    if (_sprite)
    {
        _sprite->removeFromParentAndCleanup(true);
    }
    
    if (_sprite2)
    {
        _sprite2->removeFromParentAndCleanup(true);
    }
    
    
    switch (m_iCurrentPage)
    {
        case 0:
        {
            m_pColorGalleryText->setColor(Color3B::BLACK);
        }
            break;
            
        case 1:
        {
            m_pColorGalleryText->setColor(Color3B::BLACK);
        }
            break;
            
        case 2:
        {
            m_pColorGalleryText->setColor(Color3B::BLACK);
        }
            break;
            
        case 3:
        {
            m_pColorGalleryText->setColor(Color3B::BLACK);
            
            if (!UserDefault::getInstance()->getBoolForKey(st_color_pack_Impressionism, false))
            {
                Sprite* lSprite = Sprite::create("color_new.png");
                lSprite->setPosition(Vec2( this->getContentSize().width * 0.9 ,this->getContentSize().height + ScreenAdapterHelper::getValue(25)));
                lSprite->setAnchorPoint(Vec2(1, 0.5));
                lSprite->setScale(0.8f);
                lSprite->setTag(st_color_pack_newIcon_tag);
                this->addChild(lSprite);
            }
        }
            break;
            
        case 4:
        {
            m_pColorGalleryText->setColor(Color3B::BLACK);
            
            if (!UserDefault::getInstance()->getBoolForKey(st_color_pack_Flower, false))
            {
                Sprite* lSprite = Sprite::create("color_new.png");
                lSprite->setPosition(Vec2( this->getContentSize().width * 0.9 ,this->getContentSize().height + ScreenAdapterHelper::getValue(25)));
                lSprite->setAnchorPoint(Vec2(1, 0.5));
                lSprite->setScale(0.8f);
                lSprite->setTag(st_color_pack_newIcon_tag);
                this->addChild(lSprite);
            }
        }
            break;
            
        case 5:
        {
            if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_NEON))
            {
                m_pColorGalleryText->setColor(Color3B(236, 64, 122));
                Sprite* lock = Sprite::create("color_lock.png");
                lock->setAnchorPoint(Vec2(1, 0.5));
                lock->setPosition(Vec2(this->getContentSize().width * 0.9,this->getContentSize().height + ScreenAdapterHelper::getValue(25)));
                lock->setTag(st_color_pack_lock_tag);
                this->addChild(lock);
            }else
            {
                m_pColorGalleryText->setColor(Color3B::BLACK);
            }
        }
            break;
            
        case 6:
        {
            if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_SEA))
            {
                m_pColorGalleryText->setColor(Color3B(236, 64, 122));
                m_pColorGalleryText->setColor(Color3B(236, 64, 122));
                Sprite* lock = Sprite::create("color_lock.png");
                lock->setAnchorPoint(Vec2(1, 0.5));
                lock->setPosition(Vec2(this->getContentSize().width * 0.9,this->getContentSize().height + ScreenAdapterHelper::getValue(25)));
                lock->setTag(st_color_pack_lock_tag);
                this->addChild(lock);
            }else
            {
                m_pColorGalleryText->setColor(Color3B::BLACK);
            }
        }
            break;
            
        case 7:
        {
            if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_UKIYOE))
            {
                m_pColorGalleryText->setColor(Color3B(236, 64, 122));
                m_pColorGalleryText->setColor(Color3B(236, 64, 122));
                Sprite* lock = Sprite::create("color_lock.png");
                lock->setAnchorPoint(Vec2(1, 0.5));
                lock->setPosition(Vec2(this->getContentSize().width * 0.9,this->getContentSize().height + ScreenAdapterHelper::getValue(25)));
                lock->setTag(st_color_pack_lock_tag);
                this->addChild(lock);
            }else
            {
                m_pColorGalleryText->setColor(Color3B::BLACK);
            }
        }
            break;
            
        default:
            break;
    }
}


