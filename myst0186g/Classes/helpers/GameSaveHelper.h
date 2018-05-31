//
//  GameSaveHelper.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-21.
//
//

#ifndef __LVUP002__GameSaveHelper__
#define __LVUP002__GameSaveHelper__

#include <cocos2d.h>
#include "../AppGlobal.h"
#include "../game/ScoringSystem.h"

class GameSaveHelper : public cocos2d::CCObject
{
    // just allow GameScene create this object.
    friend class GameScene;
    
private:
    GameSaveHelper(cocos2d::CCArray* leftObjs, cocos2d::CCArray* removedObjs,
                   SearchMode sm, std::string levelId, std::string levelResRoot,
                   ScoringSystem* score, float hintPercent, unsigned levelIndex);
    
public:
    virtual ~GameSaveHelper();
    
protected:
    cocos2d::CCArray                *m_pRemainedObjects;
    cocos2d::CCArray                *m_pRemovedObjects;
    const SearchMode                mSearchMode;
    const std::string               mLevelId;
    const std::string               mLevelResRoot;
    ScoringSystem                   *m_pScore;
    const float                     mHintPercent;
    const unsigned                  mLevelIndex;
    
};

#endif /* defined(__LVUP002__GameSaveHelper__) */
