//
//  AudioHelper.cpp
//  HIDD
//
//  Created by lidong on 15-4-8.
//
//

#include "AudioHelper.h"
#include "SimpleAudioEngine.h"


#define Audio_Music_Value "Audio_Music_Value"
#define Audio_Sound_Value "Audio_Sound_Value"
#define Audio_FirstLoad_Bool "Audio_FirstLoad_Bool"

static AudioHelper* _instance=NULL;

using namespace CocosDenshion;

AudioHelper* AudioHelper::getInstance()
{
    if(_instance==NULL)
    {
        _instance=new AudioHelper();
    }
    return _instance;
}

void AudioHelper::reset(float pSoundValue, float pMusicValue)
{
    UserDefault::getInstance()->setBoolForKey(Audio_FirstLoad_Bool, true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(pMusicValue);
    UserDefault::getInstance()->setFloatForKey(Audio_Music_Value, pMusicValue);
    SimpleAudioEngine::getInstance()->setEffectsVolume(pSoundValue);
    UserDefault::getInstance()->setFloatForKey(Audio_Sound_Value, pSoundValue);
    UserDefault::destroyInstance();
}

void AudioHelper::setup(float pSoundValue, float pMusicValue)
{
    bool lRes= UserDefault::getInstance()->getBoolForKey(Audio_FirstLoad_Bool);
    if(!lRes)
    {
        UserDefault::getInstance()->setBoolForKey(Audio_FirstLoad_Bool, true);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(pMusicValue);
        UserDefault::getInstance()->setFloatForKey(Audio_Music_Value, pMusicValue);
        SimpleAudioEngine::getInstance()->setEffectsVolume(pSoundValue);
        UserDefault::getInstance()->setFloatForKey(Audio_Sound_Value, pSoundValue);
        UserDefault::destroyInstance();
    }
    else
    {
        float lSoundValue=UserDefault::getInstance()->getFloatForKey(Audio_Sound_Value);
        SimpleAudioEngine::getInstance()->setEffectsVolume(lSoundValue);
        float lMusicValue=UserDefault::getInstance()->getFloatForKey(Audio_Music_Value);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(lMusicValue);
    }
}

void AudioHelper::setMusicValue(float pMusicValue)
{
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(pMusicValue);
    UserDefault::getInstance()->setFloatForKey(Audio_Music_Value, pMusicValue);
    UserDefault::destroyInstance();
}

void AudioHelper::setSoundValue(float pSoundValue)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(pSoundValue);
    UserDefault::getInstance()->setFloatForKey(Audio_Sound_Value, pSoundValue);
    UserDefault::destroyInstance();
}

float AudioHelper::getSoundValue()
{
    float lValue=UserDefault::getInstance()->getFloatForKey(Audio_Sound_Value);
    AudioHelper::setSoundValue(lValue);
    return  lValue;
}

float AudioHelper::getMusicValue()
{
    float lValue=UserDefault::getInstance()->getFloatForKey(Audio_Music_Value);
    AudioHelper::setMusicValue(lValue);
    return  lValue;
}

void AudioHelper::playBackGroundMusic(const char* music, bool bLoop/* = true*/)
{
    std::string temp(music);
    if (_currentBackMusicStr == temp && SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
    {
        // this music is already played.
        return;
    }
    else if(_currentBackMusicStr != temp)
    {
        _currentBackMusicStr = temp;
        if(SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        {
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        }
        SimpleAudioEngine::getInstance()->playBackgroundMusic(music,bLoop);
    }
    else
    {
         if(!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
         {
             SimpleAudioEngine::getInstance()->playBackgroundMusic(music,bLoop);
         }
    }
}

void AudioHelper::stopBackGroundMusic()
{
     SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void AudioHelper::playSound(const char* sound, bool bLoop /*= false*/)
{
    SimpleAudioEngine::getInstance()->playEffect(sound);
}


