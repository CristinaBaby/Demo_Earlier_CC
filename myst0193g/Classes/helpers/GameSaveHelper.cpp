//
//  GameSaveHelper.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-10-21.
//
//

#include "GameSaveHelper.h"

USING_NS_CC;
using std::string;

GameSaveHelper* s_pGameSaveHelper = NULL;


GameSaveHelper::GameSaveHelper(CCArray* leftObjs, CCArray* removedObjs, SearchMode sm,
                               string levelId, string levelResRoot,
                               ScoringSystem* score, float hintPercent, unsigned levelIndex)

: mSearchMode(sm), mLevelId(levelId), mLevelResRoot(levelResRoot),
 mHintPercent(hintPercent), mLevelIndex(levelIndex)
{
    m_pRemainedObjects = CCArray::createWithArray(leftObjs);
    m_pRemainedObjects->retain();
    
    m_pRemovedObjects = CCArray::createWithArray(removedObjs);
    m_pRemovedObjects->retain();
    
    m_pScore = score;
    m_pScore->retain();
}

GameSaveHelper::~GameSaveHelper()
{
    CC_SAFE_DELETE(m_pRemovedObjects);
    CC_SAFE_DELETE(m_pRemainedObjects);
    CC_SAFE_RELEASE(m_pScore);
}