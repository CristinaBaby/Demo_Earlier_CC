//
//  STPageItemGameImage.cpp
//  LVUP004
//
//  Created by Steven.Xc.Tian on 13-11-18.
//
//

#include "STPageItemGameImage.h"
#include "STGameUIPicture.h"
#include "../../AppGlobal.h"
#include "../../scenes/GameScene.h"


USING_NS_CC;


STPageItemGameImage* STPageItemGameImage::create(CCArray* data, CCSize size, unsigned rowNum, unsigned colNum, const unsigned curIdx,
                                                 const ccColor4B& color /*= cocos2d::ccc4(0,0,0,0)*/)
{
    STPageItemGameImage* pRet = new STPageItemGameImage(data, rowNum, colNum);
    if (pRet && pRet->initWithColor(color, size, curIdx))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

STPageItemGameImage::STPageItemGameImage(CCArray* data, unsigned rowNum, unsigned colNum)
: cm_RowNum(rowNum), cm_ColumnNum(colNum)
{
    p_a_SpriteFrameData = data;
    
    p_AllImages = new CCArray();
}

STPageItemGameImage::~STPageItemGameImage()
{
    CC_SAFE_DELETE(p_AllImages);
}

bool STPageItemGameImage::initWithColor(const ccColor4B& color, CCSize size, const unsigned curIdx)
{
    bool bRet = false;
    
    do
    {
        // super class method
        CC_BREAK_IF(! STPage::initWithColor(color, size.width, size.height));
        
        m_LableSize = CCSizeMake(size.width / cm_ColumnNum, size.height / cm_RowNum);
        
        for (int i = 0; i < cm_RowNum; i++)
        {
            for (int j = 0; j < cm_ColumnNum; j++)
            {
                STGameUIPicture *image = STGameUIPicture::create("ui04_frame.png", "ui04_frame_h.png");
                if (!image)
                    return false;
                
                image->setPosition(ccp(m_LableSize.width * j + m_LableSize.width * 0.5f,
                                     ((i & 1) == 1 ? m_LableSize.height : 0) + m_LableSize.height * 0.5f));
                
                pBaseLayer->addChild(image);
                
                p_AllImages->addObject(image);
            }
        }
        
        this->refreshPage(curIdx);
        
        bRet = true;
    } while (0);
    
    return bRet;
    
}

void STPageItemGameImage::refreshPage(unsigned idx)
{
    const unsigned baseIdx = idx * cm_RowNum * cm_ColumnNum;
    
    unsigned offset = 0;
    
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(p_AllImages, pObj)
    {
        const unsigned dataIdx = baseIdx + offset;
        offset++;
        
        STGameUIPicture* picture = dynamic_cast<STGameUIPicture*>(pObj);
        CC_BREAK_IF(!picture);
        
        if (dataIdx >= p_a_SpriteFrameData->count())
        {
            picture->setVisible(false);
            continue;
        }
        
        picture->setVisible(true);
        
        
        CCSpriteFrame *pFrame = dynamic_cast<CCSpriteFrame *>(p_a_SpriteFrameData->objectAtIndex(dataIdx));
        CC_BREAK_IF (!pFrame);

        picture->refreshPicture(pFrame);

        // current cell item is the one skill bonus wanted.
        picture->setChecked((dataIdx == GameScene::s_uSkillBonusIndex));
        
    }
    
}