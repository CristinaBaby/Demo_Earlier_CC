//
//  MapSprite.h
//  HH002
//
//  Created by houhuang on 15/8/27.
//
//

#ifndef __HH002__MapSprite__
#define __HH002__MapSprite__

#include <stdio.h>
#include "cocos2d.h"
#include "../define/Globalmacro.h"
USING_NS_CC;
using namespace std;

class MapSprite : public Node
{
public:
    bool init(int index, const string& imageName);
//    CREATE_FUNC(MapSprite);
    static MapSprite* create(int index, const string& imageName);
    
public:
    string getIamgeName();
    void setImageName(int index, const string& imageName);
    st_property(int, _index, Index);
    
protected:
    string m_sMapSpriteName;
};

#endif /* defined(__HH002__MapSprite__) */
