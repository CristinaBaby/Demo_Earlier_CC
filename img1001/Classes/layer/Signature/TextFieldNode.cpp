//
//  TextFieldNode.cpp
//  TestProject
//
//  Created by houhuang on 15/10/13.
//
//

#include "TextFieldNode.h"
#include "CursorTextField.h"
#include "../../data/DataManager.h"
#include "../../helper/ScreenAdapterHelper.h"
#include "../EditLayer.h"

bool TextFieldNode::m_bIsOpenKeyboard = false;

TextFieldNode* TextFieldNode::create(const string& bgName, const string& scaleIcon, const string& deleteIcon)
{
    TextFieldNode* lNode = new TextFieldNode();
    if (lNode && lNode->init(bgName, scaleIcon, deleteIcon))
    {
        lNode->autorelease();
        return lNode;
    }
    
    CC_SAFE_DELETE(lNode);
    return NULL;
}

TextFieldNode::~TextFieldNode()
{
    this->removeListener();
}

TextFieldNode::TextFieldNode()
{
    m_bIsMoveTextField = false;
    m_bIsClickIcon = false;
    m_bIsShowWhiteBG = false;
    m_bIsClickThis = false;
    m_bIsTouchEnable = false;
    m_bIsPlayAnimation = false;
    
    m_vStartPoint = Vec2(0, 0);
    m_vOffsetVecThis = Vec2(0, 0);
    m_vOffsetVecScaleIcon = Vec2(0, 0);
    m_vOrignalPoint = Vec2(0, 0);
    
    m_pBgSprite = NULL;
    m_pCursorTextField = NULL;
    m_pScaleIcon = NULL;

    m_pDeleteIcon = NULL;
    m_pBgWhite = NULL;
    m_pLayerColor = NULL;
    m_eListener = NULL;
    
}

bool TextFieldNode::init(const string& bgName, const string& scaleIcon, const string& deleteIcon)
{
    if (!Node::init())  return false;
    
    Sprite* bg = Sprite::create(bgName);
    this->setContentSize(bg->getContentSize());
    this->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(bg);
    m_pBgSprite = bg;
    
    
    Sprite* bg_w = Sprite::create("signatureBG_W.png");
    bg_w->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(bg_w);
    m_pBgWhite = bg_w;
    
    LayerColor* lLayerColor = LayerColor::create(Color4B(0, 0, 0, 80), bg_w->getContentSize().width, bg_w->getContentSize().height);
    lLayerColor->ignoreAnchorPointForPosition(false);
    lLayerColor->setAnchorPoint(Vec2(0.5, 0.5));
    lLayerColor->setPosition(Vec2(bg_w->getContentSize().width/2, bg_w->getContentSize().height/2));
    bg_w->addChild(lLayerColor);
    m_pLayerColor = lLayerColor;
    

    Sprite* lScaleIcon = Sprite::create(scaleIcon);
    lScaleIcon->setPosition(Vec2(this->getContentSize().width, this->getContentSize().height));
    bg_w->addChild(lScaleIcon);
    m_pScaleIcon = lScaleIcon;
    lScaleIcon->setScale(0.5f);
    
    Sprite* lDeleteIcon = Sprite::create(deleteIcon);
    lDeleteIcon->setPosition(Vec2(0, 0));
    bg_w->addChild(lDeleteIcon);
    m_pDeleteIcon = lDeleteIcon;
    lDeleteIcon->setScale(0.5f);
    
    CursorTextField* lCursorField = CursorTextField::create(Size(ScreenAdapterHelper::getValue(245.0), 0), "", "JosefinSans-Bold.ttf", ScreenAdapterHelper::getValue(50));
    lCursorField->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    lCursorField->setColor(Color3B(255, 195, 0));
    this->addChild(lCursorField);
    m_pCursorTextField = lCursorField;
    
    m_pCursorTextField->setColorSpaceHolder(Color4B::WHITE);
    
//    m_pCursorTextField->setDimensions(bg->getContentSize().width, bg->getContentSize().width);
    
    lCursorField->setString(" ");
    lCursorField->setString("");
    
    
    
    m_pBgWhite->setOpacity(0);
    m_pDeleteIcon->setOpacity(0);
    m_pScaleIcon->setOpacity(0);
    m_pLayerColor->setOpacity(0);

    
    return true;
}

