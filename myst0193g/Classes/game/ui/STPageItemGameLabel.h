//
//  STPageItemGameLabel.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-14.
//
//

#ifndef __LVUP002__STPageItemGameLabel__
#define __LVUP002__STPageItemGameLabel__

#include <cocos2d.h>
#include "../../widgets/STPage.h"

/*
class GameLabelItemCell : public ItemCell
{
public:
    GameLabelItemCell(const unsigned idx)
    : ItemCell(idx)
    {}
};
*/

class STPageItemGameLabel : public STPage
{
    const unsigned              cm_RowNum;
    const unsigned              cm_ColumnNum;
    cocos2d::CCSize             m_LableSize;
    
    cocos2d::CCArray            *p_a_Data;
    cocos2d::CCArray            *p_AllLabels;
    cocos2d::CCDictionary       *p_AllNames;
    
public:
    /**
     *  @param data can only hold GalleryItemCell or its subclass object.
     */
    static STPageItemGameLabel* create(cocos2d::CCDictionary* names, cocos2d::CCArray* data, cocos2d::CCSize size, unsigned rowNum, unsigned colNum, unsigned curIdx,
                                       const cocos2d::ccColor4B& color = cocos2d::ccc4(0,0,0,0));
    
    STPageItemGameLabel(cocos2d::CCDictionary* names, cocos2d::CCArray* data, unsigned rowNum, unsigned colNum);
    virtual ~STPageItemGameLabel();
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color, cocos2d::CCSize size, const unsigned curIdx);
    
    virtual void refreshPage(unsigned idx);
};

#endif /* defined(__LVUP002__STPageItemGameLabel__) */
