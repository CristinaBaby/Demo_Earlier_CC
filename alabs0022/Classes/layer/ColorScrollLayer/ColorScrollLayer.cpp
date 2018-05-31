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
#include "../../commpoent/ColorSprite.h"
#include "ColorScrollLayerMask.h"


enum
{
    st_btn_updown_tag = 10,
    st_btn_left_tag,
    st_btn_right_tag,
    
    st_resently_color_tag = 77,
    
    st_color_pack_newIcon_tag,

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

    
    m_bIsUpTowardsMove  = true;
    m_bIsInitColorSpriteVector = false;
    m_bIsMoveFinish     = true;
    m_bIsClickScroll    = false;
    
    m_vColorDatas = &DataManager::getInstance()->_colorDatas;
    m_vColorGalleryText.push_back("BASIC");
    m_vColorGalleryText.push_back("LIGHT");
    m_vColorGalleryText.push_back("SWEET");
    m_vColorGalleryText.push_back("AUTUMN");
    m_vColorGalleryText.push_back("SUNSET");
    m_vColorGalleryText.push_back("ROSANNE");
    m_vColorGalleryText.push_back("JOYFUL");
    m_vColorGalleryText.push_back("NEON");
    m_vColorGalleryText.push_back("NATURE");
    m_vColorGalleryText.push_back("BLACKS");

}

ColorScrollLayer::~ColorScrollLayer()
{
    m_pPreColorSprite = NULL;

    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);

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
    if (!LayerColor::initWithColor(Color4B(49, 49, 49, 255))) return false;

    m_bIsFirstCreateLayer = true;
    
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setContentSize(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(400)));
    
//    
    
    m_pScrollView = ScrollView::create();
//    m_pScrollView->setContentSize(Size(ScreenAdapterHelper::getValue(700), ScreenAdapterHelper::getValue(300)));
    m_pScrollView->setViewSize(Size(ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(200)));
    m_pScrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    m_pScrollView->ignoreAnchorPointForPosition(false);
    m_pScrollView->setAnchorPoint(Vec2(0.5, 1));
    m_pScrollView->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height - ScreenAdapterHelper::getValue(10)));
    m_pScrollView->setTouchEnabled(false);
    this->addChild(m_pScrollView);
//    m_pScrollView->setSwallowsTouches(true);
    
    m_pPageDot = PageDot::create("page_turn2.png", "page_turn1.png");
    m_pPageDot->setPageCount(10);
    m_pPageDot->setSelectPage(0);
    m_pPageDot->setPosition(Vec2(this->getContentSize().width/2 - ScreenAdapterHelper::getValue(80), ScreenAdapterHelper::getValue(120)));
    this->addChild(m_pPageDot);
    
    m_iMaxPage = m_pPageDot->getPageCount();
    
    m_pContentLayer = Layer::create();
//    m_pContentLayer = LayerColor::create(Color4B(0, 250, 0, 100));
    m_pContentLayer->setContentSize(Size(m_pScrollView->getViewSize().width * m_iMaxPage, m_pScrollView->getViewSize().height));
    m_pScrollView->addChild(m_pContentLayer);

    m_pScrollView->setDelegate(this);
    
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

    m_pLeft = MenuItemSprite::create(Sprite::create("btn_no.png"), NULL, CC_CALLBACK_1(ColorScrollLayer::doAction, this));
    m_pLeft->ignoreAnchorPointForPosition(false);
    m_pLeft->setAnchorPoint(Vec2(0.5, 0.5));
    m_pLeft->setPosition(Vec2(ScreenAdapterHelper::getValue(50), ScreenAdapterHelper::getValue(170)));
    m_pLeft->setTag(st_btn_left_tag);
    m_pLeft->setScale(0.85f);
    
    m_pRight = MenuItemSprite::create(Sprite::create("btn_right.png"), NULL, CC_CALLBACK_1(ColorScrollLayer::doAction, this));
    m_pRight->ignoreAnchorPointForPosition(false);
    m_pRight->setAnchorPoint(Vec2(0.5, 0.5));
    m_pRight->setPosition(Vec2(this->getContentSize().width - ScreenAdapterHelper::getValue(50), ScreenAdapterHelper::getValue(170)));
    m_pRight->setTag(st_btn_right_tag);
    m_pRight->setScale(0.85f);
    
    m_pMenu = Menu::create( m_pLeft, m_pRight, NULL);
    m_pMenu->setPosition(Vec2::ZERO);
    this->addChild(m_pMenu);
    
    string str = m_vColorGalleryText.at(0);
    m_pColorGalleryText = Label::createWithSystemFont(str.c_str(), "Arial", ScreenAdapterHelper::getValue(34));
