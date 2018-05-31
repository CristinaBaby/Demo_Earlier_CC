//
//  STBatchNodeLayer.h
//  CHUPS003
//
//  Created by Steven.Xc.Tian on 13-11-4.
//
//

#ifndef __CHUPS003__STBatchNodeLayer__
#define __CHUPS003__STBatchNodeLayer__

#include <cocos2d.h>

class STBatchNodeLayer : public cocos2d::CCLayerColor
{
protected:
    cocos2d::CCSpriteBatchNode *m_p_BatchNode;
    
public:
    static STBatchNodeLayer* create(std::string fileImage);
    static STBatchNodeLayer* create(std::string fileImage, const cocos2d::ccColor4B& color);
    static STBatchNodeLayer* create(std::string fileImage, unsigned capacity);
    
    static STBatchNodeLayer* create(cocos2d::CCTexture2D* tex);
    static STBatchNodeLayer* create(cocos2d::CCTexture2D* tex, const cocos2d::ccColor4B& color);
    static STBatchNodeLayer* create(cocos2d::CCTexture2D* tex, unsigned capacity);
    
    virtual bool initWithColor(const cocos2d::ccColor4B& color, GLfloat width, GLfloat height);
    
    STBatchNodeLayer(std::string fileImage, unsigned capacity);
    STBatchNodeLayer(std::string fileImage);
    STBatchNodeLayer(cocos2d::CCTexture2D* tex, unsigned capacity);
    STBatchNodeLayer(cocos2d::CCTexture2D* tex);
    
    virtual ~STBatchNodeLayer(){}
    
public:
    virtual void addChildToBatchNode(CCNode * child);
    virtual void addChildToBatchNode(CCNode * child, int zOrder);
    virtual void addChildToBatchNode(CCNode* child, int zOrder, int tag);
    
};

#endif /* defined(__CHUPS003__STBatchNodeLayer__) */