void TextFieldNode::onEnterTransitionDidFinish()
{
    Node::onEnterTransitionDidFinish();
    this->addListener();
}

void TextFieldNode::onExit()
{
    Node::onExit();
    this->removeListener();
}

void TextFieldNode::showWhiteBG()
{
    m_pBgWhite->runAction(FadeIn::create(0.5f));
    m_pDeleteIcon->runAction(FadeIn::create(0.5f));
    m_pScaleIcon->runAction(FadeIn::create(0.5f));
    m_pLayerColor->runAction(cocos2d::FadeTo::create(0.5f, 102));
    m_bIsShowWhiteBG = true;
    
    if (!TextFieldNode::m_bIsOpenKeyboard)
    {
        this->scheduleOnce(schedule_selector(TextFieldNode::hideWhiteBG), 2.0f);
    }
}

void TextFieldNode::hideWhiteBG(float dt)
{
    m_bIsShowWhiteBG = false;
    
    m_pBgWhite->runAction(FadeOut::create(0.5f));
    m_pDeleteIcon->runAction(FadeOut::create(0.5f));
    m_pScaleIcon->runAction(FadeOut::create(0.5f));
    m_pLayerColor->runAction(FadeOut::create(0.5f));
//    m_pLayerColor->setVisible(false);
}

void TextFieldNode::hideWhiteBG()
{
    m_pLayerColor->setOpacity(0);
    m_pBgWhite->setOpacity(0);
    m_pDeleteIcon->setOpacity(0);
    m_pScaleIcon->setOpacity(0);
}


void TextFieldNode::updateTextColor(Color3B& color)
{
//    m_pCursorTextField->setTextColor(Color4B(color.r, color.g, color.b, 255));
    m_pCursorTextField->setColor(color);
    m_pCursorTextField->setColorSpaceHolder(color);
}

void TextFieldNode::updareTextFont(const string& fontName)
{
    m_pCursorTextField->setSystemFontName(fontName);
//    m_pCursorTextField->setSystemFontSize(40);
    
    
//    //去除显示不到的字符
//    while (m_pCursorTextField->getContentSize().height > ScreenAdapterHelper::getValue(250.0))
//    {
//        string str = m_pCursorTextField->getString();
//        str.pop_back();
//        m_pCursorTextField->setString(str);
//    }


    if (m_pCursorTextField->getContentSize().height/ScreenAdapterHelper::getValue(250.0) > 1.0)
    {
        m_pCursorTextField->setScale(ScreenAdapterHelper::getValue(250.0)/m_pCursorTextField->getContentSize().height);
    }else
    {
        m_pCursorTextField->setScale(1.0f);
    }
    

}

void TextFieldNode::setWhiteBGVisible(bool pBool)
{
    m_pBgWhite->setVisible(pBool);
}

void TextFieldNode::openIME()
{
    this->closeIME();
    m_pCursorTextField->openIME();
}

void TextFieldNode::closeIME()
{
    m_pCursorTextField->closeIME();
}

void TextFieldNode::setTouchEnable(bool pBool)
{
    m_bIsTouchEnable = pBool;
}

