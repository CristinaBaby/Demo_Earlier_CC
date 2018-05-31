//
//  MapHelper.h
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#ifndef __HIDD__MapHelper__
#define __HIDD__MapHelper__

#include <stdio.h>

#include "../define/GlobalMacro.h"

USING_NS_CC;
using namespace std;

class MapHelper {

public:
    static std::map<string,name_vec> getNameVec(const char* pMapPath);
    static void printPoint(Vec2 pPoint);

};

#endif /* defined(__HIDD__MapHelper__) */
