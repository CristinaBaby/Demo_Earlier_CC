//
//  ColoringNode.cpp
//  DrawDemo
//
//  Created by renhong on 9/15/14.
//
//

#include "ColoringNode.h"

#define kShaderName "Coloring.frag"
#define STRINGIFY(A)  #A
#include kShaderName

ColoringNode *ColoringNode::create(string pTemplateFile, string pLineArtFile){
    ColoringNode *ret = new ColoringNode();
    if(ret && ret->init(pTemplateFile, pLineArtFile)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ColoringNode::ColoringNode() : _coloringMode(ColoringNode::ColoringMode::eFlat){
    _template = nullptr;
    _lineArt = nullptr;
    _canvas = nullptr;
    _colorTexture = nullptr;
    _endColorTexture = nullptr;
    _colorModelTexture = nullptr;
    _templateImage = nullptr;
}

ColoringNode::~ColoringNode(){
    _template->release();
    _lineArt->release();
    _canvas->release();
    _colorTexture->release();
    _endColorTexture->release();
    _colorModelTexture->release();
    _templateImage->release();
    _shapeBoundTexture->release();
}

bool ColoringNode::init(string pTemplateFile, string pLineArtFile){
    if (!Node::init()) {
        return false;
    }
    
    //构建并初始化画布
    _template = Sprite::create(pTemplateFile.c_str());
    _template->retain();
    Size drawSize = _template->getContentSize();
    _template->setPosition(Vec2(drawSize.width/2, drawSize.height/2));
    
    _lineArt = Sprite::create(pLineArtFile);
    _lineArt->retain();
    _lineArt->setPosition(Vec2(drawSize.width / 2, drawSize.height / 2));
    
    _canvas = RenderTexture::create(drawSize.width, drawSize.height);
    _canvas->retain();
    _canvas->setPosition(drawSize.width / 2, drawSize.height / 2);
    this->addChild(_canvas);
    
    _canvas->clear(1, 1, 1, 1);
    _canvas->getSprite()->getTexture()->setAntiAliasTexParameters();
    _canvas->begin();
    _lineArt->visit();
    _canvas->end();
    
    //构造着色参数
    const int texSize = gTextureSize;
    const int texLenth = texSize * texSize * 4;
    unsigned char texData[texLenth];
    memset(texData, 0x0, sizeof(unsigned char) * texLenth);
    _colorTexture = new Texture2D;
    _colorTexture->setAliasTexParameters();
    _colorTexture->initWithData(texData, texLenth, Texture2D::PixelFormat::RGBA8888, texSize, texSize, Size(texSize, texSize));
    
    memset(texData, 0x0, sizeof(unsigned char) * texLenth);
    _endColorTexture = new Texture2D;
    _endColorTexture->setAliasTexParameters();
    _endColorTexture->initWithData(texData, texLenth, Texture2D::PixelFormat::RGBA8888, texSize, texSize, Size(texSize, texSize));
    
    memset(texData, 0xFF, sizeof(unsigned char) * texLenth);
    _colorModelTexture = new Texture2D;
    _colorModelTexture->setAliasTexParameters();
    _colorModelTexture->initWithData(texData, texLenth, Texture2D::PixelFormat::RGBA8888, texSize, texSize, Size(texSize, texSize));

    setBrushShader();
    
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_template->getGLProgram());
    lState->setUniformTexture("s_texture_color", _colorTexture);
    lState->setUniformTexture("s_texture_color_end", _endColorTexture);
    lState->setUniformTexture("s_texture_color_model", _colorModelTexture);
    lState->setUniformTexture("s_texture_shape", _template->getTexture());
    lState->setUniformVec2("v_texture_shape_size", _template->getTexture()->getContentSize());

    _templateImage = new Image();
    _templateImage->initWithImageFile(pTemplateFile.c_str());
    _shapeBoundTexture = this->getShapeBounds(_templateImage);
    _shapeBoundTexture->retain();
    lState->setUniformTexture("s_texture_shape_bounds", _shapeBoundTexture);

    return true;
}

inline static void setShaderPrograms(Node *n, GLProgram *p) {
    n->setGLProgram(p);
    if (n->getChildren().size() <= 0) return;
    Vector<Node *> children = n->getChildren();
    for (Vector<Node *>::iterator itor = children.begin(); itor != children.end(); ++itor) {
        Node *lNode = *itor;
        if (lNode) {
            setShaderPrograms(lNode, p);
        }
    }
}

GLProgram *ColoringNode::loadShader(const GLchar *pShaderString, string pShaderCacheKey){
    GLProgram *p = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, pShaderString);
    CHECK_GL_ERROR_DEBUG();
    if (nullptr != p) {
        ShaderCache::getInstance()->addGLProgram(p, pShaderCacheKey);
    }
    
    return (GLProgram *)p->autorelease();
}

