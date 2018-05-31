//
//  ColoringTouchNode.cpp
//  DrawDemo
//
//  Created by renhong on 9/15/14.
//
//

#include "ColoringTouchNode.h"

ColoringTouchNode *ColoringTouchNode::create(string pTemplateFile, string pLineArtFile){
    ColoringTouchNode *ret = new ColoringTouchNode();
    if(ret && ret->init(pTemplateFile, pLineArtFile)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ColoringTouchNode::init(string pTemplateFile, string pLineArtFile){
    if (!ColoringNode::init(pTemplateFile, pLineArtFile)) {
        return false;
    }
    //响应点击事件：根据点击位置，够造相应着色参数给着色器。
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->bool{
        unsigned char *data = _templateImage->getData();
        const int bytesPerPixel = _templateImage->getBitPerPixel() / 8;
        Point pos = this->convertToNodeSpace(pTouch->getLocation());
        int row = _templateImage->getHeight() - pos.y;
        int col = pos.x;
        int index = row * _templateImage->getWidth() + col;
        index *= bytesPerPixel;
        unsigned char r = data[index];
        unsigned char g = data[index + 1];
        //        unsigned char b = data[index + 2];
        //        unsigned char a = (3== bytesPerPixel) ? 255 : data[index + 3];
        
        const int texSize = gPixelSize;
        const int texLenth = texSize * texSize * 4;
        
        unsigned char tmp[texLenth];
        for (int i = 0; i < texSize * texSize; ++i) {
            tmp[i * 4 + 0] = _startColor.r;
            tmp[i * 4 + 1] = _startColor.g;
            tmp[i * 4 + 2] = _startColor.b;
            tmp[i * 4 + 3] = _startColor.a;
        }
        _colorTexture->updateWithData(tmp, r - texSize / 2, g - texSize / 2, texSize, texSize);
        
        if (ColoringMode::eFlat != _coloringMode) {
            for (int i = 0; i < texSize * texSize; ++i) {
                tmp[i * 4 + 0] = _endColor.r;
                tmp[i * 4 + 1] = _endColor.g;
                tmp[i * 4 + 2] = _endColor.b;
                tmp[i * 4 + 3] = _endColor.a;
            }
        }
        _endColorTexture->updateWithData(tmp, r - texSize / 2, g - texSize / 2, texSize, texSize);
        
        int hSmooth = 0;
        int vSmooth = 0;
        switch (_coloringMode) {
            case ColoringMode::eHorizonSmooth:{
                hSmooth = 1;
                break;
            }
            case ColoringMode::eVerticalSmooth:{
                vSmooth = 1;
                break;
            }
            case ColoringMode::eDiagonalSmooth:{
                hSmooth = 1;
                vSmooth = 1;
                break;
            }
            default:
                break;
        }
        memset(tmp, 0, sizeof(texLenth));
        for (int i = 0; i < texSize * texSize; ++i) {
            tmp[i * 4 + 0] = hSmooth * 255;
            tmp[i * 4 + 1] = vSmooth * 255;
        }
        _colorModelTexture->updateWithData(tmp, r - texSize / 2, g - texSize / 2, texSize, texSize);
        
        _canvas->clear(1, 1, 1, 1);
        _canvas->begin();
        Texture2D::TexParams tp;
        tp.magFilter = GL_NEAREST;
        tp.minFilter = GL_NEAREST;
        tp.wrapS = GL_CLAMP_TO_EDGE;
        tp.wrapT = GL_CLAMP_TO_EDGE;
        _template->getTexture()->setTexParameters(tp);
        _template->visit();
        if (nullptr != _lineArt) {
            _lineArt->visit();
        }
        _canvas->end();
        Director::getInstance()->getRenderer()->render();
        
        return true;
    };
    listener->onTouchMoved = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    listener->onTouchEnded = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}