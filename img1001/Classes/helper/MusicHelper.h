//
//  MusicHelper.hpp
//  IMG1001
//
//  Created by houhuang on 15/10/14.
//
//

#ifndef MusicHelper_h
#define MusicHelper_h

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class MusicHelper
{
public:
    static MusicHelper* getInstance();
    MusicHelper();

public:
    void playMultipleMusic_loop();
    void pauseMusic();
    void resumeMusic();

    static bool m_bIsPlayBackgroudMusic;
};


#endif /* MusicHelper_hpp */
