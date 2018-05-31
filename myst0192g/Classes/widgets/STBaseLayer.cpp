//
//  STBaseLayer.cpp
//  CHUPS003
//
//  Created by Steven.Xc.Tian on 13-10-31.
//
//

#include "STBaseLayer.h"

USING_NS_CC;


STBaseLayer::STBaseLayer() : mp_ScaledUILayer(NULL), mp_ScaledBatchLayer(NULL)
{
}

STBaseLayer::~STBaseLayer()
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
//    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}