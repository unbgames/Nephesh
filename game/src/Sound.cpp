//
// Created by ftfnunes on 17/03/18.
//

#include <Log.h>
#include <Game.h>
#include <Resources.h>
#include "Sound.h"

Sound::Sound(GameObject &associated) : Component(associated), chunk(nullptr), channel(0) {
}

Sound::Sound(GameObject &associated, string file) : Component(associated), chunk(nullptr), channel(0) {
    Open(file);
}

void Sound::Play(int times, int msToStart) {
    channel = Mix_FadeInChannel(-1, chunk.get(), times-1, msToStart);
    if(channel == -1){
        throw "Error playing sound. Reason: " + string(SDL_GetError());
    }
}

void Sound::Stop(int msToStop) {
    if (chunk != nullptr) {
        Mix_FadeOutChannel(channel, msToStop);
    }
}

void Sound::Open(string file) {
    chunk = Resources::GetSound(move(file));
}

bool Sound::IsOpen() {
    return chunk != nullptr;
}

bool Sound::Is(string type) {
    return type == SOUND_TYPE;
}

void Sound::Update(float dt) {}

void Sound::Render() {}