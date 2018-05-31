//
//  MapHelper.cpp
//  HIDD
//
//  Created by lidong on 15-3-13.
//
//

#include "MapHelper.h"
#include "../define/ResoureMacro.h"
std::map<string,name_vec> MapHelper::getNameVec(const char *pMapPath)
{
    TMXTiledMap* lTmxMap=TMXTiledMap::create(pMapPath);
    check_assert(lTmxMap);
    
    std::map<string,name_vec> lNameVecMaps;
    
    Vector<TMXObjectGroup*> lGroupObjects = lTmxMap->getObjectGroups();
    Vector<TMXObjectGroup*>::iterator lMapGroupIter;
    TMXObjectGroup* lBgObject=lTmxMap->getObjectGroup(st_bg_objects_name);
    for (lMapGroupIter=lGroupObjects.begin(); lMapGroupIter!=lGroupObjects.end(); lMapGroupIter++) {
        TMXObjectGroup* lObject = *lMapGroupIter;
        if(lObject==lBgObject) continue;
        
        ValueVector lValueVector = lObject->getObjects();
        ValueVector::iterator iter;
        for (iter=lValueVector.begin(); iter!=lValueVector.end(); iter++) {
            Value lValue=*iter;
            ValueMap lValueMap = lValue.asValueMap();
            
            float lX=lValueMap["x"].asFloat();
            float lY=lValueMap["y"].asFloat();
            float lWidth=lValueMap["ResWidth"].asFloat();
            float lHeight=lValueMap["ResHeight"].asFloat();
            
            lX=lX+lWidth/2;
            lY=lY+lHeight/2;
            
            string lImageName=lValueMap["ID"].asString();
            lImageName=lImageName+".png";
            
            //CCLOG("ImageName=%s",lImageName.c_str());
            
            name_vec lNameVec;
            lNameVec.position=Vec2(lX,lY);
            lNameVec.size=Size(lWidth, lHeight);
            lNameVec.imagename=lImageName;
            
            lNameVecMaps.insert(pair<string,name_vec>(lImageName,lNameVec));
        }

    }
    
    return lNameVecMaps;
    
}

void MapHelper::printPoint(cocos2d::Vec2 pPoint)
{
    CCLOG("x=%f y=%f",pPoint.x,pPoint.y);
}
