//
//  AudioHelper.h
//  HIDD
//
//  Created by lidong on 15-4-8.
//
//

#ifndef __HIDD__AudioHelper__
#define __HIDD__AudioHelper__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class AudioHelper  {
    
    std::string _currentBackMusicStr;
    
public:
    static AudioHelper* getInstance();
    static void setup(float pSoundValue,float pMusicValue);
    static void setSoundValue(float pSoundValue);
    static void setMusicValue(float pMusicValue);
    static float getSoundValue();
    static float getMusicValue();
    static void reset(float pSoundValue,float pMusicValue);
public:
    void playBackGroundMusic(const char* music, bool bLoop = true);
    void stopBackGroundMusic();
    void playSound(const char* sound, bool bLoop = false);
};

#endif /* defined(__HIDD__AudioHelper__) */