void TextFieldNode::addListener()
{
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = CC_CALLBACK_2(TextFieldNode::onTouchBegan, this);
//    listener->onTouchMoved = CC_CALLBACK_2(TextFieldNode::onTouchMoved, this);
//    listener->onTouchEnded = CC_CALLBACK_2(TextFieldNode::onTouchEnded, this);
//    listener->onTouchCancelled = CC_CALLBACK_2(TextFieldNode::onTouchCancelled, this);
//    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
//    listener->setEnabled(true);
    m_eListener = listener;
    
    _isResonseSingleTouch=true;
    
    
    listener->onTouchesBegan = [this](const std::vector<Touch*>&pTouches, Event*pEvent)->void{
        
        if (!m_bIsTouchEnable)
        {
            return;
        }
        
        if (m_bIsPlayAnimation)
        {
            return;
        }

        if (pTouches.size() >= 2)
        {
            Vec2 lPosition = pTouches[0]->getLocation();
            
            lPosition = this->getParent()->convertToNodeSpace(lPosition);
            
            if (this->getBoundingBox().containsPoint(lPosition))
            {
                m_bIsClickThis = true;
                
                return;
            }
        }
        
        if (pTouches.size() == 1)
        {
            Vec2 lPosition = pTouches[0]->getLocation();
            
            lPosition = this->getParent()->convertToNodeSpace(lPosition);
            
            
            m_vStartPoint = lPosition;
            
            m_vOffsetVecThis = lPosition - this->getPosition();
            
            m_bIsClickIcon = false;
            
            
            Vec2 scaleIconPoint = this->convertToNodeSpace(pTouches[0]->getLocation());
            m_vOffsetVecScaleIcon = scaleIconPoint - m_pScaleIcon->getPosition();
            
            if (m_bIsShowWhiteBG)
            {
                if (m_pScaleIcon->getBoundingBox().containsPoint(scaleIconPoint))
                {
                    m_bIsClickThis =true;
                    m_bIsClickIcon = true;
                    return;
                }
            
                if (m_pDeleteIcon->getBoundingBox().containsPoint(scaleIconPoint))
                {
                    m_bIsClickThis =true;
                    m_bIsMoveTextField = true;
                    this->setVisible(false);
                    this->setTouchEnable(false);
                    this->reset();
                    return;
                }
            }
                        
            if (this->getBoundingBox().containsPoint(lPosition))
            {
                m_bIsClickThis =true;
                m_bIsMoveTextField = false;
                            
                return;
            }
        }
        
    };
    listener->onTouchesMoved = [this](const std::vector<Touch*>&touches, Event*pEvent)->void{

        if (!m_bIsTouchEnable)
        {
            return;
        }
        
        if (!m_bIsClickThis)
        {
            return;
        }
        
            if(touches.size() > 1 && m_bIsClickIcon == false)
            {
                // 多点进行缩放
                
            // 得到当前两触摸点
            auto point1 = this->getParent()->convertToNodeSpace(touches[0]->getLocation());
            auto point2 = this->getParent()->convertToNodeSpace(touches[1]->getLocation());
            // 计算两点之间得距离
            auto currDistance = touches[0]->getLocation().distance(touches[1]->getLocation());
                //point1.distance(point2);
                
            // 计算两触摸点上一时刻之间得距离
            auto prevDistance = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());
            
            // 根据两触摸点前后的距离计算缩放倍率
            auto scale = this->getScale() * (currDistance / prevDistance);
            // 控制缩放倍率在0.5～4倍之间，最小倍率不能太小，不让背景将不能填充满整个屏幕。
            scale = MIN(4, MAX(0.5f, scale));
            Vec2 mPoint1=this->convertToNodeSpace(touches[0]->getLocation());
            Vec2 mPoint2=this->convertToNodeSpace(touches[1]->getLocation());
                
            Vec2 centerPos=(mPoint1+mPoint2)/2;
            Vec2 offsetPos=Vec2(this->getContentSize().width/2, this->getContentSize().height/2)-centerPos;
            offsetPos*=this->getScale();
            Vec2 lAnchor=Vec2(centerPos.x/this->getContentSize().width, centerPos.y/this->getContentSize().height);
                
                
            this->setPosition(this->getPosition()-offsetPos);
            this->setAnchorPoint(lAnchor);
                
            this->setScale(scale);
            m_pDeleteIcon->setScale(1/scale);
            m_pScaleIcon->setScale(1/scale);
                
            offsetPos=Vec2(this->getContentSize().width/2, this->getContentSize().height/2)-centerPos;
            offsetPos*=this->getScale();
            this->setPosition(this->getPosition()+offsetPos);
            this->setAnchorPoint(Vec2(0.5f, 0.5f));

        }
        else if(touches.size() == 1)
        {
            
            if (m_bIsClickIcon == true)
            {
                        Vec2 lPosition = touches[0]->getLocation();
                        lPosition = this->getParent()->convertToNodeSpace(lPosition);
                        lPosition = lPosition - m_vOffsetVecScaleIcon;
                
                        this->setRotation(-atan2((lPosition.y - this->getPositionY()),(lPosition.x - this->getPositionX()))*180/3.1415926 + 45);
                
                        float distance_touch = sqrtf(powf(lPosition.x - this->getPositionX(), 2) + powf(lPosition.y - this->getPositionY(), 2));
                //
                        float distance = sqrtf(powf(this->getContentSize().width, 2) + powf(this->getContentSize().height, 2)) / 2;
                
                        float scale = distance_touch / distance;
                
                        if (scale > 0.5 && scale < 4)
                        {
                            this->setScale(scale);
                            m_pDeleteIcon->setScale(1/scale );
                            m_pScaleIcon->setScale(1/scale );
                        }
                        
                        
            }else
            {
                Vec2 currentPoint = touches[0]->getLocation();
                currentPoint = this->getParent()->convertToNodeSpace(currentPoint);
                
                Vec2 lPriviousePos=this->getParent()->convertToNodeSpace(touches[0]->getPreviousLocation());
                
                Vec2 offsetVec = currentPoint - lPriviousePos;
                
                
                this->setPosition(this->getPosition() + offsetVec);
                
                if (this->getPosition().x < 0)
                {
                    this->setPosition(Vec2(0, this->getPosition().y));
                }
                if (this->getPosition().x > this->getParent()->getContentSize().width)
                {
                    this->setPosition(Vec2(this->getParent()->getContentSize().width, this->getPosition().y));
                }
                if (this->getPosition().y < 0)
                {
                    this->setPosition(Vec2(this->getPosition().x, 0));
                }
                if (this->getPosition().y > this->getParent()->getContentSize().height)
                {
                    this->setPosition(Vec2(this->getPosition().x, this->getParent()->getContentSize().height));
                }
                
                float moveDistance = sqrt( pow(currentPoint.x - m_vStartPoint.x, 2) + pow(currentPoint.y - m_vStartPoint.y, 2) );
                if (moveDistance > 10 && m_bIsMoveTextField == false)
                {
                    m_bIsPlayAnimation = true;
                    this->scheduleOnce(schedule_selector(TextFieldNode::update), 0.35f);
                    
                    m_bIsMoveTextField = true;
                    EditLayer* lEditLayer = dynamic_cast<EditLayer*>(this->getParent()->getParent());
                    
                    this->closeIME();
                    TextFieldNode::m_bIsOpenKeyboard = false;
                    this->hideWhiteBG();
                    
                    lEditLayer->m_bFileNodeReadyMove = false;
                    lEditLayer->hideSignatureLayer();
                    
                    
                }
            }

            
        }
    };
    
    listener->onTouchesEnded = [this](const std::vector<Touch*>&pTouches, Event*pEvent)->void{
        
        if (!m_bIsTouchEnable)
        {
            return;
        }
        
            if (!m_bIsMoveTextField && m_bIsClickThis)
            {
                EditLayer* lEditLayer = dynamic_cast<EditLayer*>(this->getParent()->getParent());
                if (lEditLayer->getCurrentIsKeyboard())
                {
                    this->openIME();
                }
                TextFieldNode::m_bIsOpenKeyboard = true;
                this->showWhiteBG();
        
                lEditLayer->showSignatureLayer();
                m_bIsPlayAnimation = true;
                this->scheduleOnce(schedule_selector(TextFieldNode::update), 0.35f);
            }
            m_bIsClickThis = false;
    };
    
    dispatcher->addEventListenerWithFixedPriority(listener, 1);
    
}

void TextFieldNode::update(float dt)
{
    m_bIsPlayAnimation = false;
}

void TextFieldNode::removeListener()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    dispatcher->removeEventListenersForTarget(this);
    
    dispatcher->removeEventListener(m_eListener);
}

void TextFieldNode::reset()
{
    DataManager::getInstance()->m_bIsOpenKeyboard = false;
 
    m_pCursorTextField->closeIME();
    m_pCursorTextField->setColorSpaceHolder(Color4B::WHITE);
    m_pCursorTextField->setString(" ");
    m_pCursorTextField->setString("");
    m_pCursorTextField->setSystemFontName("JosefinSans-Bold.ttf");
    this->setRotation(0);
    m_pCursorTextField->setColor(Color3B(255, 195, 0));
    
    m_pDeleteIcon->setScale(0.5f);
    m_pScaleIcon->setScale(0.5f);
    
    EditLayer* lEditLayer = dynamic_cast<EditLayer*>(this->getParent()->getParent());
    lEditLayer->resetFieldNode();
    
    DataManager::getInstance()->initFontTypeInfoDatas("FontTypeInfo.csv");
}