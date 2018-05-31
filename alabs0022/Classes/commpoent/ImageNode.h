//
//  ImageNode.hpp
//  ColorGarden
//
//  Created by lidong on 15/11/23.
//
//

#ifndef ImageNode_hpp
#define ImageNode_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class ImageNode : public Node {
    
    
protected:
    ImageNode();
    ~ImageNode();
    
    std::string _imageName;
    int _index;
    
    Sprite* _disSprite;
    Sprite* _newSprite;
    
    bool _isShowingNew;
    
public:
    
    void onEnter();
    void onExit();
    

    bool init(std::string imagename,int index);

    static ImageNode* create(std::string imagename,int index);
    void display();
    
    void loadTextureFinish(Ref* pSender);
    void  setIsShowingNew(bool pBool);
};


#endif /* ImageNode_hpp */
