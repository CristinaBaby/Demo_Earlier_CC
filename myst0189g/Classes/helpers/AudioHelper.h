//
//  AudioHelper.h
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#ifndef __LVUP002__AudioHelper__
#define __LVUP002__AudioHelper__

#include "cocos2d.h"

#define music_game                      "sounds/in_game.mp3"

#define sound_game_complete             "sounds/game_complete.mp3"
#define sound_game_over                 "sounds/game_over.mp3"
#define sound_game_hint                 "sounds/hint.mp3"
#define sound_game_skill_bonus          "sounds/skill_bonus.mp3"
#define sound_game_correct              "sounds/tap_correct.mp3"
#define sound_game_error                "sounds/tap_error.mp3"
#define sound_popup                     "sounds/popup.mp3"
#define sound_button                    "sounds/tap_button.mp3"

class AudioHelper : public cocos2d::CCObject
{
    AudioHelper();
    
    float mMusicVolume;
    float mSoundVolume;
    
    std::string preMusicName;
    
public:
    ~AudioHelper();
    
    static AudioHelper* getInstance();
    static void destroy();
    
public:
    //
    void changeMusicVolume(float volume);
    //
    void changeSoundVolume(float volume);
    //
    float getMusicVolume()
    {
        return mMusicVolume;
    };
    
    //
    float getSoundVolume()
    {
        return mSoundVolume;
    };
    
    //
    void playSound(const char* sound, bool bLoop = false);
    //
    void playMusic(const char* music, bool bLoop = true);
    void stopMusic();
    
    //
    void reset();
};

#endif /* defined(__LVUP002__AudioHelper__) */
