//
//  MusicHelper.cpp
//  IMG1001
//
//  Created by houhuang on 15/10/14.
//
//

#include "MusicHelper.h"
#include "SimpleAudioEngine.h"
#include "../define/Globalmacro.h"
using namespace CocosDenshion;

bool MusicHelper::m_bIsPlayBackgroudMusic = true;

static MusicHelper* _instance = NULL;

MusicHelper* MusicHelper::getInstance()
{
    if (_instance == NULL)
    {
        _instance = new MusicHelper();
    
    }

    return _instance;
}

MusicHelper::MusicHelper()
{
    m_bIsPlayBackgroudMusic = UserDefault::getInstance()->getBoolForKey(st_IsPlayBackgrioundMusic, true);
}

void MusicHelper::playMultipleMusic_loop()
{
    if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) return;
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("color diary episode.mp3", true);
}

void MusicHelper::pauseMusic()
{
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void MusicHelper::resumeMusic()
{
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}