//
//  AudioHelper.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#include "SimpleAudioEngine.h"
#include "AudioHelper.h"
#include "../FileModifiers.h"
#include "../utilities/STUserDefault.h"

USING_NS_ST;
using namespace CocosDenshion;

#define GetAudioEngine              SimpleAudioEngine::sharedEngine()

#define DEFAULT_VOLUME              0.8

static AudioHelper* s_m_AudioHelper = NULL;

AudioHelper* AudioHelper::getInstance()
{
    if (NULL == s_m_AudioHelper)
    {
        s_m_AudioHelper = new AudioHelper();
    }
    
    return s_m_AudioHelper;
}

void AudioHelper::destroy()
{
    CC_SAFE_DELETE(s_m_AudioHelper);
}

void AudioHelper::changeMusicVolume(float volume)
{
    if (mMusicVolume == volume)
        return;
    
    mMusicVolume = volume;
    GetAudioEngine->setBackgroundMusicVolume(mMusicVolume);
    STSaveFloatToUD(key_music_volume, mMusicVolume);
}

void AudioHelper::changeSoundVolume(float volume)
{
    if (mSoundVolume == volume)
        return;
    
    mSoundVolume = volume;
    GetAudioEngine->setEffectsVolume(mSoundVolume);
    STSaveFloatToUD(key_sound_volume, mSoundVolume);
}

void AudioHelper::playSound(const char* sound, bool bLoop /*= false*/)
{
    if (mSoundVolume <= 0)
        return;
    
    GetAudioEngine->playEffect(sound, bLoop);
}

void AudioHelper::playMusic(const char* music, bool bLoop /*= true*/)
{    
    std::string temp(music);
    
    if (preMusicName == temp && GetAudioEngine->isBackgroundMusicPlaying())
    {
        // this music is already played.
        return;
    }
    else if(preMusicName != temp)
    {
        preMusicName = temp;
        
        if (GetAudioEngine->isBackgroundMusicPlaying())
            GetAudioEngine->stopBackgroundMusic();
    }
    
    GetAudioEngine->playBackgroundMusic(music, bLoop);
}

void AudioHelper::stopMusic()
{
    GetAudioEngine->stopBackgroundMusic();
}

void AudioHelper::reset()
{
    mMusicVolume = DEFAULT_VOLUME;
    mSoundVolume = DEFAULT_VOLUME;
    
    GetAudioEngine->setBackgroundMusicVolume(mMusicVolume);
    GetAudioEngine->setEffectsVolume(mSoundVolume);
}

AudioHelper::AudioHelper() : preMusicName()
{
    // load volume
    mMusicVolume = STLoadFloatFromUD(key_music_volume, DEFAULT_VOLUME);
    mSoundVolume = STLoadFloatFromUD(key_sound_volume, DEFAULT_VOLUME);
    
    GetAudioEngine->setBackgroundMusicVolume(mMusicVolume);
    GetAudioEngine->setEffectsVolume(mSoundVolume);
}

AudioHelper::~AudioHelper()
{
    STSaveFloatToUD(key_music_volume, mMusicVolume);
    STSaveFloatToUD(key_sound_volume, mSoundVolume);
}