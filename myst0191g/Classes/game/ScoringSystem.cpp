//
//  ScoringSystem.cpp
//  proj.ios
//
//  Created by Steven.Xc.Tian on 13-6-14.
//
//

#include "ScoringSystem.h"

using std::string;

ScoringSystem* ScoringSystem::create()
{
    ScoringSystem* pRet = new ScoringSystem();
    pRet->autorelease();
    return pRet;
}

void ScoringSystem::hitBonus(int times)
{
    if (times <= 0) {
        return;
    }
    
    finalScore += (500 + 250 * (times - 1));
}

void ScoringSystem::skillBonus()
{
    finalScore += 1500;
}

void ScoringSystem::timeBonus()
{
    if (lifetime > 20)
    {
        finalScore += (75 * (lifetime - 20));
    }
}

int ScoringSystem::getFinalScore()
{
    return finalScore;
}

std::string ScoringSystem::getFinalScoreString()
{
    return cocos2d::CCString::createWithFormat("%d", finalScore)->m_sString;
}

int ScoringSystem::getLifeTime()
{
    if (lifetime < 0)
        lifetime = 0;
    
    return lifetime;
}

string ScoringSystem::getLifeTimeString()
{
    char minuteStr[3] = {0};
    char secondStr[3] = {0};
    
    const int interval = this->getLifeTime();
    
    int minute = interval / 60;
    int second = interval - minute * 60;
    
    sprintf(minuteStr, "%02d", minute);
    sprintf(secondStr, "%02d", second);
    
    string result(minuteStr);
    result.append(":").append(secondStr);
    
    return result;
}

bool ScoringSystem::updateLifetime()
{
    return (--lifetime) <= 0;
}

float ScoringSystem::getRechargeTime()
{
    return rechargeTime;
}

ScoringSystem::ScoringSystem()
: finalScore(0), playedNumber(1), preLevelIndex(0)
{
    lifetime = 120;
    rechargeTime = 30;
}

ScoringSystem::~ScoringSystem()
{
}