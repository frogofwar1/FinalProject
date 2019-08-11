//
//  cw_sound_manager.hpp
//  GameEngine
//
//  Created by Chatchai Wangwiwattana on 3/8/2562 BE.
//  Copyright © 2562 Chatchai Wangwiwattana. All rights reserved.
//

#ifndef cw_sound_manager_h
#define cw_sound_manager_h


#include <map>
#include <memory>
#include <AL/alut.h>

namespace cw
{
    

typedef ALuint Audio;


class AudioManager final
{
private:
    
    std::map<std::string, Audio> audioCollection;
    static AudioManager* instance;
    
    AudioManager();
    ~AudioManager();
    
public:
    
    
    static AudioManager& Get();
    
    Audio LoadOrGetAudio(const std::string& fileName, const std::string& refName);
    void play(Audio audio);
    void pause(Audio audio);
    void stop(Audio audio);
    
};

}

#endif /* cw_sound_manager_h */