void ColoringNode::setBrushShader(){
    GLProgram *lProgram = this->loadShader(Coloring_frag, kShaderName);
    if (NULL != lProgram) {
        if (_template->getGLProgram() != lProgram) {
            setShaderPrograms(_template, lProgram);
        }
    }
}

Texture2D *ColoringNode::getShapeBounds(Image *pShapeImage){//获取待着色图片内容的边界，记录边界的左下角和右上角坐标。
    unsigned char *imageData = pShapeImage->getData();
    const int height = pShapeImage->getHeight();
    const int width = pShapeImage->getWidth();
    const int bytePerPixel = pShapeImage->getBitPerPixel() / 8;
    map<unsigned int, ColoringArea> colorAreaInfo;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int index = row * width + col;
            index *= bytePerPixel;
            unsigned int color;
            memset(&color, 255, sizeof(unsigned int));//因为有可能是RGB格式，需要要对Alpha通道设置默认值，使用255使图片不透明。
            memcpy(&color, imageData + index, bytePerPixel);
            map<unsigned int, ColoringArea>::iterator itor = colorAreaInfo.find(color);
            if (colorAreaInfo.end() == itor) {
                ColoringArea colorArea(col, col, row, row);
                pair<map<unsigned int, ColoringArea>::iterator, bool> res = colorAreaInfo.insert(pair<unsigned int, ColoringArea>(color, colorArea));
                if (res.second) {
                    itor = res.first;
                }
            }
            ColoringArea *colorArea = &(itor->second);
            if (col < colorArea->left) {
                colorArea->left = col;
            }
            if (col > colorArea->right) {
                colorArea->right = col;
            }
            if (row < colorArea->top) {
                colorArea->top = row;
            }
            if (row > colorArea->bottom) {
                colorArea->bottom = row;
            }
        }
    }
    
    const int texSize = gTextureSize;
    const int texLenth = texSize * texSize * 4;
    unsigned char texData[texLenth];
    memset(texData, 0x0, sizeof(unsigned char) * texLenth);
    Texture2D *colorTexture = new Texture2D;
    colorTexture->setAliasTexParameters();
    colorTexture->initWithData(texData, texLenth, Texture2D::PixelFormat::RGBA8888, texSize, texSize, Size(texSize, texSize));
    for (map<unsigned int, ColoringArea>::iterator itor = colorAreaInfo.begin(); itor != colorAreaInfo.end(); ++itor) {
        ColoringArea colorArea = itor->second;
        unsigned int tmp = itor->first;
//        unsigned char a = tmp >> 24;
//        unsigned char b = (tmp >> 16) & 0x00FF;
        unsigned char g = (tmp >> 8) & 0x00FF;
        unsigned char r = (tmp & 0x00FF);
        
        const int index = g * texSize + r;
        texData[index] = colorArea.left * 255.0 / pShapeImage->getWidth();
        texData[index + 1] = colorArea.right * 255.0 / pShapeImage->getWidth();
        texData[index + 2] = colorArea.top * 255.0 / pShapeImage->getHeight();
        texData[index + 3] = colorArea.bottom * 255.0 / pShapeImage->getHeight();
        
        const int texSize = gPixelSize;
        const int texLenth = texSize * texSize * 4;
        unsigned char tmp1[texLenth];
        for (int i = 0; i < texSize * texSize; ++i) {
            tmp1[i * 4 + 0] = texData[index];
            tmp1[i * 4 + 1] = texData[index + 1];
            tmp1[i * 4 + 2] = texData[index + 2];
            tmp1[i * 4 + 3] = texData[index + 3];
        }
        colorTexture->updateWithData(tmp1, r - texSize / 2, g - texSize / 2, texSize, texSize);
    }
    
//    Texture2D::TexParams texParams = {(GLuint)(GL_LINEAR),GL_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE};
//    colorTexture->setTexParameters(texParams);
    colorTexture->autorelease();
    return colorTexture;
}