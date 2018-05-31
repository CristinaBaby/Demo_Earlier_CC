//
//  Common.h
//  antm2
//
//  Created by lidong on 14-4-29.
//
//

#ifndef __antm2__Common__
#define __antm2__Common__

#include <iostream>

class Common {
    
    
public:
    static int getTimeDistance(time_t startTime);
    static int getCurrentTime();
    static void saveInitLoadGameTime();
    static int getInitLoadGameTime();
    static void addChallengeNum();
    static int getStatisticsChallengeNum();
    
};


#endif /* defined(__antm2__Common__) */
