//
//  Util.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 4/2/15.
//
//

#include "ScribbleUtil.h"

//namespace Scribble {
    Color4B ScribbleUtil::getColor(Image *pImage, Vec2 pPosition){
        unsigned char *data = pImage->getData();
        const int bytesPerPixel = pImage->getBitPerPixel() / 8;
        
        int row = pImage->getHeight() - pPosition.y;
        int col = pPosition.x;
        if (row < 0) {
            row = 0;
        }
        else if(row >= pImage->getHeight()){
            row = pImage->getHeight() - 1;
        }
        if (col < 0) {
            col = 0;
        }
        else if(col >= pImage->getWidth()){
            col = pImage->getWidth() - 1;
        }
        int index = row * pImage->getWidth() + col;
        index *= bytesPerPixel;
        unsigned char r = data[index];
        unsigned char g = data[index + 1];
        unsigned char b = data[index + 2];
        unsigned char a = (3 == bytesPerPixel) ? 255 : data[index + 3];
//        Debug.log("color: %d %d %d %d", r, g, b, a);
        
        return Color4B(r, g, b, a);
    }

Color4B ScribbleUtil::getColor2(Image *pImage,unsigned char* pImageData, Vec2 pPosition){
    unsigned char *data = pImageData;
    const int bytesPerPixel = pImage->getBitPerPixel() / 8;
    
    int row = pImage->getHeight() - pPosition.y;
    int col = pPosition.x;
    if (row < 0) {
        row = 0;
    }
    else if(row >= pImage->getHeight()){
        row = pImage->getHeight() - 1;
    }
    if (col < 0) {
        col = 0;
    }
    else if(col >= pImage->getWidth()){
        col = pImage->getWidth() - 1;
    }
    int index = row * pImage->getWidth() + col;
    index *= bytesPerPixel;
    unsigned char r = data[index];
    unsigned char g = data[index + 1];
    unsigned char b = data[index + 2];
    unsigned char a = (3 == bytesPerPixel) ? 255 : data[index + 3];
    //        Debug.log("color: %d %d %d %d", r, g, b, a);
    
    return Color4B(r, g, b, a);
}

bool ScribbleUtil::supportGLExtension(const char* name) {
    log("hasExtension start");
    const char* extensions = (const char*)glGetString(GL_EXTENSIONS);
    const char* end = extensions + strlen(extensions);
    size_t name_length = strlen(name);
    log("extensions %s", extensions);
    while( extensions < end ) {
        if (strchr(extensions, ' ') <= 0) {
            break;
        }
        size_t position = strchr(extensions, ' ') - extensions;
        log("position %ld", position);
        if( position == name_length &&
           strncmp(extensions, name, position) == 0 )
            return true;
        extensions += (position + 1);
    }
    return false;
}
//}
