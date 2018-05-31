//
//  ImageBox.h
//  ColorGarden
//
//  Created by lidong on 15/8/3.
//
//

#ifndef __ColorGarden__ImageBox__
#define __ColorGarden__ImageBox__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/GlobalMacro.h"
USING_NS_CC;

class ImageBox;
class ImageBoxDelegate {
public:
    virtual void reponseTouchEvent(ImageBox* pLevelBox){}
};

class ImageNode;
class ImageBox : public Node {

protected:
    Sprite* _disSprite;
    Sprite* _rotateSprite;
    bool _isShowingNew;
    
    ImageNode* _imageNode;
    Sprite* m_pRotate;
    
public:
    ImageBox();
    ~ImageBox();
    
    void loadBackColor();
    
    bool init();
    CREATE_FUNC(ImageBox);
    void removeNodes();
    void display();
    string getImageName();
    void responseTouchEvent(Vec2 pPoint);
    void loadTextureFinish(Ref* pSender);
    void removeRotateAniamtion();
    void setIsShowingNew(bool pBool);

    string getUserNewTag();
    
    void downloadingImage();

public:
    void hideNewSprite();
    st_property(int, _index, Index);
    st_property(ImageBoxDelegate*, _delegate, Delegate);
    st_property(Vec2, _priviousPos, PriviousPos);
    st_property(bool, _isMask, IsMask);
};

#endif /* defined(__ColorGarden__ImageBox__) */
