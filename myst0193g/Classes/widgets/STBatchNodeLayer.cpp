//
//  STBatchNodeLayer.cpp
//  CHUPS003
//
//  Created by Steven.Xc.Tian on 13-11-4.
//
//

#include "STBatchNodeLayer.h"
#include "../AppGlobal.h"


USING_NS_CC;
using std::string;



STBatchNodeLayer* STBatchNodeLayer::create(std::string fileImage)
{
    STBatchNodeLayer* pRet = new STBatchNodeLayer(fileImage);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

STBatchNodeLayer* STBatchNodeLayer::create(std::string fileImage, unsigned capacity)
{
    STBatchNodeLayer * pLayer = new STBatchNodeLayer(fileImage, capacity);
    
    if(pLayer && pLayer->init())
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

STBatchNodeLayer* STBatchNodeLayer::create(std::string fileImage, const cocos2d::ccColor4B& color)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    STBatchNodeLayer * pLayer = new STBatchNodeLayer(fileImage);
    
    if(pLayer && pLayer->initWithColor(color, s.width, s.height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}



STBatchNodeLayer* STBatchNodeLayer::create(cocos2d::CCTexture2D* tex)
{
    STBatchNodeLayer* pRet = new STBatchNodeLayer(tex);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

STBatchNodeLayer* STBatchNodeLayer::create(cocos2d::CCTexture2D* tex, unsigned capacity)
{
    STBatchNodeLayer* pRet = new STBatchNodeLayer(tex, capacity);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

STBatchNodeLayer* STBatchNodeLayer::create(cocos2d::CCTexture2D* tex, const cocos2d::ccColor4B& color)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    STBatchNodeLayer * pLayer = new STBatchNodeLayer(tex);
    
    if(pLayer && pLayer->initWithColor(color, s.width, s.height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}




STBatchNodeLayer::STBatchNodeLayer(string fileImage, unsigned capacity)
{
    m_p_BatchNode = CCSpriteBatchNode::create(fileImage.c_str(), capacity);
    CC_SAFE_RETAIN(m_p_BatchNode);
}

STBatchNodeLayer::STBatchNodeLayer(string fileImage)
{
    m_p_BatchNode = CCSpriteBatchNode::create(fileImage.c_str());
    CC_SAFE_RETAIN(m_p_BatchNode);
}

STBatchNodeLayer::STBatchNodeLayer(CCTexture2D* tex, unsigned capacity)
{
    m_p_BatchNode = CCSpriteBatchNode::createWithTexture(tex, capacity);
    CC_SAFE_RETAIN(m_p_BatchNode);
}

STBatchNodeLayer::STBatchNodeLayer(CCTexture2D* tex)
{
    m_p_BatchNode = CCSpriteBatchNode::createWithTexture(tex);
    CC_SAFE_RETAIN(m_p_BatchNode);
}


bool STBatchNodeLayer::initWithColor(const ccColor4B& color, GLfloat width, GLfloat height)
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(! CCLayerColor::initWithColor(color, width, height));
        
        //
        CC_BREAK_IF(!m_p_BatchNode);
        this->addChild(m_p_BatchNode, 1);
        m_p_BatchNode->setScale(AppGlobal::UI_SCALE);
        
        // calculate the actual width pixels, and put the batch node on the center screen.
        float margin = VISIBLE_RECT.getMaxX() * (1 - AppGlobal::UI_SCALE);
        m_p_BatchNode->setPosition(ccp((margin / 2), 0));
        
        // release the one retained in constructor.
        CC_SAFE_RELEASE(m_p_BatchNode);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

void STBatchNodeLayer::addChildToBatchNode(CCNode * child)
{
    if (m_p_BatchNode)
    {
        m_p_BatchNode->addChild(child);
    }
}

void STBatchNodeLayer::addChildToBatchNode(CCNode * child, int zOrder)
{
    if (m_p_BatchNode)
    {
        m_p_BatchNode->addChild(child, zOrder);
    }
}

void STBatchNodeLayer::addChildToBatchNode(CCNode* child, int zOrder, int tag)
{
    if (m_p_BatchNode)
    {
        m_p_BatchNode->addChild(child, zOrder, tag);
    }
}
