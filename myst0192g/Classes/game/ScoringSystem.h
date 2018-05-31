//
//  ScoringSystem.h
//  proj.ios
//
//  Created by Steven.Xc.Tian on 13-6-14.
//
//

#ifndef __proj_ios__ScoringSystem__
#define __proj_ios__ScoringSystem__

#include "../AppGlobal.h"

class ScoringSystem : public cocos2d::CCObject
{
    // and the final score of current level
    int finalScore;
    // game limited time
    int lifetime;
    // hint recharge time
    float rechargeTime;
    
public:
    ScoringSystem();
    ~ScoringSystem();
    
    static ScoringSystem* create();
    
    /**
     * @brief calculate hit bonus
     *
     * @param hit times
     */
    void hitBonus(int times);
    /**
     * @brief calculate skill bonus
     */
    void skillBonus();
    /**
     * @brief calculate time bonus
     */
    void timeBonus();
    
    /**
     * @brief get the rest time of current level
     */
    int getLifeTime();
    
    std::string getLifeTimeString();
    
    /**
     * @brief get the final score of current level
     */
    int getFinalScore();
    
    std::string getFinalScoreString();
    
    /**
     * @brief count down game lifetime
     *
     * @return false if game time is NOT run out, true means time up, game over. 
     */
    bool updateLifetime();
    
    /**
     * @brief get hint recharge time
     *
     * @return when time less than 0, disable hint function.
     */
    float getRechargeTime();
    
private:
    // in Blitz mode, user player level number
    unsigned playedNumber;
    
    // to restart game in Blitz mode, we need save data when level is started
    unsigned preLevelIndex;
};

#endif /* defined(__proj_ios__ScoringSystem__) */
