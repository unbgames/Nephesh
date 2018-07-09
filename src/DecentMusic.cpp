//
// Created by fabio on 08/07/18.
//

#include "DecentMusic.h"
#include <iostream>
#include <Log.h>
#include <Game.h>
#include <Resources.h>

#define INCLUDE_SDL_MIXER
#include "SDL_include.h"


DecentMusic::DecentMusic() {
    music = nullptr;
}

DecentMusic::DecentMusic(string file) {
    music = nullptr;
    Open(file);
}

void DecentMusic::Play(int times, int msToStart) {
    if (music == nullptr) {
        throw "No music loaded - DecentMusic.";
    }
    channel = Mix_FadeInChannel(-1, music.get(), times-1, msToStart);
    if(channel == -1){
        throw "Error playing sound - DecentMusic. Reason: " + string(Mix_GetError());
    }
}

void DecentMusic::Stop(int msToStop) {
    if (music != nullptr) {
        Mix_FadeOutChannel(channel, msToStop);
    }
}

void DecentMusic::Open(string file) {
    music = Resources::GetSound(file);
}

bool DecentMusic::IsOpen() {
    return music != nullptr;
}

DecentMusic::~DecentMusic() {
    Stop(0);
}
