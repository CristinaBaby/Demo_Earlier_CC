//
//  STPageItemGameLabel.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-14.
//
//

#include "STPageItemGameLabel.h"
#include "STGameUILabelTTF.h"
#include "../../AppGlobal.h"

USING_NS_CC;

#define TextSpecialColor            ccc3(255, 252, 172)

STPageItemGameLabel* STPageItemGameLabel::create(CCDictionary *names, CCArray* data, CCSize size, unsigned rowNum, unsigned colNum, const unsigned curIdx,
                                                 const ccColor4B& color /*= cocos2d::ccc4(0,0,0,0)*/)
{
    STPageItemGameLabel* pRet = new STPageItemGameLabel(names, data, rowNum, colNum);
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

STPageItemGameLabel::STPageItemGameLabel(CCDictionary *names, CCArray* data, unsigned rowNum, unsigned colNum)
: cm_RowNum(rowNum), cm_ColumnNum(colNum), p_a_Data(data), p_AllNames(names)
{
    p_AllLabels = new CCArray();
}

STPageItemGameLabel::~STPageItemGameLabel()
{
    CC_SAFE_DELETE(p_AllLabels);
}

bool STPageItemGameLabel::initWithColor(const ccColor4B& color, CCSize size, const unsigned curIdx)
{
    bool bRet = false;
    
    do
    {
        // super class method
        CC_BREAK_IF(! STPage::initWithColor(color, size.width, size.height));
        
        m_LableSize = CCSizeMake(size.width / cm_ColumnNum, size.height / cm_RowNum);
        
        for (int i = cm_RowNum-1; i >=0; i--)
        {
            for (int j = 0; j < cm_ColumnNum; j++)
            {
                STGameUILabelTTF* ttf = STGameUILabelTTF::create("", CCSizeZero);
                if (!ttf)
                    return false;
                
                ttf->setPosition(ccp(m_LableSize.width * j + m_LableSize.width * 0.5f,
                                     ((i & 1) == 1 ? m_LableSize.height : 0) + m_LableSize.height * 0.5f));
                /*
                // init destination point
                ttf->setDestinationPoint(ttf->getPosition());
                ttf->setTag(index);
                */
                 
                pBaseLayer->addChild(ttf, 100);
                
                p_AllLabels->addObject(ttf);
            }
        }
        
        this->refreshPage(curIdx);
        
        bRet = true;
    } while (0);
    
    return bRet;
    
}

void STPageItemGameLabel::refreshPage(unsigned idx)
{
    const unsigned baseIdx = idx * cm_RowNum * cm_ColumnNum;
    unsigned offset = 0;
    
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(p_AllLabels, pObj)
    {
        const unsigned dataIdx = baseIdx + offset;
        offset++;
        
        STGameUILabelTTF* ttf = dynamic_cast<STGameUILabelTTF*>(pObj);
        if (!ttf)
            continue;
        
        if (dataIdx >= p_a_Data->count())
        {
            ttf->setVisible(false);
            continue;
        }
        
        ttf->setString("");
        ttf->setVisible(true);
        
        
        CCString * idName = dynamic_cast<CCString*>(p_a_Data->objectAtIndex(dataIdx));
        if (!idName)
            continue;
        
        const CCString* realName = p_AllNames->valueForKey(idName->m_sString);
        
        ttf->setString(realName->getCString());
        ttf->setUserObject(idName);
        
        // current cell item is the one skill bonus wanted.
        if (dataIdx == GameScene::s_uSkillBonusIndex)
        {
            ttf->setColor(TextSpecialColor);
        }
        else
        {
            // normal color
            ttf->setColor(ccWHITE);
        }
        
    }
    
}