//    m_pColorGalleryText = Label::createWithTTF(str.c_str(), "Arial", ScreenAdapterHelper::getValue(34));
    m_pColorGalleryText->setPosition(Vec2(this->getContentSize().width/2,ScreenAdapterHelper::getValue(170)));
    m_pColorGalleryText->setColor(Color3B::WHITE);
    this->addChild(m_pColorGalleryText);

}

void ColorScrollLayer::doAction(Ref* pSender)
{
    MenuItemImage* clickMenu = (MenuItemImage*)pSender;
    switch (clickMenu->getTag())
    {
        case st_btn_left_tag:
        {
            m_pMenu->setEnabled(false);
            m_bIsMoveFinish = false;
            --m_iCurrentPage;
            
            if (m_iCurrentPage < 1)
            {
                m_pLeft->setNormalImage(Sprite::create("btn_no.png"));
            }else
            {
                m_pRight->setNormalImage(Sprite::create("btn_right.png"));
            }
            
            if (m_iCurrentPage < 0)
            {
                m_iCurrentPage = 0;
                
            }else
            {
                
                m_pScrollView->setContentOffsetInDuration(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0), 0.3f);
            }
            
            m_pPageDot->setSelectPage(m_iCurrentPage);
            
            string str = m_vColorGalleryText.at(m_iCurrentPage);
            m_pColorGalleryText->setString(str);
            
            this->updataColorPackText();
            
            scheduleOnce(schedule_selector(ColorScrollLayer::menuDelayEnable), 0.3f);
        }
            break;
        case st_btn_right_tag:
        {
            m_pMenu->setEnabled(false);
            ++m_iCurrentPage;
            
            if (m_iCurrentPage >= m_iMaxPage - 1)
            {
                m_pRight->setNormalImage(Sprite::create("btn_no.png"));
            }else
            {
                m_pLeft->setNormalImage(Sprite::create("btn_left.png"));
            }
            
            if (m_iCurrentPage >= m_iMaxPage)
            {
                m_iCurrentPage = m_iMaxPage - 1;
                
            }else
            {
                m_pScrollView->setContentOffsetInDuration(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0), 0.3f);
            }
            
            m_pPageDot->setSelectPage(m_iCurrentPage);
            
            string str = m_vColorGalleryText.at(m_iCurrentPage);
            m_pColorGalleryText->setString(str);
            
            this->updataColorPackText();
            
            
            
            
            scheduleOnce(schedule_selector(ColorScrollLayer::menuDelayEnable), 0.3f);
        }
            break;
            
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
        
        m_vOffset = m_pScrollView->getContentOffset();
        
        Vec2 lPos = touch->getLocationInView();
        lPos = Director::getInstance()->convertToGL(lPos);
        lPos = this->convertToNodeSpace(lPos);
        m_vFirstPoint = lPos;
        
        
        if (m_bIsMoveFinish)
        {
            //避免多点触摸问题
            if (!m_bIsClickScroll)
            {
                if (m_pScrollView->boundingBox().containsPoint(lPos))
                {
                    m_bIsClickScroll = true;
                    m_bIsMoveFinish = false;
                    m_pMenu->setEnabled(false);
                    
                    return true;
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
        
        m_pScrollView->setContentOffset(Vec2(distance + m_vOffset.x, 0));
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event)
    {
        Vec2 endPoint = touch->getLocationInView();
        endPoint = Director::getInstance()->convertToGL(endPoint);
        endPoint = this->convertToNodeSpace(endPoint);
        
        float distance = endPoint.x - m_vFirstPoint.x;
        
        if (fabs(distance) > 200)
        {
            if (distance > 0)
            {
                --m_iCurrentPage;
                m_pRight->setNormalImage(Sprite::create("btn_right.png"));
            }
            else if (distance < 0)
            {
                ++m_iCurrentPage;
                m_pLeft->setNormalImage(Sprite::create("btn_left.png"));
            }
            
            if (m_iCurrentPage == m_iMaxPage - 1)
            {
                m_pRight->setNormalImage(Sprite::create("btn_no.png"));
            }
            if (m_iCurrentPage == 0)
            {
                m_pLeft->setNormalImage(Sprite::create("btn_no.png"));
            }
        }
        
        if (m_iCurrentPage < 0)
        {
            m_iCurrentPage = 0;
        }else if (m_iCurrentPage >= m_iMaxPage)
        {
            m_iCurrentPage = m_iCurrentPage - 1;
        }
        
        float moveTime = (768 - abs(distance))/768 * 0.3;
        
        m_pScrollView->setContentOffsetInDuration(Vec2(-m_pScrollView->getViewSize().width * m_iCurrentPage, 0), moveTime);
        m_pPageDot->setSelectPage(m_iCurrentPage);
        
        string str = m_vColorGalleryText.at(m_iCurrentPage);
        m_pColorGalleryText->setString(str);
        
        this->updataColorPackText();
        
        this->scheduleOnce(schedule_selector(ColorScrollLayer::areadyMoveFinish), moveTime);
        
        m_bIsClickScroll = false;
    };
    
    
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void ColorScrollLayer::areadyMoveFinish(float dt)
{
    m_bIsMoveFinish = true;
    m_pMenu->setEnabled(true);
}

void ColorScrollLayer::initContentLayer()
{
    m_vColorBGLayer.clear();
    m_vColorSprite.clear();
    
    for (int j = 0; j < 10; j++)
    {
        LayerColor* layer = LayerColor::create(Color4B(255, 255, 255, 0), ScreenAdapterHelper::getValue(768), ScreenAdapterHelper::getValue(200));
        layer->ignoreAnchorPointForPosition(false);
        layer->setAnchorPoint(Vec2(0.5, 0.5));
        layer->setPosition(Vec2(ScreenAdapterHelper::getValue(384) + ScreenAdapterHelper::getValue(768) * j, this->getContentSize().height * 0.28));
        m_pContentLayer->addChild(layer);
        
        m_vColorBGLayer.push_back(layer);
        
        for (int i = 0 ; i < 9; i++)
        {
            ColorData colorData = m_vColorDatas->at(_index);
            
            ColorSprite* lSprite = ColorSprite::create("color01.png");
            lSprite->setAnchorPoint(Vec2(0.5, 0.5));
            lSprite->setDelegate(this);

            float lX = ScreenAdapterHelper::getValue(154) + lSprite->getContentSize().width * 0.6 * i;
            float lY = (i % 2 == 0)? ScreenAdapterHelper::getValue(50):ScreenAdapterHelper::getValue(140);
            lSprite->setPosition(Vec2(lX, lY));
            lSprite->setOriPositionX(lX);
            lSprite->setOriPositionY(lY);
            
            if (j >= 1)
            {
                lSprite->setPosition(Vec2(ScreenAdapterHelper::getValue(0), ScreenAdapterHelper::getValue(100)));
            }
            
            lSprite->addListener(callfuncN_selector(ColorScrollLayer::reponseColorSpriteTouch), this);
            lSprite->setColor(Color3B(colorData.R, colorData.G, colorData.B));
            lSprite->setColorData(colorData);
            layer->addChild(lSprite);
            
            lSprite->setClickSpite(false);
            if (colorData.R == 239 && colorData.G == 128 && colorData.B == 49 && m_bIsFirstCreateLayer)
            {
                m_bIsFirstCreateLayer = false;
                lSprite->responseTouchEvent(Vec2(0, 0));
            }
            
            m_vColorSprite.push_back(lSprite);
            ++_index;
        }
    }
}

void ColorScrollLayer::reponseTouchEvent(ColorSprite* pColorSprite)
{
    if(pColorSprite->getIsSelected())
    {
        DataManager::getInstance()->isAlreadySelectedColor=true;
        DataManager::getInstance()->selectedColorData=pColorSprite->getColorData();
    }
}

void ColorScrollLayer::scrollViewDidScroll(ScrollView* view)
{
    float scrollViewOffset = m_pScrollView->getContentOffset().x;
    
//    log("%f",scrollViewOffset + m_vColorBGLayer.at(1)->getPosition().x - ScreenAdapterHelper::getValue(384));
    
    for (auto layer : m_vColorBGLayer)
    {
        float offset = layer->getPosition().x + scrollViewOffset - ScreenAdapterHelper::getValue(384);
//        log("%f",offset);
        
        if (offset > ScreenAdapterHelper::getValue(-768) && offset < ScreenAdapterHelper::getValue(768))
        {
            for (auto lSprite : layer->getChildren())
            {
                ColorSprite* sprite = dynamic_cast<ColorSprite*>(lSprite);
                if (offset > 0 )
                {
                    float scale = offset / ScreenAdapterHelper::getValue(768);
                    float offsetX = ( - sprite->getOriPositionX()) * scale;
                    float offsetY = (100 - sprite->getOriPositionY()) * scale;
                    
                    lSprite->setPosition(Vec2(sprite->getOriPositionX() + offsetX, sprite->getOriPositionY() + offsetY));
                    
                }
               
                else
                {
                    float scale = abs(offset / ScreenAdapterHelper::getValue(768));
                    float offsetX = (768 - sprite->getOriPositionX()) * scale;
                    float offsetY = (100 - sprite->getOriPositionY()) * scale;
                    lSprite->setPosition(Vec2(sprite->getOriPositionX() + offsetX, sprite->getOriPositionY() + offsetY));
                    
                    //                log("scale = %f , offset = %f", scale, offset);
                }
            }

        }else
        {
            if (offset > 0)
            {
                for (auto lSprite : layer->getChildren())
                {
                    ColorSprite* sprite = dynamic_cast<ColorSprite*>(lSprite);
                    
                    sprite->setPosition(Vec2( 0, 100));
                }
            }else
            {
                for (auto lSprite : layer->getChildren())
                {

                    ColorSprite* sprite = dynamic_cast<ColorSprite*>(lSprite);
                    sprite->setPosition(Vec2( 768, 100));

                }
            }
        
        }
    }
    
}

void ColorScrollLayer::scrollViewDidZoom(ScrollView* view)
{

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

void ColorScrollLayer::reponseColorSpriteTouch(Node* pSender)
{
    ColorSprite* lSprite = (ColorSprite*)pSender;

    
    for (int i = 0 ; i < m_vColorSprite.size(); ++i)
    {
        ColorSprite* lSprite2 = m_vColorSprite.at(i);
        if (lSprite2)
        {
            lSprite2->selected(false);
        }
    }

    lSprite->responseTouchEvent(Vec2(0, 0));
    
//    ColorData lColorData = lSprite->getColorData();


    
//    if (m_pPreColorSprite)
//    {
//        m_pPreColorSprite->removeFromParentAndCleanup(true);
//        m_pPreColorSprite = NULL;
//    }
//    
//    DataManager::getInstance()->selectedColorData = lSprite->getColorData();
//    
//    m_pPreColorSprite = LayerColor::create(Color4B::WHITE, ScreenAdapterHelper::getValue(40), ScreenAdapterHelper::getValue(40));
//    
//    if (DataManager::getInstance()->selectedColorData.R > 245 && DataManager::getInstance()->selectedColorData.G > 245 && DataManager::getInstance()->selectedColorData.B > 245)
//    {
//        m_pPreColorSprite->setColor(Color3B::BLACK);
//    }
//    
//    m_pPreColorSprite->ignoreAnchorPointForPosition(false);
//    m_pPreColorSprite->setAnchorPoint(Vec2(0.5, 0.5));
//    m_pPreColorSprite->setPosition(Vec2(lSprite->getContentSize().width/2, lSprite->getContentSize().height/2));
//    m_pPreColorSprite->setOpacity(0);
//    lSprite->addChild(m_pPreColorSprite);
//    
//    m_pPreColorSprite->runAction(FadeIn::create(0.5f));
    

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
//    GameUILayer* lGameUILayer = dynamic_cast<GameUILayer*>(this->getParent());
//    lGameUILayer->reponseMaskAnimation(m_bIsUpTowardsMove);
//    
//    if (m_bIsUpTowardsMove)
//    {
//        m_bIsUpTowardsMove = false;
//        m_pHideScroll->setNormalImage(Sprite::create("down-arrow.png"));
//        m_pHideScroll->setSelectedImage(Sprite::create("down-arrow.png"));
//        this->stopAllActions();
//        
//        MoveTo* lMoveTo=MoveTo::create(0.3f, _orginPos+Vec2(0, ScreenAdapterHelper::getValue(250)));
//        CallFunc* lCallFunc = CallFunc::create(CC_CALLBACK_0(ColorScrollLayer::resumeMenuEnable, this));
//        this->runAction(Sequence::create(EaseExponentialOut::create(lMoveTo), lCallFunc, NULL));
//    }
//    else
//    {
//        m_bIsUpTowardsMove = true;
//        m_pHideScroll->setNormalImage(Sprite::create("up-arrow.png"));
//        m_pHideScroll->setSelectedImage(Sprite::create("up-arrow.png"));
//        this->stopAllActions();
//        
//        MoveTo* lMoveTo=MoveTo::create(0.3f, _orginPos);
//        CallFunc* lCallFunc = CallFunc::create(CC_CALLBACK_0(ColorScrollLayer::resumeMenuEnable, this));
//        this->runAction(Sequence::create(EaseExponentialIn::create(lMoveTo), lCallFunc, NULL));
//    }
    
    
}

void ColorScrollLayer::resumeMenuEnable()
{
    m_pHideScroll->setEnabled(true);
}

void ColorScrollLayer::updataColorPackText()
{
    Sprite* _sprite = (Sprite*)this->getChildByTag(st_color_pack_newIcon_tag);
    if (_sprite)
    {
        _sprite->removeFromParentAndCleanup(true);
    }
    
    m_pColorGalleryText->setColor(Color3B::WHITE);
//    switch (m_iCurrentPage)
//    {
//        case 0:
//        {
//            m_pColorGalleryText->setColor(Color3B::BLACK);
//        }
//            break;
//            
//        case 1:
//        {
//            m_pColorGalleryText->setColor(Color3B::BLACK);
//        }
//            break;
//            
//        case 2:
//        {
//            m_pColorGalleryText->setColor(Color3B::BLACK);
//        }
//            break;
//            
//        case 3:
//        {
//            m_pColorGalleryText->setColor(Color3B::BLACK);
//            
//            if (!UserDefault::getInstance()->getBoolForKey(st_color_pack_Impressionism, false))
//            {
//                Sprite* lSprite = Sprite::create("image_newIcon.png");
//                lSprite->setPosition(Vec2( this->getContentSize().width * 0.7 ,ScreenAdapterHelper::getValue(120)));
//                lSprite->setAnchorPoint(Vec2(0, 0.5));
//                lSprite->setScale(0.8f);
//                lSprite->setTag(st_color_pack_newIcon_tag);
//                this->addChild(lSprite);
//            }
//        }
//            break;
//            
//        case 4:
//        {
//            m_pColorGalleryText->setColor(Color3B::BLACK);
//            
//            if (!UserDefault::getInstance()->getBoolForKey(st_color_pack_Flower, false))
//            {
//                Sprite* lSprite = Sprite::create("image_newIcon.png");
//                lSprite->setPosition(Vec2( this->getContentSize().width * 0.6 ,ScreenAdapterHelper::getValue(120)));
//                lSprite->setAnchorPoint(Vec2(0, 0.5));
//                lSprite->setScale(0.8f);
//                lSprite->setTag(st_color_pack_newIcon_tag);
//                this->addChild(lSprite);
//            }
//        }
//            break;
//            
//        case 5:
//        {
//            if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_NEON))
//            {
//                m_pColorGalleryText->setColor(Color3B(236, 64, 122));
//            }else
//            {
//                m_pColorGalleryText->setColor(Color3B::BLACK);
//            }
//        }
//            break;
//            
//        case 6:
//        {
//            if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_SEA))
//            {
//                m_pColorGalleryText->setColor(Color3B(236, 64, 122));
//            }else
//            {
//                m_pColorGalleryText->setColor(Color3B::BLACK);
//            }
//        }
//            break;
//            
//        case 7:
//        {
//            if (!UserDefault::getInstance()->getBoolForKey(IS_PURCHASE_COLOR_UKIYOE))
//            {
//                m_pColorGalleryText->setColor(Color3B(236, 64, 122));
//            }else
//            {
//                m_pColorGalleryText->setColor(Color3B::BLACK);
//            }
//        }
//            break;
//            
//        default:
//            break;
//    }
}


