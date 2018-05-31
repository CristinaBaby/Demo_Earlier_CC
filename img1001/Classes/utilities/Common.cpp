//
//  Common.cpp
//  antm2
//
//  Created by lidong on 14-4-29.
//
//

#include "Common.h"
#include "cocos2d.h"


#define kFisrtLoadGameTime "FisrtLoadGameTime"
#define kStatisticsChallengeNum "StatisticsChallengeNum"

int Common::getTimeDistance(time_t startTime)
{
    time_t now;
    time(&now);
    int distance = (int)difftime(now, startTime);
    return distance;
}

int Common::getCurrentTime()
{
    time_t now;
    time(&now);
    return now;
}

void Common::saveInitLoadGameTime()
{
    int lTime=Common::getCurrentTime();
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(kFisrtLoadGameTime, lTime);
    cocos2d::CCUserDefault::purgeSharedUserDefault();
}

int Common::getInitLoadGameTime()
{
    int lTime=cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(kFisrtLoadGameTime);
    return lTime;
}

void Common::addChallengeNum()
{
    int lRes= cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(kStatisticsChallengeNum);
    lRes++;
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(kStatisticsChallengeNum, lRes);
    cocos2d::CCUserDefault::purgeSharedUserDefault();
}

int Common::getStatisticsChallengeNum()
{
    int lRes= cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(kStatisticsChallengeNum);
    return lRes;
}

