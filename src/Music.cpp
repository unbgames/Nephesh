#include <iostream>
#include <Log.h>
#include <Game.h>
#include <Resources.h>

#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include "Music.h"


Music::Music() {
    music = nullptr;
}

Music::Music(string file) {
    music = nullptr;
    Open(file);
}

void Music::Play(int times) {
    if (music == nullptr) {
        throw "No backgroundMusic loaded";
    }
    if (Mix_PlayMusic(music.get(), times) != 0) {
        throw "There was an error playing the backgroundMusic. Reason: " + string(SDL_GetError());
    }
}

void Music::Stop(int msToStop) {
    Mix_FadeOutMusic(msToStop);
}

void Music::Open(string file) {
    music = Resources::GetMusic(file);
}

bool Music::IsOpen() {
    return music != nullptr;
}

Music::~Music() {
    Stop(0);
}