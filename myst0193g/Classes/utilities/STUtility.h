//
//  STUtility.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-27.
//
//

#ifndef __LVUP002__STUtility__
#define __LVUP002__STUtility__

#include <cocos2d.h>
#include "STMacro.h"

NS_ST_BEGIN

class STUtility
{
    STUtility();
    
public:
    /**
     * @brief adjust array elments' sequence
     */
    static void shuffleArray(cocos2d::CCArray* src);
    
    /**
     * @brief get random number between [0, limit)
     */
    static unsigned getRandom(unsigned limit);
    
    /**
     * @return true is odd number ,other is false
     */
    inline static bool isOddNumber(int n);
    
    static bool parseBoolean(const std::string &value);
    
    static long getCurrentTimeSeconds();
    
    static long getHoursByTimeInterval(long before, long now);
};

NS_ST_END

#endif /* defined(__LVUP002__STUtility__) */
