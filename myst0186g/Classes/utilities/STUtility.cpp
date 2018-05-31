//
//  STUtility.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-27.
//
//

#include "STUtility.h"

USING_NS_CC;
USING_NS_ST;

#define HOURS_TO_SECONDS    (60 * 60)

void STUtility::shuffleArray(CCArray* src)
{
    // ensure the result is not same as before
    srand(fabs(clock()));
    
    const int size = src->count() / 2;
    // create a random squence
    for(int i = 0; i < size; i++)
    {
        float r1 = CCRANDOM_0_1();
        float r2 = CCRANDOM_0_1();
        
        if (r1 == 1) // to prevent from accessing stAvailableLevels->count() * r1, out of range.
        {
            r1 = 0;
        }
        if (r2 == 1)
        {
            r2 = 0;
        }
        
        if (r1 != r2)
        {
            src->exchangeObjectAtIndex((int)(size * r1), (int)(size * r2));
        }
    }
}

unsigned STUtility::getRandom(unsigned limit)
{
    float r = CCRANDOM_0_1();
    if (r == 1)
    {
        r = 0;
    }
    
    return (unsigned)(limit * r);
}

bool STUtility::isOddNumber(int n)
{
    return (n & 1) == 1;
}


bool STUtility::parseBoolean(const std::string &value)
{
    if (value.length() == 0)
    {
        return false;
    }
    
    if (0 == strcmp(value.c_str(), "0") || 0 == strcmp(value.c_str(), "false") || 0 == strcmp(value.c_str(), "FALSE"))
    {
        return false;
    }
    return true;
}


long STUtility::getCurrentTimeSeconds()
{
    time_t  now;
    time(&now);
    return  now;
}

long STUtility::getHoursByTimeInterval(long before, long now)
{
    long interval = now - before;
    
    long during = interval / HOURS_TO_SECONDS;
    
    return during;
}


