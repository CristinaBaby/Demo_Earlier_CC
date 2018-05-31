//
//  STUtility.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-27.
//
//

#include "STUtility.h"

USING_NS_CC;


#define HOURS_TO_SECONDS    (60 * 60)

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


