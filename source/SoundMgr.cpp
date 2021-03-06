//
//  SoundMgr.cpp
//  Rac0r
//
//  Created by Lars Peterke on 18.06.13.
//  Copyright (c) 2013 Jan Schulte. All rights reserved.
//

#include <SFML/Audio.hpp>
#include "ResourcePath.hpp"
#include "SoundMgr.h"

namespace Rac0r {

    SoundMgr::SoundMgr(){
        for(int i=0; i<SOUNDS_NUM; i++){
            sf::Music * song = new sf::Music;
            song->setLoop(true);
            if (!song->openFromFile(resourcePath() + SOUNDS[i]))
            {
                // Error...
            }
            bgm.push_back(song);
        }
    }
}
