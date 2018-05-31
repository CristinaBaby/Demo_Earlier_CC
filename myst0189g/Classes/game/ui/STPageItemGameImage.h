//
//  STPageItemGameImage.h
//  LVUP004
//
//  Created by Steven.Xc.Tian on 13-11-18.
//
//

#ifndef __LVUP004__STPageItemGameImage__
#define __LVUP004__STPageItemGameImage__

#include "../../widgets/STPage.h"

class STPageItemGameImage : public STPage
{
    const unsigned              cm_RowNum;
    const unsigned              cm_ColumnNum;
    cocos2d::CCSize             m_LableSize;
    
    cocos2d::CCArray            *p_a_SpriteFrameData;
    cocos2d::CCArray            *p_AllImages;
    
public:
    /**
     *  @param data can only hold CCSpriteFrame pointer or its subclass object.
     */
    static STPageItemGameImage* create(cocos2d::CCArray* data, cocos2d::CCSize size, unsigned rowNum, unsigned colNum, unsigned curIdx,
                                       const cocos2d::ccColor4B& color = cocos2d::ccc4(0,0,0,0));
    
    STPageItemGameImage(cocos2d::CCArray* data, unsigned rowNum, unsigned colNum);
    virtual ~STPageItemGameImage();
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color, cocos2d::CCSize size, const unsigned curIdx);
    
    virtual void refreshPage(unsigned idx);
};

#endif /* defined(__LVUP004__STPageItemGameImage__) */
