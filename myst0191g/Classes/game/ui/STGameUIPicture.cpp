//
//  STGameUIPicture.cpp
//  LVUP004
//
//  Created by Steven.Xc.Tian on 13-11-18.
//
//

#include "STGameUIPicture.h"
#include "../../AppGlobal.h"


USING_NS_CC;


STGameUIPicture* STGameUIPicture::create(std::string normalName, std::string highlightName)
{
    STGameUIPicture *pobSprite = new STGameUIPicture(normalName, highlightName);
    if (pobSprite && pobSprite->initWithSpriteFrameName(normalName.c_str()))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

STGameUIPicture::STGameUIPicture(std::string normalName, std::string highlightName)
: STToggleButton(normalName, highlightName),
m_pPicture(NULL)
{
    // in this case, we don't receive any touch events.
    this->setEnabled(false);
}

STGameUIPicture::~STGameUIPicture()
{
}

void STGameUIPicture::refreshPicture(CCSpriteFrame *pFrame)
{
    if (m_pPicture)
    {
        m_pPicture->setDisplayFrame(pFrame);
    }
    else
    {
        m_pPicture = CCSprite::createWithSpriteFrame(pFrame);
        CCAssert(m_pPicture, "STGameUIPicture refreshPicture, picture is NULL ");
        this->addChild(m_pPicture);
        m_pPicture->setPosition(ccp_get_center(this->getContentSize()));
        
    }
}

