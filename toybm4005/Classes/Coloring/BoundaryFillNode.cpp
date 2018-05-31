#include "BoundaryFillNode.h"
#include <string>
#include "ScribbleUtil.h"
#include "../game/DataManager.h"
#include "../game/GameLayer.h"
#include "cocos2d.h"
#include "../helper/ScreenAdapterHelper.h"
#include "../helper/DeviceHelper.h"
using namespace cocos2d;
using namespace std;
BoundaryFillNode::DIRECTION BoundaryFillNode::direction_8[] = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
const float gMaxAlpha = 256;//200;//
const float gMinAlpha = 200;

BoundaryFillNode *BoundaryFillNode::create(string pLineArtFile, string pLineFrameFile){
    BoundaryFillNode *ret = new BoundaryFillNode();
    if(ret && ret->init(pLineArtFile, pLineFrameFile)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

BoundaryFillNode::BoundaryFillNode(){
    _lineImage = nullptr;
    _imageData = nullptr;
//    _shapeData = nullptr;
    _filled = false;
}

BoundaryFillNode::~BoundaryFillNode(){
    _lineImage->release();
    delete [] _imageData;
//    delete [] _shapeData;
}

Color4B BoundaryFillNode::getPixelColor(int pX, int pY){
    unsigned char *data = _imageData;//_lineImage->getData();
    const int bytesPerPixel = 4;//_lineImage->getBitPerPixel() / 8;
    
    int index = pY * _imageWidth + pX;//_lineImage->getWidth()
    index *= bytesPerPixel;
    unsigned char r = data[index];
    unsigned char g = data[index + 1];
    unsigned char b = data[index + 2];
    unsigned char a = data[index + 3];
    
//    CCLOG("getPixelColor %d %d %d %d at %d %d", r, g, b, a , pX, pY);
    return Color4B(r, g, b, a);
}

void BoundaryFillNode::setPixelColor(int pX, int pY, Color4B pColor){
    unsigned char *data = _imageData;//_lineImage->getData();
    const int bytesPerPixel = 4;//_lineImage->getBitPerPixel() / 8;
    int index = pY * _imageWidth + pX;//_lineImage->getWidth()
    index *= bytesPerPixel;
    data[index] = pColor.r;
    data[index + 1] = pColor.g;
    data[index + 2] = pColor.b;
    data[index + 3] = pColor.a;
    
}

bool BoundaryFillNode::checkBounds(int pX, int pY){
    Rect lBounds(0, 0, _imageWidth - 1, _imageHeight - 1);
    return lBounds.containsPoint(Point(pX, pY));
}

void BoundaryFillNode::boundarySeedFill(const int &x, const int &y, const Color4B &new_color, const Color4B &boundary_color){
    if (checkBounds(x, y)) {
        Color4B curColor = getPixelColor(x, y);
        if( curColor.a >= gMinAlpha && (curColor != boundary_color) && (curColor != new_color)){
            setPixelColor(x, y, new_color);
            for(int i = 0; i < 8; i++){
                boundarySeedFill(x + direction_8[i].x_offset, y + direction_8[i].y_offset, new_color,boundary_color);
            }
        }
    }
}

bool BoundaryFillNode::IsPixelValid(int pXt, int pY, Color4B pNewColor, Color4B pBoundaryColor){
    if (checkBounds(pXt, pY)) {
        Color4B curColor = getPixelColor(pXt, pY);
        if( curColor.a >= gMinAlpha && (curColor != pBoundaryColor) && (curColor != pNewColor)){
            return true;
        }
    }
    
    return false;
}

int BoundaryFillNode::SkipInvalidInLine(int pXt, int pY, int pRight, Color4B pNewColor, Color4B pBoundaryColor){
    return 0;
}

void BoundaryFillNode::SearchLineNewSeed(std::stack<Point>& stk, int xLeft, int xRight,int y, Color4B new_color, Color4B boundary_color){
    int xt = xLeft;
    bool findNewSeed = false;
    
    while(xt <= xRight){
        findNewSeed = false;
        while(IsPixelValid(xt, y, new_color, boundary_color) && (xt < xRight)){
            findNewSeed = true;
            xt++;
        }
        if(findNewSeed){
            if(IsPixelValid(xt, y, new_color, boundary_color) && (xt == xRight))
                stk.push(Point(xt, y));
            else
                stk.push(Point(xt - 1, y));
        }
        
        /*向右跳过内部的无效点（处理区间右端有障碍点的情况）*/
        int xspan = SkipInvalidInLine(xt, y, xRight, new_color, boundary_color);
        xt += (xspan == 0) ? 1 : xspan;
        /*处理特殊情况,以退出while(x<=xright)循环*/
    }
}

void BoundaryFillNode::ScanLineSeedFill(const int &x, const int &y, const Color4B &new_color, const Color4B &boundary_color){
    std::stack<Point> stk;
    stk.push(Point(x, y)); //第1步，种子点入站
    while(!stk.empty()){
        Point seed = stk.top(); //第2步，取当前种子点
        stk.pop();
        //第3步，向左右填充
        int count = fillLineRight(seed.x, seed.y, new_color, boundary_color);//向'cf?右'd3?填'cc?充'b3?
        int xRight = seed.x + count - 1;
        count = fillLineLeft(seed.x - 1, seed.y, new_color, boundary_color);//向'cf?左'd7?填'cc?充'b3?
        int xLeft = seed.x - count;
        
        //第4步，处理相邻两条扫描线
        SearchLineNewSeed(stk, xLeft, xRight, seed.y - 1, new_color, boundary_color);
        SearchLineNewSeed(stk, xLeft, xRight, seed.y + 1, new_color, boundary_color);
    }
}

int BoundaryFillNode::fillLineRight(int pX, int pY, Color4B pNewColor, Color4B pBoundaryColor){
    int lX = pX;
    int lY = pY;
    while (true) {
        if (checkBounds(lX, lY)) {
            Color4B curColor = getPixelColor(lX, lY);
            if( curColor.a >= gMinAlpha && (curColor != pBoundaryColor)){
//                if (curColor != pNewColor) {
                    setPixelColor(lX, lY, pNewColor);
                    _filled = true;
//                }
                lX += 1;
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    return lX - pX;
}

int BoundaryFillNode::fillLineLeft(int pX, int pY, Color4B pNewColor, Color4B pBoundaryColor){
    int lX = pX;
    int lY = pY;
    while (true) {
        if (checkBounds(lX, lY)) {
            Color4B curColor = getPixelColor(lX, lY);
            if( curColor.a >= gMinAlpha && (curColor != pBoundaryColor)){
                //                if (curColor != pNewColor) {
                setPixelColor(lX, lY, pNewColor);
                _filled = true;
                //                }
                lX -= 1;
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    return pX - lX;
}

const unsigned char * BoundaryFillNode::getImageData()
{
    return _imageData;
}
long int BoundaryFillNode::getImageLength()
{
    return _imageLength;
}

Image* BoundaryFillNode::getLineImage()
{
    return _lineImage;
}

bool BoundaryFillNode::init(string pLineArtFile, string pLineFrameFile){
    if (!Node::init()) {
        return false;
    }
    
    stopFillColor=false;
    
    _dotColorDatas.clear();

    Sprite *lLineSprite = Sprite::create(pLineArtFile);
    lLineSprite->setPosition(lLineSprite->getContentSize() / 2);//Point(768 / 2.0, 768 / 2.0)
    this->addChild(lLineSprite);
    this->setContentSize(lLineSprite->getContentSize());
    
    _lineArtSprite = lLineSprite;
    _lineImage = new Image();
    _lineImage->retain();
    _lineImage->initWithImageFile(pLineArtFile);
    _lineImage->autorelease();
    
    _imageLength=_lineImage->getDataLen() / 4;
    _imageData = new unsigned char[_lineImage->getDataLen()];
    memcpy(_imageData, _lineImage->getData(), _lineImage->getDataLen());
   
    _imageWidth = _lineImage->getWidth();
    _imageHeight =  _lineImage->getHeight();
    
//    Texture2D *lColorTexture = new Texture2D;
//    lColorTexture->initWithImage(_lineImage);
    lLineSprite = Sprite::create(pLineFrameFile);//createWithTexture(lColorTexture);
    lLineSprite->setPosition(lLineSprite->getContentSize()/2 );//Point(768 / 2.0, 768 / 2.0)
    this->addChild(lLineSprite);
    
    //响应点击事件：根据点击位置，够造相应着色参数给着色器。
    auto listener = EventListenerTouchAllAtOnce::create();//EventListenerTouchOneByOne::create();
    _touchEventListener = listener;
//    listener->setSwallowTouches(true);
    listener->onTouchesBegan = [this](const std::vector<Touch*>&pTouches, Event*pEvent)->void{

       
    };
    listener->onTouchesEnded = [this](const std::vector<Touch*>&pTouches, Event*pEvent)->void{
        
        int *lEnabled = (int *)this->getUserData();
        if (0 == lEnabled) {
            return;
        }
        
        if(DataManager::getInstance()->isAlreadySelectedColor==false || stopFillColor)
        {
            return;
        }
        
        
        
        _filled = false;
        
        Touch *pTouch = pTouches.at(0);
        Point pos = this->convertToNodeSpace(pTouch->getLocation());
        int row = _lineImage->getHeight() - pos.y;
        int col = pos.x;
        
        GameLayer* lGameLayer=(GameLayer*)this->getParent()->getParent();
        
        float border = ScreenAdapterHelper::getValue(350);
        if (DeviceHelper::getInstance()->getCurrentDeviceType() == kType_Device_Phone)
        {
            border = ScreenAdapterHelper::getValue(380);
        }
        
        if(pTouch->getLocation().y<border ||  pTouch->getLocation().y>ScreenAdapterHelper::getRealDesignSize().height-ScreenAdapterHelper::getValue(90))
        {
            return;
        }
        
        //CCLOG("pos x＝%f y=%f",pos.x,pos.y);
        
      //  CCLOG("Touch %d %d Color %d %d %d %d", col, row, _startColor.r, _startColor.g, _startColor.b, _startColor.a);
        
        Color4B lColor = ScribbleUtil::getColor(_lineImage, pos);
        Color4B lOrginColor = ScribbleUtil::getColor2(_lineImage,_imageData, pos);
        
        bool lResBool=true;
        if(lOrginColor.r==lOrginColor.g && lOrginColor.r==lOrginColor.b && lOrginColor.r<255)
        {
            lResBool=false;
        }
        if (lColor.a > 0) {
            
            if(lResBool)
            {
                //        this->boundarySeedFill(col, row, _startColor, Color4B(0, 0, 0, 255));
                this->ScanLineSeedFill(col, row, _startColor, Color4B(0, 0, 0, 255));
                
                
                if(this->checkFilled())
                {
                    CCLOG("r=%d g=%d b=%d",lOrginColor.r,lOrginColor.g,lOrginColor.b);
                    
                    DotColorData lDotColorData;
                    lDotColorData.color=lOrginColor;
                    lDotColorData.position=pos;
                    
                    
                    
                    _dotColorDatas.push_back(lDotColorData);
//                    if(_dotColorDatas.size()>5)
//                    {
//                        std::vector<DotColorData>::iterator iter=_dotColorDatas.begin();
//                        _dotColorDatas.erase(iter);
//                    }
                }
                
                _lineArtSprite->getTexture()->updateWithData(_imageData, 0, 0, _imageWidth, _imageHeight);
                if (nullptr != _delegate) {
                    _delegate->willColoring(this);
                }

            }
            
        }
        else
        {
//            GameLayer* lGameLayer=(GameLayer*)this->getParent()->getParent();
//            if(pTouch->getLocation().y>ScreenAdapterHelper::getValue(270) && pTouch->getLocation().y<ScreenAdapterHelper::getRealDesignSize().height-ScreenAdapterHelper::getValue(120))
            {
                
                DotColorData lDotColorData;
                Color3B mColor=DataManager::getInstance()->getSaveBackColor(lGameLayer->getIndex());
                lDotColorData.color=Color4B(mColor.r,mColor.g,mColor.b,255);
                lDotColorData.position=pos;
                
                _dotColorDatas.push_back(lDotColorData);
                
                lGameLayer->changeBackColor();
                
//                if(_dotColorDatas.size()>5)
//                {
//                    std::vector<DotColorData>::iterator iter=_dotColorDatas.begin();
//                    _dotColorDatas.erase(iter);
//                }

            }
        }

//        return true;
    };
    listener->onTouchesMoved = [](const std::vector<Touch*>&pTouches, Event*pEvent)->void{};
//    listener->onTouchesEnded = [](const std::vector<Touch*>&pTouches, Event*pEvent)->void{};
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    this->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
    return true;
}

void BoundaryFillNode::undo()
{
   if(_dotColorDatas.size()>0)
   {
       DotColorData lDotColorData=_dotColorDatas.at(_dotColorDatas.size()-1);
       Vec2 lPosition=lDotColorData.position;

       Point pos = lPosition;
       int row = _lineImage->getHeight() - pos.y;
       int col = pos.x;

       Color4B lColor = ScribbleUtil::getColor(_lineImage, pos);
       if (lColor.a > 0) {
           
             CCLOG("undo r=%d g=%d b=%d",lDotColorData.color.r,lDotColorData.color.g,lDotColorData.color.b);
           
           //        this->boundarySeedFill(col, row, _startColor, Color4B(0, 0, 0, 255));
           this->ScanLineSeedFill(col, row, lDotColorData.color, Color4B(0, 0, 0, 255));
           
           _lineArtSprite->getTexture()->updateWithData(_imageData, 0, 0, _imageWidth, _imageHeight);
           if (nullptr != _delegate) {
               _delegate->willColoring(this);
           }
       }
       else
       {
           GameLayer* lGameLayer=(GameLayer*)this->getParent()->getParent();
           //if(lPosition.y>ScreenAdapterHelper::getValue(270) && lPosition.y<ScreenAdapterHelper::getRealDesignSize().height-ScreenAdapterHelper::getValue(120))
           {
               lGameLayer->undoBackColor(lDotColorData.color);
           }
       }
       
       _dotColorDatas.erase(_dotColorDatas.end()-1);
       
   }

}

Sprite* BoundaryFillNode::getLineArtSprite()
{
    return _lineArtSprite;
}

