#ifndef __BoundaryFillNode__
#define __BoundaryFillNode__

#include <stdio.h>
#include <string>
#include <cocos2d.h>
#include "ColorFillNode.h"
using namespace cocos2d;
using namespace std;

typedef struct _DotColorData
{
    Vec2 position;
    Color4B color;
}DotColorData;

class BoundaryFillNode : public ColorFillNode {
public:
    struct DIRECTION{
        int x_offset;
        int y_offset;
    };
    
public:
    std::vector<DotColorData> _dotColorDatas;
    Vec2 _startPoint;
    bool stopFillColor;
public:
    BoundaryFillNode();
    ~BoundaryFillNode();
    static BoundaryFillNode *create(string pLineFrameFile,string pLineArtFile);
    virtual bool init(string pLineArtFile, string pLineFrameFile);
    void undo();
    inline Color4B getPixelColor(int pX, int pY);
    inline void setPixelColor(int pX, int pY, Color4B pColor);
    inline bool checkBounds(int pX, int pY);
    void boundarySeedFill(const int &x, const int &y, const Color4B &new_color, const Color4B &boundary_color);
    
    void ScanLineSeedFill(const int &x, const int &y, const Color4B &new_color, const Color4B &boundary_color);
    void SearchLineNewSeed(std::stack<Point>& stk, int xLeft, int xRight,int y, Color4B new_color, Color4B boundary_color);
    int fillLineRight(int pX, int pY, Color4B pNewColor, Color4B pBoundaryColor);
    int fillLineLeft(int pX, int pY, Color4B pNewColor, Color4B pBoundaryColor);
    inline bool IsPixelValid(int pXt, int pY, Color4B pNewColor, Color4B pBoundaryColor);
    inline int SkipInvalidInLine(int pXt, int pY, int pRight, Color4B pNewColor, Color4B pBoundaryColor);
    const unsigned char * getImageData();
    long int getImageLength();
    Image* getLineImage();
    bool checkFilled(){return _filled;}
protected:
    static DIRECTION direction_8[];
    Image *_lineImage;
    unsigned char *_imageData;
    int _imageWidth;
    int _imageHeight;
    long int _imageLength;
    bool _filled;
    
};

#endif /* defined(__BoundaryFillNode__) */
